#include "Item.h"
#include "Model.h"


Item::Item(const std::string& fileName, VECTOR initPosition, bool isSeparateAnimation)
	:Object3D(initPosition)
	, m_model(nullptr)
	,m_rotationZ(0.0f)

{
	// モデルクラスの生成
	m_model = new Model(fileName, initPosition, isSeparateAnimation);
}

Item::~Item()
{
	// モデルクラスの削除
	if (m_model != nullptr)
	{
		delete m_model;
		m_model = nullptr;
	}
}

void Item::Update()
{
	// モデルの回転・座標更新
	if (m_model != nullptr)
	{
		m_model->SetRotation(VGet(0.0f, 0.0f, m_rotationZ));
		m_model->SetPosition(m_position);
		m_model->Update();
	}

}

void Item::Draw()
{
	// モデルの描画
	if (m_model != nullptr)
	{
		m_model->Draw();
	}

}

