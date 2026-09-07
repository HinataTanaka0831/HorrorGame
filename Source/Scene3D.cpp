#include "Scene3D.h"
#include "Model.h"
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
{

}

Scene3D::~Scene3D()
{

}

void Scene3D::Initialize()
{

	// ローディングマネージャーの作成
	LoadingManager loader;

	// タスクを追加
	loader.AddTask(std::make_unique<InitializePlayerTask>());
	loader.AddTask(std::make_unique<InitializeEnemyTask>());
	loader.AddTask(std::make_unique<InitializeItemTask>());
	loader.AddTask(std::make_unique<InitializeSkyBoxTask>());
	loader.AddTask(std::make_unique<InitializeStageTask>());
	loader.AddTask(std::make_unique<InitializeExitDoorTask>());

	// カメラの初期化
	Master::m_camera = new Camera();

	// FPS シーンでロック有効化
	InputManager::GetInstance().EnableMouseLock(true);

	// ローディング実行
	loader.ExecuteScene3D();

	// 画像の生成
	if (m_playerCrouchingHandle == -1) m_playerCrouchingHandle = LoadGraph("Resource/3D_UI/Player_Crouching.png");

	// 画像の生成
	if (m_playerMoveHandle == -1) m_playerMoveHandle = LoadGraph("Resource/3D_UI/Player_Move.png");

	EscapeItem::NowNeedItem = 0;  // 脱出のために必要なアイテム数を
}

void Scene3D::InitializePlayerData()
{
	// プレイヤーの生成
	new Player3D(VGet(-4000.0f, 82.0f, 1700.0f));
}

void Scene3D::InitializeEnemyData()
{
	// 敵の生成
	m_enemy = new Enemy3D(
		"Resource/Mixamo3DModel/Whiteclown_N_Hallin.mv1",
		VGet(-50.0f, 70.0f, 1700.0f),
		1,
		true
	);
	m_enemy->AddAnimation(AnimationNeutral, "Resource/Mixamo3DModel/Idle.mv1");
	m_enemy->AddAnimation(AnimationWalking, "Resource/Mixamo3DModel/Walking.mv1");
	m_enemy->AddAnimation(AnimationRun, "Resource/Mixamo3DModel/Running.mv1");

	// 敵の生成
	m_enemy2 = new Enemy3D(
		"Resource/Mixamo3DModel/Whiteclown_N_Hallin.mv1",
		VGet(1000.0f, 70.0f, -1700.0f),
		2,
		true
	);
	m_enemy2->AddAnimation(AnimationNeutral, "Resource/Mixamo3DModel/Idle.mv1");
	m_enemy2->AddAnimation(AnimationWalking, "Resource/Mixamo3DModel/Walking.mv1");
	m_enemy2->AddAnimation(AnimationRun, "Resource/Mixamo3DModel/Running.mv1");

}

void Scene3D::InitializeItemData()
{
	// 脱出用アイテムの生成
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

	// 時間停止アイテムの生成
	new TimeItem("Resource/3D/TimeItem.mv1", VGet(-542.0f, 438.0f, -2622.0f), false);
	new TimeItem("Resource/3D/TimeItem.mv1", VGet(416.0f, 440.0f, 2927.0f), false);
	new TimeItem("Resource/3D/TimeItem.mv1", VGet(-1176.0f, 120.0f, 2351.0f), false);

}

void Scene3D::InitializeSkyBoxData()
{
	// スカイボックスの生成
	m_skybox = new SkyBox("Resource/SkyBox/SkyBox.x");
	m_skybox->SetScale(8.0f);
	m_skybox->SetModelTexture("Resource/SkyBox/sky001.jpg");
}

void Scene3D::InitializeStageData()
{
	// ステージの生成
	new Stage("Resource/Stage2/School_02.mv1", "Resource/Stage2/School_02_C.mv1");
}

void Scene3D::InitializeExitDoorData()
{
	// 脱出ドアの生成
	new Exitdoor("Resource/Door/Door_Blender.mv1", "Resource/Door/Door_Blender_C.mv1", VGet(500.0f, 0.0f, 1900.0f));
}


void Scene3D::Update()
{
	Scene::Update();
}

void Scene3D::Draw()
{
	Scene::Draw();

	// Zバッファを使用する設定にする
	SetUseZBufferFlag(FALSE);

	// Zバッファへの書き込みはしない
	SetWriteZBufferFlag(FALSE);

	// 残りアイテム数の表示
	DrawFormatStringToHandle(0, 10, GetColor(255, 255, 255), m_itemFontHandle, " 残りアイテム\n  %d / 4", EscapeItem::NowNeedItem);

	auto pPlayerObj = Master::m_sceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DByTag(Object3D::TagPlayer3D);
	Player3D* pPlayer = dynamic_cast<Player3D*>(pPlayerObj);

	if (pPlayer == nullptr)
	{
		return;
	}

	pPlayer->DrawStamina();

	if (pPlayer->GetCrouching())
	{
		// プレイヤーのしゃがみ状態画像の表示
		DrawGraph(200, Utility::SCREEN_HEIGHT / 2 + 350, m_playerCrouchingHandle, true);
	}
	else
	{
		// プレイヤーの歩き状態画像の表示
		DrawGraph(200, Utility::SCREEN_HEIGHT / 2 + 350, m_playerMoveHandle, true);
	}

	

	auto pTimeItemObj = Master::m_sceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DByTag(Object3D::TagTimeItem);
	TimeItem* pTimeItem = dynamic_cast<TimeItem*>(pTimeItemObj);

	if (pTimeItem != nullptr)
	{
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
	if (m_playerCrouchingHandle != -1)
	{
		DeleteGraph(m_playerCrouchingHandle);
	}

	// 画像の削除
	if (m_playerMoveHandle != -1)
	{
		DeleteGraph(m_playerMoveHandle);
	}

	if (m_enemy != nullptr)
	{
		delete m_enemy;
		m_enemy = nullptr;
	}

	if (m_enemy2 != nullptr)
	{
		delete m_enemy2;
		m_enemy2 = nullptr;
	}

	if (m_skybox != nullptr)
	{
		delete m_skybox;
		m_skybox = nullptr;
	}
}