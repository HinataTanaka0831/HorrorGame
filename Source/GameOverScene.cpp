#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "GameOverScene.h"
#include "DxLib.h"
#include "Master.h"
#include "InputManager.h"
#include "EscapeItem.h"
#include "Button.h"

GameOverScene::GameOverScene()
	: Scene()
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

	// UIボタン操作のためマウスロックを解除
	InputManager::GetInstance().EnableMouseLock(false);
}

void GameOverScene::Update()
{
	Master::mpSoundManager->PlayBGM(SoundManager::BGM_RESULT);

	if (mpRetoryButton)
	{
		mpRetoryButton->Update();

		if (mpRetoryButton->IsClick())
		{
			Master::mpSoundManager->PlaySE(SoundManager::SE_DECIDE);
			Master::mpSceneManager->SetNextScene(SceneManager::SCENE_3D);
		}
	}

	if (mpTitleButton)
	{
		mpTitleButton->Update();

		if (mpTitleButton->IsClick())
		{
			Master::mpSoundManager->PlaySE(SoundManager::SE_DECIDE);
			Master::mpSceneManager->SetNextScene(SceneManager::SCENE_TITLE);
		}
	}

	Scene::Update();
}

void GameOverScene::Draw()
{
	for (int i = 1; i < 7; ++i)
	{
		ClearDrawScreen();

		char Buf[256];
		sprintf(Buf, "Resource/3D_UI/sandStorm%d.png", i);
		Noise = LoadGraph(Buf);

		SetBackgroundColor(0, 0, 0);
		DrawGraph(0, 0, Noise, true);

		DrawStringToHandle(Utility::SCREEN_WIDTH / 2 - 150, Utility::SCREEN_HEIGHT / 2 - 140, "Game Over", GetColor(255, 255, 255), fontSize90);

		if (mpRetoryButton)
		{
			mpRetoryButton->Draw();
		}

		if (mpTitleButton)
		{
			mpTitleButton->Draw();
		}

		ScreenFlip();
		DeleteGraph(Noise);
	}

	Scene::Draw();
}

void GameOverScene::Finalize()
{
	Master::mpSoundManager->StopBGM();
}