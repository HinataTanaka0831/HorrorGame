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

TitleScene::TitleScene() 
	: Scene()
{
}

TitleScene::~TitleScene()
{
}

// タイトル画面のボタンUI生成、敵グラフィック読み込みおよびマウスロック解除
// 入力: なし / 出力: なし / 副作用: 画像ハンドル読み込み、Buttonインスタンス生成
void TitleScene::Initialize()
{
	if (enemyPictureHandle == -1)
	{
		enemyPictureHandle = LoadGraph("Resource/3D_UI/TitleScene_EnemyPicture.png");
	}

	if (mpPlayButton == nullptr)
	{
		mpPlayButton = std::make_unique<Button>(DrawX, PlayY - 10, DrawX + 250, PlayY + 60, " プレイ ", GetColor(255, 126, 115), GetColor(250, 250, 250), fontSize20);
	}

	if (mpPlayRuleButton == nullptr)
	{
		mpPlayRuleButton = std::make_unique<Button>(DrawX, PlayRuleY - 10, DrawX + 250, PlayRuleY + 60, "操作方法", GetColor(255, 126, 115), GetColor(250, 250, 250), fontSize20);
	}

	if (mpQuitButton == nullptr)
	{
		mpQuitButton = std::make_unique<Button>(DrawX, QuitY - 10, DrawX + 250, QuitY + 60, "終了", GetColor(255, 126, 115), GetColor(250, 250, 250), fontSize20);
	}

	// メニューUI操作を可能にするためマウスカーソル拘束を解除
	InputManager::GetInstance().EnableMouseLock(false);
}

// BGM再生の維持、ボタン入力の監視および各シーンへの遷移要求
// 入力: なし / 出力: なし / 副作用: BGM再生、SE再生、SceneManagerへの遷移予約
void TitleScene::Update()
{
	Master::mpSoundManager->PlayBGM(SoundManager::BGM_TITLE);

	if (mpPlayButton)
	{
		mpPlayButton->Update();

		if (mpPlayButton->IsClick())
		{
			Master::mpSoundManager->PlaySE(SoundManager::SE_DECIDE);
			Master::mpSceneManager->SetNextScene(SceneManager::SCENE_3D);
		}
	}

	if (mpPlayRuleButton)
	{
		mpPlayRuleButton->Update();

		if (mpPlayRuleButton->IsClick())
		{
			Master::mpSoundManager->PlaySE(SoundManager::SE_DECIDE);
			Master::mpSceneManager->SetNextScene(SceneManager::SCENE_GAME_RULE);
		}
	}

	if (mpQuitButton)
	{
		mpQuitButton->Update();

		if (mpQuitButton->IsClick())
		{
			Master::mpSoundManager->PlaySE(SoundManager::SE_DECIDE);
			Master::mpSceneManager->RequestQuit();
		}
	}

	Scene::Update();
}

// ホラー演出の砂嵐アニメーション、敵キャラクターグラフィック、タイトルロゴ、UIボタンの描画
// 入力: なし / 出力: なし / 副作用: バックバッファへの描画およびScreenFlip
void TitleScene::Draw()
{
	// 6枚の連番砂嵐テクスチャを順次切り替えてノイズアニメーションを表現
	for (int i = 1; i < 7; ++i)
	{
		ClearDrawScreen();

		char Buf[256];
		sprintf(Buf, "Resource/3D_UI/sandStorm%d.png", i);
		noise = LoadGraph(Buf);

		SetBackgroundColor(0, 0, 0);

		DrawGraph(Utility::SCREEN_WIDTH / 2 + 350, Utility::SCREEN_HEIGHT / 2 - 200, enemyPictureHandle, true);
		DrawGraph(0, 0, noise, true);

		if (mpPlayButton)
		{
			mpPlayButton->Draw();
		}

		if (mpPlayRuleButton)
		{
			mpPlayRuleButton->Draw();
		}

		if (mpQuitButton)
		{
			mpQuitButton->Draw();
		}

		DrawStringToHandle(Utility::SCREEN_WIDTH / 4 - 250, Utility::SCREEN_HEIGHT / 2 - 330, "呪われた校舎", GetColor(255, 0, 0), fontSize130);

		ScreenFlip();
		DeleteGraph(noise);
	}

	Scene::Draw();
}

// タイトル画面固有グラフィックリソースの解放およびBGM停止
// 入力: なし / 出力: なし / 副作用: 画像ハンドルの削除、BGM停止
void TitleScene::Finalize()
{
	if (enemyPictureHandle != -1)
	{
		DeleteGraph(enemyPictureHandle);
		enemyPictureHandle = -1;
	}

	Master::mpSoundManager->StopBGM();
}