#include "ResultScene.h"
#include "DxLib.h"
#include "Utility.h"
#include "Master.h"
#include "InputManager.h"
#include "Button.h"

ResultScene::ResultScene()
	: Scene()
{
}

ResultScene::~ResultScene()
{
}

// リトライボタン・タイトルボタンの生成およびマウスロック解除
// 入力: なし / 出力: なし / 副作用: Buttonインスタンスの生成
void ResultScene::Initialize()
{
	if (mpRetoryButton == nullptr)
	{
		mpRetoryButton = std::make_unique<Button>(DrawX, RetoryY - 10, DrawX + 250, RetoryY + 60, "リトライ", GetColor(255, 126, 115), GetColor(250, 250, 250), fontSize20);
	}

	if (mpTitleButton == nullptr)
	{
		mpTitleButton = std::make_unique<Button>(DrawX, TitleY - 10, DrawX + 250, TitleY + 60, "タイトルへ", GetColor(255, 126, 115), GetColor(250, 250, 250), fontSize20);
	}

	// UIボタン操作のためマウスロックを解除
	InputManager::GetInstance().EnableMouseLock(false);
}

// ボタン入力検知とシーン遷移要求
// 入力: なし / 出力: なし / 副作用: SE再生、SceneManagerへの遷移予約
void ResultScene::Update()
{
	if (mpRetoryButton)
	{
		mpRetoryButton->Update();

		if (mpRetoryButton->IsClick())
		{
			Master::mpSoundManager->PlaySE(SoundManager::SE_DECIDE);
			Master::mpSceneManager->SetNextScene(SceneManager::SCENE_3D);
		}
	}

	if (mpTitleButton)
	{
		mpTitleButton->Update();

		if (mpTitleButton->IsClick())
		{
			Master::mpSoundManager->PlaySE(SoundManager::SE_DECIDE);
			Master::mpSceneManager->SetNextScene(SceneManager::SCENE_TITLE);
		}
	}

	Scene::Update();
}

// クリアメッセージおよびUIボタンの描画
// 入力: なし / 出力: なし / 副作用: バックバッファへの描画
void ResultScene::Draw()
{
	SetBackgroundColor(255, 255, 255);

	DrawStringToHandle(Utility::SCREEN_WIDTH / 2 - 150, Utility::SCREEN_HEIGHT / 2 - 140, "Game Clear", GetColor(255, 255, 255), fontSize90);

	if (mpRetoryButton)
	{
		mpRetoryButton->Draw();
	}

	if (mpTitleButton)
	{
		mpTitleButton->Draw();
	}

	Scene::Draw();
}

void ResultScene::Finalize()
{
}