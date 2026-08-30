#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "GameRuleScene.h"
#include "DxLib.h"
#include "Utility.h"
#include "Master.h"
#include "InputManager.h"
#include "Button.h"

GameRuleScene::GameRuleScene()
	: Scene()
{
}

GameRuleScene::~GameRuleScene()
{
}

// 操作キーUIグラフィックの読み込みおよび戻るボタンの生成
// 入力: なし / 出力: なし / 副作用: キーバインド画像ハンドルのロード、Button生成
void GameRuleScene::Initialize()
{
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

	// UIボタン選択のためマウスロックを解除
	InputManager::GetInstance().EnableMouseLock(false);
}

// タイトルへ戻るボタン入力の監視と遷移要求
// 入力: なし / 出力: なし / 副作用: SE再生、SceneManagerへの遷移予約
void GameRuleScene::Update()
{
	Master::mpSoundManager->PlayBGM(SoundManager::BGM_TITLE);

	if (mpBackButton)
	{
		mpBackButton->Update();

		if (mpBackButton->IsClick())
		{
			Master::mpSoundManager->PlaySE(SoundManager::SE_DECIDE);
			Master::mpSceneManager->SetNextScene(SceneManager::SCENE_TITLE);
		}
	}

	Scene::Update();
}

// 操作説明画像、対応キーアイコン、解説テキストおよび戻るボタンの描画
// 入力: なし / 出力: なし / 副作用: バックバッファへの描画およびScreenFlip
void GameRuleScene::Draw()
{
	// 砂嵐アニメーションの連続描画
	for (int i = 1; i < 7; ++i)
	{
		ClearDrawScreen();

		char Buf[256];
		sprintf(Buf, "Resource/3D_UI/sandStorm%d.png", i);
		noise = LoadGraph(Buf);

		SetBackgroundColor(0, 0, 0);
		DrawGraph(0, 0, noise, true);

		DrawGraph(Utility::SCREEN_WIDTH / 3 - 280, Utility::SCREEN_HEIGHT / 3 - 200, keyHandle, true);
		DrawStringToHandle(Utility::SCREEN_WIDTH / 3 - 240, Utility::SCREEN_HEIGHT / 3, "移動", GetColor(255, 255, 255), fontSize50);

		DrawGraph(Utility::SCREEN_WIDTH / 3 + 140, Utility::SCREEN_HEIGHT / 3 - 160, key_R_Handle, true);
		DrawGraph(Utility::SCREEN_WIDTH / 2 + 225, Utility::SCREEN_HEIGHT / 3 - 160, key_F_Handle, true);
		DrawGraph(Utility::SCREEN_WIDTH / 2 + 575, Utility::SCREEN_HEIGHT / 3 - 160, key_E_Handle, true);

		DrawGraph(Utility::SCREEN_WIDTH / 3 - 240, Utility::SCREEN_HEIGHT / 2 + 60, mouseHandle, true);
		DrawGraph(Utility::SCREEN_WIDTH / 2 + 135, Utility::SCREEN_HEIGHT / 2, mouseMoveHandle, true);

		DrawStringToHandle(Utility::SCREEN_WIDTH / 3 + 50, Utility::SCREEN_HEIGHT / 3, "アイテム取得　  アイテム使用　  しゃがみ", GetColor(255, 255, 255), fontSize50);
		DrawStringToHandle(Utility::SCREEN_WIDTH / 3 - 290, 920, "ライトのON/OFF", GetColor(255, 255, 255), fontSize50);
		DrawStringToHandle(Utility::SCREEN_WIDTH / 2 + 205, 920, "視点移動", GetColor(255, 255, 255), fontSize50);

		if (mpBackButton)
		{
			mpBackButton->Draw();
		}

		ScreenFlip();
		DeleteGraph(noise);
	}

	Scene::Draw();
}

// ロードした全操作キー画像リソースの解放
// 入力: なし / 出力: なし / 副作用: 画像ハンドルの破棄、BGM停止
void GameRuleScene::Finalize()
{
	if (keyHandle != -1) { DeleteGraph(keyHandle); keyHandle = -1; }
	if (key_R_Handle != -1) { DeleteGraph(key_R_Handle); key_R_Handle = -1; }
	if (key_F_Handle != -1) { DeleteGraph(key_F_Handle); key_F_Handle = -1; }
	if (key_E_Handle != -1) { DeleteGraph(key_E_Handle); key_E_Handle = -1; }
	if (mouseHandle != -1) { DeleteGraph(mouseHandle); mouseHandle = -1; }
	if (mouseMoveHandle != -1) { DeleteGraph(mouseMoveHandle); mouseMoveHandle = -1; }

	Master::mpSoundManager->StopBGM();
}