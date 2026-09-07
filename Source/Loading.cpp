#include "Loading.h"
#include <string>
#include <cstdio>
#include "Master.h"
#include "Scene3D.h"
#include "Scene.h"
#include "ObjectManager.h"



// ========================
// LoadSoundTask 実装
// ========================

LoadSoundTask::LoadSoundTask(const char* path)
    : m_path(path), m_handle(-1)
{}

int LoadSoundTask::Execute() {
    m_handle = LoadSoundMem(m_path);
    if (m_handle == -1) {
        // 読み込み失敗時はログなどに出しても良い
        char buf[256];
        //std::sprintf(buf, "Failed to load sound: %s", m_path);
        OutputDebugStringA(buf);
    }

    return m_handle;
}

const char* LoadSoundTask::GetTaskName() const {
    return m_path;
}

int LoadSoundTask::GetHandle() const {
    return m_handle;
}


// ========================
// InitializeSoundManagerTask 実装
// ========================

InitializeSoundManagerTask::InitializeSoundManagerTask() {}

int InitializeSoundManagerTask::Execute() {
    // サウンドマネージャーの初期化処理をここに書く
    
    // サウンドマネージャーの初期化
    Master::m_soundManager->Initialize();    // 全てのサウンドが読み込まれる

    // 今はダミーで待機だけ
    //WaitTimer(100);

    return 0;
}

const char* InitializeSoundManagerTask::GetTaskName() const {
    return "Initialize Sound Manager";
}


// ========================
// InitializeSceneManagerTask 実装
// ========================

InitializeSceneManagerTask::InitializeSceneManagerTask() {}

int InitializeSceneManagerTask::Execute() {
    // シーンマネージャーの初期化処理をここに書く
    
    // シーンマネージャーの初期化
    Master::m_sceneManager->Initialize();

    // 今はダミーで待機だけ
    //WaitTimer(100);

    return 0;
}

const char* InitializeSceneManagerTask::GetTaskName() const {
    return "Initialize Scene Manager";
}


// ========================
// InitializeCameraTask 実装
// ========================

InitializeCameraTask::InitializeCameraTask() {}

int InitializeCameraTask::Execute() {


    // カメラ生成
    Master::m_camera->Initialize();   // 初期化

    return 0;
}

const char* InitializeCameraTask::GetTaskName() const {
    return "Initialize Camera";
}




// ========================
// InitializePlayerTask 実装
// ========================

InitializePlayerTask::InitializePlayerTask() {}

int InitializePlayerTask::Execute(){

    Scene3D* pScene3D = dynamic_cast<Scene3D*>(Master::m_sceneManager->GetCurrentScene());

    if (pScene3D != nullptr)
    {
        pScene3D->InitializePlayerData();
    }

    return 0;
}

const char* InitializePlayerTask::GetTaskName() const {
    return "Initialize Player";
}


// ========================
// InitializeEnemyTask 実装
// ========================

InitializeEnemyTask::InitializeEnemyTask() {}

int InitializeEnemyTask::Execute(){

    Scene3D* pScene3D = dynamic_cast<Scene3D*>(Master::m_sceneManager->GetCurrentScene());

    if (pScene3D != nullptr)
    {
        pScene3D->InitializeEnemyData();
    }

    return 0;
}

const char* InitializeEnemyTask::GetTaskName() const {
    return "Initialize Enemy";
}


// ========================
// InitializeItemTask 実装
// ========================

InitializeItemTask::InitializeItemTask() {}

int InitializeItemTask::Execute() {

    Scene3D* pScene3D = dynamic_cast<Scene3D*>(Master::m_sceneManager->GetCurrentScene());

    if (pScene3D != nullptr)
    {
        pScene3D->InitializeItemData();
    }

    return 0;
}

const char* InitializeItemTask::GetTaskName() const {
    return "Initialize Item";
}


// ========================
// InitializeSkyBoxTask 実装
// ========================

InitializeSkyBoxTask::InitializeSkyBoxTask() {}

int InitializeSkyBoxTask::Execute() {

    Scene3D* pScene3D = dynamic_cast<Scene3D*>(Master::m_sceneManager->GetCurrentScene());

    if (pScene3D != nullptr)
    {
        pScene3D->InitializeSkyBoxData();
    }

    return 0;
}

const char* InitializeSkyBoxTask::GetTaskName() const {
    return "Initialize SkyBox";
}


// ========================
// InitializeStageTask 実装
// ========================

InitializeStageTask::InitializeStageTask() {}

int InitializeStageTask::Execute() {

    Scene3D* pScene3D = dynamic_cast<Scene3D*>(Master::m_sceneManager->GetCurrentScene());

    if (pScene3D != nullptr)
    {
        pScene3D->InitializeStageData();
    }

    return 0;
}

const char* InitializeStageTask::GetTaskName() const {
    return "Initialize Stage";
}


// ========================
// InitializeExitDoorTask 実装
// ========================

InitializeExitDoorTask::InitializeExitDoorTask() {}

int InitializeExitDoorTask::Execute() {

    Scene3D* pScene3D = dynamic_cast<Scene3D*>(Master::m_sceneManager->GetCurrentScene());

    if (pScene3D != nullptr)
    {
        pScene3D->InitializeExitDoorData();
    }

    return 0;
}

const char* InitializeExitDoorTask::GetTaskName() const {
    return "Initialize ExitDoor";
}



// ========================
// LoadingManager 実装
// ========================

void LoadingManager::AddTask(std::unique_ptr<ILoadTask> task) {
    m_tasks.push_back(std::move(task));
}

void LoadingManager::ExecuteAll() {
    const int total = static_cast<int>(m_tasks.size());

    for (int i = 0; i < total; ++i) {
        ClearDrawScreen();

        float progress = static_cast<float>(i) / total;

        // 進捗バーの描画
        const int barX = 1280, barY = 950, barWidth = 580, barHeight = 40;
        DrawBox(barX, barY, barX + barWidth, barY + barHeight, GetColor(255, 255, 255), FALSE);

        int filledWidth = static_cast<int>(barWidth * progress);
        DrawBox(barX, barY, barX + filledWidth, barY + barHeight, GetColor(100, 200, 255), TRUE);

        // ローディング文字列
        std::string loadingText = "Loading: ";
        loadingText += m_tasks[i]->GetTaskName();

        DrawStringToHandle(1280, 900, loadingText.c_str(), GetColor(255, 255, 255), m_fontSize);

        ScreenFlip();

        // タスク実行
        m_tasks[i]->Execute();

        // 少し待つ（UI更新時間）
        WaitTimer(100);
    }

    // 完了画面描画
    ClearDrawScreen();

    DrawBox(1280, 950, 1280 + 580, 990, GetColor(255, 255, 255), FALSE);
    DrawBox(1280, 950, 1280 + 580, 990, GetColor(100, 200, 255), TRUE);

    DrawStringToHandle(1280, 900, "Loading Complete!", GetColor(255, 255, 255), m_fontSize);

    ScreenFlip();

    WaitTimer(300);
}

void LoadingManager::ExecuteScene3D()
{
    const int total = static_cast<int>(m_tasks.size());

    for (int i = 0; i < total; ++i)
    {
        ClearDrawScreen();

       if (m_scene3DGameRuleHandle == -1) m_scene3DGameRuleHandle = LoadGraph("Resource/3D_UI/GameRulePicture.png");

        DrawGraph(0, 0, m_scene3DGameRuleHandle, false);

        float progress = static_cast<float>(i) / total;

        // 進行バー
        const int barX = 1280, barY = 950, barWidth = 580, barHeight = 40;
        DrawBox(barX, barY, barX + barWidth, barY + barHeight, GetColor(255, 255, 255), FALSE);

        int filledWidth = static_cast<int>(barWidth * progress);
        DrawBox(barX, barY, barX + filledWidth, barY + barHeight, GetColor(100, 200, 255), TRUE);

        ScreenFlip();

        // タスク実行
        m_tasks[i]->Execute();

        // 少し待つ
        WaitTimer(100);
    }

    // 完了画面描画
    ClearDrawScreen();

    DrawBox(1280, 950, 1280 + 580, 990, GetColor(255, 255, 255), FALSE);
    DrawBox(1280, 950, 1280 + 580, 990, GetColor(100, 200, 255), TRUE);

   if (m_scene3DGameRuleHandle != -1) DeleteGraph(m_scene3DGameRuleHandle);

    ScreenFlip();

    WaitTimer(300);
}