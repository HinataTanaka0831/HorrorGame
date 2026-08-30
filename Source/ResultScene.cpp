#include "ResultScene.h"
#include "DxLib.h"
#include "Utility.h"
#include "Master.h"
#include "InputManager.h"
#include "Button.h"

// コンストラクタ
ResultScene::ResultScene()
	: Scene()     // 基底クラスのコンストラクタを呼び出す
{

}

// デストラクタ
ResultScene::~ResultScene()
{

}

// 初期化
void ResultScene::Initialize()
{
	// ボタンの生成
	if (mpRetoryButton == nullptr)
	{
		mpRetoryButton = std::make_unique<Button>(StringX, RetoryY - 10, StringX + 250, RetoryY + 60, "リトライ", GetColor(255, 126, 115), GetColor(250, 250, 250), fontSize20);
	}

	if (mpTitleButton == nullptr)
	{
		mpTitleButton = std::make_unique<Button>(StringX, TitleY - 10, StringX + 250, TitleY + 60, "タイトルへ", GetColor(255, 126, 115), GetColor(250, 250, 250), fontSize20);
	}

	// Mouseのロックを解除
	InputManager::GetInstance().EnableMouseLock(false);
}

// 更新
void ResultScene::Update()
{
	// ボタンの更新処理を呼ぶ
	if (mpRetoryButton)
	{
		mpRetoryButton->Update();

		if (mpRetoryButton->IsClick())
		{
			// SE再生
			Master::mpSoundManager->PlaySE(SoundManager::SE_DECIDE);
			Master::mpSceneManager->SetNextScene(SceneManager::SCENE_3D);
		}

	}

	if (mpTitleButton)
	{
		mpTitleButton->Update();

		if (mpTitleButton->IsClick())
		{
			// SE再生
			Master::mpSoundManager->PlaySE(SoundManager::SE_DECIDE);
			Master::mpSceneManager->SetNextScene(SceneManager::SCENE_TITLE);
		}
	}

	// 基底クラスの更新処理を呼び出す
	Scene::Update();

}

// 描画
void ResultScene::Draw()
{
	// 背景の表示
	SetBackgroundColor(255, 255, 255);

	// 文字列の表示 //
	DrawStringToHandle(Utility::SCREEN_WIDTH / 2 - 150, Utility::SCREEN_HEIGHT / 2 - 140, "Game Clear", GetColor(255, 255, 255), fontSize90);

	// ボタンの表示
	if (mpRetoryButton)
	{
		mpRetoryButton->Draw();
	}

	if (mpTitleButton)
	{
		mpTitleButton->Draw();
	}

	// 基底クラスの描画処理を呼び出す
	Scene::Draw();

}

// 終了処理
void ResultScene::Finalize()
{

}
