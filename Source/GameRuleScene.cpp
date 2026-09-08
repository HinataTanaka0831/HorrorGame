#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
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
	if (m_keyHandle == -1) m_keyHandle = LoadGraph("Resource/UI/playRule_Key_UI.png");
	if (m_keyRHandle == -1) m_keyRHandle = LoadGraph("Resource/UI/playRule_Key_R_UI.png");
	if (m_keyFHandle == -1) m_keyFHandle = LoadGraph("Resource/UI/playRule_Key_F_UI.png");
	if (m_keyEHandle == -1) m_keyEHandle = LoadGraph("Resource/UI/playRule_Key_E_UI.png");
	if (m_mouseHandle == -1) m_mouseHandle = LoadGraph("Resource/UI/playRule_Mouse_UI.png");
	if (m_mouseMoveHandle == -1) m_mouseMoveHandle = LoadGraph("Resource/UI/playRule_MouseMove_UI.png");

	if (m_backButton == nullptr)
	{
		m_backButton = std::make_unique<Button>(DrawX, BackY - 10, DrawX + 250, BackY + 60, "戻る", GetColor(255, 126, 115), GetColor(250, 250, 250), m_fontSize20);
	}


	InputManager::GetInstance().EnableMouseLock(false);

}

void GameRuleScene::Update()
{
	Master::m_soundManager->PlayBGM(SoundManager::BGMTitle);

	if (m_backButton)
	{
		m_backButton->Update();

		if (m_backButton->IsClick())
		{
			Master::m_soundManager->PlaySE(SoundManager::SEDecide);
			Master::m_sceneManager->SetNextScene(SceneManager::SceneType::SceneTitle);
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
		sprintf(Buf, "Resource/UI/sandStorm%d.png", i);
		m_noiseHandle = LoadGraph(Buf);

		// 背景の色を設定
		SetBackgroundColor(0, 0, 0);

		// 背景の表示
		DrawGraph(0, 0, m_noiseHandle, true);



		// 四角形の表示
		DrawBox(Utility::SCREEN_WIDTH / 4 - 300, 30, Utility::SCREEN_WIDTH / 2 + 780, 1060, GetColor(255, 255, 255), false);
		// 文字列の表示
		DrawStringToHandle(Utility::SCREEN_WIDTH / 2 - 240, Utility::SCREEN_HEIGHT / 3 - 260, "～～ 操作方法 ～～", GetColor(255, 255, 255), m_fontSize50);

		// 画像の表示
		DrawGraph(Utility::SCREEN_WIDTH / 4 - 250, Utility::SCREEN_HEIGHT / 3 - 260, m_keyHandle, true);

		DrawGraph(Utility::SCREEN_WIDTH / 3 + 140, Utility::SCREEN_HEIGHT / 3 - 160, m_keyRHandle, true);

		DrawGraph(Utility::SCREEN_WIDTH / 2 + 225, Utility::SCREEN_HEIGHT / 3 - 160, m_keyFHandle, true);

		DrawGraph(Utility::SCREEN_WIDTH / 2 + 575, Utility::SCREEN_HEIGHT / 3 - 160, m_keyEHandle, true);

		DrawGraph(Utility::SCREEN_WIDTH / 3 - 240, Utility::SCREEN_HEIGHT / 2 + 60, m_mouseHandle, true);

		DrawGraph(Utility::SCREEN_WIDTH / 2 + 135, Utility::SCREEN_HEIGHT / 2, m_mouseMoveHandle, true);


		// 文字列の表示
		DrawStringToHandle(Utility::SCREEN_WIDTH / 2 - 570, Utility::SCREEN_HEIGHT / 2 - 90, "移動", GetColor(255, 255, 255), m_fontSize50);

		DrawStringToHandle(Utility::SCREEN_WIDTH / 3 + 50, Utility::SCREEN_HEIGHT / 3, "アイテム取得　  アイテム使用　  しゃがみ", GetColor(255, 255, 255), m_fontSize50);

		DrawStringToHandle(Utility::SCREEN_WIDTH / 3 - 290, 920, "ライトのON/OFF", GetColor(255, 255, 255), m_fontSize50);

		DrawStringToHandle(Utility::SCREEN_WIDTH / 2 + 205, 920, "視点移動", GetColor(255, 255, 255), m_fontSize50);

		// 戻るボタンの表示
		if (m_backButton)
		{
			m_backButton->Draw();
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
	if (m_keyHandle != -1) DeleteGraph(m_keyHandle);
	if (m_keyRHandle != -1) DeleteGraph(m_keyRHandle);
	if (m_keyFHandle != -1) DeleteGraph(m_keyFHandle);
	if (m_keyEHandle != -1) DeleteGraph(m_keyEHandle);
	if (m_mouseHandle != -1) DeleteGraph(m_mouseHandle);
	if (m_mouseMoveHandle != -1) DeleteGraph(m_mouseMoveHandle);
	DeleteGraph(m_noiseHandle);

	// BGM停止
	Master::m_soundManager->StopBGM();
}

