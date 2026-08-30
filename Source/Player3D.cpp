#include "Player3D.h"
#include "Master.h"
#include "InputManager.h"
#include "Scene.h"
#include "ObjectManager.h"
#include "Enemy3D.h"
#include "Stage.h"
#include "Collision.h"
#include "Exitdoor.h"
#include "EscapeItem.h"
#include "TimeItem.h"


Player3D::Player3D(VECTOR initPos)
	:Object3D(initPos)
	, isMove(false)
	, isCrouching(false)
	, isFreeze(false)
	, isActiveLight(true)
	, isUseStopItem(false)
	, getEscapeItem(false)
	, getTimeItem(false)
	, oldPosition(VGet(0.0f, 0.0f, 0.0f))
	, oldPlayerPosition(mvPosition)
	, lightModelPosition(VGet(0.0f, 0.0f, 0.0f))
	, DoorPos(VGet(0.0f, 0.0f, 0.0f))
	, dis(0.0f)
	, mfAngle(0.0f)
	, mfTargetAngle(0.0f)
	, playerHeight(0.0f)
	, playerSpeed(0.0f)
	, gaugeWidth(0)
	, mncount(0)
	, RecordDis(0)
	, AddItemID(0)

{
	// タグ設定
	SetTag(Object3D::TagPlayer3D);

	// 懐中電灯のモデルを生成
	lightHandle = MV1LoadModel("Resource/3D/Linterna/Linterna.mv1");

}


Player3D::~Player3D()
{
	// 懐中電灯のモデルを削除
	MV1DeleteModel(lightHandle);
}


void Player3D::Update()
{

	// 移動処理
	MoveEx();

	// アイテムとの当たり判定処理
	ItemCollision();

	// アイテムの使用処理
	UseItem();

	// アイテムボックスの表示処理
	ItemBox();

	// ライトの処理
	HaveLight();

	// ライトのON/OFFを変更
	SetLightEnable(isActiveLight);


}


void Player3D::Draw()
{
	// 懐中電灯のモデルを描画
	MV1DrawModel(lightHandle);
}



void Player3D::MoveEx()
{
	// フリーズさせる
	if (isFreeze)
	{
		return;
	}



	VECTOR moveVec = VGet(0.0f, 0.0f, 0.0f);   // 移動方向
	VECTOR upMoveVector = VGet(0.0f, 0.0f, 0.0f);    // カメラの上方向（奥方向）ベクトル
	VECTOR leftMoveVector = VGet(0.0f, 0.0f, 0.0f);  // カメラの左方向ベクトル
	VECTOR downMoveVector = VGet(0.0f, 0.0f, 0.0f);  // カメラの下方向ベクトル
	VECTOR rightMoveVector = VGet(0.0f, 0.0f, 0.0f); // カメラの右方向ベクトル


	// カメラの向きから移動ベクトルを求める
	{
		// 上方向への移動ベクトルは、カメラの視線方向からY成分を抜いたものとする
		upMoveVector = VSub(Master::mpCamera->GetLookAtPosition(), Master::mpCamera->GetPosition());
		upMoveVector.y = 0.0f;

		// 下方向
		downMoveVector = VSub(Master::mpCamera->GetLookAtPosition(), Master::mpCamera->GetPosition());
		downMoveVector.y = 0.0f;

		// 左方向への移動ベクトルは、上方向への移動ベクトルと、Y軸のプラス方向へのベクトルに垂直な方向（外積）
		leftMoveVector = VCross(upMoveVector, VGet(0.0f, 1.0f, 0.0f));
		leftMoveVector.y = 0.0f;

		// 右方向
		rightMoveVector = VCross(upMoveVector, VGet(0.0f, 1.0f, 0.0f));
		rightMoveVector.y = 0.0f;


		// 移動ベクトルは移動量を加味しないので、正規化しておく（ベクトルの長さを１にすること）
		upMoveVector = VNorm(upMoveVector);
		leftMoveVector = VNorm(leftMoveVector);
		downMoveVector = VNorm(downMoveVector);
		rightMoveVector = VNorm(rightMoveVector);
	}

	// プレイヤーのしゃがみ状態の変更処理
	PlayerSquat();

	if (CheckHitKey(KEY_INPUT_A))  // 左方向
	{
		moveVec = VAdd(moveVec, leftMoveVector);
	}

	if (CheckHitKey(KEY_INPUT_D))  // 右方向
	{
		moveVec = VAdd(moveVec, VScale(leftMoveVector, -1.0f)); // VScale...掛け算　-1.0fを掛けて反転している
	}

	if (CheckHitKey(KEY_INPUT_W))  // 奥方向
	{
		moveVec = VAdd(moveVec, upMoveVector);
	}

	if (CheckHitKey(KEY_INPUT_S))  // 手前方向
	{
		moveVec = VAdd(moveVec, VScale(upMoveVector, -1.0f));
	}



	// 移動している状態であれば
	isMove = (moveVec.x != 0.0f || moveVec.z != 0.0f);
	if (isMove)
	{
		// 移動方向を正規化しておく
		moveVec = VNorm(moveVec);
	}

	// 前の座標を設定
	oldPosition = mvPosition;

	// 走る処理
	if (StaminaUpdate(CheckHitKey(KEY_INPUT_LSHIFT)))
	{
		// 走り状態かつ動いている状態なら
		if (state == RUN && isMove)
		{
			mvPosition = VAdd(mvPosition, VScale(moveVec, playerSpeed * 3.0f));

			// 走りSEを流す時間を増やす
			RunSETimer++;

			// フレーム以上なら
			if (RunSETimer >= SEframe_RUN)
			{
				Master::mpSoundManager->PlaySE(SoundManager::SE_Walk);
				RunSETimer = 0;
			}
		}
		else
		{
			return;
		}
	}
	else  // 歩き処理
	{
		// 歩き状態かつ動いている状態なら
		if (state == WALK && isMove)
		{
			mvPosition = VAdd(mvPosition, VScale(moveVec, playerSpeed));

			// 歩きSEを流す時間を増やす
			WalkSETimer++;

			// フレーム以上なら
			if (WalkSETimer >= SEframe_Walk)
			{
				Master::mpSoundManager->PlaySE(SoundManager::SE_Walk);
				WalkSETimer = 0;
			}
		}
	}



	// 足跡の記録処理 //
   // 敵の情報を取得
	Object3D* eobj = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DByTag(Object3D::TagEnemy3D);

	if (eobj != nullptr)
	{
		Enemy3D* Enemy = dynamic_cast<Enemy3D*>(eobj);

		if (Enemy != nullptr)
		{
			// プレイヤーを見つけたかどうかのフラグを取得
			bool found = Enemy->GetfoundPlayer();

			// 見つけたら
			if (found)
			{
				// ベクトルのサイズを取得・距離を計測
				dis = VSize(VSub(mvPosition, oldPlayerPosition));

				// 距離が一定距離以上なら
				if (dis >= 25.0f)
				{
					// プレイヤーの座標を保存
					playerRecord.push_back(mvPosition);
					oldPlayerPosition = mvPosition;
				}

			}


		}
	}


	// ステージとの当たり判定をする
	VECTOR hitPos = VGet(0.0f, 0.0f, 0.0f);
	bool isHit = false;
	int count = 0;
	auto obj = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DByTag(Object3D::TagStage);
	if (obj != nullptr)
	{
		Stage* pStage = dynamic_cast<Stage*>(obj);
		if (pStage != nullptr)
		{
			// ステージとプレイヤーのカプセルが当たっている場合 (ステージの地面とプレイヤーとの当たり判定処理)
			if (pStage->CheckHit_Capsule(mvPosition, VAdd(mvPosition, VGet(0.0f, playerHeight, 0.0f)), 40.0f))
			{
				// 当たっているであろうポリゴンとの接触点を求める
				hitPos = pStage->CheckHit_Line(
					VAdd(mvPosition, VGet(0.0f, 50.0f, 0.0f)),  // プレイヤーの膝辺り（多分）と
					VAdd(mvPosition, VGet(0.0f, -50.0f, 0.0f))  // プレイヤーの少し下あたりを線分として指定
				);


				// 当たった判定を取っておく
				isHit = true;

			}

			// ---------------------------------------------------------
			// 壁との当たり判定とスライド処理（ダッシュ対応版）
			// ---------------------------------------------------------

			// ① すでに「歩き・ダッシュ処理」で計算された最新の mvPosition をそのまま「目標位置」とします
			// （ここで playerSpeed を掛け直さないことで、ダッシュの速度移動量が正しく反映されます）
			VECTOR nextPos = mvPosition;

			// ② スライド・すり抜け防止のための最大5回ループ
			const int SLIDE_MAX_ITERATION = 15;

			for (int i = 0; i < SLIDE_MAX_ITERATION; ++i)
			{
				// 目標位置（nextPos）でカプセルを作り、ステージの壁と当たっているかチェック
				// ※ Y座標の playerHeight や 90.0f の数値は、敵に使う場合は敵のサイズ（70.0f等）に合わせてください
				VECTOR capBottom = VAdd(nextPos, VGet(0.0f, playerHeight, 0.0f));
				VECTOR capTop = VAdd(nextPos, VGet(0.0f, 90.0f, 0.0f));


				// -------------------------------------------------
				// ★ すべてのヒット情報を利用した新しい壁抜け防止ロジック ★
				// -------------------------------------------------
				if (pStage->CheckHit_Capsule(capBottom, capTop, 40.0f))
				{
					// ① まずは現在の壁の法線で勢いを打ち消す（1 回だけ）
					VECTOR movePath = VSub(nextPos, oldPosition);
					VECTOR firstNorm = VNorm(pStage->GetNormal());
					float  dot = VDot(movePath, firstNorm);
					if (dot < 0.0f)
					{
						VECTOR pushBack = VScale(firstNorm, -dot);
						nextPos = VAdd(nextPos, pushBack);
						capBottom = VAdd(capBottom, pushBack);
						capTop = VAdd(capTop, pushBack);
					}

					// ② MV1 の衝突関数から **全ヒット情報** を取得し、合成法線で押し出す
					const int SLIDE_MAX_ITERATION = 150;
					for (int i = 0; i < SLIDE_MAX_ITERATION; ++i)
					{
						// まだめり込んでいるか確認
						MV1_COLL_RESULT_POLY_DIM result =
							MV1CollCheck_Capsule(pStage->GetCollisionHandle(), -1, capBottom, capTop, 40.0f);
						if (result.HitNum == 0) break;   // めり込みが解消された

						// すべてのヒット法線を合成
						VECTOR summed = VGet(0.0f, 0.0f, 0.0f);
						for (int h = 0; h < result.HitNum; ++h)
						{
							summed = VAdd(summed, result.Dim[h].Normal);
						}
						VECTOR pushDir = VNorm(summed);

						// 合成法線方向へ 1.0f だけ押し出す
						nextPos = VAdd(nextPos, VScale(pushDir, 1.0f));
						capBottom = VAdd(capBottom, VScale(pushDir, 1.0f));
						capTop = VAdd(capTop, VScale(pushDir, 1.0f));
					}
				}
			}

			// 最終的な安全な位置を反映
			mvPosition = nextPos;

			if (isHit)
			{
				// 地面に沿って歩いている状態として、Y座標をステージに合わせる
				mvPosition.y = hitPos.y;

			}


		}


		// 脱出口との当たり判定をする
		VECTOR hitPosition = VGet(0.0f, 0.0f, 0.0f);
		bool ishit = false;
		auto object = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DByTag(Object3D::TagExitdoor);
		if (object != nullptr)
		{
			DoorPos = object->GetPosition();

			Exitdoor* pDoor = dynamic_cast<Exitdoor*>(object);
			if (pDoor != nullptr)
			{
				// ステージとプレイヤーのカプセルが当たっている場合
				if (pDoor->CheckHit_Capsule(mvPosition, VAdd(mvPosition, VGet(0.0f, playerHeight, 0.0f)), 40.0f))
				{

					// 当たっているであろうポリゴンとの接触点を求める
					hitPosition = pDoor->CheckHit_Line(
						VAdd(mvPosition, VGet(0.0f, playerHeight, 0.0f)),  // プレイヤーの膝辺り（多分）と
						VAdd(mvPosition, VGet(0.0f, -playerHeight, 0.0f))  // プレイヤーの少し下あたりを線分として指定
					);

					// 当たった判定を取っておく
					ishit = true;
				}

			}

			// 当たった場合
			if (ishit)
			{
				if (pDoor != nullptr)
				{
					// 脱出口の法線を取得
					VECTOR nurm = pDoor->GetNormal();
					// 壁に沿っていくようなベクトルを取得
					VECTOR slider = VGet(0.0f, 0.0f, 0.0f);  // 壁沿いベクトル
					float a = VDot(VScale(moveVec, -1.0f), nurm); // 移動方向ベクトルの反対方向と、脱出口の法線との内積を求める
					slider = VAdd(moveVec, VScale(nurm, a));   // 壁沿いベクトルを計算

					mvPosition = VAdd(oldPosition, VScale(slider, playerSpeed));

				}

			}

		}



	}
}



void Player3D::PlayerSquat()
{
	// Eキーを押した場合
	if (CheckHitKey(KEY_INPUT_E))
	{
		// しゃがみ状態
		isCrouching = true;
	}
	else
	{
		// しゃがみ状態ではない
		isCrouching = false;
	}

	// しゃがみ状態ならば
	if (isCrouching)
	{
		// プレイヤーの高さを変更
		playerHeight = 60.0f;
		// プレイヤーのスピードを変更
		playerSpeed = 3.0f;

	}
	else
	{
		// 初期値と同じ
		playerHeight = 80.0f;
		playerSpeed = 6.0f;
	}

}

void Player3D::HaveLight()
{

	// ライトモデルの光らせる位置を取得
	VECTOR lightPosition = MV1GetFramePosition(lightHandle, 0);

	// カメラの座標を取得
	VECTOR camPos = Master::mpCamera->GetPosition();

	// ワールド座標の上方向
	VECTOR worldUp = VGet(0.0f, 1.0f, 0.0f);

	// カメラの注視点を取得
	VECTOR camLook = Master::mpCamera->GetLookAtPosition();

	//// カメラの向きを取得
	VECTOR camDir = VNorm(VSub(camLook, camPos));

	// カメラの右方向を取得
	VECTOR camRight = VNorm(VCross(worldUp, camDir));

	// カメラの上方向を取得
	VECTOR camUp = VNorm(VCross(camDir, camRight));


	// ライトのモデルの位置を設定
	lightModelPosition = camPos;
	lightModelPosition = VAdd(lightModelPosition, VScale(camDir, 100.0f));
	lightModelPosition = VAdd(lightModelPosition, VScale(camRight, 85.0f));
	lightModelPosition = VAdd(lightModelPosition, VScale(camUp, -310.0f));

	// 懐中電灯のモデルの位置を設定
	MV1SetPosition(lightHandle, lightModelPosition);



	// ライトモデルを視点を動かしたら同じ方向に回転させる
	float angle = atan2f(camDir.x, camDir.z);
	float angle_UpDown = atanf(camDir.y);
	MV1SetRotationXYZ(lightHandle, VGet(-angle_UpDown, angle, 0.0f));

	// ディフューズカラー（オレンジっぽい光）
	SetLightDifColor(GetColorF(1.0f, 0.8f, 0.4f, 0.0f)); //R,G,B

	// ライトをスポットライトに変更する
	ChangeLightTypeSpot(
	lightPosition,// ライト位置
	camDir,       // カメラの向き
	DX_PI_F / 3.5f, 
	DX_PI_F / 2.0f, 
	2000.0f,      // 有効距離
	0.05f,        // 減衰係数1
	0.00020f,   // 減衰係数2 0.00020f
	0.0f         // 減衰係数3
	);


	Object3D* Eobj = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DByTag(Object3D::TagEnemy3D);
	if (Eobj != nullptr)
	{
		Enemy3D* Ene = dynamic_cast<Enemy3D*>(Eobj);
		if (Ene != nullptr)
		{
			bool getScare = Ene->GetScare();

			if (getScare)
			{
				//SetLightPosition(VAdd(camPos, VScale(camDir, -400.0f)));

				//SetLightDirection(camDir);

				isActiveLight = false;
			}




			if (InputManager::GetInstance().CheckTriggerMouseClick(MOUSE_INPUT_LEFT))
			{
				// ライトを点ける
				isActiveLight = true;
			}


			// ライトが照らされていたら
			if (GetLightEnable() == TRUE && !getScare)
			{
				if (InputManager::GetInstance().CheckTriggerMouseClick(MOUSE_INPUT_LEFT))
				{
					// ライトを消す
					isActiveLight = false;
				}

			}


		}
	}


}


void Player3D::ItemCollision()
{
	// 脱出アイテムの情報を取得
	auto pObj = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DListByTag(Object3D::TagEscapeItem);

	for (int i = 0; i < pObj.size(); i++)
	{
		auto Escape = pObj[i];

		if (Escape != nullptr)
		{
				// 当たり判定
				if (Collision::CheckCircleToCircle(
					mvPosition,
					60.0f,
					Escape->GetPosition(),
					90.0f)
					)
				{

					if (!getEscapeItem && !getTimeItem)
					{
						// 近づいたら表示
						DrawStringToHandle(Utility::SCREEN_WIDTH / 2 - 100, 900, "R : 取得", GetColor(255, 255, 255), FontHandle);
					}

					// Rキーを押した場合
					if (CheckHitKey(KEY_INPUT_R))
					{
						getEscapeItem = true;

						EscapeItem* pEscape = dynamic_cast<EscapeItem*>(Escape);
						
						if (pEscape != nullptr)
						{

							// 脱出アイテムをインベントリに追加
							AddItem(pEscape->escape);

							pEscape->SetDeleteFlag(true);

							// ItemIDを1にする
							AddItemID = 1;

						}

					}

				}

		}

	}


	// 時間止めアイテムの情報を取得
	auto pobj = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DListByTag(Object3D::TagTimeItem);

	for (int i = 0; i < pobj.size(); i++)
	{
		auto StopTime = pobj[i];

		if (StopTime != nullptr)
		{
				if (Collision::CheckCircleToCircle(
					mvPosition,
					60.0f,
					StopTime->GetPosition(),
					90.0f)
					)
				{
					if (!getTimeItem && !getEscapeItem)
					{
						// 近づいたら表示
						DrawStringToHandle(Utility::SCREEN_WIDTH / 2 - 100, 900, "R : 取得", GetColor(255, 255, 255), FontHandle);
					}

					// もしRキーを押したら
					if (CheckHitKey(KEY_INPUT_R))
					{
						getTimeItem = true;


						TimeItem* pTimeItem = dynamic_cast<TimeItem*>(StopTime);

						if (pTimeItem != nullptr)
						{

							// 時間止めアイテムをインベントリに追加
							AddItem(pTimeItem->stoptime);

							pTimeItem->SetDeleteFlag(true);

							// ItemIDを2にする
							AddItemID = 2;


						}

					}
				}
			
		}
	}



}


void Player3D::UseItem()
{
	if (AddItemID == 1)
	{
		auto door = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DByTag(Object3D::TagExitdoor);
		if (door != nullptr)
		{
			Exitdoor* ExitDoor = dynamic_cast<Exitdoor*>(door);

			if (ExitDoor != nullptr)
			{
				// 当たり判定
				if (Collision::CheckCircleToCircle(
					mvPosition,
					100.0f,
					ExitDoor->GetPosition(),
					100.0f)
					)
				{
					// 脱出アイテムを持っていたら
					if (getEscapeItem)
					{
						DrawStringToHandle(Utility::SCREEN_WIDTH / 2 - 100, 900, "F : 使用", GetColor(255, 255, 255), FontHandle);
					}

					// Fキーを押した場合
					if (CheckHitKey(KEY_INPUT_F))
					{
						LoseItem();

						// 脱出アイテムを持っていない
						getEscapeItem = false;
					}




				}
			}

		}
	}

	// 時間止めアイテムを持っていたら
	if (getTimeItem)
	{
		DrawStringToHandle(Utility::SCREEN_WIDTH / 2 - 100, 900, "F : 使用", GetColor(255, 255, 255), FontHandle);
	}

	if (AddItemID == 2)
	{
		if (CheckHitKey(KEY_INPUT_F))
		{
			if (!isUseStopItem)
			{
				LoseItem();
			}

			// 時間止めアイテムを持っていない
			getTimeItem = false;
		}
	}

}

void Player3D::ItemBox()
{
	// アイテムインベントリにアイテムがある場合
	for (size_t i = 0; i < items.size(); i++)
	{
		x = HUD_X + (int)i * (ItemSize + ItemMargin);

		y = HUD_Y;

		// アイテムボックスの枠
		DrawBox(x, y, x + ItemSize, y + ItemSize, GetColor(200, 200, 200), false);


		// アイテム画像の表示
		DrawGraph(x + 50, y, items[i].image, true);

	}

	selX = HUD_X + currentItemIndex * (ItemSize + ItemMargin);

	selY = HUD_Y;

	// アイテムの現在選択している所を枠で囲む
	DrawBox(selX, selY, selX + ItemSize, selY + ItemSize, GetColor(0, 255, 0), false);

}


bool Player3D::AddItem(const ItemData item)
{
	// アイテムのサイズが最大数より少ない場合構造体に情報を追加する
	if (items.size() < maxSize)
	{
		items.push_back(item);

		return true;

	}

	// それ以外なら何もしない
	return false;
}





void Player3D::LoseItem()
{
	// アイテムのサイズが最大数より小さい場合何もしない
	if (items.size() < maxSize)
	{
		return;
	}


	for (size_t i = 0; i < items.size(); i++)
	{
		// アイテム番号が1の場合アイテムを脱出口で使用したときの処理を呼び出す
		if (items[i].id == 1)
		{
			EscapeItem::Escape();

		}

		// アイテム番号が 2 の場合のアイテムを使用したときの処理を呼び出す
		if (items[i].id == 2)
		{
			// アイテムを使用
			isUseStopItem = true;

		}

		// アイテムの情報を削除する
		items.clear();
		

	}

}

void Player3D::DrawStamina()
{
	int r = 20, g = 150, b = 20;

	if ((float)stamina <= 0.5f)
	{
		int noise = GetRand(30) - 20;
		r = max(0, min(255, r + noise));

		int shake = 2 + (int)((0.2f - (float)stamina) * 20);
		stamina_X += GetRand(shake * 2) - shake;
		stamina_Y += GetRand(shake * 2) - shake;

		float flicker = (float)sin(GetNowCount() * 0.02f);
		int alpha = 180 + (int)(75 * abs(flicker));

		SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
	}
	else
	{
		stamina_X = Utility::SCREEN_WIDTH / 2 - 260; // スタミナゲージのX座標
		stamina_Y = Utility::SCREEN_HEIGHT / 2 + 430; // スタミナゲージのY座標

		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	}

	// スタミナゲージの枠
	DrawBox(stamina_X, stamina_Y, stamina_X + width, stamina_Y + height, GetColor(0, 0, 0), true);

	// スタミナゲージ（緑）
	DrawBox(stamina_X, stamina_Y, stamina_X + gaugeWidth, stamina_Y + height, GetColor(r, g, b), true);

	// 枠線
	DrawBox(stamina_X, stamina_Y, stamina_X + width, stamina_Y + height, GetColor(255, 255, 255), false);

}


bool Player3D::StaminaUpdate(bool isKeyProsses)
{
	// 現在値に応じたスタミナゲージの幅
	gaugeWidth = (int)((float)stamina / staminaMAX * width);


	// 走りキーを押して移動しているならならスタミナゲージを減らす
	if (isKeyProsses && stamina > 0 && isMove)
	{
		// 走り状態にする
		state = RUN;
		// スタミナゲージを減らす
		stamina--;

		// 0以下なら歩き状態にする
		if (stamina <= 0)
		{
			state = WALK;
		}

		return true;
	}
	else
	{
		// 歩き状態にする
		state = WALK;

		// 現在のスタミナゲージが最大値より少ない場合スタミナを増やす
		if (stamina < staminaMAX)
		{
			stamina++;
		}
		return false;
	}

}


