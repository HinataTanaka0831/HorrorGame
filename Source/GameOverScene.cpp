#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "GameOverScene.h"
#include "DxLib.h"
#include "Master.h"
#include "InputManager.h"
#include "EscapeItem.h"
#include "Button.h"


GameOverScene::GameOverScene()
	: Scene()     // 基底クラスのコンストラクタを呼び出す
{

}

GameOverScene::~GameOverScene()
{

}

void GameOverScene::Initialize()
{
	if (mpRetoryButton == nullptr)
	{
		mpRetoryButton = std::make_unique<Button>(DrawX, RetoryY - 10, DrawX + 250, RetoryY + 60, "リトライ", GetColor(255, 126, 115), GetColor(250, 250, 250), fontSize20);
	}

	if (mpTitleButton == nullptr)
	{
		mpTitleButton = std::make_unique<Button>(DrawX, TitleY - 10, DrawX + 250, TitleY + 60, "タイトルへ", GetColor(255, 126, 115), GetColor(250, 250, 250), fontSize20);
	}

	// Mouseのロックを解除
	InputManager::GetInstance().EnableMouseLock(false);

}

void GameOverScene::Update()
{
	// BGMの再生
	Master::mpSoundManager->PlayBGM(SoundManager::BGM_RESULT);

	// ボタンの更新処理を呼ぶ
	if (mpRetoryButton)
	{
		mpRetoryButton->Update();

		if (mpRetoryButton->IsClick())
		{
			// SE再生
			Master::mpSoundManager->PlaySE(SoundManager::SE_DECIDE);
			Master::mpSceneManager->SetNextScene(SceneManager::SCENE_3D);
		}

	}

	if (mpTitleButton)
	{
		mpTitleButton->Update();

		if (mpTitleButton->IsClick())
		{
			// SE再生
			Master::mpSoundManager->PlaySE(SoundManager::SE_DECIDE);
			Master::mpSceneManager->SetNextScene(SceneManager::SCENE_TITLE);
		}
	}


	// 基底クラスの更新処理を呼び出す
	Scene::Update();

}

void GameOverScene::Draw()
{
	for (int i = 1; i < 7; i++)
	{
		// 画面をクリア
		ClearDrawScreen();

		char Buf[256];
		sprintf(Buf, "Resource/3D_UI/sandStorm%d.png", i);
		noise = LoadGraph(Buf);

		// 背景の色を設定
		SetBackgroundColor(0, 0, 0);

		// 背景の表示
		DrawGraph(0, 0, noise, true);

		// 文字列の表示
		DrawStringToHandle(Utility::SCREEN_WIDTH / 2 - 350, Utility::SCREEN_HEIGHT / 2 - 140, "Game Over", GetColor(255, 255, 255), fontSize140);

		// ボタンの表示
		if (mpRetoryButton)
		{
			mpRetoryButton->Draw();
		}

		if (mpTitleButton)
		{
			mpTitleButton->Draw();
		}

		// 裏画面の内容を表画面に映す
		ScreenFlip();
	}

	// 基底クラスの描画処理を呼び出す
	Scene::Draw();

}

void GameOverScene::Finalize()
{
	// 画像を削除
	DeleteGraph(noise);

	// BGM停止
	Master::mpSoundManager->StopBGM();
}
