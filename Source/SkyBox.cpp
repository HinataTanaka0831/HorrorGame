#include "SkyBox.h"
#include "Model.h"

SkyBox::SkyBox(std::string fileName)
: Object3D(VGet(0.0f, 0.0f, 0.0f))
{
	// スカイボックスモデルの生成
	// note: スカイボックスの座標は、基本的には原点。
	//       ものによっては、座標を常にプレイヤーの座標にすることで、空が見切れないようにすることもある。
	m_model = new Model(fileName, VGet(0.0f, 0.0f, 0.0f));
}

SkyBox::~SkyBox()
{
	// モデルクラスの破棄
	if (m_model != nullptr)
	{
		delete m_model;
	}
}

void SkyBox::Update()
{
	if (m_model != nullptr)
	{
		m_model->Update();
	}
}

void SkyBox::Draw()
{
	if (m_model != nullptr)
	{
		// 一時的にライトの影響をOFFにして描画する
		// note: 影響すると影が出来て暗くなってしまう場合があるため。
		//       逆に影を付けたりしたい場合はこの処理は外してもよい。
		SetUseLighting(FALSE);
		m_model->Draw();
		SetUseLighting(TRUE);
	}
}

void SkyBox::SetScale(float scale)
{
	if (m_model != nullptr)
	{
		m_model->SetScale(scale);
	}
}

void SkyBox::SetModelTexture(std::string fileName, int index)
{
	if (m_model != nullptr)
	{
		m_model->SetTexture(fileName, index);
	}
}