#include "Model.h"
#include "AttachmentModel.h"

Model::Model(std::string filename, VECTOR initPos, bool isSeparateAnimation)
	: mvPosition(initPos)
	, mvRotation(VGet(0.0f, 0.0f, 0.0f))
	, mpAttachment(nullptr)
	, mfScale(1.0f)
	, mnChangeTextureHandle(-1)
	, mpAnimation(nullptr)
	, mpSeparateAnimation(nullptr)
{
	mnHandle = MV1LoadModel(filename.c_str());

	// 単一埋め込みアニメーションか外部分割ファイルかで制御クラスを分岐
	if (isSeparateAnimation)
	{
		mpSeparateAnimation = new SeparateModelAnimation(mnHandle);
	}
	else
	{
		mpAnimation = new ModelAnimation(mnHandle);
	}
}

// 外部モーションファイルの追加登録（Mixamo分割ファイル用）
// 入力: state(紐づけるステート), filename(アニメーションモデルパス) / 出力: なし / 副作用: SeparateModelAnimationへの登録
void Model::AddAnimation(AnimationState state, std::string filename)
{
	if (mpSeparateAnimation != nullptr)
	{
		mpSeparateAnimation->AddAnimation(state, filename);
	}
}

Model::~Model()
{
	if (mpAnimation != nullptr) delete mpAnimation;
	if (mpSeparateAnimation != nullptr) delete mpSeparateAnimation;
	if (mpAttachment != nullptr) mpAttachment->SetDeleteFlag(true);
	if (mnChangeTextureHandle != -1) DeleteGraph(mnChangeTextureHandle);

	MV1DeleteModel(mnHandle);
}

// アニメーション再生時間の進行および座標・回転の3Dモデルへの反映
// 入力: なし / 出力: なし / 副作用: DXライブラリモデル変換行列の更新
void Model::Update()
{
	if (mpAnimation != nullptr) mpAnimation->Update();
	if (mpSeparateAnimation != nullptr) mpSeparateAnimation->Update();

	MV1SetPosition(mnHandle, mvPosition);
	MV1SetRotationXYZ(mnHandle, mvRotation);
}

// 3Dモデル描画
// 入力: なし / 出力: なし / 副作用: バックバッファへの3D描画
void Model::Draw()
{
	MV1DrawModel(mnHandle);
}

// 指定アニメーションステートへの切り替え
// 入力: state(変更先ステート) / 出力: なし / 副作用: アニメーションコントローラのステート変更
void Model::ChangeAnimation(AnimationState state)
{
	if (mpAnimation != nullptr) mpAnimation->ChangeAnimation(state);
	if (mpSeparateAnimation != nullptr) mpSeparateAnimation->ChangeAnimation(state);
}

void Model::SetLoop(bool loop)
{
	if (mpAnimation != nullptr) mpAnimation->SetLoop(loop);
	if (mpSeparateAnimation != nullptr) mpSeparateAnimation->SetLoop(loop);
}

void Model::SetLoopFinishState(AnimationState state)
{
	if (mpAnimation != nullptr) mpAnimation->SetLoopFinishState(state);
	if (mpSeparateAnimation != nullptr) mpSeparateAnimation->SetLoopFinishState(state);
}

void Model::SetAnimationBlend(bool isBlend)
{
	if (mpAnimation != nullptr) mpAnimation->SetAnimationBlend(isBlend);
	if (mpSeparateAnimation != nullptr) mpSeparateAnimation->SetAnimationBlend(isBlend);
}

AnimationState Model::GetNowState()
{
	if (mpAnimation != nullptr) return mpAnimation->GetNowState();
	if (mpSeparateAnimation != nullptr) return mpSeparateAnimation->GetNowState();
	return AnimationState::ANIMATION_MAX;
}

bool Model::IsAnimationLoopFinish()
{
	if (mpAnimation != nullptr) return mpAnimation->IsLoopFinish();
	if (mpSeparateAnimation != nullptr) return mpSeparateAnimation->IsLoopFinish();
	return false;
}

// 親モデル特定ボーンへのアタッチメントモデル追加
// 入力: filename(追加モデルパス), attachFrameName(ボーンフレーム名) / 出力: なし / 副作用: AttachmentModel生成
void Model::AddAttachment(std::string filename, std::string attachFrameName)
{
	int frameIndex = MV1SearchFrame(mnHandle, attachFrameName.c_str());
	mpAttachment = new AttachmentModel(filename, mnHandle, frameIndex);
}

// アタッチされたモデルのワールド座標を取得
// 入力: なし / 出力: ワールド座標 / 副作用: なし
VECTOR Model::GetAttachmentPosition()
{
	if (mpAttachment != nullptr)
	{
		VECTOR vec = VGet(0.0f, -50.0f, 0.0f);
		MATRIX matrix = MV1GetFrameLocalWorldMatrix(mpAttachment->GetHandle(), 0);
		return VTransform(vec, matrix);
	}
	return VGet(0.0f, 0.0f, 0.0f);
}

void Model::SetScale(float scale)
{
	MV1SetScale(mnHandle, VGet(scale, scale, scale));
}

void Model::SetTexture(std::string filename, int index)
{
	if (mnChangeTextureHandle != -1)
	{
		DeleteGraph(mnChangeTextureHandle);
	}
	mnChangeTextureHandle = LoadGraph(filename.c_str());
	MV1SetTextureGraphHandle(mnHandle, index, mnChangeTextureHandle, FALSE);
}