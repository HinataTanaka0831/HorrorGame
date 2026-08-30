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

// 各種グローバル管理インスタンスの生成と保持
SceneManager* Master::mpSceneManager = new SceneManager();
SoundManager* Master::mpSoundManager = new SoundManager();
Camera* Master::mpCamera = new Camera();
InputManager& input = InputManager::GetInstance();

// アプリケーションのエントリポイントおよびメインゲームループの制御
// 入力: hInstance, hPrevInstance, lpCmdLine, nCmdShow / 出力: 0(正常終了), -1(初期化失敗) / 副作用: ウィンドウ生成、DXライブラリ駆動、各マネージャー初期化・破棄
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	// 没入感向上のためフルスクリーンモードで起動
	ChangeWindowMode(false);

	SetGraphMode(Utility::SCREEN_WIDTH, Utility::SCREEN_HEIGHT, 32);

	// 3Dホラー演出の陰影品質向上のためピクセルライティングを有効化
	SetUsePixelLighting(true);

	if (DxLib_Init() == -1)
	{
		return -1;
	}

	// 画面チラつき（ティアリング）防止のため裏画面描画を設定
	SetDrawScreen(DX_SCREEN_BACK);

	// 起動時負荷の分散および初期化順序担保のためタスクキューで初期化
	LoadingManager loader;
	loader.AddTask(std::make_unique<InitializeSoundManagerTask>());
	loader.AddTask(std::make_unique<InitializeSceneManagerTask>());
	loader.AddTask(std::make_unique<InitializeCameraTask>());
	loader.ExecuteAll();

	// 3Dモデル描画時の深度関係を正しく判定するためZバッファを有効化
	SetUseZBufferFlag(true);
	SetWriteZBufferFlag(true);

	SetUseLighting(TRUE);

	// ホラー演出の暗闇を表現するため環境光を低輝度に設定
	SetGlobalAmbientLight(GetColorF(0.2f, 0.2f, 0.2f, 0.0f));

	int animationCounter = 0;
	int textureCurrentNum = 0;

	// OSメッセージ処理失敗またはESCキー押下でゲームループを終了
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		int time = GetNowCount();

		ClearDrawScreen();

		Master::mpCamera->Update();
		Master::mpSceneManager->Update();

		// 毎フレームのクリック・トリガー判定を正確に取得するため更新
		input.MouseUpdate();

		Master::mpSceneManager->Draw();
		
		if (Master::mpSceneManager->IsQuitRequest())
		{
			break;
		}

		// デバッグ・記録用スクリーンショット保存機能 (F1キー)
		if (input.CheckDownKey(KEY_INPUT_F1))
		{
			SaveDrawScreen(0, 0, Utility::SCREEN_WIDTH, Utility::SCREEN_HEIGHT, "screenshot.bmp");
		}

		ScreenFlip();

		// 60FPS（約16.6ms/フレーム）の描画レート維持およびCPU過負荷防止のための待機
		while (GetNowCount() - time < 17)
		{
		}

		// 参照中の不正アクセスを防ぐためフレーム末尾で不要オブジェクト削除とシーン遷移を実行
		Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->DeleteAll2DIfNeeded();
		Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->DeleteAll3DIfNeeded();
		Master::mpSceneManager->ChangeSceneIfNeeded();
	}

	// 各マネージャーの動的メモリ解放
	Master::mpSceneManager->Finalize();
	delete Master::mpSceneManager;
	Master::mpSoundManager->Finalize();
	delete Master::mpSoundManager;

	DxLib_End();
	return 0;
}


