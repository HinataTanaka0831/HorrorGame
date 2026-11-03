#include "Player3D.h"
#include "Master.h"
#include "InputManager.h"
#include "Scene.h"
#include "ObjectManager.h"
#include "Enemy3D.h"
#include "Stage.h"
#include "Exitdoor.h"
#include "EscapeItem.h"
#include "TimeItem.h"


Player3D::Player3D(VECTOR initPosition)
	: Object3D(initPosition)
	, SEframeWalk(25)
	, SEframeRUN(18)
	, ItemSize(170)
	, ItemMargin(10)
	, DrawX(Utility::SCREEN_WIDTH / 2 + 850 - (ItemSize + ItemMargin))
	, DrawY(Utility::SCREEN_HEIGHT / 2 + 550 - ItemSize - 20)
	, MaxSize(1)
	, MaxStamina(100)
	, SplitCount(4)
	, m_oldPosition(VGet(0.0f, 0.0f, 0.0f))
	, m_oldFootprintPosition(VGet(0.0f, 0.0f, 0.0f))
	, m_playerHeight(80.0f)
	, m_distance(0.0f)
	, m_speed(70.0f)
	, m_capsule{ VGet(0.0f, 0.0f, 0.0f), VGet(0.0f, 0.0f, 0.0f), 0.0f }
	, m_stamina(MaxStamina)
	, m_staminaX(Utility::SCREEN_WIDTH / 2 - 260)
	, m_staminaY(Utility::SCREEN_HEIGHT / 2 + 430)
	, m_width(490)
	, m_height(30)
	, m_gaugeWidth(0)
	, m_walkSETimer(0)
	, m_runSETimer(0)
	, m_state(MoveState::Walk)
	, m_isMove(false)
	, m_isCrouching(false)
	, m_isFreeze(false)
	, m_isActiveLight(true)
	, m_isGetEscapeItem(false)
	, m_isGetTimeItem(false)
	, m_isUseStopItem(false)
	, m_lightHandle(-1)
	, m_fontHandle(CreateFontToHandle(NULL, 40, -1, DX_FONTTYPE_ANTIALIASING))
	, m_itemBoxX(0)
	, m_itemBoxY(0)
	, m_addItemID(0)

{
	// タグ設定
	SetTag(Object3D::TagPlayer3D);

	// 懐中電灯のモデルを生成
	m_lightHandle = MV1LoadModel("Resource/Object/light/object_light.mv1");

	// 足跡記録用のコンテナの容量を大体の個数(300個)事前に確保しておくことで、大体の個数(300個)以下までは再確保が発生しないようにする
	m_footprintsRecord.reserve(300);
}


Player3D::~Player3D()
{
	// 懐中電灯のモデルを削除
	MV1DeleteModel(m_lightHandle);
}


void Player3D::Update()
{
	// 移動処理
	MoveEx();

	// アイテムとの当たり判定処理
	ItemCollision();

	// アイテムの使用処理
	UseItem();

	// ライトの処理
	//HaveLight();

	// ライトのON/OFFを変更
	//SetLightEnable(m_isActiveLight);

}


void Player3D::Draw()
{
	// 懐中電灯のモデルを描画
	MV1DrawModel(m_lightHandle);

	// デバッグ表示
	DrawFormatStringToHandle(0, 80, GetColor(255, 255, 255), m_fontHandle, "m_position.x : %.2f, m_position.y : %.2f, m_position.z : %.2f", m_position.x, m_position.y, m_position.z);

	//DrawFormatString(
	//	0,
	//	250,
	//	GetColor(255, 255, 255),
	//	"RecordSize = %d",
	//	(int)m_footprintsRecord.size());

	//DrawFormatString(
	//	0, 350,
	//	GetColor(255, 255, 255),
	//	"Distance = %.2f",
	//	m_distance);

	//DrawFormatString(
	//	0, 390,
	//	GetColor(255, 255, 255),
	//	"OldPos %.2f %.2f %.2f",
	//	m_oldPosition.x,
	//	m_oldPosition.y,
	//	m_oldPosition.z);

	DrawCube3D(
		m_capsule.start,
		m_capsule.end,
		GetColor(0, 255, 0),
		GetColor(0, 255, 0),
		FALSE
	);
}


void Player3D::UpdateAABB()
{

	m_capsule.start = m_position;
	m_capsule.end = VAdd(m_position, VGet(0.0f, 160.0f, 0.0f));
	m_capsule.radius = 30.0f;
}


void Player3D::MoveEx()
{
	//  前回の座標を更新
	m_oldPosition = m_position;

	// フリーズさせる
	if (m_isFreeze)
	{
		return;
	}

	VECTOR moveVec = VGet(0.0f, 0.0f, 0.0f);         // 移動方向
	VECTOR upMoveVector = VGet(0.0f, 0.0f, 0.0f);    // カメラの上方向（奥方向）ベクトル
	VECTOR leftMoveVector = VGet(0.0f, 0.0f, 0.0f);  // カメラの左方向ベクトル
	VECTOR downMoveVector = VGet(0.0f, 0.0f, 0.0f);  // カメラの下方向ベクトル
	VECTOR rightMoveVector = VGet(0.0f, 0.0f, 0.0f); // カメラの右方向ベクトル


	// カメラの向きから移動ベクトルを求める
	{
		// 上方向への移動ベクトルは、カメラの視線方向からY成分を抜いたものとする
		upMoveVector = VSub(Master::m_camera->GetLookAtPosition(), Master::m_camera->GetPosition());
		upMoveVector.y = 0.0f;

		// 左方向への移動ベクトルは、上方向への移動ベクトルと、Y軸のプラス方向へのベクトルに垂直な方向（外積）
		leftMoveVector = VCross(upMoveVector, VGet(0.0f, 1.0f, 0.0f));
		leftMoveVector.y = 0.0f;


		// 移動ベクトルは移動量を加味しないので、正規化しておく（ベクトルの長さを１にすること）
		upMoveVector = VNorm(upMoveVector);
		leftMoveVector = VNorm(leftMoveVector);
	}

	// プレイヤーのしゃがみ状態の変更処理
	PlayerSquat();

	if (InputManager::GetInstance().CheckPressKey(KEY_INPUT_A))  // 左方向
	{
		moveVec = VAdd(moveVec, leftMoveVector);
	}

	if (InputManager::GetInstance().CheckPressKey(KEY_INPUT_D))  // 右方向
	{
		moveVec = VAdd(moveVec, VScale(leftMoveVector, -1.0f)); // VScale...掛け算　-1.0fを掛けて反転している
	}

	if (InputManager::GetInstance().CheckPressKey(KEY_INPUT_W))  // 奥方向
	{
		moveVec = VAdd(moveVec, upMoveVector);
	}

	if (InputManager::GetInstance().CheckPressKey(KEY_INPUT_S))  // 手前方向
	{
		moveVec = VAdd(moveVec, VScale(upMoveVector, -1.0f));
	}



	// 移動している状態であれば
	m_isMove = (moveVec.x != 0.0f || moveVec.z != 0.0f);
	if (m_isMove)
	{
		// 移動方向を正規化しておく
		moveVec = VNorm(moveVec);
	}


	// 走る処理
	if (StaminaUpdate(InputManager::GetInstance().CheckPressKey(KEY_INPUT_LSHIFT)))
	{
		// 走り状態かつ動いている状態なら
		// プレイヤーの速度を*3.0fにして、走っているスピードに合わせるように足音を再生させる
		if (m_state == MoveState::Run && m_isMove)
		{
			m_speed = m_speed * 3.0f;

			m_runSETimer++;

			if (m_runSETimer >= SEframeRUN)
			{
				Master::m_soundManager->PlaySE(SoundManager::SeFootSteps);
				m_runSETimer = 0;
			}
		}
		else
		{
			return;
		}
	}
	else  // 歩き処理
	{
		// 歩き状態かつ動いている状態なら、歩くスピードに合わせるように足音を再生させる
		if (m_state == MoveState::Walk && m_isMove)
		{
			m_walkSETimer++;

			if (m_walkSETimer >= SEframeWalk)
			{
				Master::m_soundManager->PlaySE(SoundManager::SeFootSteps);
				m_walkSETimer = 0;
			}
		}
	}


	//---- 足跡の記録処理 ----//
    // 足跡リストが200より大きくなったら古いものから消していく
	if (m_footprintsRecord.size() > 200)
	{
		m_footprintsRecord.erase(m_footprintsRecord.begin());
	}

	// ベクトルのサイズを取得・距離を計測
	m_distance = VSize(VSub(m_position, m_oldFootprintPosition));

	// 距離が一定距離以上なら
	// プレイヤーの座標を保存して、古い足跡座標を更新する
	if (m_distance >= 15.0f)
	{
		m_footprintsRecord.push_back(m_position);
		m_oldFootprintPosition = m_position;
	}



	// 移動処理を分割して行うことで、壁にめり込むのを防ぐ
	for (int i = 0; i < 4; i++)
	{
		// プレイヤーの座標を更新
		m_position = VAdd(m_position, VScale(moveVec, m_speed / 4));


		// 移動処理の後にプレイヤーのAABB判定を更新させる
		UpdateAABB();


		//---- ステージとの当たり判定 ----//
		auto pStageObj = Master::m_sceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DByTag(Object3D::TagStage);
		if (pStageObj == nullptr)
		{
			return;
		}

		Stage* pStage = dynamic_cast<Stage*>(pStageObj);
		if (pStage == nullptr)
		{
			return;
		}

		const auto& getWall = CollisionManager::GetInstance().GetWalls();

		for (const auto& wall : getWall)
		{
			// AABB判定が行われたら、プレイヤーと壁の中心から衝突法線を取得し、侵入量（その方向にどれだけめり込んでいるのか）を比較して法線を決定させる
			// 壁に沿って行くようなスライド処理を行うためのベクトルを取得してプレイヤーの座標を更新する
			if (CollisionManager::GetInstance().CheckCapsuleAABB(m_capsule, wall))
			{
				VECTOR playerCenter = VGet((m_capsule.start.x + m_capsule.end.x) * 0.5f, 0.0f, (m_capsule.start.z + m_capsule.end.z) * 0.5f);
				VECTOR wallCenter = VGet((wall.min.x + wall.max.x) * 0.5f, 0.0f, (wall.min.z + wall.max.z) * 0.5f);

				// 壁の中心から見てプレイヤーがどっち側にいるのかを取得するためのベクトルを求める。
				// 壁の中心からプレイヤーの中心への方向ベクトル
				VECTOR directionToPlayer = VSub(playerCenter, wallCenter);

				// プレイヤーが壁にどれだけめり込んだのかを求める
				float overlapX = min(m_capsule.end.x, wall.max.x) - max(m_capsule.start.x, wall.min.x);
				float overlapZ = min(m_capsule.end.z, wall.max.z) - max(m_capsule.start.z, wall.min.z);

				VECTOR normal = VGet(0.0f, 0.0f, 0.0f);
				// 最も少ない移動で壁の外に出られる方向を求めるために、X軸方向の侵入量とZ軸方向の侵入量を比較して、少ない方の法線を取得する
				if (overlapX < overlapZ)
				{
					normal.x = (directionToPlayer.x > 0.0f) ? 1.0 : -1.0f;
				}
				else
				{
					normal.z = (directionToPlayer.z > 0.0f) ? 1.0f : -1.0f;
				}

				// Debug用に侵入量と法線を表示
				//DrawFormatString(0, 450, GetColor(255, 255, 255), "overlapX = %.5f, overlapZ = %.5f", overlapX, overlapZ);
				//DrawFormatString(0, 500, GetColor(255, 255, 255), "normal.x = %.5f, normal.z = %.5f", normal.x, normal.z);

				VECTOR wallSlider = VGet(0.0f, 0.0f, 0.0f);
				// 移動方向ベクトルと取得した衝突法線との内積を求める
				float dot = VDot(moveVec, normal);
				//　壁沿いベクトルの計算
				wallSlider = VSub(moveVec, VScale(normal, dot));
				// 壁沿いベクトルを正規化する(ベクトルの長さを1にする)
				wallSlider = VNorm(wallSlider);
				m_position = VAdd(m_oldPosition, VScale(wallSlider, m_speed));

				// ステージに当たった後にもう一度プレイヤーのAABB判定を更新させる
				UpdateAABB();

				break;
			}

		}


	}


	// 脱出口との当たり判定をする
	const auto& getExitDoor = CollisionManager::GetInstance().GetExitDoor();

	for (const auto& exitDoor : getExitDoor)
	{
		// AABB判定が行われたら、プレイヤーと脱出口の中心から衝突法線を取得し、侵入量（その方向にどれだけめり込んでいるのか）を比較して法線を決定させる
		// 脱出口に沿って行くようなスライド処理を行うためのベクトルを取得してプレイヤーの座標を更新する
		if (CollisionManager::GetInstance().CheckCapsuleAABB(m_capsule, exitDoor))
		{
			VECTOR playerCenter = VGet((m_capsule.start.x + m_capsule.end.x) * 0.5f, 0.0f, (m_capsule.start.z + m_capsule.end.z) * 0.5f);
			VECTOR exitDoorCenter = VGet((exitDoor.min.x + exitDoor.max.x) * 0.5f, 0.0f, (exitDoor.min.z + exitDoor.max.z) * 0.5f);

			// 脱出口の中心から見てプレイヤーがどっち側にいるのかを取得するためのベクトルを求める。
			VECTOR directionToPlayer = VSub(playerCenter, exitDoorCenter);

			// 脱出口にどれだけめり込んだのかを求める
			float overlapX = min(m_capsule.end.x, exitDoor.max.x) - max(m_capsule.start.x, exitDoor.min.x);
			float overlapZ = min(m_capsule.end.z, exitDoor.max.z) - max(m_capsule.start.z, exitDoor.min.z);

			// 最も少ない移動で当たり判定の外に出られる方向を求めるために侵入量を比較して、値が少ないほうの法線を取得する。
			VECTOR normal = VGet(0.0f, 0.0f, 0.0f);
			if (overlapX < overlapZ)
			{
				normal.x = (directionToPlayer.x > 0.0f) ? overlapX : -overlapX;
			}
			else
			{
				normal.z = (directionToPlayer.z > 0.0f) ? overlapZ : -overlapZ;
			}

			// Debug用に侵入量と法線を表示
			//DrawFormatString(0, 450, GetColor(255, 255, 255), "overlapX = %.5f, overlapZ = %.5f", overlapX, overlapZ);
			//DrawFormatString(0, 500, GetColor(255, 255, 255), "normal.x = %.5f, normal.z = %.5f", normal.x, normal.z);

			VECTOR wallSlider = VGet(0.0f, 0.0f, 0.0f);
			// 移動方向ベクトルと取得した衝突法線との内積を求める
			float dot = VDot(moveVec, normal);
			//　壁沿いベクトルの計算
			wallSlider = VSub(moveVec, VScale(normal, dot));
			// 壁沿いベクトルを正規化する(ベクトルの長さを1にする)
			wallSlider = VNorm(wallSlider);
			m_position = VAdd(m_oldPosition, VScale(wallSlider, m_speed));


			UpdateAABB();

			break;
		}
	}



}



void Player3D::PlayerSquat()
{
	// Eキーを押した場合
	if (InputManager::GetInstance().CheckPressKey(KEY_INPUT_E))
	{
		// しゃがみ状態
		m_isCrouching = true;
	}
	else
	{
		// しゃがみ状態ではない
		m_isCrouching = false;
	}

	// しゃがみ状態ならば
	if (m_isCrouching)
	{
		// プレイヤーの高さを変更
		m_playerHeight = 60.0f;
		// プレイヤーのスピードを変更
		m_speed = 3.0f;

	}
	else
	{
		// 初期値と同じ
		m_playerHeight = 80.0f;
		m_speed = 6.0f;
	}

}

void Player3D::HaveLight()
{

	// ライトモデルの光らせる位置を取得
	VECTOR lightPosition = MV1GetFramePosition(m_lightHandle, 0);

	// カメラの座標を取得
	VECTOR camPos = Master::m_camera->GetPosition();

	// ワールド座標の上方向
	VECTOR worldUp = VGet(0.0f, 1.0f, 0.0f);

	// カメラの注視点を取得
	VECTOR camLook = Master::m_camera->GetLookAtPosition();

	//// カメラの向きを取得
	VECTOR camDirection = VNorm(VSub(camLook, camPos));

	// カメラの右方向を取得
	VECTOR camRight = VNorm(VCross(worldUp, camDirection));

	// カメラの上方向を取得
	VECTOR camUp = VNorm(VCross(camDirection, camRight));

	// ライトのモデルの位置を設定
	VECTOR lightModelPosition = camPos;
	lightModelPosition = VAdd(lightModelPosition, VScale(camDirection, 100.0f));
	lightModelPosition = VAdd(lightModelPosition, VScale(camRight, 85.0f));
	lightModelPosition = VAdd(lightModelPosition, VScale(camUp, -310.0f));

	// 懐中電灯のモデルの位置を設定
	MV1SetPosition(m_lightHandle, lightModelPosition);



	// ライトモデルを視点を動かしたら同じ方向に回転させる
	float angle = atan2f(camDirection.x, camDirection.z);
	float angleUpDown = atanf(camDirection.y);
	MV1SetRotationXYZ(m_lightHandle, VGet(-angleUpDown, angle, 0.0f));

	// ディフューズカラー（オレンジっぽい光）
	SetLightDifColor(GetColorF(1.0f, 0.8f, 0.4f, 0.0f)); //R,G,B

	// ライトをスポットライトに変更する
	ChangeLightTypeSpot(
	lightPosition,      // ライト位置
	camDirection,       // カメラの向き
	DX_PI_F / 3.5f, 
	DX_PI_F / 2.0f, 
	2000.0f,            // 有効距離
	0.05f,              // 減衰係数1
	0.00020f,           // 減衰係数2
	0.0f                // 減衰係数3
	);


	Object3D* Eobj = Master::m_sceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DByTag(Object3D::TagEnemy3D);
	if (Eobj != nullptr)
	{
		Enemy3D* Ene = dynamic_cast<Enemy3D*>(Eobj);
		if (Ene != nullptr)
		{
			bool getScare = Ene->GetScare();

			if (getScare)
			{
				m_isActiveLight = false;
			}




			if (InputManager::GetInstance().CheckTriggerMouseClick(MOUSE_INPUT_LEFT))
			{
				// ライトを点ける
				m_isActiveLight = true;
			}


			// ライトが照らされていたら
			if (GetLightEnable() == TRUE && !getScare)
			{
				if (InputManager::GetInstance().CheckTriggerMouseClick(MOUSE_INPUT_LEFT))
				{
					// ライトを消す
					m_isActiveLight = false;
				}

			}


		}
	}


}


void Player3D::ItemCollision()
{
	// 脱出アイテムの情報を取得
	auto pObj = Master::m_sceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DListByTag(Object3D::TagEscapeItem);

	for (int i = 0; i < pObj.size(); i++)
	{
		auto Escape = pObj[i];

		if (Escape == nullptr)
		{
			continue;
		}

				// 当たり判定
				if (CollisionManager::GetInstance().CheckCircleToCircle(
					m_position,
					60.0f,
					Escape->GetPosition(),
					90.0f)
					)
				{

					if (!m_isGetEscapeItem && !m_isGetTimeItem)
					{
						// 近づいたら表示
						DrawStringToHandle(Utility::SCREEN_WIDTH / 2 - 100, 900, "R : 取得", GetColor(255, 255, 255), m_fontHandle);
					}

					// Rキーを押した場合
					if (InputManager::GetInstance().CheckPressKey(KEY_INPUT_R))
					{
						m_isGetEscapeItem = true;

						EscapeItem* pEscape = dynamic_cast<EscapeItem*>(Escape);
						
						if (pEscape == nullptr)
						{
							continue;
						}

							// 脱出アイテムをインベントリに追加
							AddItem(pEscape->escape);

							pEscape->SetDeleteFlag(true);

							// ItemIDを1にする
							m_addItemID = 1;
						

					}

				}


	}


	// 時間止めアイテムの情報を取得
	auto pobj = Master::m_sceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DListByTag(Object3D::TagTimeItem);

	for (int i = 0; i < pobj.size(); i++)
	{
		auto StopTime = pobj[i];

		if (StopTime == nullptr)
		{
			continue;
		}
				if (CollisionManager::GetInstance().CheckCircleToCircle(
					m_position,
					60.0f,
					StopTime->GetPosition(),
					90.0f)
					)
				{
					if (!m_isGetTimeItem && !m_isGetEscapeItem)
					{
						// 近づいたら表示
						DrawStringToHandle(Utility::SCREEN_WIDTH / 2 - 100, 900, "R : 取得", GetColor(255, 255, 255), m_fontHandle);
					}

					// もしRキーを押したら
					if (InputManager::GetInstance().CheckPressKey(KEY_INPUT_R))
					{
						m_isGetTimeItem = true;


						TimeItem* pTimeItem = dynamic_cast<TimeItem*>(StopTime);

						if (pTimeItem == nullptr)
						{
							continue;
						}

							// 時間止めアイテムをインベントリに追加
							AddItem(pTimeItem->stoptime);

							pTimeItem->SetDeleteFlag(true);

							// ItemIDを2にする
							m_addItemID = 2;


						


					}
				}
			
		
	}



}


void Player3D::UseItem()
{
	if (m_addItemID == 1)
	{
		auto door = Master::m_sceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DByTag(Object3D::TagExitdoor);
		if (door != nullptr)
		{
			Exitdoor* ExitDoor = dynamic_cast<Exitdoor*>(door);

			if (ExitDoor == nullptr)
			{
				return;
			}
				// 当たり判定
				if (CollisionManager::GetInstance().CheckCircleToCircle(
					m_position,
					100.0f,
					ExitDoor->GetPosition(),
					100.0f)
					)
				{
					// 脱出アイテムを持っていたら
					if (m_isGetEscapeItem)
					{
						DrawStringToHandle(Utility::SCREEN_WIDTH / 2 - 100, 900, "F : 使用", GetColor(255, 255, 255), m_fontHandle);
					}

					// Fキーを押した場合
					if (InputManager::GetInstance().CheckPressKey(KEY_INPUT_F))
					{
						LoseItem();

						// 脱出アイテムを持っていない
						m_isGetEscapeItem = false;
					}

				}
			

		}
	}

	// 時間止めアイテムを持っていたら
	if (m_isGetTimeItem)
	{
		DrawStringToHandle(Utility::SCREEN_WIDTH / 2 - 100, 900, "F : 使用", GetColor(255, 255, 255), m_fontHandle);
	}

	if (m_addItemID == 2)
	{
		if (InputManager::GetInstance().CheckPressKey(KEY_INPUT_F))
		{
			if (!m_isUseStopItem)
			{
				LoseItem();
			}

			// 時間止めアイテムを持っていない
			m_isGetTimeItem = false;
		}
	}

}

void Player3D::ItemBox()
{
	// アイテムインベントリにアイテムがある場合
	for (size_t i = 0; i < m_items.size(); i++)
	{
		m_itemBoxX = DrawX + (int)i * (ItemSize + ItemMargin);

		m_itemBoxY = DrawY;

		// アイテムボックスの枠
		DrawBox(m_itemBoxX, m_itemBoxY, m_itemBoxX + ItemSize, m_itemBoxY + ItemSize, GetColor(200, 200, 200), false);

		// アイテム名の表示
		DrawStringToHandle(m_itemBoxX + 5, m_itemBoxY - 45, m_items[i].name, GetColor(255, 255, 255), m_fontHandle);

		// アイテム画像の表示
		DrawGraph(m_itemBoxX + 50, m_itemBoxY, m_items[i].image, true);

	}

}


bool Player3D::AddItem(const ItemData item)
{
	// アイテムのサイズが最大数より少ない場合構造体に情報を追加する
	if (m_items.size() < MaxSize)
	{
		m_items.push_back(item);

		return true;

	}

	// それ以外なら何もしない
	return false;
}





void Player3D::LoseItem()
{
	// アイテムのサイズが最大数より小さい場合何もしない
	if (m_items.size() < MaxSize)
	{
		return;
	}


	for (size_t i = 0; i < m_items.size(); i++)
	{
		// アイテム番号が1の場合アイテムを脱出口で使用したときの処理を呼び出す
		if (m_items[i].id == 1)
		{
			EscapeItem::Escape();

		}

		// アイテム番号が 2 の場合のアイテムを使用したときの処理を呼び出す
		if (m_items[i].id == 2)
		{
			// アイテムを使用
			m_isUseStopItem = true;

		}

		// アイテムの情報を削除する
		m_items.clear();
		

	}

}

void Player3D::DrawStamina()
{
	int r = 20, g = 150, b = 20;

	if ((float)m_stamina <= 0.5f)
	{
		int noise = GetRand(30) - 20;
		r = max(0, min(255, r + noise));

		int shake = 2 + (int)((0.2f - (float)m_stamina) * 20);
		m_staminaX += GetRand(shake * 2) - shake;
		m_staminaY += GetRand(shake * 2) - shake;

		float flicker = (float)sin(GetNowCount() * 0.02f);
		int alpha = 180 + (int)(75 * abs(flicker));

		SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
	}
	else
	{
		m_staminaX = Utility::SCREEN_WIDTH / 2 - 260; // スタミナゲージのX座標
		m_staminaY = Utility::SCREEN_HEIGHT / 2 + 430; // スタミナゲージのY座標

		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	}

	// スタミナゲージの枠
	DrawBox(m_staminaX, m_staminaY, m_staminaX + m_width, m_staminaY + m_height, GetColor(0, 0, 0), true);

	// スタミナゲージ
	DrawBox(m_staminaX, m_staminaY, m_staminaX + m_gaugeWidth, m_staminaY + m_height, GetColor(r, g, b), true);

	// 枠線
	DrawBox(m_staminaX, m_staminaY, m_staminaX + m_width, m_staminaY + m_height, GetColor(255, 255, 255), false);

}


bool Player3D::StaminaUpdate(bool isKeyProsses)
{
	// 現在値に応じたスタミナゲージの幅
	m_gaugeWidth = (int)((float)m_stamina / MaxStamina * m_width);


	// 走りキーを押して移動しているならならスタミナゲージを減らす
	if (isKeyProsses && m_stamina > 0 && m_isMove)
	{
		// 走り状態にする
		m_state = MoveState::Run;
		// スタミナゲージを減らす
		m_stamina--;

		// 0以下なら歩き状態にする
		if (m_stamina <= 0)
		{
			m_state = MoveState::Walk;
		}

		return true;
	}
	else
	{
		// 歩き状態にする
		m_state = MoveState::Walk;

		// 現在のスタミナゲージが最大値より少ない場合スタミナを増やす
		if (m_stamina < MaxStamina)
		{
			m_stamina++;
		}
		return false;
	}

}


