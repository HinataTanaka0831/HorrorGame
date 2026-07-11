#include "Enemy.h"
#include "Utility.h"
#include "Master.h"
#include "ObjectManager.h"
#include "Scene.h"
#include "Collision.h"
#include "Player.h"
#include "SelectScene.h"




Enemy::Enemy(VECTOR initPos, std::string filename, int t, int s)
	: Object2D(filename, initPos)
	,mvDirection(VGet(-1.0f, 0.0f, 0.0f))   // 最初はX軸の右方向へ動くようにしておく
	
{
	SetTag(Object2D::Enemy2D);

	Settype(t);

	SetEnemytype(s);   	// 敵の種類を分岐する変数を設定 //

}


Enemy::~Enemy()
{
	
}

void Enemy::Update()
{

	// 移動処理
	Move();


	if (IsScreenOut())
	{
		SetDeleteFlag(true);
		
		Initialize();
		
	}

	// スコアが目標に達したのであればリザルト画面へ //
	if (score == 200)
	{
		Master::mpSceneManager->SetNextScene(SceneManager::SCENE_TYPE::SCENE_RESULT);
		// スコアを初期化 //
		score = 0;
	}


	Object2D::Update();
}

void Enemy::Draw()
{
	Object2D::Draw();
}

void Enemy::Move()
{
	// 敵の出現時間
	switch (type)
	{
	case 1:time = 1000; break;
	case 2:time = 3000; break;
	case 3:time = 5000; break;
	case 4:time = 7000; break;
	}

	if (GetNowCount() - StartTime > time)
	{
		// mvDirection.x の方向へ進むようにする
		mvPosition.x += (float)MOVE_SPEED * mvDirection.x;
	}

	// テクスチャに座標を伝える
	mpTexture->SetPosition(mvPosition);
}

bool Enemy::IsScreenOut()
{
	return(mvPosition.x + mpTexture->GetSizeX() / 2) < 0.0f;
}



void Enemy::Initialize()
{
	int work = rand() % 4;


	switch (work)
	{
	case 0:

		new Enemy(
			VGet((float)Utility::SCREEN_WIDTH + 115.0f, GetRand(480 - 37), 0.0f),
			"Resource/kusikatsu_gyu.png", 1, 1
		);
		// 串揚げ塩
		new Enemy(
			VGet((float)Utility::SCREEN_WIDTH + 115.0f, GetRand(480 - 53), 0.0f),
			"Resource/kusikatsu_ton.png", 1, 2
		);
		break;
		
	case 1:
		new Enemy(VGet((float)Utility::SCREEN_WIDTH + 115.0f, GetRand(480 - 55), 0.0f),
			"Resource/torimomoniku.png", 2, 1
		);
		// 串揚げ塩
		new Enemy(
			VGet((float)Utility::SCREEN_WIDTH + 115.0f, GetRand(480 - 60), 0.0f),
			"Resource/tamanegi.png", 2, 2
		);

		break;

	case 2:

		new Enemy(VGet((float)Utility::SCREEN_WIDTH + 130.0f, GetRand(480 - 54), 0.0f),
			"Resource/hamukatsu.png", 3, 1
		);
		// 串揚げ塩
		new Enemy(
			VGet((float)Utility::SCREEN_WIDTH + 130.0f, GetRand(480 - 55), 0.0f),
			"Resource/renkon.png", 3, 2
		);
		break;

	case 3:
		new Enemy(VGet((float)Utility::SCREEN_WIDTH + 140.0f, GetRand(480 - 59), 0.0f),
			"Resource/Rebakatsu.png", 4, 1
		);
		// 串揚げ塩
		new Enemy(
			VGet((float)Utility::SCREEN_WIDTH + 150.0f, GetRand(480 - 55), 0.0f),
			"Resource/torisasami.png", 4, 2
		);
		break;


	}

}



// enemytype が１の敵を倒したらプレイヤーにダメージを与える //
void Enemy::ChangeDamage(int damage)
{
	mnHp -= damage;

	if (mnHp <= 0)
	{
		SetDeleteFlag(true);

		// スコアを加算 //
		score += 10;
		

		auto pPlayer = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject2DByTag(Object2D::Player2D);

		Player* cPlayer = dynamic_cast<Player*>(pPlayer);

		if (cPlayer != nullptr)
		{

       		if (enemytype == 1)
			{
				// スコアを減算 //
				score -= 10;

				cPlayer->PDamage(1);
			}
		}

	}
	// スコアが０より小さい場合０にする //
	if (score < 0)
	{
		score = 0;
	}


}



	


// enemytype が２の敵を倒したらプレイヤーにダメージを与える //
void Enemy::ChangeDamage2(int damage)
{
	mnHp -= damage;

	if (mnHp <= 0)
	{

		SetDeleteFlag(true);
		
		// スコアを加算 //
		score += 10;
		

		auto pPlayer = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject2DByTag(Object2D::Player2D);

		Player* cPlayer = dynamic_cast<Player*>(pPlayer);

		if (cPlayer != nullptr)
		{

			if (enemytype == 2)
			{
				// スコアを減算 //
				score -= 10;

				cPlayer->PDamage(1);

			}
			

		}


	}
	// スコアが０より小さい場合０にする //
	if (score < 0)
	{
		score = 0;
	}



}












