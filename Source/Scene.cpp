#include "Scene.h"
#include "ObjectManager.h"

Scene::Scene()
{
	mpObjectManager = new ObjectManager();
}

Scene::~Scene()
{
	if (mpObjectManager != nullptr)
	{
		delete mpObjectManager;
		mpObjectManager = nullptr;
	}
}

// 登録オブジェクトのUpdate一括実行
// 入力: なし / 出力: なし / 副作用: 登録された2D/3Dオブジェクトの状態更新
void Scene::Update()
{
	if (mpObjectManager != nullptr)
	{
		mpObjectManager->Update();
	}
}

// 登録オブジェクトのDraw一括実行
// 入力: なし / 出力: なし / 副作用: 登録された2D/3Dオブジェクトの画面描画
void Scene::Draw()
{
	if (mpObjectManager != nullptr)
	{
		mpObjectManager->Draw();
	}
}