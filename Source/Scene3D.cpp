#include "Scene3D.h"
#include "Loading.h"
#include <memory>
#include "Master.h"
#include "ObjectManager.h"
#include "Player3D.h"
#include "Enemy3D.h"
#include "Stage.h"
#include "EscapeItem.h"
#include "TimeItem.h"
#include "Exitdoor.h"
#include "SkyBox.h"

Scene3D::Scene3D()
	: Scene()
	, m_playerCrouchingHandle(-1)
	, m_playerMoveHandle(-1)
	, m_player(nullptr)
	, m_stage(nullptr)
	, m_exitDoor(nullptr)
	, m_skybox(nullptr)

{

}

Scene3D::~Scene3D()
{
	if (Master::m_camera != nullptr)
	{
		Master::m_camera->ResetTarget();
	}

	m_player = nullptr;
	m_enemies.clear();
	m_timeItems.clear();
	m_stage = nullptr;
	m_skybox = nullptr;
}

void Scene3D::Initialize()
{
	// ローディングマネージャーの作成
	LoadingManager loader;
	// タスクを追加
	loader.AddTask(std::make_unique<InitializeCharacterTask>());
	loader.AddTask(std::make_unique<InitializeItemTask>());
	loader.AddTask(std::make_unique<InitializeSkyBoxTask>());
	loader.AddTask(std::make_unique<InitializeStageTask>());
	loader.AddTask(std::make_unique<InitializeExitDoorTask>());
	// カメラの初期化
	delete Master::m_camera;
	Master::m_camera = new Camera();
	// FPS シーンでロック有効化
	Master::m_input->EnableMouseLock(true);
	// ローディング実行
	loader.ExecuteScene3D();
	// 画像の生成
	if (m_playerCrouchingHandle == -1) m_playerCrouchingHandle = LoadGraph("Resource/UI/ui_playercrouching.png");
	if (m_playerMoveHandle == -1) m_playerMoveHandle = LoadGraph("Resource/UI/ui_playermove.png");
	// 脱出のために必要なアイテム数を初期化
	EscapeItem::m_nowNeedItem = 0;
}

void Scene3D::InitializeCharacterData()
{
	// プレイヤーの生成
	m_player = new Player3D(VGet(-4000.0f, 0.0f, 1700.0f));
	// 敵の生成
	Enemy3D* m_enemyA = new Enemy3D(
		"Resource/Object/Mixamo3DModel/object_enemy.mv1",
		VGet(-50.0f, 70.0f, 1700.0f),
		1,
		true
	);
	m_enemyA->AddAnimation(AnimationNeutral, "Resource/Object/Mixamo3DModel/animation_idle.mv1");
	m_enemyA->AddAnimation(AnimationWalking, "Resource/Object/Mixamo3DModel/animation_walking.mv1");
	m_enemyA->AddAnimation(AnimationRun, "Resource/Object/Mixamo3DModel/animation_running.mv1");
	m_enemies.push_back(m_enemyA);
	Enemy3D* m_enemyB = new Enemy3D(
		"Resource/Object/Mixamo3DModel/object_enemy.mv1",
		VGet(1000.0f, 70.0f, -1700.0f),
		2,
		true
	);
	m_enemyB->AddAnimation(AnimationNeutral, "Resource/Object/Mixamo3DModel/animation_idle.mv1");
	m_enemyB->AddAnimation(AnimationWalking, "Resource/Object/Mixamo3DModel/animation_walking.mv1");
	m_enemyB->AddAnimation(AnimationRun, "Resource/Object/Mixamo3DModel/animation_running.mv1");
	m_enemies.push_back(m_enemyB);
}

void Scene3D::InitializeItemData()
{
	// 脱出用アイテムの生成
	// あらかじめ指定したアイテムの座標をゲーム起動時、ランダムに構造体に格納する
	// その格納した座標を初期座標に設定してアイテムの位置を毎回別の場所に置かれるようにする
	int currentPosition1 = GetRand(4);
	int currentPosition2 = GetRand(3);
	Point RandomPoint1[] = { VGet(-1615.0f, 120.0f, 2120.0f), VGet(350.0f, 53.0f, 2986.0f), VGet(-3270.0f, 52.0f, 2220.0f), VGet(-1440.0f, 444.0f, 2666.0f), VGet(3416.0f, 453.0f, 2537.0f) };
	Point RandomPoint2[] = { VGet(3920.0f, 1228.0f, 2800.0f), VGet(999.0f, 836.0f, 2426.0f), VGet(-2029.0f, 833.0f, 2933.0f), VGet(539.0f, 1228.0f, 2536.0f), VGet(-1853.0f, 839.0f, 2205.0f) };
	Point RandomPoint3[] = { VGet(-570.0f, 120.0f, -2687.0f), VGet(1851.0f, 446.0f, -2155.0f), VGet(-2120.0f, 905.0f, -2895.0f), VGet(-37.0f, 1228.0f, -2715.0f), VGet(2160.0f, 975.0f, -2460.0f) };
	Point RandomPoint4[] = { VGet(2050.0f, 510.0f, -2912.0f), VGet(-2132.0f, 831.0f, -2581.0f),  VGet(70.0f, 1224.0f, 2809.0f), VGet(3920.0f, 1228.0f, -2800.0f) };
	VECTOR RandomPosition1 = RandomPoint1[currentPosition1].PointPosition;
	VECTOR RandomPosition2 = RandomPoint2[currentPosition1].PointPosition;
	VECTOR RandomPosition3 = RandomPoint3[currentPosition1].PointPosition;
	VECTOR RandomPosition4 = RandomPoint4[currentPosition2].PointPosition;
	// 脱出アイテムの生成
	m_escapeItems.push_back(new EscapeItem("Resource/Object/EscapeItem.mv1", RandomPosition1, false));
	m_escapeItems.push_back(new EscapeItem("Resource/Object/EscapeItem.mv1", RandomPosition2, false));
	m_escapeItems.push_back(new EscapeItem("Resource/Object/EscapeItem.mv1", RandomPosition3, false));
	m_escapeItems.push_back(new EscapeItem("Resource/Object/EscapeItem.mv1", RandomPosition4, false));
	// 時間停止アイテムの生成
	m_timeItems.push_back(new TimeItem("Resource/Object/TimeItem.mv1", VGet(-542.0f, 438.0f, -2622.0f), false));
	m_timeItems.push_back(new TimeItem("Resource/Object/TimeItem.mv1", VGet(416.0f, 440.0f, 2927.0f), false));
	m_timeItems.push_back(new TimeItem("Resource/Object/TimeItem.mv1", VGet(-1176.0f, 120.0f, 2351.0f), false));
}

void Scene3D::InitializeSkyBoxData()
{
	// スカイボックスの生成
	m_skybox = new SkyBox("Resource/Object/SkyBox/skyBox.x");
	m_skybox->SetScale(8.0f);
	m_skybox->SetModelTexture("Resource/Object/SkyBox/skybox_nightsky.jpg");
}

void Scene3D::InitializeStageData()
{
	// ステージの生成
	m_stage = new Stage("Resource/Object/Stage/object_school.mv1");
}

void Scene3D::InitializeExitDoorData()
{
	// 脱出ドアの生成
	m_exitDoor = new Exitdoor("Resource/Object/Door/object_door.mv1", VGet(500.0f, 0.0f, 1900.0f));
}

void Scene3D::SetPointer()
{
	if (m_player == nullptr) return;
	if (m_stage == nullptr) return;
	if (m_exitDoor == nullptr) return;
	m_player->SetExitdoor(m_exitDoor);
	m_player->SetStage(m_stage);
	for (auto* pEnemy : m_enemies) {
		pEnemy->SetPlayer(m_player);
		m_player->SetEnemy3D(pEnemy);
	}
	for (auto* pEscape : m_escapeItems) {
		m_player->SetEscapeItem(pEscape);
	}
	for (auto* pTime : m_timeItems) {
		m_player->SetTimeItem(pTime);
	}
}

void Scene3D::Update()
{
	Scene::Update();
}

void Scene3D::Draw()
{
	Scene::Draw();
	// Zバッファを使用しない設定にする
	SetUseZBufferFlag(FALSE);
	// Zバッファへの書き込みはしない
	SetWriteZBufferFlag(FALSE);
	// 残りアイテム数の表示
	DrawFormatStringToHandle(0, 10, GetColor(255, 255, 255), m_itemFontHandle, " 残りアイテム\n  %d / 4", EscapeItem::m_nowNeedItem);
	// アイテムボックスの描画
	m_player->ItemBox();
	// スタミナゲージの描画
	m_player->DrawStamina();
	if (m_player->GetCrouching()) {
		// プレイヤーのしゃがみ状態画像の表示
		DrawGraph(200, Utility::SCREEN_HEIGHT / 2 + 350, m_playerCrouchingHandle, true);
	}
	else {
		// プレイヤーの歩き状態画像の表示
		DrawGraph(200, Utility::SCREEN_HEIGHT / 2 + 350, m_playerMoveHandle, true);
	}
	// 時間止めアイテムタイマー表示
	for (auto pTimeItem : m_timeItems) {
		pTimeItem->DrawTimer();
	}

	// Zバッファを使用する設定にする
	SetUseZBufferFlag(TRUE);
	// Zバッファへの書き込みをする
	SetWriteZBufferFlag(TRUE);

}

void Scene3D::Finalize()
{
	// 画像の削除
	if (m_playerCrouchingHandle != -1) {
		DeleteGraph(m_playerCrouchingHandle);
		m_playerCrouchingHandle = -1;
	}
	// 画像の削除
	if (m_playerMoveHandle != -1) {
		DeleteGraph(m_playerMoveHandle);
		m_playerMoveHandle = -1;
	}
}