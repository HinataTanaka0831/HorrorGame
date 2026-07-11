#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "GameOverScene.h"
#include "DxLib.h"
#include "Master.h"
#include "InputManager.h"
#include "EscapeItem.h"


// コンストラクタ
GameOverScene::GameOverScene()
	: Scene()     // 基底クラスのコンストラクタを呼び出す
{

}

// デストラクタ
GameOverScene::~GameOverScene()
{

}

// 初期化
void GameOverScene::Initialize()
{
	// ボタンの生成
	if (mpRetoryBtn == nullptr) mpRetoryBtn = new Button(RetoryBtn.left, RetoryBtn.top, RetoryBtn.right, RetoryBtn.bottom, " リトライ　", GetColor(255, 126, 115), GetColor(250, 250, 250), FontSize20);
	if (mpTitleBtn == nullptr) mpTitleBtn = new Button(TitleBtn.left, TitleBtn.top, TitleBtn.right, TitleBtn.bottom, "タイトルへ　", GetColor(255, 126, 115), GetColor(250, 250, 250), FontSize20);

	// Mouseのロックを解除
	g_MouseMgr.EnableMouseLock(false);

}

// 更新
void GameOverScene::Update()
{
	// BGMの再生
	Master::mpSoundManager->PlayBGM(SoundManager::BGM_RESULT);

	// ボタンの更新処理を呼ぶ
	if (mpRetoryBtn)
	{
		mpRetoryBtn->Update();

		if (mpRetoryBtn->IsClicked())
		{
			NowSelect4 = select_Retory;
		}

	}

	if (mpTitleBtn)
	{
		mpTitleBtn->Update();

		if (mpTitleBtn->IsClicked())
		{
			NowSelect4 = select_Title;
		}
	}


	// エンターキーが押されたら画面の切り替え処理
	if (g_MouseMgr.isLeftDown)
	{
		// SE再生
		Master::mpSoundManager->PlaySE(SoundManager::SE_DECIDE);

		switch (NowSelect4)
		{
		case select_Retory:  // プレイ画面へ
			Master::mpSceneManager->SetNextScene(SceneManager::SCENE_3D);
			break;

		case select_Title:  // タイトル画面へ
			Master::mpSceneManager->SetNextScene(SceneManager::SCENE_TITLE);
			break;
		}

	}

	// 基底クラスの更新処理を呼び出す
	Scene::Update();

}

// 描画
void GameOverScene::Draw()
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

		// 背景の表示
		DrawGraph(0, 0, Noise, true);

		// ボタンの表示
		if (mpRetoryBtn)
		{
			mpRetoryBtn->Draw();
		}

		if (mpTitleBtn)
		{
			mpTitleBtn->Draw();
		}

		// 文字列の表示
		DrawStringToHandle(Utility::SCREEN_WIDTH / 3 - 10, Utility::SCREEN_HEIGHT / 2 - 300, "GAME OVER", GetColor(255, 0, 0), FontSize);
		DrawStringToHandle(Utility::SCREEN_WIDTH / 3 - 160, 950, "マウス移動 : 選択　マウス左クリック:決定", GetColor(255, 0, 0), FontSize50);

		// 裏画面の内容を表画面に映す
		ScreenFlip();
	}

	// 基底クラスの描画処理を呼び出す
	Scene::Draw();

}

// 終了処理
void GameOverScene::Finalize()
{
	// 画像を削除
	DeleteGraph(Noise);

	// BGM停止
	Master::mpSoundManager->StopBGM();
}
