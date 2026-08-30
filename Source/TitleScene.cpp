#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "TitleScene.h"
#include "DxLib.h"
#include "Master.h"
#include "InputManager.h"
#include "Enemy3D.h"
#include "TextureAnimation.h"
#include "Loading.h"
#include <memory>
#include "Button.h"


// コンストラクタ
TitleScene::TitleScene() 
: Scene()     // 基底クラスのコンストラクタを呼び出す
{

}

// デストラクタ
TitleScene::~TitleScene()
{

}

// 初期化
void TitleScene::Initialize()
{
	// タイトルロゴのクラスの作成
	// プレイヤーの生成
	// などをここで行う
	// ->タイトル画面で必要なオブジェクトをここで生成する

	if (EnemyPictureHandle == -1)
	{
		EnemyPictureHandle = LoadGraph("Resource/3D_UI/TitleScene_EnemyPicture.png");
	}

	if (mpPlayButton == nullptr)
	{
		mpPlayButton = std::make_unique<Button>(StringX, PlayY - 10, StringX + 250, PlayY + 60, " プレイ ", GetColor(255, 126, 115), GetColor(250, 250, 250), fontSize20);
	}

	if (mpPlayRuleButton == nullptr)
	{
		mpPlayRuleButton = std::make_unique<Button>(StringX, PlayRuleY - 10, StringX + 250, PlayRuleY + 60, "操作方法", GetColor(255, 126, 115), GetColor(250, 250, 250), fontSize20);
	}

	if (mpQuitButton == nullptr)
	{
		mpQuitButton = std::make_unique<Button>(StringX, QuitY - 10, StringX + 250, QuitY + 60, "終了", GetColor(255, 126, 115), GetColor(250, 250, 250), fontSize20);
	}

	// Mouseのロックを解除
	InputManager::GetInstance().EnableMouseLock(false);
	
	// BGM再生
	//Master::mpSoundManager->PlayBGM(SoundManager::BGM_TITLE);
}

// 更新
void TitleScene::Update()
{
	// BGMの再生
	Master::mpSoundManager->PlayBGM(SoundManager::BGM_TITLE);

	// ボタンの更新処理を呼ぶ
	if (mpPlayButton)
	{
		mpPlayButton->Update();

		if (mpPlayButton->IsClick())
		{
			// SE再生
			Master::mpSoundManager->PlaySE(SoundManager::SE_DECIDE);
			Master::mpSceneManager->SetNextScene(SceneManager::SCENE_3D);
		}
	}

	if (mpPlayRuleButton)
	{
		mpPlayRuleButton->Update();

		if (mpPlayRuleButton->IsClick())
		{
			// SE再生
			Master::mpSoundManager->PlaySE(SoundManager::SE_DECIDE);
			Master::mpSceneManager->SetNextScene(SceneManager::SCENE_GAME_RULE);
		}
	}

	// 基底クラスの更新処理を呼び出す
	Scene::Update();
}

// 描画
void TitleScene::Draw()
{
	for (int i = 1; i < 7; i++)
	{
		// 画面をクリア
		ClearDrawScreen();

		char Buf[256];
		sprintf(Buf, "Resource/3D_UI/sandStorm%d.png", i);
		Noise = LoadGraph(Buf);

		// 背景の色を設定
		SetBackgroundColor(0, 0, 0);

		// 敵の画像を表示
		DrawGraph(Utility::SCREEN_WIDTH / 2 + 350, Utility::SCREEN_HEIGHT / 2 - 200, EnemyPictureHandle, true);

		// 背景の表示
		DrawGraph(0, 0, Noise, true);

		// ボタンの表示
		if (mpPlayButton)
		{
			mpPlayButton->Draw();
		}

		if (mpPlayRuleButton)
		{
			mpPlayRuleButton->Draw();
		}

		// 文字列の表示
		DrawStringToHandle(Utility::SCREEN_WIDTH / 4 - 250, Utility::SCREEN_HEIGHT / 2 - 330, "呪われた校舎", GetColor(255, 0, 0), fontSize130);

		// 裏画面の内容を表画面に映す
		ScreenFlip();
	}



	// 基底クラスの描画処理を呼び出す
	Scene::Draw();


}

// 終了処理
void TitleScene::Finalize()
{
	// 画像を削除
	DeleteGraph(Noise);
	if (EnemyPictureHandle != -1)
	{
		DeleteGraph(EnemyPictureHandle);
	}

	// BGM停止
	Master::mpSoundManager->StopBGM();
}
