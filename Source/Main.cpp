#include "DxLib.h"
#include "SceneManager.h"
#include "ObjectManager.h"
#include "InputManager.h"
#include "Master.h"
#include "Scene.h"
#include "Utility.h"
#include "Camera.h"
#include "Loading.h"
#include <memory>
#include <math.h>
#include "Button.h"

/* @note リファレンス https://dxlib.xsrv.jp/dxfunc.html
*/

// Master クラスの静的メンバ変数定義
SceneManager* Master::m_sceneManager = new SceneManager();
SoundManager* Master::m_soundManager = new SoundManager();
Camera* Master::m_camera = new Camera();
// InputManager クラスのシングルトンインスタンス取得
InputManager& input = InputManager::GetInstance();

/**
* @fn WinMain
* @brief Main関数
* @param[in] HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow
* @return int 0 正常終了／-1 エラー
* @details Main関数
*/
int WINAPI  WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	// ウインドウモードで起動
	ChangeWindowMode(false);

	// 画面サイズ調整
	SetGraphMode(Utility::SCREEN_WIDTH, Utility::SCREEN_HEIGHT, 32);

	SetUsePixelLighting(true); // ピクセル単位でのライティングを有効にする

	// DXライブラリ初期化
	if(DxLib_Init() == -1)
	{
		return -1;
	}



	// 描画先画面を裏画面に設定する
	SetDrawScreen(DX_SCREEN_BACK);


	// ローディングマネージャーの作成
	LoadingManager loader;

	// タスクを追加
	loader.AddTask(std::make_unique<InitializeSoundManagerTask>());
	loader.AddTask(std::make_unique<InitializeSceneManagerTask>());
	loader.AddTask(std::make_unique<InitializeCameraTask>());


	// ローディング実行
	loader.ExecuteAll();


	// Zバッファに書き込む準備
	SetUseZBufferFlag(true);
	SetWriteZBufferFlag(true);


	// --------------------
	// ライト設定
	// --------------------
	SetUseLighting(TRUE);  // ライト処理ON

	// アンビエント（環境光）は少し暗め
	SetGlobalAmbientLight(GetColorF(0.2f, 0.2f, 0.2f, 0.0f));
	


	// ゲームのメインループ
	// ProcessMessage() == 0 -> ウィンドウの×ボタンを押されていないかどうか
	// CheckHitKey(KEY_INPUT_ESCAPE) == 0 -> エスケープキーが押されていないかどうか
	int animationCounter = 0;
	int textureCurrentNum = 0;
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		int time = GetNowCount();

		// 画面を初期化する
		ClearDrawScreen();

		// カメラの更新
		Master::m_camera->Update();

		// 更新
		Master::m_sceneManager->Update();

		// 毎フレームのクリック・リリース状態を検知するため入力状態を更新
		input.MouseUpdate();

		// 描画
		Master::m_sceneManager->Draw();
		
		// ゲームループの終了フラグがtrueならループを抜ける
		if (Master::m_sceneManager->IsQuitRequest())
		{
			break;
		}

		// ゲーム画面のスクリーンショットを撮影する処理
		if (input.CheckDownKey(KEY_INPUT_F1))
		{
			SaveDrawScreen(0, 0, Utility::SCREEN_WIDTH, Utility::SCREEN_HEIGHT, "screenshot.bmp");
		}


		// 裏画面の内容を表画面に映す
		ScreenFlip();


		// 17ミリ秒（秒間約60フレームだった場合の1フレーム当たりの経過時間）
		// 経過するまでここで待つ
		while (GetNowCount() - time < 17)
		{
			// 待つだけなのでここでは何も処理はしない
		}



		// 削除する必要のあるオブジェクトがあれば削除する
		Master::m_sceneManager->GetCurrentScene()->GetObjectManager()->DeleteAll2DIfNeeded();
		Master::m_sceneManager->GetCurrentScene()->GetObjectManager()->DeleteAll3DIfNeeded();

		// ループする直前にシーン遷移チェックを入れておく
		Master::m_sceneManager->ChangeSceneIfNeeded();

	}




	// 終了処理
	//Finalize();
	Master::m_sceneManager->Finalize();
	delete Master::m_sceneManager;
	Master::m_soundManager->Finalize();
	delete Master::m_soundManager;


	// DXライブラリ使用の終了
	DxLib_End();

	// ソフトの終了
	return 0;
}


