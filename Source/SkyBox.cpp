#include "SkyBox.h"
#include "Model.h"

SkyBox::SkyBox(std::string filename)
	: Object3D(VGet(0.0f, 0.0f, 0.0f))
	, mpModel(nullptr)
{
	mpModel = new Model(filename, VGet(0.0f, 0.0f, 0.0f));
}

SkyBox::~SkyBox()
{
	if (mpModel != nullptr)
	{
		delete mpModel;
		mpModel = nullptr;
	}
}

void SkyBox::Update()
{
	if (mpModel != nullptr)
	{
		mpModel->Update();
	}
}

// ライト計算を一時的に無効化してスカイボックスを描画（陰影による黒ずみ防止）
// 入力: なし / 出力: なし / 副作用: バックバッファ描画、SetUseLightingフラグ変更
void SkyBox::Draw()
{
	if (mpModel != nullptr)
	{
		// 背景空テクスチャが環境光・ディレクショナルライトの影で暗化しないよう一時OFF
		SetUseLighting(FALSE);
		mpModel->Draw();
		SetUseLighting(TRUE);
	}
}

void SkyBox::SetScale(float scale)
{
	if (mpModel != nullptr)
	{
		mpModel->SetScale(scale);
	}
}

void SkyBox::SetModelTexture(std::string filename, int index)
{
	if (mpModel != nullptr)
	{
		mpModel->SetTexture(filename, index);
	}
}