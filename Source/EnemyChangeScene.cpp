#include "EnemyChangeScene.h"
#include "DxLib.h"
#include "Utility.h"
#include "Master.h"
#include "ObjectManager.h"
#include "InputManager.h"


EnemyChangeScene::EnemyChangeScene()
	:SelectScene()    // 基底クラスのコンストラクタを呼び出す
{

}

EnemyChangeScene::~EnemyChangeScene()
{

}

void EnemyChangeScene::Initialize()
{

}

void EnemyChangeScene::Update()
{
	// エンターキーが押されたら
	if (InputManager::CheckDownKey(KEY_INPUT_RETURN))
	{
		// SE再生
		Master::mpSoundManager->PlaySE(SoundManager::SE_DECIDE);

		// GameSceneへ
		Master::mpSceneManager->SetNextScene(SceneManager::SCENE_TYPE::SCENE_GAME);

	}

	// 基底クラスの更新処理を呼び出す
	Scene::Update();
}

void EnemyChangeScene::Draw()
{

	// 背景の表示


	// SelectSceneで選択した選択肢によって文字列を分岐
	switch (chag)
	{
	case 2:
		// 四角形の表示
		DrawBox(Utility::SCREEN_WIDTH / 2 - 150, 100, Utility::SCREEN_WIDTH / 2 + 160, 270, GetColor(255, 255, 255), false);

		// 文字列の表示
		DrawString(Utility::SCREEN_WIDTH / 2 - 140, Utility::SCREEN_HEIGHT / 2 - 100, "[ソース]は敵だ！\n    よけるかうちおとせ！\n\n[塩]は味方！うつとダメージ！", GetColor(255, 255, 255));
		break;

	case 1:
		// 四角形の表示
		DrawBox(Utility::SCREEN_WIDTH / 2 - 160, 100, Utility::SCREEN_WIDTH / 2 + 200, 270, GetColor(255, 255, 255), false);

		// 文字列の表示
		DrawString(Utility::SCREEN_WIDTH / 2 - 140, Utility::SCREEN_HEIGHT / 2 - 100, "[塩]は敵だ！\n   よけるかうちおとせ！\n\n[ソース]は味方！うつとダメージ！", GetColor(255, 255, 255));
		break;
	}

	// 文字列の表示
	DrawString(Utility::SCREEN_WIDTH / 2 - 55, Utility::SCREEN_HEIGHT / 2 + 100, "Enterで決定", GetColor(255, 255, 255));

	// 基底クラスの描画処理を呼び出す
	Scene::Draw();
}

void EnemyChangeScene::Finalize()
{

}
