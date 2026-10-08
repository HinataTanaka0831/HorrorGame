#include "Scene.h"
#include "ObjectManager.h"

Scene::Scene()
	: m_fontSize140(CreateFontToHandle(NULL, 140, -1, -1))
	, m_fontSize130(CreateFontToHandle(NULL, 130, -1, DX_FONTTYPE_ANTIALIASING))
	, m_fontSize20(CreateFontToHandle(NULL, 20, -1, -1))
	, m_fontSize50(CreateFontToHandle(NULL, 50, -1, -1))
	, m_itemFontHandle(CreateFontToHandle(NULL, 40, -1, DX_FONTTYPE_ANTIALIASING))
	, m_objectManager(nullptr)

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


