#include "ResultScene.h"
#include "Master.h"
#include "InputManager.h"
#include "Button.h"

ResultScene::ResultScene()
	: Scene()     // 基底クラスのコンストラクタを呼び出す
{

}

ResultScene::~ResultScene()
{

}

void ResultScene::Initialize()
{
	// ボタンの生成
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

void ResultScene::Update()
{
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

void ResultScene::Draw()
{
	// 背景の表示
	SetBackgroundColor(255, 255, 255);

	// 文字列の表示 //
	DrawStringToHandle(Utility::SCREEN_WIDTH / 2 - 350, Utility::SCREEN_HEIGHT / 2 - 140, "Game Clear", GetColor(255, 255, 255), m_fontSize140);

	// ボタンの表示
	if (m_retoryButton)
	{
		m_retoryButton->Draw();
	}

	if (m_titleButton)
	{
		m_titleButton->Draw();
	}

	// 基底クラスの描画処理を呼び出す
	Scene::Draw();

}

void ResultScene::Finalize()
{

}
