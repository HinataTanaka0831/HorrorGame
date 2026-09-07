#include "Enemy3D.h"
#include "Model.h"
#include "Master.h"
#include "ObjectManager.h"
#include "Scene.h"
#include "Stage.h"
#include "Collision.h"
#include "Player3D.h"


Enemy3D::Enemy3D(std::string fileName, VECTOR initPosition, int enemyType, bool isSeparateAnimation)
	:Object3D(initPosition)
{
	// タグ設定
	SetTag(Object3D::TagEnemy3D);

	// 敵のモデルによって目標地点を変更する
	SetEnemyType(enemyType);

	m_scareLight = CreateDirLightHandle(VGet(0.0f, m_position.y, 0.0f));
	SetLightDifColorHandle(m_scareLight, GetColorF(0.6f, 0.2f, 0.2f, 0.0f));
	SetLightAmbColorHandle(m_scareLight, GetColorF(0.8f, 0.8f, 0.8f, 0.0f));
	SetLightEnableHandle(m_scareLight, false);

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
	DeleteLightHandle(m_scareLight);

	if (m_model != nullptr)
	{
		delete m_model;
	}
}

void Enemy3D::Update()
{

	// 移動処理
	Move();

	// プレイヤーが敵と当たった時の処理
	CollScare();

	if (!m_isScare)
	{
		// 移動による回転処理
		RotationByMove();
	}

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

	// デバック表示用
	//DrawCapsule3D(mvPosition, VAdd(mvPosition, VGet(0.0f, 150.0f, 0.0f)), 40.0f, 8, GetColor(255, 255, 255), GetColor(255, 255, 255), false);

}



void Enemy3D::Move()
{
	// ジャンプスケア中は移動処理を全てスキップする
	if (m_isScare)
	{
		return;
	}

	if (m_isStopItem)
	{
		m_model->ChangeAnimation(AnimationNeutral);
		return;
	}

	// 現在再生中のアニメーションを取得
	AnimationState state = m_model->GetNowState();
	if (state == AnimationAttack || state == AnimationJumpOut)
	{
		return; // 特定のモーション中であれば何もしない
	}

	// 目標座標の設定 ステージの横3680.0f高さ400.0f 階段-180.0f z2590.0f h:1027, h(中間):1224
	Point Weipoint[] = { {0.0f, 44.0f, 1700.0f},                                                                                          // 初期座標に行く
					   {3680.0f, 44.0f, 1700.0f }, {3680.0f, 241.0f, 2590.0f},  {3980.0f, 241.0f, 2590.0f},  {3980.0f, 438.0f, 1700.0f},  // 右階段1→2
					   {-3680.0f, 438.0f, 1700.0f},{-3680.0f, 634.0f, 2590.0f}, {-3980.0f, 634.0f, 2590.0f}, {-3980.0f, 831.0f, 1700.0f}, // 左階段2→3
					   {0.0f, 831.0f, 1700.0f},    {-100.0f, 831.0, -1600.0f},  {-100.0f, 831.0f, 1700.0f},  {-100.0f, 1027.0f, 2590.0f}, // 中央階段3→4
					   {150.0f, 1027.0f, 2590.0f}, {150.0f, 1224.0f, 1800.0f},  {3960.0f, 1224.0f, 1800.0f},                              // 4階の移動
					   {3960.0f, 1027.0f, 2580.0f},{3680.0f, 1027.0f, 2580.0f}, {3680.0f, 831.0f, 1700.0f}, {180.0f, 831.0f, 1700.0f},    // 3階の中央階段へ移動
					   {180.0f, 634.0f, 2590.0f},  {-180.0f, 634.0f, 2590.0f }, {-180.0f, 438.0f, 1700.0f},                               // 中央階段3→2
					   {-3960.0f, 438.0f, 1700.0f},{-3960.0f, 241.0f, 2590.0f}, {-3680.0f, 241.0f, 2590.0f},{-3680.0f, 44.0f, 1700.0f},   // 左階段2→1
					   {100.0f, 44.0f, 1700.0f} };                                                                                        // 初期座標に戻る


	Point WeipointNex[] = { {0.0f, 44.0f, -1700.0f},                                                                                               // 初期座標に行く
						  {-3680.0f, 44.0f, -1700.0f},  {-3680.0f, 241.0f, -2500.0f}, {-3980.0f, 241.0f, -2500.0f}, {-3980.0f, 438.0f, -1700.0f}, // 左階段1→2
						  {3680.0f, 438.0f, -1700.0f},  {3680.0f, 634.0f, -2500.0f},  {3980.0f, 634.0f, -2500.0f},  {3980.0f, 831.0f, -1700.0f},  // 右階段2→3
						  {0.0f, 831.0f, -1700.0f},     {100.0f, 831.0f, 1600.0f},    {100.0f, 831.0f, -1700.0f},   {100.0f, 1027.0f, -2500.0f},  // 中央階段3→4
						  {-150.0f, 1027.0f, -2500.0f}, {-150.0f, 1224.0f, -1500.0f}, {-3960.0f, 1224.0f, -1500.0f},                              // 4階の移動
						  {-3960.0f, 1027.0f, -2500.0f},{-3680.0f, 1027.0f, -2500.0f},{-3680.0f, 831.0f, -1700.0f}, {3960.0f, 831.0f, -1700.0f},  // 3階の右階段へ移動
						  {3960.0f, 634.0f, -2500.0f},  {3680.0f, 634.0f, -2500.0f},  {3680.0f, 438.0f, -1700.0f},                                // 右階段3→2
						  {-3960.0f, 438.0f, -1700.0f}, {-3960.0f, 241.0f, -2500.0f,},{-3680.0f, 241.0f, -2500.0f}, {-3680.0f, 44.0f, -1700.0f},  // 左階段2→1
						  {100.0f, 44.0f, -1700.0f} };                                                                                            // 初期座標に戻る


	m_oldPosition = m_position;   // 前回の座標をいったん保持


		// プレイヤーの情報を取得
		Object3D* Player = Master::m_sceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DByTag(Object3D::TagPlayer3D);

		if (Player != nullptr)
		{
			// プレイヤーの座標を取得
			m_playerPos = Player->GetPosition();

			// プレイヤーと敵の座標の距離を計算
			VECTOR distance = VSub(m_playerPos, m_position);

			// ベクトルのサイズを取得
			float dis = VSize(distance);


			// プレイヤーが敵に近づいたらゲームオーバー処理を行う
			if (dis <= 60.0f)
			{
				Player3D* player = dynamic_cast<Player3D*>(Player);
				if (player != nullptr)
				{
					// プレイヤーの移動処理を止める
					player->SetFreeze(true);
					// カメラの移動を止める
					Master::m_camera->SetFreeze(true);
					m_isRun = false;
					// ジャンプスケアを発生させる
					m_isScare = true;
				}

			}


			// 確認する角度
			float visionAngle = 45.0f;

			// プレイヤーを確認する範囲
			float sight = visionAngle * DX_PI_F / 180.0f;

			// 見つける距離
			float founddis = 1500.0f;

			// プレイヤーと敵の高さの差を計算する
			float heightDis = (m_playerPos.y + 80.0f) - (m_position.y + 80.0f);

			// スパイク防止のためのスムージング処理
			static float smoothHeight = 0.0f;
			smoothHeight = (smoothHeight * 0.8f) + (heightDis * 0.2f);

			// 高さの差が一定距離より大きいならプレイヤーを見つけていない
			if (fabs(smoothHeight) > 150.0f)
			{
				m_foundPlayer = false;
			}


			// 敵の視点を設定
			VECTOR forward = VGet(sinf(m_angle), 0.0f, cosf(m_angle));

			// 距離を正規化
			distance = VNorm(distance);

			// 視点と距離から内積を求める
			float dot = VDot(forward, distance);


			// 敵の視点からプレイヤーが見えた場合かつ一定の距離以下なら
			if (dot >= sight && dis <= founddis)
			{
				// プレイヤーを見つけた
				m_foundPlayer = true;
			}
			else  // 一定の距離を離れたら目標座標に行くようにする
			{
				m_foundPlayer = false;
			}



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
					if (m_waitTimer >= WaitFram)
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
						m_targetX = Weipoint[m_currentPoint].pointX;
						m_targetY = Weipoint[m_currentPoint].pointY;
						m_targetZ = Weipoint[m_currentPoint].pointZ;

						// 目標座標と敵の座標の距離を求める
						m_distanceX = m_targetX - m_position.x;
						m_distanceY = m_targetY - m_position.y;
						m_distanceZ = m_targetZ - m_position.z;

						break;

					case 2:
						// 目標座標の変数に設定
						m_targetX = WeipointNex[m_currentPoint].pointX;
						m_targetY = WeipointNex[m_currentPoint].pointY;
						m_targetZ = WeipointNex[m_currentPoint].pointZ;

						// 目標座標と敵の座標の距離を求める
						m_distanceX = m_targetX - m_position.x;
						m_distanceY = m_targetY - m_position.y;
						m_distanceZ = m_targetZ - m_position.z;

						break;
					}

					// 距離を計算
					float distance = sqrtf(m_distanceX * m_distanceX + m_distanceY * m_distanceY + m_distanceZ * m_distanceZ);


					// 距離が速さより大きいなら
					if (distance > Speed)
					{
						//求めた距離を設定
						VECTOR d = VGet(m_distanceX, m_distanceY, m_distanceZ);
						// 目標座標を正規化
						d = VNorm(d);

						// 移動ベクトルに加える
						m_moveVec = VAdd(m_moveVec, d);

						// 移動させる
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
						if (m_position.x == Weipoint[27].pointX && m_position.y == Weipoint[27].pointY && m_position.z == Weipoint[27].pointZ)
						{
							m_isback = true;
						}
						if (m_position.x == WeipointNex[27].pointX && m_position.y == WeipointNex[27].pointY && m_position.z == WeipointNex[27].pointZ)
						{
							m_isback = true;
						}

						// 目標地点を全部通るまで設定した目標地点を通っていく
						if (!m_isback)
						{
							m_currentPoint = (m_currentPoint + 1) % m_pointcount;
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

					Player3D* getPlayer = dynamic_cast<Player3D*>(Player);

					if (getPlayer != nullptr)
					{
						// ========== 追加実装：敵がプレイヤーに近づいたらステージを避けながら追尾する処理 ==========
						// プレイヤーとの距離が一定距離以下（例：800.0f 以下）なら直接追尾を試みる
						// 距離が遠い場合は、プレイヤーの足跡（迂回ルート）をたどる
						bool isDirectChase = (dis <= 800.0f);

						// プレイヤーの足跡リストを取得
						std::vector<VECTOR>& playerRecord = getPlayer->GetPlayerRecord();

						// 直接追尾を行う場合
						if (isDirectChase)
						{
							// プレイヤーの方向へ向かうベクトルを計算
							VECTOR toPlayer = VSub(m_playerPos, m_position);
							
							// 高さを無視して平面で追尾するように Y を 0 にする
							toPlayer.y = 0.0f; 
							
							// ゼロ除算を避けるためのチェック
							if (VSize(toPlayer) > 0.001f)
							{
								// ベクトルを正規化（長さを1にする）して方向だけにする
								toPlayer = VNorm(toPlayer);

								// 慣性を弱めて、プレイヤーの方向に素早く向き直れるようにする
								m_moveVec = VAdd(VScale(m_moveVec, 0.5f), VScale(toPlayer, 0.5f));
								m_moveVec = VNorm(m_moveVec); // 常の長さを1に保つことで、移動速度が爆発するのを防ぐ

								// 敵をプレイヤーの方向に移動させる（速度は走るスピード）
								m_position = VAdd(m_position, VScale(m_moveVec, Speed * 1.8f));
							}

							// ※ステージの壁を避ける（滑り抜ける）処理自体は、
							// Move()関数の後半にある「全ヒット情報で押し出すロジック」で自動的に行われます。
							
							// プレイヤーに直接追尾できている間は、通過したとみなして足跡を古いものから少し消しておく
							if (!playerRecord.empty())
							{
								playerRecord.erase(playerRecord.begin());
							}
						}
						else
						{
							// 遠い場合などは、ステージを避けるためにプレイヤーの足跡をたどる
							// 足跡リストが空の場合
							if (playerRecord.empty())
							{
								// 追尾すべき足跡がないので終了
								return;
							}

							// 一番古い足跡（向かうべきターゲット）を取得
							VECTOR TargetPos = playerRecord.front();

							// ターゲットと敵の距離を計算
							VECTOR vec = VSub(TargetPos, m_position);
							vec.y = 0.0f; // 高さは無視

							// ベクトルのサイズ（距離）を取得
							float Dis = VSize(vec);

							// ターゲットとの距離が一定以下（到着した）なら
							if (Dis <= 30.0f)
							{
								// 足跡リストが空でない場合
								if (!playerRecord.empty())
								{
									// 通過した足跡をリストから削除し、次の足跡に向かうようにする
									playerRecord.erase(playerRecord.begin());
								}
							}
							else
							{
								// ゼロ除算を避けるためのチェック
								if (Dis > 0.001f)
								{
									// 方向ベクトルを正規化
									vec = VNorm(vec);

									// 慣性を弱める
									m_moveVec = VAdd(VScale(m_moveVec, 0.5f), VScale(vec, 0.5f));
									m_moveVec = VNorm(m_moveVec);

									// 敵を足跡の方向に移動させる
									m_position = VAdd(m_position, VScale(m_moveVec, Speed * 1.8f));
								}
							}


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



		// ステージとの当たり判定をする
		VECTOR hitPos = VGet(0.0f, 0.0f, 0.0f);
		bool isHit = false;
		int count = 0;
		auto obj = Master::m_sceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DByTag(Object3D::TagStage);
		if (obj != nullptr)
		{
			Stage* pStage = dynamic_cast<Stage*>(obj);
			if (pStage != nullptr)
			{
				// ステージとプレイヤーのカプセルが当たっている場合 (ステージの地面とプレイヤーとの当たり判定処理)
				if (pStage->CheckHit_Capsule(m_position, VAdd(m_position, VGet(0.0f, 70.0f, 0.0f)), 40.0f))
				{
					// 当たっているであろうポリゴンとの接触点を求める
					hitPos = pStage->CheckHit_Line(
						VAdd(m_position, VGet(0.0f, 50.0f, 0.0f)),  // プレイヤーの膝辺り（多分）と
						VAdd(m_position, VGet(0.0f, -50.0f, 0.0f))  // プレイヤーの少し下あたりを線分として指定
					);


					// 当たった判定を取っておく
					isHit = true;

				}


				// -------------------------------------------------
				// ★ 敵用：全ヒット情報で押し出すロジック ★
				// -------------------------------------------------
					VECTOR capBottom = VAdd(m_position, VGet(0.0f, 70.0f, 0.0f));
				    VECTOR capTop = VAdd(m_position, VGet(0.0f, 90.0f, 0.0f));
				
				if (pStage->CheckHit_Capsule(capBottom, capTop, 50.0f))
				{
				       // ① 現在の壁の法線で勢いを打ち消す
					   VECTOR stageNormal = VNorm(pStage->GetNormal());
				       float   b = VDot(m_moveVec, stageNormal);
				      if (b < 0.0f)
				      {
				          VECTOR pushBack = VScale(stageNormal, -b);
				          m_moveVec = VAdd(m_moveVec, pushBack);
				          m_position = VAdd(m_oldPosition, VScale(m_moveVec, Speed));
				          capBottom = VAdd(m_position, VGet(0.0f, 70.0f, 0.0f));
				          capTop = VAdd(m_position, VGet(0.0f, 90.0f, 0.0f));
				      }
				     
				     // ② MV1 の衝突関数で全ヒット情報取得し、合成法線で押し出す
				     const int SLIDE_MAX_ITERATION = 100;
				      for (int i = 0; i < SLIDE_MAX_ITERATION; ++i)
				      {
				             MV1_COLL_RESULT_POLY_DIM result =
				         	MV1CollCheck_Capsule(pStage->GetCollisionHandle(),
				         	-1,
				         	capBottom,
				         	capTop,
				         	50.0f);
				          if (result.HitNum == 0) break;
				          
				         
				           VECTOR summed = VGet(0.0f, 0.0f, 0.0f);
				          for (int h = 0; h < result.HitNum; ++h)
				          {
				          	summed = VAdd(summed, result.Dim[h].Normal);
				          }

				          VECTOR pushDir = VNorm(summed);
				          
				          	m_position = VAdd(m_position, VScale(pushDir, 1.0f));
				            capBottom = VAdd(capBottom, VScale(pushDir, 1.0f));
				            capTop = VAdd(capTop, VScale(pushDir, 1.0f));
				      }
				}    
			}


			if (isHit)
			{
				// 地面に沿って歩いている状態として、Y座標をステージに合わせる
				m_position.y = hitPos.y;

			}


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
	Object3D* pobj = Master::m_sceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DByTag(Object3D::TagPlayer3D);
	if (pobj != nullptr)
	{
		// ジャンプスケアなら
		if (m_isScare)
		{
			// ジャンプスケア中は移動ベクトルをゼロにして敵を停止させる
			m_moveVec = VGet(0.0f, 0.0f, 0.0f);

			// ジャンプスケア中は敵のアニメーションをIdle（待機）状態にする
			m_model->ChangeAnimation(AnimationNeutral);

			SetLightEnableHandle(m_scareLight, true);

			// ジャンプスケアの時間を進める
			m_scareTimer++;

			// SE再生
			Master::m_soundManager->PlayBGM(SoundManager::BGMGame);

			// カメラの座標を取得
			VECTOR camPos = Master::m_camera->GetPosition();

			// カメラの注視点を取得
			VECTOR camLook = Master::m_camera->GetLookAtPosition();

			// ワールド座標の上方向
			VECTOR worldUp = VGet(0.0f, 1.0f, 0.0f);

			// カメラが向いている方向を計算
			VECTOR camDir = VNorm(VSub(camLook, camPos));

			// カメラの右方向
			VECTOR camRight = VNorm(VCross(worldUp, camDir));

			// カメラの上方向
			VECTOR camUp = VNorm(VCross(camDir, camRight));

			// 敵のモデルを設定する位置を計算
			m_currentPos = camPos;
			m_currentPos = VAdd(m_currentPos, VScale(camDir, 55.0f));
			m_currentPos = VAdd(m_currentPos, VScale(camRight, 5.0f));
			m_currentPos = VAdd(m_currentPos, VScale(camUp, -185.0f));

			// 敵のモデルの位置を設定する
			m_position = m_currentPos;
			


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
				// ゲームオーバーシーンを呼び出す
				Master::m_sceneManager->SetNextScene(SceneManager::SceneGameOver);

				// BGMを止める
				Master::m_soundManager->StopBGM();

				SetLightEnableHandle(m_scareLight, false);

				// 終了
				m_isScare = false;

				// タイマーを初期化
				m_scareTimer = 0;

			}


		}



	}

}


