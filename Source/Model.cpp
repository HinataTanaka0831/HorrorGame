#include "Model.h"
#include "AttachmentModel.h"

Model::Model(std::string filename, VECTOR initPos, bool isSeparateAnimation)
	: mvPosition(initPos)
	, mvRotation(VGet(0.0f, 0.0f, 0.0f))
	, mpAttachment(nullptr)
	, mfScale(1.0f)
	, mnChangeTextureHandle(-1)
{
	// モデルの読み込み
	mnHandle = MV1LoadModel(filename.c_str());

	// Mixamo用の追加プログラム
	// 条件分岐を追加
	if (isSeparateAnimation)
	{
		// 分割アニメーションクラスの生成
		mpSeparateAnimation = new SeparateModelAnimation(mnHandle);
		mpAnimation = nullptr;
	}
	else
	{
		// 通常アニメーションクラスの生成
		mpAnimation = new ModelAnimation(mnHandle);
		mpSeparateAnimation = nullptr;
	}

}

void Model::AddAnimation(AnimationState state, std::string filename)
{
	if (mpSeparateAnimation != nullptr)
	{
		mpSeparateAnimation->AddAnimation(state, filename);
	}
}


Model::~Model()
{
	// アニメーションクラスの破棄
	if (mpAnimation != nullptr)
	{
		delete mpAnimation;
	}

	// Mixamo用処理
	// 分割アニメーションクラスの破棄
	if (mpSeparateAnimation != nullptr)
	{
		delete mpSeparateAnimation;
	}

	// アタッチモデルクラスの破棄
	if (mpAttachment != nullptr)
	{
		mpAttachment->SetDeleteFlag(true);
	}

	// テクスチャを切り替えている場合はそのテクスチャを破棄
	if (mnChangeTextureHandle != -1)
	{
		DeleteGraph(mnChangeTextureHandle);
	}

	// 読み込んだモデルの削除
	// note : 読み込んだモデルは勝手に破棄してくれないので、必要なくなったら手動で破棄する
	MV1DeleteModel(mnHandle);
}

void Model::Update()
{
	// アニメーションの更新
	if (mpAnimation != nullptr)
	{
		mpAnimation->Update();
	}

	// Mixamo用処理
	// 分割アニメーションの更新
	if (mpSeparateAnimation != nullptr)
	{
		mpSeparateAnimation->Update();
	}

	// 座標設定
	MV1SetPosition(mnHandle, mvPosition);

	// 回転設定
	MV1SetRotationXYZ(mnHandle, mvRotation);
}

void Model::Draw()
{
	// モデルの描画
	MV1DrawModel(mnHandle);
}

void Model::ChangeAnimation(AnimationState state)
{
	// Mixamo用処理
	// 通常 or 分割のどちらかを使っているかで分岐
	if (mpAnimation != nullptr)
	{
		mpAnimation->ChangeAnimation(state);
	}
	if (mpSeparateAnimation != nullptr)
	{
		mpSeparateAnimation->ChangeAnimation(state);
	}
}

void Model::SetLoop(bool loop)
{
	// Mixamo用処理
	// 通常 or 分割のどちらかを使っているかで分岐
	if (mpAnimation != nullptr)
	{
		mpAnimation->SetLoop(loop);
	}
	if (mpSeparateAnimation != nullptr)
	{
		mpSeparateAnimation->SetLoop(loop);
	}
}

void Model::SetLoopFinishState(AnimationState state)
{
	// Mixamo用処理
	// 通常 or 分割のどちらかを使っているかで分岐
	if (mpAnimation != nullptr)
	{
		mpAnimation->SetLoopFinishState(state);
	}
	if (mpSeparateAnimation != nullptr)
	{
		mpSeparateAnimation->SetLoopFinishState(state);
	}
}

void Model::SetAnimationBlend(bool isBlend)
{
	// Mixamo用処理
	// 通常 or 分割のどちらかを使っているかで分岐
	if (mpAnimation != nullptr)
	{
		mpAnimation->SetAnimationBlend(isBlend);
	}
	if (mpSeparateAnimation != nullptr)
	{
		mpSeparateAnimation->SetAnimationBlend(isBlend);
	}
}

AnimationState Model::GetNowState()
{
	// Mixamo用処理
	// 通常 or 分割のどちらかを使っているかで分岐
    // note: （ほぼありえないが）もしどちらもなければ、特に設定のない最大値を返すようにする
	AnimationState ret = AnimationState::ANIMATION_MAX;

	if (mpAnimation != nullptr)
	{
		ret = mpAnimation->GetNowState();
	}
	if (mpSeparateAnimation != nullptr)
	{
		ret = mpSeparateAnimation->GetNowState();
	}

	return ret;
}

bool Model::IsAnimationLoopFinish()
{
	// Mixamo用処理
    // 通常 or 分割のどちらかを使っているかで分岐
    // note: （ほぼありえないが）もしどちらもなければ、false を返すようにしておく

	bool ret = false;

	if (mpAnimation != nullptr)
	{
		ret = mpAnimation->IsLoopFinish();
	}
	if (mpSeparateAnimation != nullptr)
	{
		ret = mpSeparateAnimation->IsLoopFinish();
	}

	return ret;
}


void Model::AddAttachment(std::string filename, std::string attachFrameName)
{
	// アタッチ先のフレーム番号を取得
	int frameIndex = MV1SearchFrame(mnHandle, attachFrameName.c_str());

	// アタッチメントモデルの生成
	mpAttachment = new AttachmentModel(filename, mnHandle, frameIndex);
}

VECTOR Model::GetAttachmentPosition()
{
	if (mpAttachment != nullptr)
	{
		
		VECTOR vec = VGet(0.0f, -50.0f, 0.0f);

		// 行列の取得
		MATRIX matrix = MV1GetFrameLocalWorldMatrix(mpAttachment->GetHandle(), 0);  

		// 行列情報をもとに座標変換する
		vec = VTransform(vec, matrix);

		return vec;
	}


	// アタッチメントが無い場合は原点を返しておく
	return VGet(0.0f, 0.0f, 0.0f);
}

void Model::SetScale(float scale)
{
	MV1SetScale(mnHandle, VGet(scale, scale, scale));
}

void Model::SetTexture(std::string filename, int index)
{
	// テクスチャを切り替えているならそのテクスチャを破棄
	if (mnChangeTextureHandle != -1)
	{
		DeleteGraph(mnChangeTextureHandle);
	}

	// テクスチャの読み込み
	mnChangeTextureHandle = LoadGraph(filename.c_str());

	// 読み込んだテクスチャを反映
	MV1SetTextureGraphHandle(mnHandle, index, mnChangeTextureHandle, FALSE);
}