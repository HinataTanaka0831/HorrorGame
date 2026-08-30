#include "ModelAnimation.h"
#include "DxLib.h"

ModelAnimation::ModelAnimation(int modelHandle)
	: mnModelHandle(modelHandle)
	, mfAnimationTime(0.0f)
	, mnAnimationIndex(-1)
	, mfOldAnimationTime(0.0f)
	, mnOldAnimationIndex(-1)
	, mfAnimBlendRate(1.0f)
	, mnState(AnimationState::ANIMATION_MAX)
	, mbLoop(true)
	, mnLoopFinishState(AnimationState::ANIMATION_MAX)
	, mbLoopFinish(false)
{
	ChangeAnimation(AnimationState::ANIMATION_NEUTRAL);
}

ModelAnimation::~ModelAnimation()
{
	if (mnAnimationIndex != -1) MV1DetachAnim(mnModelHandle, mnAnimationIndex);
	if (mnOldAnimationIndex != -1) MV1DetachAnim(mnModelHandle, mnOldAnimationIndex);
}

// アニメーション再生時間の進行および前後モーションのクロスフェードブレンド率更新
// 入力: なし / 出力: なし / 副作用: モデルのアタッチアニメーション時間・ブレンド率の変更
void ModelAnimation::Update()
{
	if (mfAnimBlendRate < 1.0f)
	{
		mfAnimBlendRate += 0.1f;
		if (mfAnimBlendRate > 1.0f) mfAnimBlendRate = 1.0f;
	}

	float fAnimTotalTime = 0.0f;
	if (mnAnimationIndex != -1)
	{
		fAnimTotalTime = MV1GetAttachAnimTotalTime(mnModelHandle, mnAnimationIndex);
		mfAnimationTime += 0.5f;

		if (mfAnimationTime > fAnimTotalTime)
		{
			if (!mbLoop)
			{
				if (mnLoopFinishState == ANIMATION_MAX)
				{
					mbLoopFinish = true;
					return;
				}
				ChangeAnimation(mnLoopFinishState);
				SetAnimationBlend(false);
				fAnimTotalTime = MV1GetAttachAnimTotalTime(mnModelHandle, mnAnimationIndex);
			}
			mfAnimationTime = 0.0f;
		}

		MV1SetAttachAnimTime(mnModelHandle, mnAnimationIndex, mfAnimationTime);
		MV1SetAttachAnimBlendRate(mnModelHandle, mnAnimationIndex, mfAnimBlendRate);
	}

	if (mnOldAnimationIndex != -1)
	{
		fAnimTotalTime = MV1GetAttachAnimTotalTime(mnModelHandle, mnOldAnimationIndex);
		mfOldAnimationTime += 0.5f;

		if (mfOldAnimationTime > fAnimTotalTime)
		{
			mfOldAnimationTime = 0.0f;
		}

		MV1SetAttachAnimTime(mnModelHandle, mnOldAnimationIndex, mfOldAnimationTime);
		MV1SetAttachAnimBlendRate(mnModelHandle, mnOldAnimationIndex, 1.0f - mfAnimBlendRate);
	}
}

// 指定アニメーションステートへの切り替えとブレンド初期化
// 入力: state(ステート), index(アニメーションインデックス) / 出力: なし / 副作用: 旧アニメーション保持、新アニメーションのアタッチ
void ModelAnimation::ChangeAnimation(AnimationState state, int index)
{
	if (mnState == state) return;

	mnState = state;
	mbLoop = true;
	mnLoopFinishState = AnimationState::ANIMATION_MAX;
	mbLoopFinish = false;

	if (mnOldAnimationIndex != -1)
	{
		MV1DetachAnim(mnModelHandle, mnOldAnimationIndex);
		mnOldAnimationIndex = -1;
	}

	mnOldAnimationIndex = mnAnimationIndex;
	mfOldAnimationTime = mfAnimationTime;

	mnAnimationIndex = MV1AttachAnim(mnModelHandle, index, -1, FALSE);
	mfAnimationTime = 0.0f;
	mfAnimBlendRate = (mnOldAnimationIndex == -1 ? 1.0f : 0.0f);
}

void ModelAnimation::SetAnimationBlend(bool isBlend)
{
	if (isBlend)
	{
		mfAnimBlendRate = (mnOldAnimationIndex == -1 ? 1.0f : 0.0f);
	}
	else
	{
		mfAnimBlendRate = 1.0f;
		if (mnOldAnimationIndex != -1)
		{
			MV1DetachAnim(mnModelHandle, mnOldAnimationIndex);
			mnOldAnimationIndex = -1;
		}
	}
}