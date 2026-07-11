#pragma once
#include <string>
#include <vector>
#include <memory>
#include "DxLib.h"


// ========== ILoadTaskインタフェース ==========
// ILoadTask はタスクの基底クラス（例）
class ILoadTask {
public:
    virtual ~ILoadTask() = default;
    virtual int Execute() = 0;
    virtual const char* GetTaskName() const = 0;
    virtual int GetHandle() const { return -1; }
};



// ========== サウンド読み込みタスク ==========
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


// ========== 初期化タスク ==========
class InitializeSoundManagerTask : public ILoadTask {
public:
    InitializeSoundManagerTask();
    int Execute() override;
    const char* GetTaskName() const override;
};

class InitializeSceneManagerTask : public ILoadTask {
public:
    InitializeSceneManagerTask();
    int Execute() override;
    const char* GetTaskName() const override;
};

class InitializeCameraTask : public ILoadTask {
public:
    InitializeCameraTask();
    int Execute() override;
    const char* GetTaskName() const override;
};



// ========== 初期化Scene3Dタスク ==========
class InitializePlayerTask : public ILoadTask {
public:
    InitializePlayerTask();
    int Execute() override;
    const char* GetTaskName() const override;
};


class InitializeEnemyTask : public ILoadTask {
public:
    InitializeEnemyTask();
    int Execute() override;
    const char* GetTaskName() const override;
};


class InitializeSkyBoxTask : public ILoadTask {
public:
    InitializeSkyBoxTask();
    int Execute() override;
    const char* GetTaskName() const override;
};


class InitializeStageTask : public ILoadTask {
public:
    InitializeStageTask();
    int Execute() override;
    const char* GetTaskName() const override;
};


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


class InitializeTimeItemTask : public ILoadTask {
public:
    InitializeTimeItemTask();
    int Execute() override;
    const char* GetTaskName() const override;
};


class InitializeExitDoorTask : public ILoadTask {
public:
    InitializeExitDoorTask();
    int Execute() override;
    const char* GetTaskName() const override;
};




// ========== ローディングマネージャー ==========
class LoadingManager {
private:
    std::vector<std::unique_ptr<ILoadTask>> tasks;
    std::vector<std::unique_ptr<ILoadTask>> m_tasks;
    int Scene3D_GameRuleHandle = -1;
    int FontSize = CreateFontToHandle(NULL, 30, -1, -1);

public:
    void AddTask(std::unique_ptr<ILoadTask> task);

    void ExecuteAll();

    void ExecuteScene3D();

    // ここを追加
    const std::vector<std::unique_ptr<ILoadTask>>& GetTasks() const {
        return tasks;
    }
};
