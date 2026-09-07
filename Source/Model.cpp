#include "Model.h"
#include "AttachmentModel.h"
#include "SeparateModelAnimation.h"
#include "ModelAnimation.h"

Model::Model(std::string fileName, VECTOR initPosition, bool isSeparateAnimation)
	: m_position(initPosition)
	, m_rotation(VGet(0.0f, 0.0f, 0.0f))
	, m_attachment(nullptr)
	, m_scale(1.0f)
	, m_changeTextureHandle(-1)
{
	// モデルの読み込み
	m_handle = MV1LoadModel(fileName.c_str());

	// Mixamo用の追加プログラム
	// 条件分岐を追加
	if (isSeparateAnimation)
	{
		// 分割アニメーションクラスの生成
		m_separateAnimation = new SeparateModelAnimation(m_handle);
		m_animation = nullptr;
	}
	else
	{
		// 通常アニメーションクラスの生成
		m_animation = new ModelAnimation(m_handle);
		m_separateAnimation = nullptr;
	}

}

void Model::AddAnimation(AnimationState state, std::string fileName)
{
	if (m_separateAnimation != nullptr)
	{
		m_separateAnimation->AddAnimation(state, fileName);
	}
}


Model::~Model()
{
	// アニメーションクラスの破棄
	if (m_animation != nullptr)
	{
		delete m_animation;
	}

	// Mixamo用処理
	// 分割アニメーションクラスの破棄
	if (m_separateAnimation != nullptr)
	{
		delete m_separateAnimation;
	}

	// アタッチモデルクラスの破棄
	if (m_attachment != nullptr)
	{
		m_attachment->SetDeleteFlag(true);
	}

	// テクスチャを切り替えている場合はそのテクスチャを破棄
	if (m_changeTextureHandle != -1)
	{
		DeleteGraph(m_changeTextureHandle);
	}

	// 読み込んだモデルの削除
	// note : 読み込んだモデルは勝手に破棄してくれないので、必要なくなったら手動で破棄する
	MV1DeleteModel(m_handle);
}

void Model::Update()
{
	// アニメーションの更新
	if (m_animation != nullptr)
	{
		m_animation->Update();
	}

	// Mixamo用処理
	// 分割アニメーションの更新
	if (m_separateAnimation != nullptr)
	{
		m_separateAnimation->Update();
	}

	// 座標設定
	MV1SetPosition(m_handle, m_position);

	// 回転設定
	MV1SetRotationXYZ(m_handle, m_rotation);
}

void Model::Draw()
{
	// モデルの描画
	MV1DrawModel(m_handle);
}

void Model::ChangeAnimation(AnimationState state)
{
	// Mixamo用処理
	// 通常 or 分割のどちらかを使っているかで分岐
	if (m_animation != nullptr)
	{
		m_animation->ChangeAnimation(state);
	}
	if (m_separateAnimation != nullptr)
	{
		m_separateAnimation->ChangeAnimation(state);
	}
}

void Model::SetLoop(bool loop)
{
	// Mixamo用処理
	// 通常 or 分割のどちらかを使っているかで分岐
	if (m_animation != nullptr)
	{
		m_animation->SetLoop(loop);
	}
	if (m_separateAnimation != nullptr)
	{
		m_separateAnimation->SetLoop(loop);
	}
}

void Model::SetLoopFinishState(AnimationState state)
{
	// Mixamo用処理
	// 通常 or 分割のどちらかを使っているかで分岐
	if (m_animation != nullptr)
	{
		m_animation->SetLoopFinishState(state);
	}
	if (m_separateAnimation != nullptr)
	{
		m_separateAnimation->SetLoopFinishState(state);
	}
}

void Model::SetAnimationBlend(bool isBlend)
{
	// Mixamo用処理
	// 通常 or 分割のどちらかを使っているかで分岐
	if (m_animation != nullptr)
	{
		m_animation->SetAnimationBlend(isBlend);
	}
	if (m_separateAnimation != nullptr)
	{
		m_separateAnimation->SetAnimationBlend(isBlend);
	}
}

AnimationState Model::GetNowState()
{
	// Mixamo用処理
	// 通常 or 分割のどちらかを使っているかで分岐
    // note: （ほぼありえないが）もしどちらもなければ、特に設定のない最大値を返すようにする
	AnimationState ret = AnimationState::AnimationMax;

	if (m_animation != nullptr)
	{
		ret = m_animation->GetNowState();
	}
	if (m_separateAnimation != nullptr)
	{
		ret = m_separateAnimation->GetNowState();
	}

	return ret;
}

bool Model::IsAnimationLoopFinish()
{
	// Mixamo用処理
    // 通常 or 分割のどちらかを使っているかで分岐
    // note: （ほぼありえないが）もしどちらもなければ、false を返すようにしておく

	bool ret = false;

	if (m_animation != nullptr)
	{
		ret = m_animation->IsLoopFinish();
	}
	if (m_separateAnimation != nullptr)
	{
		ret = m_separateAnimation->IsLoopFinish();
	}

	return ret;
}


void Model::AddAttachment(std::string filename, std::string attachFrameName)
{
	// アタッチ先のフレーム番号を取得
	int frameIndex = MV1SearchFrame(m_handle, attachFrameName.c_str());

	// アタッチメントモデルの生成
	m_attachment = new AttachmentModel(filename, m_handle, frameIndex);
}

VECTOR Model::GetAttachmentPosition()
{
	if (m_attachment != nullptr)
	{
		
		VECTOR vec = VGet(0.0f, -50.0f, 0.0f);

		// 行列の取得
		MATRIX matrix = MV1GetFrameLocalWorldMatrix(m_attachment->GetHandle(), 0);  

		// 行列情報をもとに座標変換する
		vec = VTransform(vec, matrix);

		return vec;
	}


	// アタッチメントが無い場合は原点を返しておく
	return VGet(0.0f, 0.0f, 0.0f);
}

void Model::SetScale(float scale)
{
	MV1SetScale(m_handle, VGet(scale, scale, scale));
}

void Model::SetTexture(std::string filename, int index)
{
	// テクスチャを切り替えているならそのテクスチャを破棄
	if (m_changeTextureHandle != -1)
	{
		DeleteGraph(m_changeTextureHandle);
	}

	// テクスチャの読み込み
	m_changeTextureHandle = LoadGraph(filename.c_str());

	// 読み込んだテクスチャを反映
	MV1SetTextureGraphHandle(m_handle, index, m_changeTextureHandle, FALSE);
}