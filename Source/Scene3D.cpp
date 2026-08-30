#include "Scene3D.h"
#include "Model.h"
#include "Loading.h"
#include <memory>
#include "Master.h"
#include "ObjectManager.h"
#include "EscapeItem.h"
#include "TimeItem.h"
#include "Player3D.h"
#include "Stage.h"

Scene3D::Scene3D()
	: Scene()
{
}

Scene3D::~Scene3D()
{
}

// ステージ・敵・アイテム生成タスクの実行およびFPSマウス拘束の有効化
// 入力: なし / 出力: なし / 副作用: LoadingManagerによる非同期アセット生成、Cameraインスタンス生成
void Scene3D::Initialize()
{
	LoadingManager loader;
	loader.AddTask(std::make_unique<InitializePlayerTask>());
	loader.AddTask(std::make_unique<InitializeEnemyTask>());
	loader.AddTask(std::make_unique<InitializeStageTask>());
	loader.AddTask(std::make_unique<InitializeSkyBoxTask>());
	loader.AddTask(std::make_unique<InitializeEscapeItemTask>());
	loader.AddTask(std::make_unique<InitializeTimeItemTask>());
	loader.AddTask(std::make_unique<InitializeExitDoorTask>());

	Master::mpCamera = new Camera();

	// FPS視点操作のためマウスカーソルを画面中央へロック
	InputManager::GetInstance().EnableMouseLock(true);

	loader.ExecuteScene3D();

	if (mnGraph == -1) mnGraph = LoadGraph("Resource/3D_UI/Player_Crouching.png");
	if (mnGraph2 == -1) mnGraph2 = LoadGraph("Resource/3D_UI/Player_Move.png");

	EscapeItem::NowNeedItem = 0;
}

void Scene3D::Update()
{
	Scene::Update();
}

// 3Dワールド描画後のZバッファ一時無効化による2D HUD（残りアイテム数・スタミナ・制限時間）の最前面描画
// 入力: なし / 出力: なし / 副作用: バックバッファ描画、Zバッファフラグ切り替え
void Scene3D::Draw()
{
	Scene::Draw();

	// 2D HUD表示が3D空間ジオメトリに遮蔽されないようZバッファテストをOFF
	SetUseZBufferFlag(FALSE);
	SetWriteZBufferFlag(FALSE);

	DrawFormatStringToHandle(0, 10, GetColor(255, 255, 255), ItemFontHandle, " 残りアイテム\n  %d / 4", EscapeItem::NowNeedItem);

	auto pPlayerObj = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DByTag(Object3D::TagPlayer3D);
	Player3D* pPlayer = dynamic_cast<Player3D*>(pPlayerObj);

	if (pPlayer != nullptr)
	{
		pPlayer->DrawStamina();

		if (pPlayer->GetCrouching())
		{
			DrawGraph(200, Utility::SCREEN_HEIGHT / 2 + 350, mnGraph, true);
		}
		else
		{
			DrawGraph(200, Utility::SCREEN_HEIGHT / 2 + 350, mnGraph2, true);
		}
	}

	auto pTimeItemObj = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DByTag(Object3D::TagTimeItem);
	TimeItem* pTimeItem = dynamic_cast<TimeItem*>(pTimeItemObj);

	if (pTimeItem != nullptr)
	{
		pTimeItem->DrawTimer();
	}

	// 次フレームの3D描画のためにZバッファを再有効化
	SetUseZBufferFlag(TRUE);
	SetWriteZBufferFlag(TRUE);
}

void Scene3D::Finalize()
{
	if (mnGraph != -1) { DeleteGraph(mnGraph); mnGraph = -1; }
	if (mnGraph2 != -1) { DeleteGraph(mnGraph2); mnGraph2 = -1; }
}