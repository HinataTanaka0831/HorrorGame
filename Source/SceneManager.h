#pragma once

class Scene;

// シーンライフサイクルの管理および状態遷移を安全に調停するマネージャークラス
class SceneManager
{
public:
	enum SCENE_TYPE
	{
		SCENE_NONE = 0,
		SCENE_TITLE,      // タイトル画面
		SCENE_RESULT,     // クリアリザルト画面
		SCENE_GAME_RULE,  // 操作説明画面
		SCENE_GAMEOVER,   // ゲームオーバー画面
		SCENE_3D,         // 3Dホラーゲーム本編
	};

public:
	SceneManager();
	~SceneManager();

	// 初期シーンの設定と起動
	// 入力: なし / 出力: なし / 副作用: タイトルシーンの生成と初期化
	void Initialize();

	// 現在アクティブなシーンの更新
	// 入力: なし / 出力: なし / 副作用: 現在シーンのUpdate呼び出し
	void Update();

	// 現在アクティブなシーンの描画
	// 入力: なし / 出力: なし / 副作用: 現在シーンのDraw呼び出し
	void Draw();

	// シーンマネージャーの終了処理
	// 入力: なし / 出力: なし / 副作用: なし
	void Finalize();

	// 次のシーンへの遷移要求があれば旧シーン破棄と新シーン生成を実行
	// 入力: なし / 出力: なし / 副作用: 旧シーンFinalize/delete、新シーンnew/Initialize
	void ChangeSceneIfNeeded();

	// 次フレームで遷移するシーン種別の予約設定
	// 入力: next(遷移先シーン種別) / 出力: なし / 副作用: mnNextSceneTypeの更新
	void SetNextScene(SCENE_TYPE next) { mnNextSceneType = next; }

	// ゲームループ終了要求の発行
	// 入力: なし / 出力: なし / 副作用: mbQuitRequestフラグのtrue化
	void RequestQuit();

	bool IsQuitRequest() const { return mbQuitRequest; }

	Scene* GetCurrentScene() { return mpCurrentScene; }

private:
	SCENE_TYPE mnSceneType;        // 現在実行中のシーン種別
	SCENE_TYPE mnNextSceneType;    // 遷移予約されている次シーン種別
	Scene* mpCurrentScene = nullptr;

	bool mbQuitRequest = false;    // メインループ終了フラグ
};