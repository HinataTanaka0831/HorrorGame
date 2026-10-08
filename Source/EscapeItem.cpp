#include "EscapeItem.h"
#include "Model.h"
#include "Master.h"
#include "Scene.h"
#include "ObjectManager.h"
#include "Exitdoor.h"

int EscapeItem::m_nowNeedItem = 0;

EscapeItem::EscapeItem(std::string fileName, VECTOR initPosition)
	:Item(initPosition)
	, m_model(nullptr)
	
{
	// タグの設定
	SetTag(Object3D::TagEscapeItem);

	// モデルクラスを生成
	m_model = new Model(fileName, initPosition);

}

EscapeItem::~EscapeItem()
{
	if (m_model != nullptr)
	{
		delete m_model;
	}
}

void EscapeItem::Update()
{
	if (m_model != nullptr)
	{
		m_model->SetRotation(VGet(0.0f, 0.0f, -4.712f));
		m_model->SetPosition(m_position);
		m_model->Update();
	}


}

void EscapeItem::Draw()
{
	if (m_model != nullptr)
	{
		m_model->Draw();
	}

}

void EscapeItem::Escape()
{
	// 残り必要なアイテムを減らす
	m_nowNeedItem++;

	// もし残り必要なアイテム数が0になったらゲームクリアする
	if (m_nowNeedItem == 4)
	{
		Master::m_sceneManager->SetNextScene(SceneManager::SceneType::SceneResult);
	}

}
