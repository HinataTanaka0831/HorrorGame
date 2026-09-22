#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "GameRuleScene.h"
#include "InputManager.h"
#include "TextureAnimation.h"
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
	if (m_keyHandle == -1) m_keyHandle = LoadGraph("Resource/UI/ui_ruleMove.png");
	if (m_keyRHandle == -1) m_keyRHandle = LoadGraph("Resource/UI/ui_ruleGet.png");
	if (m_keyFHandle == -1) m_keyFHandle = LoadGraph("Resource/UI/ui_ruleAttack.png");
	if (m_keyEHandle == -1) m_keyEHandle = LoadGraph("Resource/UI/ui_ruleCrouching.png");
	if (m_mouseHandle == -1) m_mouseHandle = LoadGraph("Resource/UI/ui_ruleMouseInputLeft.png");
	if (m_mouseMoveHandle == -1) m_mouseMoveHandle = LoadGraph("Resource/UI/playRule_MouseMove_UI.png");

	if (m_backButton == nullptr)
	{
		m_backButton = std::make_unique<Button>(DrawX, BackY - 10, DrawX + 250, BackY + 60, "戻る", GetColor(255, 126, 115), GetColor(250, 250, 250), m_fontSize20);
	}

	if (m_textureAnimation == nullptr)
	{
		m_textureAnimation = std::make_unique<TextureAnimation>(VGet(0, 0, 0), "Resource/UI/sandStorm.png", 9, 3, 3, 4);
	}


	InputManager::GetInstance().EnableMouseLock(false);

}

void GameRuleScene::Update()
{
	Master::m_soundManager->PlayBGM(SoundManager::BGMTitle);

	if (m_textureAnimation)
	{
		m_textureAnimation->Update();
	}

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
	// 砂嵐アニメーションの描画
	if (m_textureAnimation)
	{
		m_textureAnimation->Draw();
	}

	// 四角形の表示
	DrawBox(180, 30, Utility::SCREEN_WIDTH / 2 + 780, 1060, GetColor(255, 255, 255), false);
	// 文字列の表示
	DrawStringToHandle(720, Utility::SCREEN_HEIGHT / 3 - 260, "～～ 操作方法 ～～", GetColor(255, 255, 255), m_fontSize50);

	// 画像の表示
	DrawExtendGraph(300, 140, 300 + 543 / 2, 140 + 380 / 2, m_keyHandle, true);

	DrawExtendGraph(780, 230, 780 + 210 / 2, 230 + 215 / 2, m_keyRHandle, true);

	DrawExtendGraph(1185, 230, 1185 + 225 / 2, 230 + 214 / 2, m_keyFHandle, true);

	DrawExtendGraph(1545, 230, 1545 + 210 / 2, 230 + 215 / 2, m_keyEHandle, true);
	DrawExtendGraph(440, 530, 440 + 400 / 3, 530 + 660 / 3, m_mouseHandle, true);

	DrawGraph(1095, 420, m_mouseMoveHandle, true);


	// 文字列の表示
	DrawStringToHandle(390, 360, "移動", GetColor(255, 255, 255), m_fontSize50);

	DrawStringToHandle(680, 360, "アイテム取得　  アイテム使用　  しゃがみ", GetColor(255, 255, 255), m_fontSize50);

	DrawStringToHandle(350, 760, "ライトのON/OFF", GetColor(255, 255, 255), m_fontSize50);

	DrawStringToHandle(1165, 920, "視点移動", GetColor(255, 255, 255), m_fontSize50);


	// 戻るボタンの表示
	if (m_backButton)
	{
		m_backButton->Draw();
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

	// BGM停止
	Master::m_soundManager->StopBGM();
}

