#include "DxLib.h"
#include "ModelAnimation.h"

// コンストラクタ
ModelAnimation::ModelAnimation(int modelHandle)
	: mnModelHandle(modelHandle)
	, mfAnimationTime(0.0f)
	, mnAnimationIndex(-1)   
	, mfOldAnimationTime(0.0f)
	, mnOldAnimationIndex(-1)
	, mfAnimBlendRate(1.0f)
	, mnState(AnimationState::ANIMATION_MAX)     // 最初は最大値としておく
	, mbLoop(true)
	, mnLoopFinishState(AnimationState::ANIMATION_MAX)
	, mbLoopFinish(false)
{
	// アニメーションで移動をしているフレーム番号を検索する
	int moveAnimFrameIndex = MV1SearchFrame(mnModelHandle, "root");
	 //移動を無効にする
	MV1SetFrameUserLocalMatrix(
		mnModelHandle,
		moveAnimFrameIndex,
		MV1GetFrameLocalMatrix(mnModelHandle, moveAnimFrameIndex)
	);

	// 初期状態は待機モーションにしておく
	ChangeAnimation(AnimationState::ANIMATION_NEUTRAL);
}

// デストラクタ
ModelAnimation::~ModelAnimation()
{

}

// 更新
void ModelAnimation::Update()
{
	// アニメーションのブレンド率を進める
	if (mfAnimBlendRate < 1.0f)
	{
		mfAnimBlendRate += 0.1f;     // += 0.1f はブレンド速度。自由に変えてもok
		if (mfAnimBlendRate > 1.0f)
		{
			mfAnimBlendRate = 1.0f;
		}
	}

	// アニメーションの更新
	float fAnimTotalTime = 0.0f;
	if (mnAnimationIndex != -1)
	{
		// 総再生時間の取得
		fAnimTotalTime = MV1GetAttachAnimTotalTime(mnModelHandle, mnAnimationIndex);

		// アニメーションを進める
		mfAnimationTime += 0.5f;

		// ループさせる
		if (mfAnimationTime > fAnimTotalTime)
		{
			// ループしない設定であれば
			if (!mbLoop)
			{
				// 次のアニメーションが設定されていない場合
				if (mnLoopFinishState == ANIMATION_MAX)
				{
					// アニメーションはこれ以上進めず、処理を中断させる
					mbLoopFinish = true;
					return;
				}

				// ループ終了時のアニメーションへ変更
				ChangeAnimation(mnLoopFinishState);
				// ブレンドはしない
				SetAnimationBlend(false);
				// 変更されたので改めて総再生時間を取っておく
				fAnimTotalTime = MV1GetAttachAnimTotalTime(mnModelHandle, mnAnimationIndex);
			}

			mfAnimationTime = 0.0f;
		}

		// アニメーションを反映
		MV1SetAttachAnimTime(mnModelHandle, mnAnimationIndex, mfAnimationTime);

		// ブレンド率を設定
		MV1SetAttachAnimBlendRate(mnModelHandle, mnAnimationIndex, mfAnimBlendRate);

	}

	// 1つ前のアニメーションを更新
	if (mnOldAnimationIndex != -1)
	{
		// 総再生時間の取得
		fAnimTotalTime = MV1GetAttachAnimTotalTime(mnModelHandle, mnOldAnimationIndex);

		// アニメーションを進める
		mfOldAnimationTime += 0.5f;

		// ループさせる
		if (mfOldAnimationTime > fAnimTotalTime)
		{
			mfOldAnimationTime = 0.0f;
		}

		// アニメーションを反映
		MV1SetAttachAnimTime(mnModelHandle, mnOldAnimationIndex, mfOldAnimationTime);

		// ブレンド率を設定
		MV1SetAttachAnimBlendRate(mnModelHandle, mnOldAnimationIndex, 1.0f - mfAnimBlendRate);

	}

}

// アニメーションの切り替え処理
void ModelAnimation::ChangeAnimation(AnimationState state, int index)
{
	// 切り替えようとしているアニメーションが既に設定されている場合
	if (mnState == state)
	{
		return;   // 何もしない
	}

	// 切り替え先の番号を保持
	mnState = state;

	// ループ情報の初期化
	mbLoop = true;    // 設定が特にない場合はループさせる
	mnLoopFinishState = AnimationState::ANIMATION_MAX;  // ループ終了時のアニメーションは特になし
	mbLoopFinish = false;

	// 1つ前のアニメーションが有効状態であれば
	if (mnOldAnimationIndex != -1)
	{
		// アニメーションのデタッチ（取り外す）
		MV1DetachAnim(mnModelHandle, mnOldAnimationIndex);
		mnOldAnimationIndex = -1;
	}

	// 現在のアニメーション状態を保持する
	mnOldAnimationIndex = mnAnimationIndex;
	mfOldAnimationTime = mfAnimationTime;

	// アニメーションのアタッチ
	mnAnimationIndex = MV1AttachAnim(mnModelHandle, (int)state);

	// 再生時間の初期化
	mfAnimationTime = 0.0f;

	// ブレンド状態を初期化
	// ブレンド率は、古いモーションが有効でない場合は1.0f（ブレンドしない状態）にしておく
	mfAnimBlendRate = (mnOldAnimationIndex == -1 ? 1.0f : 0.0f);  //→→if (mnOldAnimationIndex == -1)
                                                                       	//{
	                                                                    //	mfAnimBlendRate = 1.0f;
	                                                                    //}
	                                                                    //else 
	                                                                    //{
	                                                                    //	mfAnimBlendRate = 0.0f;
	                                                                    //}



}

// アニメーションのブレンド設定
void ModelAnimation::SetAnimationBlend(bool isBlend)
{
	if (isBlend)  // ブレンドする場合
	{
		// ブレンド率は、古いモーションが有効でない場合は1.0f（ブレンドしない状態）にしておく
		mfAnimBlendRate = (mnOldAnimationIndex == -1 ? 1.0f : 0.0f);
	}
	else    // ブレンドしない場合
	{
		// ブレンドしない状態にする
		mfAnimBlendRate = 1.0f;

		// ブレンドする必要がないので、古いアニメーションはデタッチしておく
		if (mnOldAnimationIndex != -1)
		{
			MV1DetachAnim(mnModelHandle, mnOldAnimationIndex);
			mnOldAnimationIndex = -1;
		}
	}
}