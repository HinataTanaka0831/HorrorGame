#include "Enemy3D.h"
#include "Model.h"
#include "Master.h"
#include "ObjectManager.h"
#include "Scene.h"
#include "Stage.h"
#include "Player3D.h"



Enemy3D::Enemy3D(const std::string& fileName, VECTOR initPosition, int enemyType, bool isSeparateAnimation)
	: Object3D(initPosition)
	, RotateSpeed(0.2f)
	, WaitFrame(60)
	, MaxPointCount(28)
	, m_oldPosition(VGet(0.0f, 0.0f, 0.0f))
	, m_angle(0.0f)
	, m_targetAngle(0.0f)
	, m_speed(6.0f)
	, m_moveVec(VGet(0.0f, 0.0f, 0.0f))
	, m_playerPosition(VGet(0.0f, 0.0f, 0.0f))
	, toPlayer(VGet(0.0f, 0.0f, 0.0f))
	, m_smoothHeight(0.0f)
	, distanceToPlayer(0.0f)
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
	, m_player(nullptr)
	, m_model(nullptr)
	, m_enemyType(enemyType)

{
	// タグ設定
	SetTag(Object3D::TagEnemy3D);
	// カメラモードをプレイヤーが操作できるように設定
	Master::m_camera->SetCameraMode(Camera::CameraMode::PlayerControl);
	// ジャンプすケア用のライトを作成
	m_scareLightHandle = CreateDirLightHandle(VGet(0.0f, m_position.y, 0.0f));
	SetLightDifColorHandle(m_scareLightHandle , GetColorF(0.6f, 0.2f, 0.2f, 0.0f));
	SetLightAmbColorHandle(m_scareLightHandle, GetColorF(0.8f, 0.8f, 0.8f, 0.0f));
	SetLightEnableHandle(m_scareLightHandle, false);
	// Mixamo用処理
	// モデルの生成
	m_model = new Model(fileName, initPosition, isSeparateAnimation);
}

void Enemy3D::AddAnimation(AnimationState state, const std::string& fileName)
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
	m_player = nullptr;
}

void Enemy3D::Update()
{
	// 移動処理
	Move();
	// 移動による回転処理
	if (!m_isScare) {
		RotationByMove();
	}
	// プレイヤーが敵と当たった時の処理
	CollScare();
	// モデルの更新
	if (m_model != nullptr) {
		m_model->SetPosition(m_position);
		m_model->Update();
	}
}

void Enemy3D::Draw()
{
	if (m_model != nullptr) {
		m_model->Draw();
	}
}

bool Enemy3D::CanMove()
{
	if (m_isScare) {
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
	m_oldPosition = m_position;         // 前回の座標をいったん保持
	m_moveVec = VGet(0.0f, 0.0f, 0.0f); // 毎フレーム移動ベクトルを初期化
	//  高低差判定 //
	float heightDistance = (m_playerPosition.y + 80.0f) - (m_position.y + 80.0f); 	// プレイヤーと敵の高さの差を計算する
	m_smoothHeight = (m_smoothHeight * 0.8f) + (heightDistance * 0.2f);             // スパイク防止のためのスムージング処理
	// 視界判定 //
	toPlayer = VSub(m_playerPosition, m_position);
	distanceToPlayer = VSize(toPlayer);                                // プレイヤーまでの長さ
	VECTOR forward = VNorm(VGet(sinf(m_angle), 0.0f, cosf(m_angle)));  // 敵の前方向を設定
	VECTOR directionToPlayer = VNorm(toPlayer);                        // プレイヤーまでの方向ベクトルを正規化
	float dot = VDot(forward, directionToPlayer);                      // 正面ベクトルとプレイヤーへの方向ベクトルから内積を求める
	float foundPlayerDistance = 1500.0f;                               // プレイヤーを見つける距離
	float viewAngle = 45.0f;                                           // 視野角度
	float cosAngle = cosf(viewAngle * DX_PI_F / 180.0f);               // プレイヤーを確認する視野範囲
	// 高低差と視野角を判定し、条件を満たしたらプレイヤーを発見。それ以外は未発見
	if (fabs(m_smoothHeight) <= 150.0f &&
		dot >= cosAngle &&
		distanceToPlayer <= foundPlayerDistance) {
		m_foundPlayer = true;
	}
	else {
		m_foundPlayer = false;
	}


	m_playerPosition = m_player->GetPosition();
	if (distanceToPlayer <= 80.0f) {                                                // プレイヤーが敵に近づいたらゲームオーバー処理を行う							                    
		m_player->SetFreeze(true);
		m_isRun = false;
		m_isScare = true;
	}
}
//m_position = VAdd(m_position, VScale(m_moveVec, Speed * 1.8f));     // 敵をプレイヤーの方向に移動させる（速度は走るスピード）

void Enemy3D::ChasePlayer()
{
	m_isRun = true;
	bool isDistanceChase = (distanceToPlayer <= 800.0f);  // 直接追尾を行うかのフラグ
	if (isDistanceChase) {
		toPlayer.y = 0.0f;   // 高さを無視して平面で追尾するように Y を 0 にする
		// ゼロ除算を避けるためのチェック
		if (VSize(toPlayer) > 0.001f) {
			toPlayer = VNorm(toPlayer);                                         // ベクトルを正規化（長さを1にする）して方向だけにする
			m_moveVec = VAdd(VScale(m_moveVec, 0.5f), VScale(toPlayer, 0.5f)); 	// 敵がプレイヤーの方向を向いて向かってくるように設定
			m_speed *= 1.8f;                                                    // 敵の移動速度は走る時の速度に変更
		}
	}
	else {   // 遠い場合などは、ステージを避けるためにプレイヤーの足跡をたどる
		const std::vector<VECTOR>& playerRecord = m_player->GetFootprintsRecord();
		if (m_trackIndex >= playerRecord.size()) {
			m_trackIndex = 0;       // 足跡のリストを初期化
			return;                	// 追尾すべき足跡がないので終了
		}
		VECTOR playerRecordPosition = playerRecord[m_trackIndex];  		// 一番古い足跡（向かうべきターゲット）を取得
		VECTOR vec = VSub(playerRecordPosition, m_position);    		// 敵からプレイヤーの足跡へのベクトル
		vec.y = 0.0f;                                                   // 高さは無視
		float fDistanceToPlayerRecord = VSize(vec);          		    // 敵からプレイヤーの足跡までの距離
		if (fDistanceToPlayerRecord <= 80.0f) {
			m_trackIndex++;         // ターゲットとの距離が一定以下（到着した）なら、次の足跡に向かうようにする
		}
		// ゼロ除算を避けるためのチェック
		if (fDistanceToPlayerRecord > 0.001f) {
			m_moveVec = vec;   				    // 敵を足跡の方向に移動させる
			m_speed *= 1.8f;                    // 敵の移動速度を走る時の速度に変更
		}

	}
}

void Enemy3D::WayPoint()
{
	// CSVから読み取ったウェイポイントの情報を種類ごとコンテナに格納
	for (auto wayPoints : m_wayPointCsv)
	{
		WayPoints point;
		std::string type = wayPoints[0];
		point.pointX = std::stof(wayPoints[1]);
		point.pointY = std::stof(wayPoints[2]);
		point.pointZ = std::stof(wayPoints[3]);
		VECTOR position;
		position = { point.pointX, point.pointY, point.pointZ };
		if (type == "wayPointsRouteA") {
			m_wayPointA.push_back(position);
		}
		else if (type == "wayPointsRouteB") {
			m_wayPointB.push_back(position);
		}
	}
}

void Enemy3D::Patrol()
{
	WayPoint();
	m_isRun = false;
	// 待機状態の場合
	if (m_isWait) {
		m_waitTimer++;
		// 待機時間終了
		if (m_waitTimer >= WaitFrame) {
			m_isWait = false;
			m_waitTimer = 0;
		}
	}
	else {
		// 敵のモデルによって目標地点を変更する
		switch (m_enemyType) {
		case 1:
			// 目標座標の変数に設定
			m_targetX = m_wayPointA[m_currentPoint].x;
			m_targetY = m_wayPointA[m_currentPoint].y;
			m_targetZ = m_wayPointA[m_currentPoint].z;
			// 目標座標と敵の座標の距離を求める
			m_distanceX = m_targetX - m_position.x;
			m_distanceY = m_targetY - m_position.y;
			m_distanceZ = m_targetZ - m_position.z;
			break;
		case 2:
			m_targetX = m_wayPointB[m_currentPoint].x;
			m_targetY = m_wayPointB[m_currentPoint].y;
			m_targetZ = m_wayPointB[m_currentPoint].z;
			m_distanceX = m_targetX - m_position.x;
			m_distanceY = m_targetY - m_position.y;
			m_distanceZ = m_targetZ - m_position.z;
			break;
		}
		float fDistanceToTarget = sqrtf(m_distanceX * m_distanceX + m_distanceY * m_distanceY + m_distanceZ * m_distanceZ); // 距離を計算
		if (fDistanceToTarget > m_speed) {
			VECTOR d = VGet(m_distanceX, m_distanceY, m_distanceZ); //求めた距離を設定
			m_moveVec = VAdd(m_moveVec, d);                         // 移動ベクトルに目標座標を加える
		}
		else {
			// 目標座標をモデルに設定
			m_position.x = m_targetX;
			m_position.y = m_targetY;
			m_position.z = m_targetZ;
			m_moveVec = VGet(0.0f, 0.0f, 0.0f);  // 移動方向を初期化
			// 階段の上り下りの際には待機状態を無くす
			if (m_currentPoint == 2 || m_currentPoint == 3 || m_currentPoint == 6 || m_currentPoint == 7
				|| m_currentPoint == 12 || m_currentPoint == 13 || m_currentPoint == 16 || m_currentPoint == 17
				|| m_currentPoint == 20 || m_currentPoint == 21 || m_currentPoint == 24 || m_currentPoint == 25) {
				m_isWait = false;
			}
			else {
				m_isWait = true;
			}
			// 目標地点の最後まで行ったら通ってきた座標を順番に戻るようにする
			if (m_position.x == m_wayPointA[27].x && m_position.y == m_wayPointA[27].y && m_position.z == m_wayPointA[27].z) m_isback = true;
			if (m_position.x == m_wayPointB[27].x && m_position.y == m_wayPointB[27].y && m_position.z == m_wayPointB[27].z) m_isback = true;
			// 目標地点を全部通るまで設定した目標地点を通っていく
			if (!m_isback) m_currentPoint = (m_currentPoint + 1) % MaxPointCount;
			else {  // 目標地点を全部通ったら通ってきた地点を戻る
				m_currentPoint--;
				if (m_currentPoint <= 0) {
					m_isback = false;
					m_currentPoint = 0;
				}
			}
		}
	}
}

void Enemy3D::UpdateByMove()
{
	bool isMove = (m_moveVec.x != 0.0f || m_moveVec.z != 0.0f);   // 移動状態フラグ
	if (isMove) {
		m_moveVec = VNorm(m_moveVec); 		                      // 移動方向を正規化しておく
		m_targetAngle = atan2f(m_moveVec.x, m_moveVec.z);         // 新しい回転をセット
		// 移動状態なら移動（走りまたは歩き）アニメーションを再生
		if (m_isRun) {
			m_model->ChangeAnimation(AnimationRun);
		}
		else {
			m_model->ChangeAnimation(AnimationWalking);
		}
	}
	else {
		m_model->ChangeAnimation(AnimationNeutral);
	}
}

void Enemy3D::Move()
{
	if (m_player == nullptr) return;
	if (!CanMove()) return;           // スケア中・時間停止中なら動かない

	UpdatePlayerSearch();
	// プレイヤーを見つけた場合(追尾処理～プレイヤーの座標を通る～)
	if (m_foundPlayer){
		ChasePlayer();
    }
	else  {   // プレイヤーを見つけていない場合(目標座標に向かう処理)
		Patrol();
    }
	UpdateByMove();        
	m_position = VAdd(m_position, VScale(m_moveVec, m_speed));   // 敵の座標を更新（移動させる）
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
	if (m_player == nullptr) return;
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
			m_player->SetFreeze(false);

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


