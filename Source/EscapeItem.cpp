#include "EscapeItem.h"
#include "Master.h"

int EscapeItem::m_nowNeedItem = 0;

EscapeItem::EscapeItem(std::string fileName, VECTOR initPosition, bool isSeparateAnimation)
	:Item(fileName, initPosition)
	
{
	// タグの設定
	SetTag(Object3D::TagEscapeItem);
}

EscapeItem::~EscapeItem()
{

}

void EscapeItem::Update()
{
	Item::SetRotationZ(-4.712f);
	Item::Update();
}

void EscapeItem::Draw()
{
	Item::Draw();
}

void EscapeItem::Escape()
{
	// 残り必要なアイテムを減らす
	m_nowNeedItem++;

	// もし残り必要なアイテム数が0になったらゲームクリアする
	if (m_nowNeedItem == 4)
	{
		Master::m_sceneManager->SetResultType(SceneManager::ResultType::ResultGameClear);
		Master::m_sceneManager->SetNextScene(SceneManager::SceneType::SceneResult);
	}

}
