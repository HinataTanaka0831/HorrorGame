#include "Loading.h"
#include "DxLib.h"
#include <string>
#include <cstdio>
#include "Master.h"
#include "Scene3D.h"
#include "Player3D.h"
#include "Enemy3D.h"
#include "Stage.h"
#include "EscapeItem.h"
#include "TimeItem.h"
#include "Exitdoor.h"
#include "SkyBox.h"
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
    Master::mpSoundManager->Initialize();    // 全てのサウンドが読み込まれる

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
    Master::mpSceneManager->Initialize();

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
    Master::mpCamera->Initialize();   // 初期化

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

    // プレイヤーの生成
    new Player3D(VGet(-4000.0f, 82.0f, 1700.0f));

    return 0;
}

const char* InitializePlayerTask::GetTaskName() const {
    return "Initialize Player3D";
}



// ========================
// InitializeEnemyTask 実装
// ========================

InitializeEnemyTask::InitializeEnemyTask() {}

int InitializeEnemyTask::Execute() {

    auto enemy = new Enemy3D(
        "Resource/Mixamo3DModel/Whiteclown_N_Hallin.mv1",
        VGet(-50.0f, 70.0f, 1700.0f),
        1,
        true
    );
    enemy->AddAnimation(ANIMATION_NEUTRAL, "Resource/Mixamo3DModel/Idle.mv1");
    enemy->AddAnimation(ANIMATION_WALKING, "Resource/Mixamo3DModel/Walking.mv1");
    enemy->AddAnimation(ANIMATION_RUN, "Resource/Mixamo3DModel/Running.mv1");

    auto enemy2 = new Enemy3D(
        "Resource/Mixamo3DModel/Whiteclown_N_Hallin.mv1",
        VGet(1000.0f, 70.0f, -1700.0f),
        2,
        true
    );
    enemy2->AddAnimation(ANIMATION_NEUTRAL, "Resource/Mixamo3DModel/Idle.mv1");
    enemy2->AddAnimation(ANIMATION_WALKING, "Resource/Mixamo3DModel/Walking.mv1");
    enemy2->AddAnimation(ANIMATION_RUN, "Resource/Mixamo3DModel/Running.mv1");

    return 0;
}

const char* InitializeEnemyTask::GetTaskName() const {
    return "Initialize Enemy3D";
}



// ========================
// InitializeSkyBoxTask 実装
// ========================

InitializeSkyBoxTask::InitializeSkyBoxTask() {}

int InitializeSkyBoxTask::Execute() {

    SkyBox* pSky = new SkyBox("Resource/SkyBox/SkyBox.x");
    pSky->SetScale(8.0f);
    pSky->SetModelTexture("Resource/SkyBox/sky001.jpg");

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

    new Stage("Resource/Stage2/School_02.mv1", "Resource/Stage2/School_02_C.mv1");

    return 0;
}

const char* InitializeStageTask::GetTaskName() const {
    return "Initialize Stage";
}



// ========================
// InitializeEscapeItemTask 実装
// ========================

InitializeEscapeItemTask::InitializeEscapeItemTask() {}

int InitializeEscapeItemTask::Execute() {

    int CurrentPos = GetRand(4);
    int currentpos = GetRand(3);


    Point RandomPoint1[] = { VGet(-1615.0f, 120.0f, 2120.0f), VGet(350.0f, 53.0f, 2986.0f), VGet(-3270.0f, 52.0f, 2220.0f), VGet(-1440.0f, 444.0f, 2666.0f), VGet(3416.0f, 453.0f, 2537.0f) };

    Point RandomPoint2[] = { VGet(3920.0f, 1228.0f, 2800.0f), VGet(999.0f, 836.0f, 2426.0f), VGet(-2029.0f, 833.0f, 2933.0f), VGet(539.0f, 1228.0f, 2536.0f), VGet(-1853.0f, 839.0f, 2205.0f) };

    Point RandomPoint3[] = { VGet(-570.0f, 120.0f, -2687.0f), VGet(1851.0f, 446.0f, -2155.0f), VGet(-2120.0f, 905.0f, -2895.0f), VGet(-37.0f, 1228.0f, -2715.0f), VGet(2160.0f, 975.0f, -2460.0f) };

    Point RandomPoint4[] = { VGet(2050.0f, 510.0f, -2912.0f), VGet(-2132.0f, 831.0f, -2581.0f),  VGet(70.0f, 1224.0f, 2809.0f), VGet(3920.0f, 1228.0f, -2800.0f) };

   VECTOR RandomPosition1 = RandomPoint1[CurrentPos].PointPosition;
   VECTOR RandomPosition2 = RandomPoint2[CurrentPos].PointPosition;
   VECTOR RandomPosition3 = RandomPoint3[CurrentPos].PointPosition;
   VECTOR RandomPosition4 = RandomPoint4[currentpos].PointPosition;

    new EscapeItem("Resource/3D/EscapeItem.mv1", RandomPosition1);

    new EscapeItem("Resource/3D/EscapeItem.mv1", RandomPosition2);

    new EscapeItem("Resource/3D/EscapeItem.mv1", RandomPosition3);

    new EscapeItem("Resource/3D/EscapeItem.mv1", RandomPosition4);

    return 0;

}

const char* InitializeEscapeItemTask::GetTaskName() const {
    return "Initialize EscapeItem";
}



// ========================
// InitializeTimeItemTask 実装
// ========================

InitializeTimeItemTask::InitializeTimeItemTask() {}

int InitializeTimeItemTask::Execute() {

    new TimeItem("Resource/3D/TimeItem.mv1", VGet(-542.0f, 438.0f, -2622.0f), false);

    new TimeItem("Resource/3D/TimeItem.mv1", VGet(416.0f, 440.0f, 2927.0f), false);

    new TimeItem("Resource/3D/TimeItem.mv1", VGet(-1176.0f, 120.0f, 2351.0f), false);

    return 0;
}

const char* InitializeTimeItemTask::GetTaskName() const {
    return "Initialize TimeItem";
}



// ========================
// InitializeExitDoorTask 実装
// ========================

InitializeExitDoorTask::InitializeExitDoorTask() {}

int InitializeExitDoorTask::Execute() {

    new Exitdoor("Resource/Door/Door_Blender.mv1", "Resource/Door/Door_Blender_C.mv1", VGet(500.0f, 0.0f, 1900.0f));

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

        DrawStringToHandle(1280, 900, loadingText.c_str(), GetColor(255, 255, 255), FontSize);

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

    DrawStringToHandle(1280, 900, "Loading Complete!", GetColor(255, 255, 255), FontSize);

    ScreenFlip();

    WaitTimer(300);
}

void LoadingManager::ExecuteScene3D()
{
    const int total = static_cast<int>(m_tasks.size());

    for (int i = 0; i < total; ++i)
    {
        ClearDrawScreen();

       if (Scene3D_GameRuleHandle == -1) Scene3D_GameRuleHandle = LoadGraph("Resource/3D_UI/GameRulePicture.png");

        DrawGraph(0, 0, Scene3D_GameRuleHandle, false);

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

   if (Scene3D_GameRuleHandle != -1) DeleteGraph(Scene3D_GameRuleHandle);

    ScreenFlip();

    WaitTimer(300);
}