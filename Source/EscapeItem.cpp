#include "EscapeItem.h"
#include "Model.h"
#include "Master.h"
#include "Scene.h"
#include "ObjectManager.h"
#include "Exitdoor.h"

int EscapeItem::NowNeedItem = 0;

EscapeItem::EscapeItem(std::string filename, VECTOR initPos)
	: Item(initPos)
	, mpModel(nullptr)
{
	SetTag(Object3D::TagEscapeItem3D);
	mpModel = new Model(filename, initPos);
}

EscapeItem::~EscapeItem()
{
	if (mpModel != nullptr)
	{
		delete mpModel;
		mpModel = nullptr;
	}
}

// アイテムの浮遊回転アニメーション更新
// 入力: なし / 出力: なし / 副作用: 3Dモデルの回転・座標更新
void EscapeItem::Update()
{
	if (mpModel != nullptr)
	{
		mpModel->SetRotation(VGet(0.0f, 0.0f, -4.712f));
		mpModel->SetPosition(mvPosition);
		mpModel->Update();
	}
}

// アイテム3Dモデル描画
// 入力: なし / 出力: なし / 副作用: バックバッファへの描画
void EscapeItem::Draw()
{
	if (mpModel != nullptr)
	{
		mpModel->Draw();
	}
}

// 収集数のインクリメントおよび4個達成時のクリアシーン遷移
// 入力: なし / 出力: なし / 副作用: NowNeedItemの加算、クリア時のSceneManager遷移予約
void EscapeItem::Escape()
{
	NowNeedItem++;
	if (NowNeedItem == 4)
	{
		Master::mpSceneManager->SetNextScene(SceneManager::SCENE_RESULT);
	}
}