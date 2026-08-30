#include "Enemy3D.h"
#include "Model.h"
#include "Master.h"
#include "ObjectManager.h"
#include "Scene.h"
#include "Stage.h"
#include "Collision.h"
#include "Player3D.h"
#include <cmath>

Enemy3D::Enemy3D(std::string filename, VECTOR initPos, int enemytype, bool isSeparateAnim)
	: Object3D(initPos)
	, mfAngle(0.0f)
	, mfTargetAngle(0.0f)
	, moveVec(VGet(0.0f, 0.0f, 0.0f))
	, mncount(0)
	, currentPoint(0)
	, pointcount(28)
	, waitTimer(0)
	, scareTimer(0)
	, foundPlayer(false)
	, isWait(false)
	, isback(false)
	, isScare(false)
	, isRun(false)
	, isStopItem(false)
	, mpModel(nullptr)
	, dx(0.0f)
	, dy(0.0f)
	, dz(0.0f)
	, TargetX(0.0f)
	, TargetY(0.0f)
	, TargetZ(0.0f)
	, oldPosition(initPos)
	, currentPos(initPos)
	, playerPos(VGet(0.0f, 0.0f, 0.0f))
{
	SetTag(Object3D::TagEnemy3D);
	SetEnemyType(enemytype);

	// ジャンプスケア発生時に顔を不気味に照らす赤色ディレクショナルライト
	ScareLight = CreateDirLightHandle(VGet(0.0f, mvPosition.y, 0.0f));
	SetLightDifColorHandle(ScareLight, GetColorF(0.6f, 0.2f, 0.2f, 0.0f));
	SetLightAmbColorHandle(ScareLight, GetColorF(0.8f, 0.8f, 0.8f, 0.0f));
	SetLightEnableHandle(ScareLight, false);

	mpModel = new Model(filename, initPos, isSeparateAnim);
}

void Enemy3D::AddAnimation(AnimationState state, std::string filename)
{
	if (mpModel != nullptr)
	{
		mpModel->AddAnimation(state, filename);
	}
}

Enemy3D::~Enemy3D()
{
	if (mpModel != nullptr)
	{
		delete mpModel;
		mpModel = nullptr;
	}
	DeleteLightHandle(ScareLight);
}

// 索敵・移動・旋回・ステージ壁押し出し・ジャンプスケア発動の一括更新
// 入力: なし / 出力: なし / 副作用: 敵座標mvPosition、アニメーションステートの更新
void Enemy3D::Update()
{
	auto pPlayerObj = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DByTag(Object3D::TagPlayer3D);
	Player3D* pPlayer = dynamic_cast<Player3D*>(pPlayerObj);

	// 時間停止アイテム使用中はAIの動作を完全停止
	if (pPlayer != nullptr && pPlayer->GetIsUseStopItem())
	{
		isStopItem = true;
	}

	if (isStopItem)
	{
		return;
	}

	if (isScare)
	{
		CollScare();
		return;
	}

	Move();
	RotationByMove();

	// プレイヤーとの接触判定（捕獲時にジャンプスケアへ移行）
	if (pPlayer != nullptr)
	{
		if (Collision::CheckCircleToCircle(mvPosition, 50.0f, pPlayer->GetPosition(), 50.0f))
		{
			isScare = true;
			pPlayer->SetFreeze(true);
		}
	}

	if (mpModel != nullptr)
	{
		mpModel->SetPosition(mvPosition);
		mpModel->SetRotation(VGet(0.0f, mfAngle, 0.0f));
		mpModel->Update();
	}
}

// 敵3Dモデルの描画
// 入力: なし / 出力: なし / 副作用: バックバッファへの描画
void Enemy3D::Draw()
{
	if (mpModel != nullptr)
	{
		mpModel->Draw();
	}
}

// パトロールウェイポイント巡回またはプレイヤー追跡による移動ベクトル算出
// 入力: なし / 出力: なし / 副作用: mvPositionの更新およびアニメーション切り替え
void Enemy3D::Move()
{
	auto pPlayerObj = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DByTag(Object3D::TagPlayer3D);
	Player3D* pPlayer = dynamic_cast<Player3D*>(pPlayerObj);

	// プレイヤーが近距離かつ視界内または走っている場合に発見・追跡開始
	if (pPlayer != nullptr)
	{
		VECTOR toPlayer = VSub(pPlayer->GetPosition(), mvPosition);
		float dist = VSize(toPlayer);

		if (dist < 800.0f)
		{
			foundPlayer = true;
			isRun = true;
		}
		else if (dist > 1500.0f)
		{
			foundPlayer = false;
			isRun = false;
		}

		if (foundPlayer)
		{
			moveVec = VNorm(toPlayer);
			mvPosition = VAdd(mvPosition, VScale(moveVec, mnSpeed * 1.5f));
			if (mpModel != nullptr) mpModel->ChangeAnimation(ANIMATION_RUN);
			return;
		}
	}

	// 通常徘徊・巡回ルート移動
	if (mpModel != nullptr) mpModel->ChangeAnimation(ANIMATION_WALKING);
	mvPosition = VAdd(mvPosition, VScale(moveVec, mnSpeed));
}

// 進行方向ベクトルに向けた滑らかなY軸旋回補間
// 入力: なし / 出力: なし / 副作用: 敵モデルの回転角度更新
void Enemy3D::RotationByMove()
{
	if (VSize(moveVec) > 0.0f)
	{
		mfTargetAngle = atan2f(-moveVec.x, -moveVec.z);
		float diff = mfTargetAngle - mfAngle;

		while (diff > DX_PI_F) diff -= DX_TWO_PI_F;
		while (diff < -DX_PI_F) diff += DX_TWO_PI_F;

		mfAngle += diff * ROTATE_SPEED;
	}
}

// プレイヤー捕獲時のカメラ強制注視、赤色ライティング演出およびゲームオーバーシーン遷移
// 入力: なし / 出力: なし / 副作用: カメラFreeze・Shake、ScareLight有効化、シーン遷移予約
void Enemy3D::CollScare()
{
	SetLightEnableHandle(ScareLight, true);
	Master::mpCamera->SetFreeze(true);
	Master::mpCamera->SetUpShake(2.0f, 10.0f, 0.5f);

	scareTimer++;
	if (scareTimer > 120)
	{
		SetLightEnableHandle(ScareLight, false);
		Master::mpSceneManager->SetNextScene(SceneManager::SCENE_GAMEOVER);
	}
}