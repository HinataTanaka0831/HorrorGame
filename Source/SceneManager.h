#pragma once

// クラスの前方宣言
class Scene;

// シーンライフサイクルの管理および状態遷移を安全に調停するマネージャークラス
class SceneManager
{
public:  // enum, struct の定義

	// シーンの種類
	// note: シーンを増やす必要があれば、ここも追加していくこと
	enum SceneType
	{
		SceneNone = 0,
		SceneTitle,      // タイトル画面
		SceneResult,     // クリアリザルト画面
		SceneGameRule,  // 操作説明画面
		SceneGameOver,   // ゲームオーバー画面
		SceneGame3D,         // 3Dホラーゲーム本編
		// SceneOption,  // オプション
	};


public:  // メンバ関数の定義
	// コンストラクタ
	SceneManager();
	// デストラクタ
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
	// 入力: next(遷移先シーン種別) / 出力: なし / 副作用: m_nextSceneTypeの更新
	void SetNextScene(SceneType next) { m_nextSceneType = next; }

	// ゲームループ終了要求の発行
	// 入力: なし / 出力: なし / 副作用: m_quitRequestフラグのtrue化
	void RequestQuit();

	bool IsQuitRequest() const { return m_quitRequest; }

	// 現在シーンの取得
	Scene* GetCurrentScene() { return m_currentScene; }

private:
	SceneType m_sceneType = SceneType::SceneNone;        // 現在実行中のシーン種別
	SceneType m_nextSceneType = SceneType::SceneNone;    // 遷移予約されている次シーン種別
	Scene* m_currentScene = nullptr;
	bool m_quitRequest = false;    // メインループ終了フラグ
};
