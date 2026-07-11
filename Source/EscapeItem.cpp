#include "EscapeItem.h"
#include "Model.h"
#include "Master.h"
#include "Scene.h"
#include "ObjectManager.h"
#include "Exitdoor.h"

int EscapeItem::NowNeedItem = 0;

// コンストラクタ
EscapeItem::EscapeItem(std::string filename, VECTOR initPos)
	:Item(initPos)
	
{
	// タグの設定
	SetTag(Object3D::TagEscapeItem);

	// モデルクラスを生成
	mpModel = new Model(filename, initPos);

}

// デストラクタ
EscapeItem::~EscapeItem()
{
	if (mpModel != nullptr)
	{
		delete mpModel;
	}
}

// 更新
void EscapeItem::Update()
{
	if (mpModel != nullptr)
	{
		mpModel->SetRotation(VGet(0.0f, 0.0f, -4.712f));
		mpModel->SetPosition(mvPosition);
		mpModel->Update();
	}


}

// 描画
void EscapeItem::Draw()
{
	if (mpModel != nullptr)
	{
		mpModel->Draw();
	}

}

// アイテムを脱出口で使用したらゲームクリアする処理
void EscapeItem::Escape()
{
	// 残り必要なアイテムを減らす
	NowNeedItem++;

	// もし残り必要なアイテム数が0になったらゲームクリアする
	if (NowNeedItem == 4)
	{
		Master::mpSceneManager->SetNextScene(SceneManager::SCENE_TYPE::SCENE_RESULT);
	}

}
