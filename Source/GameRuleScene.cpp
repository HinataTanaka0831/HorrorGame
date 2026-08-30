#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "DxLib.h"
#include "GameRuleScene.h"
#include "InputManager.h"
#include "Master.h"
#include "Button.h"

GameRuleScene::GameRuleScene()
	:Scene()
{

}

GameRuleScene::~GameRuleScene()
{

}

void GameRuleScene::Initialize()
{
	// 画像を生成
	if (keyHandle == -1) keyHandle = LoadGraph("Resource/3D_UI/playRule_Key_UI.png");
	if (key_R_Handle == -1) key_R_Handle = LoadGraph("Resource/3D_UI/playRule_Key_R_UI.png");
	if (key_F_Handle == -1) key_F_Handle = LoadGraph("Resource/3D_UI/playRule_Key_F_UI.png");
	if (key_E_Handle == -1) key_E_Handle = LoadGraph("Resource/3D_UI/playRule_Key_E_UI.png");
	if (mouseHandle == -1) mouseHandle = LoadGraph("Resource/3D_UI/playRule_Mouse_UI.png");
	if (mouseMoveHandle == -1) mouseMoveHandle = LoadGraph("Resource/3D_UI/playRule_MouseMove_UI.png");

	if (mpBackButton == nullptr)
	{
		mpBackButton = std::make_unique<Button>(DrawX, BackY - 10, DrawX + 250, BackY + 60, "戻る", GetColor(255, 126, 115), GetColor(250, 250, 250), fontSize20);
	}


	InputManager::GetInstance().EnableMouseLock(false);

}

void GameRuleScene::Update()
{
	Master::mpSoundManager->PlayBGM(SoundManager::BGM_TITLE);

	if (mpBackButton)
	{
		mpBackButton->Update();

		if (mpBackButton->IsClick())
		{
			Master::mpSoundManager->PlaySE(SoundManager::SE_DECIDE);
			Master::mpSceneManager->SetNextScene(SceneManager::SCENE_TYPE::SCENE_TITLE);
		}
	}

	// 基底クラスの更新処理を呼び出す
	Scene::Update();
}

void GameRuleScene::Draw()
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



		// 四角形の表示
		DrawBox(Utility::SCREEN_WIDTH / 4 - 300, 30, Utility::SCREEN_WIDTH / 2 + 780, 1060, GetColor(255, 255, 255), false);
		// 文字列の表示
		DrawStringToHandle(Utility::SCREEN_WIDTH / 2 - 240, Utility::SCREEN_HEIGHT / 3 - 260, "～～ 操作方法 ～～", GetColor(255, 255, 255), fontSize50);

		// 画像の表示
		DrawGraph(Utility::SCREEN_WIDTH / 4 - 250, Utility::SCREEN_HEIGHT / 3 - 180, keyHandle, true);

		DrawGraph(Utility::SCREEN_WIDTH / 3 + 140, Utility::SCREEN_HEIGHT / 3 - 160, key_R_Handle, true);

		DrawGraph(Utility::SCREEN_WIDTH / 2 + 225, Utility::SCREEN_HEIGHT / 3 - 160, key_F_Handle, true);

		DrawGraph(Utility::SCREEN_WIDTH / 2 + 575, Utility::SCREEN_HEIGHT / 3 - 160, key_E_Handle, true);

		DrawGraph(Utility::SCREEN_WIDTH / 3 - 240, Utility::SCREEN_HEIGHT / 2 + 60, mouseHandle, true);

		DrawGraph(Utility::SCREEN_WIDTH / 2 + 135, Utility::SCREEN_HEIGHT / 2, mouseMoveHandle, true);


		// 文字列の表示
		DrawStringToHandle(Utility::SCREEN_WIDTH / 3 + 50, Utility::SCREEN_HEIGHT / 3, "アイテム取得　  アイテム使用　  しゃがみ", GetColor(255, 255, 255), fontSize50);

		DrawStringToHandle(Utility::SCREEN_WIDTH / 3 - 290, 920, "ライトのON/OFF", GetColor(255, 255, 255), fontSize50);

		DrawStringToHandle(Utility::SCREEN_WIDTH / 2 + 205, 920, "視点移動", GetColor(255, 255, 255), fontSize50);


		if (mpBackButton)
		{
			mpBackButton->Draw();
		}


		// 裏画面の内容を表画面に映す
		ScreenFlip();
	}

	// 基底クラスの描画処理を呼び出す
	Scene::Draw();
}

void GameRuleScene::Finalize()
{
	// 画像を削除
	if (keyHandle != -1) DeleteGraph(keyHandle);
	if (key_R_Handle != -1) DeleteGraph(key_R_Handle);
	if (key_F_Handle != -1) DeleteGraph(key_F_Handle);
	if (key_E_Handle != -1) DeleteGraph(key_E_Handle);
	if (mouseHandle != -1) DeleteGraph(mouseHandle);
	if (mouseMoveHandle != -1) DeleteGraph(mouseMoveHandle);
	DeleteGraph(noise);

	// BGM停止
	Master::mpSoundManager->StopBGM();
}

