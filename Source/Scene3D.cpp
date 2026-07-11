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

// コンストラクタ
Scene3D::Scene3D()
	: Scene()
{

}

// デストラクタ
Scene3D::~Scene3D()
{

}

// 初期化
void Scene3D::Initialize()
{

	// ローディングマネージャーの作成
	LoadingManager loader;

	// タスクを追加
	loader.AddTask(std::make_unique<InitializePlayerTask>());
	loader.AddTask(std::make_unique<InitializeEnemyTask>());
	loader.AddTask(std::make_unique<InitializeStageTask>());
	loader.AddTask(std::make_unique<InitializeSkyBoxTask>());
	loader.AddTask(std::make_unique<InitializeEscapeItemTask>());
	loader.AddTask(std::make_unique<InitializeTimeItemTask>());
	loader.AddTask(std::make_unique<InitializeExitDoorTask>());

	// カメラの初期化
	Master::mpCamera = new Camera();

	// FPS シーンでロック有効化
	g_MouseMgr.EnableMouseLock(true);

	// ローディング実行
	loader.ExecuteScene3D();

	// 画像の生成
	if (mnGraph == -1) mnGraph = LoadGraph("Resource/3D_UI/Player_Crouching.png");

	// 画像の生成
	if (mnGraph2 == -1) mnGraph2 = LoadGraph("Resource/3D_UI/Player_Move.png");

	EscapeItem::NowNeedItem = 0;  // 脱出のために必要なアイテム数を
}

// 更新
void Scene3D::Update()
{
	Scene::Update();
}

// 描画
void Scene3D::Draw()
{
	Scene::Draw();

	// Zバッファを使用する設定にする
	SetUseZBufferFlag(FALSE);

	// Zバッファへの書き込みはしない
	SetWriteZBufferFlag(FALSE);

	// 残りアイテム数の表示
	DrawFormatStringToHandle(0, 10, GetColor(255, 255, 255), ItemFontHandle, " 残りアイテム\n  %d / 4", EscapeItem::NowNeedItem);

	auto pPlayerObj = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DByTag(Object3D::TagPlayer3D);
	Player3D* pPlayer = dynamic_cast<Player3D*>(pPlayerObj);

	if (pPlayer == nullptr)
	{
		return;
	}

	pPlayer->DrawStamina();

	if (pPlayer->GetCrouching())
	{
		// プレイヤーのしゃがみ状態画像の表示
		DrawGraph(200, Utility::SCREEN_HEIGHT / 2 + 350, mnGraph, true);
	}
	else
	{
		// プレイヤーの歩き状態画像の表示
		DrawGraph(200, Utility::SCREEN_HEIGHT / 2 + 350, mnGraph2, true);
	}

	

	auto pTimeItemObj = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DByTag(Object3D::TagTimeItem);
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

// 終了処理
void Scene3D::Finalize()
{
	// 画像の削除
	if (mnGraph != -1) DeleteGraph(mnGraph);

	// 画像の削除
	if (mnGraph2 != -1) DeleteGraph(mnGraph2);
}