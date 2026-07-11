#include "ResultScene.h"
#include "DxLib.h"
#include "Utility.h"
#include "Master.h"
#include "InputManager.h"

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
{	// ボタンの生成
	if (mpRetoryBtn == nullptr) mpRetoryBtn = new Button(RetoryBtn.left, RetoryBtn.top, RetoryBtn.right, RetoryBtn.bottom, " リトライ　", GetColor(255, 126, 115), GetColor(250, 250, 250), FontSize20);
	if (mpTitleBtn == nullptr) mpTitleBtn = new Button(TitleBtn.left, TitleBtn.top, TitleBtn.right, TitleBtn.bottom, "タイトルへ　", GetColor(255, 126, 115), GetColor(250, 250, 250), FontSize20);

	// Mouseのロックを解除
	g_MouseMgr.EnableMouseLock(false);
}

// 更新
void ResultScene::Update()
{
	// ボタンの更新処理を呼ぶ
	if (mpRetoryBtn)
	{
		mpRetoryBtn->Update();

		if (mpRetoryBtn->IsClicked())
		{
			NowSelect = select_Retory;
		}

	}

	if (mpTitleBtn)
	{
		mpTitleBtn->Update();

		if (mpTitleBtn->IsClicked())
		{
			NowSelect = select_Title;
		}
	}


	// エンターキーが押されたら画面の切り替え処理
	if (g_MouseMgr.isLeftDown)
	{
		// SE再生
		Master::mpSoundManager->PlaySE(SoundManager::SE_DECIDE);

		switch (NowSelect)
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
void ResultScene::Draw()
{
	// 背景の表示
	SetBackgroundColor(255, 255, 255);

	// ボタンの表示
	if (mpRetoryBtn)
	{
		mpRetoryBtn->Draw();
	}

	if (mpTitleBtn)
	{
		mpTitleBtn->Draw();
	}

	// 文字列の表示 //
	DrawStringToHandle(Utility::SCREEN_WIDTH / 3 - 280, Utility::SCREEN_HEIGHT / 2 - 230, "G A M E C L E A R", GetColor(0, 255, 255), FontSize);

	// 文字列の表示 //
	DrawStringToHandle(Utility::SCREEN_WIDTH / 3 - 160, 950, "マウス移動 : 選択　マウス左クリック:決定", GetColor(255, 0, 0), FontSize50);

	// 基底クラスの描画処理を呼び出す
	Scene::Draw();

}

// 終了処理
void ResultScene::Finalize()
{

}
