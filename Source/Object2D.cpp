#include "Object2D.h"
#include "Texture.h"
#include "Master.h"
#include "ObjectManager.h"
#include "Scene.h"

int Object2D::score = 0;

Object2D::Object2D(std::string filename, VECTOR initPos)
	: mvPosition(initPos)
	, mbDeleteFlag(false)
	, mfAngle(0.0f)
	, mfspeed(0.0f)
	, mvDirection(VGet(0.0f, 0.0f, 0.0f))
	, mnTag(Tag::None2D)
{
	// シーン破棄時の一括解放を保証するためマネージャーへ登録
	Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->AddObject(this);
	mpTexture = new Texture(filename, initPos, true);
}

Object2D::~Object2D()
{
	if (mpTexture != nullptr)
	{
		delete mpTexture;
		mpTexture = nullptr;
	}
}

// 座標同期およびスプライト更新
// 入力: なし / 出力: なし / 副作用: 内部Texture座標の更新
void Object2D::Update()
{
	if (mpTexture != nullptr)
	{
		mpTexture->Update();
		mpTexture->SetPosition(mvPosition);
	}
}

// スプライト描画
// 入力: なし / 出力: なし / 副作用: バックバッファへの描画
void Object2D::Draw()
{
	if (mpTexture != nullptr)
	{
		mpTexture->Draw();
	}
}

float Object2D::GetRadius()
{
	return mpTexture ? mpTexture->GetRadius() : 0.0f;
}