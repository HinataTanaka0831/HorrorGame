#include "EscapeItem.h"
#include "Master.h"

int EscapeItem::m_nowNeedItem = 0;

EscapeItem::EscapeItem(const std::string& fileName, VECTOR initPosition, bool isSeparateAnimation)
	:Item(fileName, initPosition)
	
{
	SetTag(Object3D::TagEscapeItem);  	// タグの設定
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
	m_nowNeedItem++;  	// 残り必要なアイテムを減らす
	// もし残り必要なアイテム数が0になったらゲームクリアする
	if (m_nowNeedItem == 4) {
		Master::m_sceneManager->SetResultType(SceneManager::ResultType::ResultGameClear);
		Master::m_sceneManager->SetNextScene(SceneManager::SceneType::SceneResult);
	}
}
