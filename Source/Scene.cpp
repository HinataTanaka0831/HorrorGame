#include "Scene.h"
#include "ObjectManager.h"

Scene::Scene()
{
	// オブジェクトマネージャーの生成
	m_objectManager = new ObjectManager();
}

Scene::~Scene()
{
	if (m_objectManager != nullptr)
	{
		delete m_objectManager;
	}
}

void Scene::Update()
{
	if (m_objectManager != nullptr)
	{
		m_objectManager->Update();
	}

}

void Scene::Draw()
{
	if (m_objectManager != nullptr)
	{
		m_objectManager->Draw();
	}

}


