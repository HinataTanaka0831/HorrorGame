#pragma once
#include <string>
#include <vector>
#include <memory>
#include "DxLib.h"

// リソース読み込みおよび初期化処理を抽象化するタスクインターフェース
class ILoadTask {
public:
    virtual ~ILoadTask() = default;

    // タスク本体の実行
    // 入力: なし / 出力: 処理結果ステータス(0:正常, -1:異常, ハンドル値など) / 副作用: リソース生成またはマネージャー初期化
    virtual int Execute() = 0;

    // 進捗UI表示用のタスク名称取得
    // 入力: なし / 出力: タスク名文字列ポインタ / 副作用: なし
    virtual const char* GetTaskName() const = 0;

    virtual int GetHandle() const { return -1; }
};

// サウンドファイルの非同期/キューイング読み込みタスク
class LoadSoundTask : public ILoadTask {
public:
    explicit LoadSoundTask(const char* path);
    int Execute() override;
    const char* GetTaskName() const override;
    int GetHandle() const;
private:
    const char* m_path;
    int m_handle;
};

// サウンドマネージャー初期化タスク
class InitializeSoundManagerTask : public ILoadTask {
public:
    InitializeSoundManagerTask();
    int Execute() override;
    const char* GetTaskName() const override;
};

// シーンマネージャー初期化タスク
class InitializeSceneManagerTask : public ILoadTask {
public:
    InitializeSceneManagerTask();
    int Execute() override;
    const char* GetTaskName() const override;
};

// カメラ初期化タスク
class InitializeCameraTask : public ILoadTask {
public:
    InitializeCameraTask();
    int Execute() override;
    const char* GetTaskName() const override;
};

// 3Dプレイヤーオブジェクト生成タスク
class InitializePlayerTask : public ILoadTask {
public:
    InitializePlayerTask();
    int Execute() override;
    const char* GetTaskName() const override;
};

// 3D敵キャラクター生成およびモーション設定タスク
class InitializeEnemyTask : public ILoadTask {
public:
    InitializeEnemyTask();
    int Execute() override;
    const char* GetTaskName() const override;
};

// 背景スカイボックス生成タスク
class InitializeSkyBoxTask : public ILoadTask {
public:
    InitializeSkyBoxTask();
    int Execute() override;
    const char* GetTaskName() const override;
};

// ステージコリジョンおよびマップモデル生成タスク
class InitializeStageTask : public ILoadTask {
public:
    InitializeStageTask();
    int Execute() override;
    const char* GetTaskName() const override;
};

// 脱出アイテムのランダム配置タスク
class InitializeEscapeItemTask : public ILoadTask {
public:
    InitializeEscapeItemTask();
    int Execute() override;
    const char* GetTaskName() const override;

private:
    struct Point {
        VECTOR PointPosition;
    };
};

// 制限時間延長アイテム配置タスク
class InitializeTimeItemTask : public ILoadTask {
public:
    InitializeTimeItemTask();
    int Execute() override;
    const char* GetTaskName() const override;
};

// 脱出用ドアオブジェクト生成タスク
class InitializeExitDoorTask : public ILoadTask {
public:
    InitializeExitDoorTask();
    int Execute() override;
    const char* GetTaskName() const override;
};

// ロードタスクを順次実行し、進捗バーとタスク名をリアルタイム描画する管理クラス
class LoadingManager {
private:
    std::vector<std::unique_ptr<ILoadTask>> tasks;
    std::vector<std::unique_ptr<ILoadTask>> m_tasks;
    int Scene3D_GameRuleHandle = -1;
    int FontSize = CreateFontToHandle(NULL, 30, -1, -1);

public:
    // 実行キューへのタスク追加
    // 入力: task(タスクポインタ) / 出力: なし / 副作用: 内部タスクリストへの追加
    void AddTask(std::unique_ptr<ILoadTask> task);

    // 全タスクを順次実行しプログレスバーを更新描画
    // 入力: なし / 出力: なし / 副作用: 各タスク実行および画面描画・フリップ
    void ExecuteAll();

    // 3Dシーン用ルール画像背景付きローディング画面の描画実行
    // 入力: なし / 出力: なし / 副作用: 各タスク実行、テクスチャロード・破棄、画面描画
    void ExecuteScene3D();

    const std::vector<std::unique_ptr<ILoadTask>>& GetTasks() const {
        return tasks;
    }
};