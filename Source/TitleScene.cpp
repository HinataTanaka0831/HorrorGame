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
#include "MouseManager.h"
#include "Button.h"


// コンストラクタ
TitleScene::TitleScene() 
: Scene()     // 基底クラスのコンストラクタを呼び出す
{
	// 敵の画像を生成
	EnemyPictureHandle = LoadGraph("Resource/3D_UI/TitleScene_EnemyPicture.png");
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

	// ボタンの生成
	if (mpPlay == nullptr) mpPlay= new Button(playBtn.left, playBtn.top, playBtn.right, playBtn.bottom, "　プレイ　", GetColor(255, 126, 115), GetColor(250, 250, 250), FontSize20);
	if (mpRule == nullptr) mpRule = new Button(playRule.left, playRule.top, playRule.right, playRule.bottom, " 操作方法　", GetColor(255, 126, 115), GetColor(250, 250, 250), FontSize20);

	// Mouseのロックを解除
	g_MouseMgr.EnableMouseLock(false);
	
	// BGM再生
	//Master::mpSoundManager->PlayBGM(SoundManager::BGM_TITLE);
}

// 更新
void TitleScene::Update()
{
	// BGMの再生
	Master::mpSoundManager->PlayBGM(SoundManager::BGM_TITLE);

	// ボタンの更新処理を呼ぶ
	if (mpPlay)
	{
		mpPlay->Update();

		if (mpPlay->IsClicked())
		{
			NowSelect3 = select_Play;
		}
	}

	if (mpRule)
	{
		mpRule->Update();

		if (mpRule->IsClicked())
		{
			NowSelect3 = select_PlayRule;
		}
	}


	// エンターキーが押されたら画面の切り替え処理
	if (g_MouseMgr.isLeftDown)
	{
		// SE再生
		Master::mpSoundManager->PlaySE(SoundManager::SE_DECIDE);

		switch (NowSelect3)
		{
		case select_Play:  // プレイ画面へ
			Master::mpSceneManager->SetNextScene(SceneManager::SCENE_3D);
			break;

		case select_PlayRule:  // 遊び方へ
			Master::mpSceneManager->SetNextScene(SceneManager::SCENE_GAME_RULE);
			break;
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
		if (mpPlay)
		{
			mpPlay->Draw();
		}

		if (mpRule)
		{
			mpRule->Draw();
		}

		// 文字列の表示
		DrawStringToHandle(Utility::SCREEN_WIDTH / 4 - 250, Utility::SCREEN_HEIGHT / 2 - 330, "呪われた校舎", GetColor(255, 0, 0), FontSize130);
		DrawStringToHandle(Utility::SCREEN_WIDTH / 3 - 160, 950, "マウス移動 : 選択　マウス左クリック:決定", GetColor(255, 0, 0), FontSize50);

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
	if (EnemyPictureHandle != -1) DeleteGraph(EnemyPictureHandle);

	// BGM停止
	Master::mpSoundManager->StopBGM();
}
