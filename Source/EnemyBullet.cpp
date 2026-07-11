#include "EnemyBullet.h"
#include "Master.h"
#include "ObjectManager.h"
#include "Scene.h"
#include "Collision.h"
#include "Player.h"

EnemyBullet::EnemyBullet(VECTOR initPos, std::string filename)
	: Object2D(filename, initPos)

{
	SetTag(Object2D::EnemyBullet2D);
}

EnemyBullet::~EnemyBullet()
{

}

void EnemyBullet::Update()
{
	// 移動処理
	Move();

	// 画面外に出たら削除する
	if (IsScreenOut())
	{
		SetDeleteFlag(true);  // 削除フラグを立てて、後で消してもらう
	}

	Object2D::Update();
}

void EnemyBullet::Draw()
{
	Object2D::Draw();
}

void EnemyBullet::Move()
{
	// 座標移動（画面の上のほうに向かって移動）
	mvPosition = VAdd(mvPosition, VScale(mvDirection, mfspeed));

	// テクスチャに座標を伝える
	mpTexture->SetPosition(mvPosition);
}

bool EnemyBullet::IsScreenOut()
{
	return (mvPosition.x + mpTexture->GetSizeX() / 2) > 640.0f;
}

void EnemyBullet::CalcCollision()
{
	auto pTarget = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject2DByTag(Object2D::Player2D);

	if (pTarget != nullptr)
	{

		if (Collision::CheckCircleToCircle(
			mvPosition,
			GetRadius(),
			pTarget->GetPosition(),
			pTarget->GetRadius())
			)
		{

			// 当たった弾を削除
			this->SetDeleteFlag(true);

			Player* pPlayer = dynamic_cast<Player*>(pTarget);

			if (pPlayer != nullptr)   // dynamic_cast をした後は必ずnullチェックを行う
			{
				pPlayer->PDamage(1);
			}

		}

	}

}

