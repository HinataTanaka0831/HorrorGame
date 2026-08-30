#include "Scene.h"
#include "ObjectManager.h"

Scene::Scene()
{
	// オブジェクトマネージャーの生成
	mpObjectManager = new ObjectManager();
}

Scene::~Scene()
{
	if (mpObjectManager != nullptr)
	{
		delete mpObjectManager;
	}
}

void Scene::Update()
{
	if (mpObjectManager != nullptr)
	{
		mpObjectManager->Update();
	}

}

void Scene::Draw()
{
	if (mpObjectManager != nullptr)
	{
		mpObjectManager->Draw();
	}

}


