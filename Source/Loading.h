#pragma once
#include <string>
#include <vector>
#include <memory>
#include "DxLib.h"


// ========== ILoadTaskインタフェース ==========
// ILoadTask はタスクの基底クラス（例）

// これを継承して、各タスクの実装を作成する
class ILoadTask {
public:
	// デストラクタは仮想関数にすることで、派生クラスのデストラクタが正しく呼ばれるようにする
	// 入力: なし / 出力: なし / 副作用: 派生クラスのデストラクタが呼ばれる
    virtual ~ILoadTask() = default;

	// Execute はタスクの実行を行う純粋仮想関数
	// 入力: なし / 出力: 成功時は0、失敗時は負の値 / 副作用: タスクの実行
    virtual int Execute() = 0;

	// GetTaskName はタスクの名前を取得する純粋仮想関数
	// 入力: なし / 出力: タスクの名前 / 副作用: なし
    virtual const char* GetTaskName() const = 0;

	// GetHandle はタスクのハンドルを取得する仮想関数
	// 入力: なし / 出力: タスクのハンドル / 副作用: なし
    virtual int GetHandle() const { return -1; }
};



// ========== サウンド読み込みタスク ==========

// LoadSoundTask はサウンドファイルを読み込むタスクの実装
class LoadSoundTask : public ILoadTask {
public:
	// コンストラクタで読み込むサウンドファイルのパスを指定
	// 入力: path(サウンドファイルのパス) / 出力: なし / 副作用: m_pathにパスを保存
    explicit LoadSoundTask(const char* path);

	// Execute はサウンドファイルを読み込み、ハンドルを取得する
	// 入力: なし / 出力: 成功時は0、失敗時は負の値 / 副作用: サウンドファイルの読み込み
    int Execute() override;

	// GetTaskName はタスクの名前を取得する
	// 入力: なし / 出力: タスクの名前 / 副作用: なし
    const char* GetTaskName() const override;

	// GetHandle は読み込んだサウンドのハンドルを取得する
	// 入力: なし / 出力: サウンドのハンドル / 副作用: なし
    int GetHandle() const;
private:
	const char* m_path; // 読み込むサウンドファイルのパス
	int m_handle;       // 読み込んだサウンドのハンドル
};


// ========== 初期化タスク ==========

// InitializeSoundManagerTask はサウンドマネージャーの初期化を行うタスクの実装
class InitializeSoundManagerTask : public ILoadTask {
public:
	// コンストラクタ
    InitializeSoundManagerTask();

	// Execute はサウンドマネージャーの初期化を行う
	// 入力: なし / 出力: 成功時は0、失敗時は負の値 / 副作用: サウンドマネージャーの初期化
    int Execute() override;
    
	// GetTaskName はタスクの名前を取得する
	// 入力: なし / 出力: タスクの名前 / 副作用: なし
    const char* GetTaskName() const override;
};

// InitializeSceneManagerTask はシーンマネージャーの初期化を行うタスクの実装
class InitializeSceneManagerTask : public ILoadTask {
public:
	// コンストラクタ
    InitializeSceneManagerTask();

	// Execute はシーンマネージャーの初期化を行う
	// 入力: なし / 出力: 成功時は0、失敗時は負の値 / 副作用: シーンマネージャーの初期化
    int Execute() override;

	// GetTaskName はタスクの名前を取得する
	// 入力: なし / 出力: タスクの名前 / 副作用: なし
    const char* GetTaskName() const override;
};

// InitializeCameraTask はカメラの初期化を行うタスクの実装
class InitializeCameraTask : public ILoadTask {
public:
	// コンストラクタ
    InitializeCameraTask();
    
	// Execute はカメラの初期化を行う
	// 入力: なし / 出力: 成功時は0、失敗時は負の値 / 副作用: カメラの初期化
    int Execute() override;

	// GetTaskName はタスクの名前を取得する
	// 入力: なし / 出力: タスクの名前 / 副作用: なし
    const char* GetTaskName() const override;
};



// ========== 初期化Scene3Dタスク ==========

// InitializePlayerTask はプレイヤーの初期化を行うタスクの実装
class InitializePlayerTask : public ILoadTask {
public:
	// コンストラクタ
    InitializePlayerTask();

	// Execute はプレイヤーの初期化を行う
	// 入力: なし / 出力: 成功時は0、失敗時は負の値 / 副作用: プレイヤーの初期化
    int Execute() override;
    
	// GetTaskName はタスクの名前を取得する
	// 入力: なし / 出力: タスクの名前 / 副作用: なし
    const char* GetTaskName() const override;
};

// InitializeEnemyTask は敵の初期化を行うタスクの実装
class InitializeEnemyTask : public ILoadTask {
public:
	// コンストラクタ
    InitializeEnemyTask();
    
	// Execute は敵の初期化を行う
	// 入力: なし / 出力: 成功時は0、失敗時は負の値 / 副作用: 敵の初期化
    int Execute() override;

	// GetTaskName はタスクの名前を取得する
	// 入力: なし / 出力: タスクの名前 / 副作用: なし
    const char* GetTaskName() const override;
};

// InitializeItemTask はアイテムの初期化を行うタスクの実装
class InitializeItemTask : public ILoadTask {
public:
	// コンストラクタ
    InitializeItemTask();
    
	// Execute はアイテムの初期化を行う
	// 入力: なし / 出力: 成功時は0、失敗時は負の値 / 副作用: アイテムの初期化
    int Execute() override;

	// GetTaskName はタスクの名前を取得する
	//  入力: なし / 出力: タスクの名前 / 副作用: なし
    const char* GetTaskName() const override;
};

// InitializeSkyBoxTask はスカイボックスの初期化を行うタスクの実装
class InitializeSkyBoxTask : public ILoadTask {
public:
	// コンストラクタ
    InitializeSkyBoxTask();
    
	// Execute はスカイボックスの初期化を行う
	// 入力: なし / 出力: 成功時は0、失敗時は負の値 / 副作用: スカイボックスの初期化
    int Execute() override;

	// GetTaskName はタスクの名前を取得する
	// 入力: なし / 出力: タスクの名前 / 副作用: なし
    const char* GetTaskName() const override;
};

// InitializeStageTask はステージの初期化を行うタスクの実装
class InitializeStageTask : public ILoadTask {
public:
	// コンストラクタ
    InitializeStageTask();
	
	// Execute はステージの初期化を行う
	// 入力: なし / 出力: 成功時は0、失敗時は負の値 / 副作用: ステージの初期化
    int Execute() override;

	// GetTaskName はタスクの名前を取得する
	// 入力: なし / 出力: タスクの名前 / 副作用: なし
    const char* GetTaskName() const override;
};

// InitializeExitDoorTask は出口ドアの初期化を行うタスクの実装
class InitializeExitDoorTask : public ILoadTask {
public:
	// コンストラクタ
    InitializeExitDoorTask();
    
	// Execute は出口ドアの初期化を行う
	// 入力: なし / 出力: 成功時は0、失敗時は負の値 / 副作用: 出口ドアの初期化
    int Execute() override;

	// GetTaskName はタスクの名前を取得する
	// 入力: なし / 出力: タスクの名前 / 副作用: なし
    const char* GetTaskName() const override;
};




// ========== ローディングマネージャー ==========

// LoadingManager はロードタスクを管理し、順番に実行するクラス
class LoadingManager {
private:
	std::vector<std::unique_ptr<ILoadTask>> tasks;          // ロードタスクのリスト
	std::vector<std::unique_ptr<ILoadTask>> m_tasks;        // ロードタスクのリスト（ExecuteAll用）
	int m_scene3DGameRuleHandle = -1;                       // 3Dシーン用のゲームルール画像ハンドル
	int m_fontSize = CreateFontToHandle(NULL, 30, -1, -1);  // フォントサイズのハンドル

public:
	
	// AddTask はロードタスクを追加する
	// 入力: task(ロードタスクのポインタ) / 出力: なし / 副作用: tasksにタスクを追加
    void AddTask(std::unique_ptr<ILoadTask> task);

	// ExecuteAll は追加された全てのロードタスクを順番に実行する
	// 入力: なし / 出力: なし / 副作用: tasksの全タスクを実行
    void ExecuteAll();

	// ExecuteScene3D は3Dシーン用のロードタスクを順番に実行する
	// 入力: なし / 出力: なし / 副作用: tasksの全タスクを実行
    void ExecuteScene3D();

    // ここを追加
	// GetTasks はロードタスクのリストを取得する
	// 入力: なし / 出力: ロードタスクのリスト / 副作用: なし
    const std::vector<std::unique_ptr<ILoadTask>>& GetTasks() const {
        return tasks;
    }
};
