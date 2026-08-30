#include "GameScene.h"
#include "DxLib.h"
#include "Utility.h"
#include "Master.h"
#include "ObjectManager.h"
#include "Scene.h"
#include "InputManager.h"
#include "Player.h"
#include "Enemy.h"

GameScene::GameScene()
	: SelectScene()     // 基底クラスのコンストラクタを呼び出す
{
	
}

GameScene::~GameScene()
{

}

void GameScene::Initialize()
{
	// SelectScene で選択した選択肢によって弾を変更するためプレイヤーを分岐 //
	switch (chag)
	{
	case 2:
		new Player(
			VGet((float)Utility::SCREEN_WIDTH / 9, (float)Utility::SCREEN_HEIGHT / 2, 0.0f), 2);
		break;

	case 1:
		new Player(
			VGet((float)Utility::SCREEN_WIDTH / 9, (float)Utility::SCREEN_HEIGHT / 2, 0.0f), 1);
		break;
	}
		
	

	new Enemy(
		VGet((float)Utility::SCREEN_WIDTH + 115.0f, GetRand(480 - 37), 0.0f),
		"Resource/kusikatsu_gyu.png", 1, 1
	);
		new Enemy(VGet((float)Utility::SCREEN_WIDTH + 115.0f, GetRand(480 - 55), 0.0f),
			"Resource/torimomoniku.png", 2, 1
		);
		new Enemy(VGet((float)Utility::SCREEN_WIDTH + 130.0f, GetRand(480 - 54), 0.0f),
			"Resource/hamukatsu.png", 3, 1
		);
		new Enemy(VGet((float)Utility::SCREEN_WIDTH + 140.0f, GetRand(480 - 59), 0.0f),
			"Resource/Rebakatsu.png", 4, 1
		);



		// 串揚げ塩
		new Enemy(
			VGet((float)Utility::SCREEN_WIDTH + 115.0f, GetRand(480 - 53), 0.0f),
			"Resource/kusikatsu_ton.png", 1, 2
		);
		new Enemy(
			VGet((float)Utility::SCREEN_WIDTH + 115.0f, GetRand(480 - 60), 0.0f),
			"Resource/tamanegi.png", 2, 2
		);
		new Enemy(
			VGet((float)Utility::SCREEN_WIDTH + 130.0f, GetRand(480 - 55), 0.0f),
			"Resource/renkon.png", 3, 2
		);
		new Enemy(
			VGet((float)Utility::SCREEN_WIDTH + 150.0f, GetRand(480 - 55), 0.0f),
			"Resource/torisasami.png", 4, 2
		);

		
		
}

void GameScene::Update()
{

	// .empty() ... 空っぽかどうか、という関数  	

	// 更新処理を呼び出す
	Scene::Update();

}

void GameScene::Draw()
{

	// 背景の表示

	// 敵情報を取得
	auto pTargetList = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject2DListByTag(Object2D::Enemy2D);

	for (int i = 0; i < pTargetList.size(); i++)
	{
		auto pTarget = pTargetList[i];

		if (pTarget != nullptr)
		{
			
			Enemy* pEnemy = dynamic_cast<Enemy*>(pTarget);

			if (pEnemy != nullptr)
			{

				sprintfDx(font, "スコア： %d / 200", pEnemy->GetScore());
			}
		}

		
	}

	DrawString(100, 10, font, GetColor(255, 255, 255));


	// HP の表示
	auto pPlayer = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject2DByTag(Object2D::Player2D);

	Player* cPlayer = dynamic_cast<Player*>(pPlayer);

	if (cPlayer != nullptr)
	{
		sprintfDx(font, "HP: %d", cPlayer->GetHp());
	}


	DrawString(10, 10, font, GetColor(255, 255, 255));



	// 描画処理を呼び出す
	Scene::Draw();

}

void GameScene::Finalize()
{

}




