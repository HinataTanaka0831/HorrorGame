#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "TitleScene.h"
#include "Master.h"
#include "InputManager.h"
#include "Enemy3D.h"
#include "TextureAnimation.h"
#include "Loading.h"
#include <memory>
#include "Button.h"


TitleScene::TitleScene() 
: Scene()     // 基底クラスのコンストラクタを呼び出す
{

}

TitleScene::~TitleScene()
{

}

void TitleScene::Initialize()
{
	// タイトルロゴのクラスの作成
	// プレイヤーの生成
	// などをここで行う
	// ->タイトル画面で必要なオブジェクトをここで生成する

	if (m_enemyPictureHandle == -1)
	{
		m_enemyPictureHandle = LoadGraph("Resource/3D_UI/TitleScene_EnemyPicture.png");
	}

	if (m_playButton == nullptr)
	{
		m_playButton = std::make_unique<Button>(DrawX, PlayY - 10, DrawX + 250, PlayY + 60, " プレイ ", GetColor(255, 126, 115), GetColor(250, 250, 250), m_fontSize20);
	}

	if (m_playRuleButton == nullptr)
	{
		m_playRuleButton = std::make_unique<Button>(DrawX, PlayRuleY - 10, DrawX + 250, PlayRuleY + 60, "操作方法", GetColor(255, 126, 115), GetColor(250, 250, 250), m_fontSize20);
	}

	if (m_quitButton == nullptr)
	{
		m_quitButton = std::make_unique<Button>(DrawX, QuitY - 10, DrawX + 250, QuitY + 60, "終了", GetColor(255, 126, 115), GetColor(250, 250, 250), m_fontSize20);
	}

	// Mouseのロックを解除
	InputManager::GetInstance().EnableMouseLock(false);
	
	// BGM再生
	//Master::mpSoundManager->PlayBGM(SoundManager::BGM_TITLE);
}

void TitleScene::Update()
{
	// BGMの再生
	Master::m_soundManager->PlayBGM(SoundManager::BGMTitle);

	// ボタンの更新処理を呼ぶ
	if (m_playButton)
	{
		m_playButton->Update();

		if (m_playButton->IsClick())
		{
			// SE再生
			Master::m_soundManager->PlaySE(SoundManager::SEDecide);
			Master::m_sceneManager->SetNextScene(SceneManager::SceneGame3D);
		}
	}

	if (m_playRuleButton)
	{
		m_playRuleButton->Update();

		if (m_playRuleButton->IsClick())
		{
			// SE再生
			Master::m_soundManager->PlaySE(SoundManager::SEDecide);
			Master::m_sceneManager->SetNextScene(SceneManager::SceneGameRule);
		}
	}

	if (m_quitButton)
	{
		m_quitButton->Update();

		if (m_quitButton->IsClick())
		{
			// SE再生
			Master::m_soundManager->PlaySE(SoundManager::SEDecide);
			Master::m_sceneManager->RequestQuit();
		}
	}

	// 基底クラスの更新処理を呼び出す
	Scene::Update();
}

void TitleScene::Draw()
{
	for (int i = 1; i < 7; i++)
	{
		// 画面をクリア
		ClearDrawScreen();

		char Buf[256];
		sprintf(Buf, "Resource/3D_UI/sandStorm%d.png", i);
		m_noiseHandle = LoadGraph(Buf);

		// 背景の色を設定
		SetBackgroundColor(0, 0, 0);

		// 敵の画像を表示
		DrawGraph(Utility::SCREEN_WIDTH / 2 + 350, Utility::SCREEN_HEIGHT / 2 - 200, m_enemyPictureHandle, true);

		// 背景の表示
		DrawGraph(0, 0, m_noiseHandle, true);

		// ボタンの表示
		if (m_playButton)
		{
			m_playButton->Draw();
		}

		if (m_playRuleButton)
		{
			m_playRuleButton->Draw();
		}

		if (m_quitButton)
		{
			m_quitButton->Draw();
		}

		// 文字列の表示
		DrawStringToHandle(Utility::SCREEN_WIDTH / 4 - 250, Utility::SCREEN_HEIGHT / 2 - 330, "呪われた校舎", GetColor(255, 0, 0), m_fontSize130);

		// 裏画面の内容を表画面に映す
		ScreenFlip();
	}



	// 基底クラスの描画処理を呼び出す
	Scene::Draw();


}

void TitleScene::Finalize()
{
	// 画像を削除
	DeleteGraph(m_noiseHandle);
	if (m_enemyPictureHandle != -1)
	{
		DeleteGraph(m_enemyPictureHandle);
	}

	// BGM停止
	Master::m_soundManager->StopBGM();
}
