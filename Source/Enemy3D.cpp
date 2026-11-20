#include "Enemy3D.h"
#include "Model.h"
#include "Master.h"
#include "ObjectManager.h"
#include "Scene.h"
#include "Stage.h"
#include "Player3D.h"
#include <vector>



Enemy3D::Enemy3D(std::string fileName, VECTOR initPosition, int enemyType, bool isSeparateAnimation)
	: Object3D(initPosition)
	, RotateSpeed(0.2f)
	, Speed(6.0f)
	, WaitFrame(60)
	, MaxPointCount(28)
	, m_oldPosition(VGet(0.0f, 0.0f, 0.0f))
	, m_angle(0.0f)
	, m_targetAngle(0.0f)
	, m_moveVec(VGet(0.0f, 0.0f, 0.0f))
	, m_playerPosition(VGet(0.0f, 0.0f, 0.0f))
	, m_targetX(0.0f)
	, m_targetY(0.0f)
	, m_targetZ(0.0f)
	, m_distanceX(0.0f)
	, m_distanceY(0.0f)
	, m_distanceZ(0.0f)
	, m_currentPoint(0)
	, m_waitTimer(0)
	, m_trackIndex(0)
	, m_scareLightHandle(-1)
	, m_foundPlayer(false)
	, m_isScare(false)
	, m_scareTimer(0)
	, m_isRun(false)
	, m_isWait(false)
	, m_isback(false)
	, m_isStopItem(false)
	, m_model(nullptr)
	, m_enemyType(-1)

{
	// タグ設定
	SetTag(Object3D::TagEnemy3D);

	// カメラモードをプレイヤーが操作できるように設定
	Master::m_camera->SetCameraMode(Camera::CameraMode::PlayerControl);

	// 敵のモデルによって目標地点を変更する
	SetEnemyType(enemyType);

	// ジャンプすケア用のライトを作成
	m_scareLightHandle = CreateDirLightHandle(VGet(0.0f, m_position.y, 0.0f));
	SetLightDifColorHandle(m_scareLightHandle , GetColorF(0.6f, 0.2f, 0.2f, 0.0f));
	SetLightAmbColorHandle(m_scareLightHandle, GetColorF(0.8f, 0.8f, 0.8f, 0.0f));
	SetLightEnableHandle(m_scareLightHandle, false);


	// Mixamo用処理
	// モデルの生成
	m_model = new Model(fileName, initPosition, isSeparateAnimation);
}

void Enemy3D::AddAnimation(AnimationState state, std::string fileName)
{
	m_model->AddAnimation(state, fileName);
}


Enemy3D::~Enemy3D()
{
	DeleteLightHandle(m_scareLightHandle);

	if (m_model != nullptr)
	{
		delete m_model;
	}
}

void Enemy3D::Update()
{

	// 移動処理
	Move();

	// 移動による回転処理
	if (!m_isScare)
	{
		RotationByMove();
	}

	// プレイヤーが敵と当たった時の処理
	CollScare();

	// モデルの更新
	if (m_model != nullptr)
	{
		m_model->SetPosition(m_position);
		m_model->Update();
	}

}

void Enemy3D::Draw()
{
	if (m_model != nullptr)
	{
		m_model->Draw();
	}
}



void Enemy3D::Move()
{
	if (!CanMove()) return;  // スケア中・時間停止中なら動かない

	// 目標座標の設定 ステージの横3680.0f高さ400.0f 階段-180.0f z2590.0f h:1027, h(中間):1224
	Point waypointsRouteA[] = { {0.0f, 44.0f, 1700.0f},                                                                                          // �������W�ɍs��
					   {3680.0f, 44.0f, 1700.0f }, {3680.0f, 241.0f, 2590.0f},  {3980.0f, 241.0f, 2590.0f},  {3980.0f, 438.0f, 1700.0f},  // �E�K�i1��2
					   {-3680.0f, 438.0f, 1700.0f},{-3680.0f, 634.0f, 2590.0f}, {-3980.0f, 634.0f, 2590.0f}, {-3980.0f, 831.0f, 1700.0f}, // ���K�i2��3
					   {0.0f, 831.0f, 1700.0f},    {-100.0f, 831.0, -1600.0f},  {-100.0f, 831.0f, 1700.0f},  {-100.0f, 1027.0f, 2590.0f}, // �����K�i3��4
					   {150.0f, 1027.0f, 2590.0f}, {150.0f, 1224.0f, 1800.0f},  {3960.0f, 1224.0f, 1800.0f},                              // 4�K�̈ړ�
					   {3960.0f, 1027.0f, 2580.0f},{3680.0f, 1027.0f, 2580.0f}, {3680.0f, 831.0f, 1700.0f}, {180.0f, 831.0f, 1700.0f},    // 3�K�̒����K�i�ֈړ�
					   {180.0f, 634.0f, 2590.0f},  {-180.0f, 634.0f, 2590.0f }, {-180.0f, 438.0f, 1700.0f},                               // �����K�i3��2
					   {-3960.0f, 438.0f, 1700.0f},{-3960.0f, 241.0f, 2590.0f}, {-3680.0f, 241.0f, 2590.0f},{-3680.0f, 44.0f, 1700.0f},   // ���K�i2��1
					   {100.0f, 44.0f, 1700.0f} };                                                                                        // �������W�ɖ߂�


	Point waypointsRouteB[] = { {0.0f, 44.0f, -1700.0f},                                                                                               // �������W�ɍs��
						  {-3680.0f, 44.0f, -1700.0f},  {-3680.0f, 241.0f, -2500.0f}, {-3980.0f, 241.0f, -2500.0f}, {-3980.0f, 438.0f, -1700.0f}, // ���K�i1��2
						  {3680.0f, 438.0f, -1700.0f},  {3680.0f, 634.0f, -2500.0f},  {3980.0f, 634.0f, -2500.0f},  {3980.0f, 831.0f, -1700.0f},  // �E�K�i2��3
						  {0.0f, 831.0f, -1700.0f},     {100.0f, 831.0f, 1600.0f},    {100.0f, 831.0f, -1700.0f},   {100.0f, 1027.0f, -2500.0f},  // �����K�i3��4
						  {-150.0f, 1027.0f, -2500.0f}, {-150.0f, 1224.0f, -1500.0f}, {-3960.0f, 1224.0f, -1500.0f},                              // 4�K�̈ړ�
						  {-3960.0f, 1027.0f, -2500.0f},{-3680.0f, 1027.0f, -2500.0f},{-3680.0f, 831.0f, -1700.0f}, {3960.0f, 831.0f, -1700.0f},  // 3�K�̉E�K�i�ֈړ�
						  {3960.0f, 634.0f, -2500.0f},  {3680.0f, 634.0f, -2500.0f},  {3680.0f, 438.0f, -1700.0f},                                // �E�K�i3��2
						  {-3960.0f, 438.0f, -1700.0f}, {-3960.0f, 241.0f, -2500.0f,},{-3680.0f, 241.0f, -2500.0f}, {-3680.0f, 44.0f, -1700.0f},  // ���K�i2��1
						  {100.0f, 44.0f, -1700.0f} };                                                                                            // �������W�ɖ߂�





			// プレイヤーを見つけていない場合(目標座標に向かう処理)
			if (!m_foundPlayer)
			{
				// 走っていない
				m_isRun = false;

				// 待機状態の場合
				if (m_isWait)
				{
					// 待機時間増加
					m_waitTimer++;

					// 待機時間が待機するフレーム数以上の場合
					if (m_waitTimer >= WaitFrame)
					{
						// 待機時間終了
						m_isWait = false;

						// 待機時間の初期化
						m_waitTimer = 0;
					}

				}
				else
				{
					// 敵のモデルによって目標地点を変更する
					switch (m_enemyType)
					{
					case 1:
						// 目標座標の変数に設定
						m_targetX = waypointsRouteA[m_currentPoint].pointX;
						m_targetY = waypointsRouteA[m_currentPoint].pointY;
						m_targetZ = waypointsRouteA[m_currentPoint].pointZ;

						// 目標座標と敵の座標の距離を求める
						m_distanceX = m_targetX - m_position.x;
						m_distanceY = m_targetY - m_position.y;
						m_distanceZ = m_targetZ - m_position.z;

						break;

					case 2:
						// 目標座標の変数に設定
						m_targetX = waypointsRouteB[m_currentPoint].pointX;
						m_targetY = waypointsRouteB[m_currentPoint].pointY;
						m_targetZ = waypointsRouteB[m_currentPoint].pointZ;

						// 目標座標と敵の座標の距離を求める
						m_distanceX = m_targetX - m_position.x;
						m_distanceY = m_targetY - m_position.y;
						m_distanceZ = m_targetZ - m_position.z;

						break;
					}

					// 距離を計算
					float fDistanceToTarget = sqrtf(m_distanceX * m_distanceX + m_distanceY * m_distanceY + m_distanceZ * m_distanceZ);


					// 距離が速さより大きいなら
					if (fDistanceToTarget > Speed)
					{
						//求めた距離を設定
						VECTOR d = VGet(m_distanceX, m_distanceY, m_distanceZ);
						// 目標座標を正規化
						d = VNorm(d);

						// 移動ベクトルに目標座標を加えて移動させる
						m_moveVec = VAdd(m_moveVec, d);
						m_position = VAdd(m_position, VScale(m_moveVec, Speed));

					}
					else
					{
						// 目標座標をモデルに設定
						m_position.x = m_targetX;

						m_position.y = m_targetY;

						m_position.z = m_targetZ;

						// 移動方向を初期化
						m_moveVec = VGet(0.0f, 0.0f, 0.0f);

						// 階段の上り下りの際には待機状態を無くす
						if (m_currentPoint == 2 || m_currentPoint == 3 || m_currentPoint == 6 || m_currentPoint == 7
							|| m_currentPoint == 12 || m_currentPoint == 13 || m_currentPoint == 16 || m_currentPoint == 17
							|| m_currentPoint == 20 || m_currentPoint == 21 || m_currentPoint == 24 || m_currentPoint == 25
							)
						{
							m_isWait = false;
						}
						else
						{
							m_isWait = true;
						}

						// 目標地点の最後まで行ったら通ってきた座標を順番に戻るようにする
						if (m_position.x == waypointsRouteA[27].pointX && m_position.y == waypointsRouteA[27].pointY && m_position.z == waypointsRouteA[27].pointZ)
						{
							m_isback = true;
						}
						if (m_position.x == waypointsRouteB[27].pointX && m_position.y == waypointsRouteB[27].pointY && m_position.z == waypointsRouteB[27].pointZ)
						{
							m_isback = true;
						}

						// 目標地点を全部通るまで設定した目標地点を通っていく
						if (!m_isback)
						{
							m_currentPoint = (m_currentPoint + 1) % MaxPointCount;
						}
						else  // 目標地点を全部通ったら通ってきた地点を戻る
						{
							// 現在の目標座標を示す変数を減らしていく
							m_currentPoint--;

							// 最大配列数が0になったら
							if (m_currentPoint <= 0)
							{
								m_isback = false;
								m_currentPoint = 0;
							}


						}

					}

				}


			}
			else   // プレイヤーを見つけた場合(追尾処理～プレイヤーの座標を通る～)
			{
				// 走る
				m_isRun = true;

				// ========== 追加実装：敵がプレイヤーに近づいたらステージを避けながら追尾する処理 ==========
				// プレイヤーとの距離が一定距離以下（例：800.0f 以下）なら直接追尾を試みる
				// 距離が遠い場合は、プレイヤーの足跡（迂回ルート）をたどる
				bool isDistanceChase = (fDistanceToPlayer <= 800.0f);

				// 直接追尾を行う場合
				if (isDistanceChase)
				{
					// プレイヤーの方向へ向かうベクトルを計算
					VECTOR toPlayer = VSub(m_playerPosition, m_position);
						
					// 高さを無視して平面で追尾するように Y を 0 にする
					toPlayer.y = 0.0f;
						
					// ゼロ除算を避けるためのチェック
					if (VSize(toPlayer) > 0.001f)
					{
						// ベクトルを正規化（長さを1にする）して方向だけにする
						toPlayer = VNorm(toPlayer);

						// 敵がプレイヤーの方向を向いて向かってくるように設定
						m_moveVec = VAdd(VScale(m_moveVec, 0.5f), VScale(toPlayer, 0.5f));

						// 敵をプレイヤーの方向に移動させる（速度は走るスピード）
						m_position = VAdd(m_position, VScale(m_moveVec, Speed * 1.8f));
					}

				}
				else
				{
					// プレイヤーの足跡リストを取得
					const std::vector<VECTOR>& playerRecord = pPlayer->GetFootprintsRecord();


				    // 遠い場合などは、ステージを避けるためにプレイヤーの足跡をたどる
				    // 足跡リストが空の場合
					if (m_trackIndex >= playerRecord.size())
					{
						// 追尾すべき足跡がないので終了
						m_trackIndex = 0;
						return;
					}
				    
				    // 一番古い足跡（向かうべきターゲット）を取得
				    VECTOR playerRecordPosition = playerRecord[m_trackIndex];
				    
				    // ターゲットと敵の距離を計算
				    VECTOR vec = VSub(playerRecordPosition, m_position);
				    	vec.y = 0.0f; // 高さは無視
				    
				    // ベクトルのサイズ（距離）を取得
				    float fDistanceToPlayerRecord = VSize(vec);

				    // ターゲットとの距離が一定以下（到着した）なら
				    if (fDistanceToPlayerRecord <= 80.0f)
				    {
				    	// 通過した足跡をリストから削除し、次の足跡に向かうようにする
						m_trackIndex++;
				    }
				    else
				    {
				    	// ゼロ除算を避けるためのチェック
				    	if (fDistanceToPlayerRecord > 0.001f)
				    	{
				    		// 方向ベクトルを正規化
				    		vec = VNorm(vec);

							m_moveVec = vec;
				    
				    		// 敵を足跡の方向に移動させる
				    		m_position = VAdd(m_position, VScale(m_moveVec, Speed * 1.8f));
				    	}
					}

				}

			



			}



		// 移動している状態であれば
		bool isMove = (m_moveVec.x != 0.0f || m_moveVec.z != 0.0f);
		if (isMove)
		{
			// 移動方向を正規化しておく
			m_moveVec = VNorm(m_moveVec);

			// 新しい回転をセット
			m_targetAngle = atan2f(m_moveVec.x, m_moveVec.z);
		}


		// 移動している状態であれば
		if (isMove)
		{
			if (m_isRun)
			{
				m_model->ChangeAnimation(AnimationRun);
			}
			else
			{
				m_model->ChangeAnimation(AnimationWalking);
			}
		}
		else
		{
			m_model->ChangeAnimation(AnimationNeutral);
		}


}

bool Enemy3D::CanMove()
{
	if (m_isScare){
		return false;
	}
	else if (m_isStopItem) {
		m_model->ChangeAnimation(AnimationNeutral);
		return false;
	}
	else {
		return true;
	}
}

void Enemy3D::UpdatePlayerSearch()
{
	m_oldPosition = m_position;  // 前回の座標をいったん保持

	Object3D* pPlayerObj = Master::m_sceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DByTag(Object3D::TagPlayer3D); 	// プレイヤーの情報を取得
	if (pPlayerObj == nullptr) return;
	Player3D* pPlayer = dynamic_cast<Player3D*>(pPlayerObj);
	if (pPlayer == nullptr) return;

	m_playerPosition = pPlayer->GetPosition();
	float fDistanceToPlayer = VSize(VSub(m_playerPosition, m_position));
	if (fDistanceToPlayer <= 80.0f)                                                 // プレイヤーが敵に近づいたらゲームオーバー処理を行う
	{															                    
		pPlayer->SetFreeze(true);                                                   
		m_isRun = false;                      	                                    
		m_isScare = true;             	                                             
	}

	float visionAngle = 45.0f;                                                      // 確認する角度
	float sight = visionAngle * DX_PI_F / 180.0f;                                   // プレイヤーを確認する範囲
	float heightDistance = (m_playerPosition.y + 80.0f) - (m_position.y + 80.0f); 	// プレイヤーと敵の高さの差を計算する

	static float smoothHeight = 0.0f;                                               // スパイク防止のためのスムージング処理
	smoothHeight = (smoothHeight * 0.8f) + (heightDistance * 0.2f);
	if (fabs(smoothHeight) > 150.0f) m_foundPlayer = false;         	           // 高さの差が一定距離より大きいならプレイヤーを見つけていない

	VECTOR forward = VGet(sinf(m_angle), 0.0f, cosf(m_angle));              	  // 敵の前方向を設定
	VECTOR vDistanceToPlayer = VNorm(VSub(m_playerPosition, m_position));     	  // 距離を正規化
	float dot = VDot(forward, vDistanceToPlayer);                                 // 視点と距離から内積を求める
	float foundDistance = 1500.0f;            	                                  // プレイヤーを見つける距離

	if (dot >= sight && fDistanceToPlayer <= foundDistance) {                 	  
		m_foundPlayer = true;		// 敵の視点からプレイヤーが見えた場合かつ一定の距離以下ならプレイヤーを見つけた
	} else {
		m_foundPlayer = false;
	}
}

void Enemy3D::RotationByMove()
{
	// 現在の回転値から目標の回転値の差分を求める
	float subAngle = m_targetAngle - m_angle;

	// ある方向からある方向の差が180度以上（以下）になることがないはずなので、
	// 差の値が180度以上（以下）になっていたら矯正する
	if (subAngle < -DX_PI_F)
	{
		subAngle += DX_TWO_PI_F;
	}
	else if (subAngle > DX_PI_F)
	{
		subAngle -= DX_TWO_PI_F;
	}

	// 角度の差分を徐々に 0 に近づける
	if (subAngle > 0.0f)
	{
		subAngle -= RotateSpeed;

		if (subAngle < 0.0f)
		{
			subAngle = 0.0f;
		}
	}
	else if (subAngle < 0.0f)
	{
		subAngle += RotateSpeed;

		if (subAngle > 0.0f)
		{
			subAngle = 0.0f;
		}
	}

	// 今向いてほしい角度を算出
	m_angle = m_targetAngle - subAngle;

	// 回転値を設定
	m_rotation.y = m_angle + DX_PI_F;

	// モデルに伝える
	m_model->SetRotation(m_rotation);

}


void Enemy3D::CollScare()
{
	Object3D* pObj = Master::m_sceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DByTag(Object3D::TagPlayer3D);
	if (pObj == nullptr)
	{
		return;
	}

	Player3D* pPlayer = dynamic_cast<Player3D*>(pObj);
	if (pPlayer == nullptr)
	{
		return;
	}

	// ジャンプスケアなら
	if (m_isScare)
	{
		// カメラの注視点を敵の頭の位置に設定する
		Master::m_camera->SetTargetPosition(m_model->GetModelFramePosition("mixamorig:Head"));

		// カメラの視点を自動で敵の方に向けるようにする
		Master::m_camera->SetCameraMode(Camera::CameraMode::AutoLook);

		// ジャンプスケア中は移動ベクトルをゼロにして敵を停止させる
		m_moveVec = VGet(0.0f, 0.0f, 0.0f);

		// ジャンプスケア中は敵のアニメーションをIdle（待機）状態にする
		m_model->ChangeAnimation(AnimationNeutral);

		// ジャンプスケア用のライトを点灯させる
		SetLightEnableHandle(m_scareLightHandle, true);

		// ジャンプスケアの時間を進める
		m_scareTimer++;

		// SE再生
		Master::m_soundManager->PlaySE(SoundManager::SeJumpScare);

		//// カメラの座標を取得
		VECTOR camPos = Master::m_camera->GetPosition();		

		// カメラと敵の位置から距離を計算
		VECTOR Enemydir = VSub(camPos, m_position);

		// 正規化する
		Enemydir = VNorm(Enemydir);

		// 敵のモデルが向く方向を計算
		float angle = atan2f(Enemydir.x, Enemydir.z);

		// 回転値を設定
		m_rotation.y = angle + DX_PI_F;

		// モデルに伝える
		m_model->SetRotation(m_rotation);

		Master::m_camera->SetUpShake((float)m_scareTimer, (float)Utility::SCREEN_WIDTH / 500, 15.0f);

		// ジャンプスケアが一定時間たった場合
		if (m_scareTimer > 100)
		{
			// プレイヤーの移動処理を再開する
			pPlayer->SetFreeze(false);

			// リザルトの種類をゲームオーバーに設定
			Master::m_sceneManager->SetResultType(SceneManager::ResultType::ResultGameOver);

			// リザルトシーンを呼び出す
			Master::m_sceneManager->SetNextScene(SceneManager::SceneResult);

			// SEを止める
			Master::m_soundManager->StopSE();

			// 作成したジャンプすケア用のライトを消す
			SetLightEnableHandle(m_scareLightHandle, false);

			// 終了
			m_isScare = false;

			// タイマーを初期化
			m_scareTimer = 0;

		}


	}



	

}


