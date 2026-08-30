#include "SkyBox.h"
#include "Model.h"

SkyBox::SkyBox(std::string filename)
: Object3D(VGet(0.0f, 0.0f, 0.0f))
{
	// スカイボックスモデルの生成
	// note: スカイボックスの座標は、基本的には原点。
	//       ものによっては、座標を常にプレイヤーの座標にすることで、空が見切れないようにすることもある。
	mpModel = new Model(filename, VGet(0.0f, 0.0f, 0.0f));
}

SkyBox::~SkyBox()
{
	// モデルクラスの破棄
	if (mpModel != nullptr)
	{
		delete mpModel;
	}
}

void SkyBox::Update()
{
	if (mpModel != nullptr)
	{
		mpModel->Update();
	}
}

void SkyBox::Draw()
{
	if (mpModel != nullptr)
	{
		// 一時的にライトの影響をOFFにして描画する
		// note: 影響すると影が出来て暗くなってしまう場合があるため。
		//       逆に影を付けたりしたい場合はこの処理は外してもよい。
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