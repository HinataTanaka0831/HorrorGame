#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "GameOverScene.h"
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
	if (m_retoryButton == nullptr)
	{
		m_retoryButton = std::make_unique<Button>(DrawX, RetoryY - 10, DrawX + 250, RetoryY + 60, "リトライ", GetColor(255, 126, 115), GetColor(250, 250, 250), m_fontSize20);
	}

	if (m_titleButton == nullptr)
	{
		m_titleButton = std::make_unique<Button>(DrawX, TitleY - 10, DrawX + 250, TitleY + 60, "タイトルへ", GetColor(255, 126, 115), GetColor(250, 250, 250), m_fontSize20);
	}

	// Mouseのロックを解除
	InputManager::GetInstance().EnableMouseLock(false);

}

void GameOverScene::Update()
{
	// BGMの再生
	Master::m_soundManager->PlayBGM(SoundManager::BGMResult);

	// ボタンの更新処理を呼ぶ
	if (m_retoryButton)
	{
		m_retoryButton->Update();

		if (m_retoryButton->IsClick())
		{
			// SE再生
			Master::m_soundManager->PlaySE(SoundManager::SEDecide);
			Master::m_sceneManager->SetNextScene(SceneManager::SceneGame3D);
		}

	}

	if (m_titleButton)
	{
		m_titleButton->Update();

		if (m_titleButton->IsClick())
		{
			// SE再生
			Master::m_soundManager->PlaySE(SoundManager::SEDecide);
			Master::m_sceneManager->SetNextScene(SceneManager::SceneTitle);
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
		sprintf(Buf, "Resource/UI/sandStorm%d.png", i);
		m_noiseHandle = LoadGraph(Buf);

		// 背景の色を設定
		SetBackgroundColor(0, 0, 0);

		// 背景の表示
		DrawGraph(0, 0, m_noiseHandle, true);

		// 文字列の表示
		DrawStringToHandle(Utility::SCREEN_WIDTH / 2 - 350, Utility::SCREEN_HEIGHT / 2 - 140, "Game Over", GetColor(255, 255, 255), m_fontSize140);

		// ボタンの表示
		if (m_retoryButton)
		{
			m_retoryButton->Draw();
		}

		if (m_titleButton)
		{
			m_titleButton->Draw();
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
	DeleteGraph(m_noiseHandle);

	// BGM停止
	Master::m_soundManager->StopBGM();
}
