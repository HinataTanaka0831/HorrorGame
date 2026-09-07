#include "ModelAnimation.h"

ModelAnimation::ModelAnimation(int modelHandle)
	: m_modelHandle(modelHandle)
{
	// アニメーションで移動をしているフレーム番号を検索する
	int moveAnimFrameIndex = MV1SearchFrame(m_modelHandle, "root");
	 //移動を無効にする
	MV1SetFrameUserLocalMatrix(
		m_modelHandle,
		moveAnimFrameIndex,
		MV1GetFrameLocalMatrix(m_modelHandle, moveAnimFrameIndex)
	);

	// 初期状態は待機モーションにしておく
	ChangeAnimation(AnimationState::AnimationNeutral);
}

ModelAnimation::~ModelAnimation()
{

}

void ModelAnimation::Update()
{
	// アニメーションのブレンド率を進める
	if (m_animationBlendRate < 1.0f)
	{
		m_animationBlendRate += 0.1f;     // += 0.1f はブレンド速度。自由に変えてもok
		if (m_animationBlendRate > 1.0f)
		{
			m_animationBlendRate = 1.0f;
		}
	}

	// アニメーションの更新
	float fAnimTotalTime = 0.0f;
	if (m_animationIndex != -1)
	{
		// 総再生時間の取得
		fAnimTotalTime = MV1GetAttachAnimTotalTime(m_modelHandle, m_animationIndex);

		// アニメーションを進める
		m_animationTime += 0.5f;

		// ループさせる
		if (m_animationTime > fAnimTotalTime)
		{
			// ループしない設定であれば
			if (!m_loop)
			{
				// 次のアニメーションが設定されていない場合
				if (m_loopFinishState == AnimationMax)
				{
					// アニメーションはこれ以上進めず、処理を中断させる
					m_loopFinish = true;
					return;
				}

				// ループ終了時のアニメーションへ変更
				ChangeAnimation(m_loopFinishState);
				// ブレンドはしない
				SetAnimationBlend(false);
				// 変更されたので改めて総再生時間を取っておく
				fAnimTotalTime = MV1GetAttachAnimTotalTime(m_modelHandle, m_animationIndex);
			}

			m_animationTime = 0.0f;
		}

		// アニメーションを反映
		MV1SetAttachAnimTime(m_modelHandle, m_animationIndex, m_animationTime);

		// ブレンド率を設定
		MV1SetAttachAnimBlendRate(m_modelHandle, m_animationIndex, m_animationBlendRate);

	}

	// 1つ前のアニメーションを更新
	if (m_oldAnimationIndex != -1)
	{
		// 総再生時間の取得
		fAnimTotalTime = MV1GetAttachAnimTotalTime(m_modelHandle, m_oldAnimationIndex);

		// アニメーションを進める
		m_oldAnimationTime += 0.5f;

		// ループさせる
		if (m_oldAnimationTime > fAnimTotalTime)
		{
			m_oldAnimationTime = 0.0f;
		}

		// アニメーションを反映
		MV1SetAttachAnimTime(m_modelHandle, m_oldAnimationIndex, m_oldAnimationTime);

		// ブレンド率を設定
		MV1SetAttachAnimBlendRate(m_modelHandle, m_oldAnimationIndex, 1.0f - m_animationBlendRate);

	}

}

void ModelAnimation::ChangeAnimation(AnimationState state, int index)
{
	// 切り替えようとしているアニメーションが既に設定されている場合
	if (m_state == state)
	{
		return;   // 何もしない
	}

	// 切り替え先の番号を保持
	m_state = state;

	// ループ情報の初期化
	m_loop = true;    // 設定が特にない場合はループさせる
	m_loopFinishState = AnimationState::AnimationMax;  // ループ終了時のアニメーションは特になし
	m_loopFinish = false;

	// 1つ前のアニメーションが有効状態であれば
	if (m_oldAnimationIndex != -1)
	{
		// アニメーションのデタッチ（取り外す）
		MV1DetachAnim(m_modelHandle, m_oldAnimationIndex);
		m_oldAnimationIndex = -1;
	}

	// 現在のアニメーション状態を保持する
	m_oldAnimationIndex = m_animationIndex;
	m_oldAnimationTime = m_animationTime;

	// アニメーションのアタッチ
	m_animationIndex = MV1AttachAnim(m_modelHandle, (int)state);

	// 再生時間の初期化
	m_animationTime = 0.0f;

	// ブレンド状態を初期化
	// ブレンド率は、古いモーションが有効でない場合は1.0f（ブレンドしない状態）にしておく
	m_animationBlendRate = (m_oldAnimationIndex == -1 ? 1.0f : 0.0f);  //→→if (m_oldAnimationIndex == -1)
                                                                       	//{
	                                                                    //	m_animationBlendRate = 1.0f;
	                                                                    //}
	                                                                    //else 
	                                                                    //{
	                                                                    //	m_animationBlendRate = 0.0f;
	                                                                    //}



}

void ModelAnimation::SetAnimationBlend(bool isBlend)
{
	if (isBlend)  // ブレンドする場合
	{
		// ブレンド率は、古いモーションが有効でない場合は1.0f（ブレンドしない状態）にしておく
		m_animationBlendRate = (m_oldAnimationIndex == -1 ? 1.0f : 0.0f);
	}
	else    // ブレンドしない場合
	{
		// ブレンドしない状態にする
		m_animationBlendRate = 1.0f;

		// ブレンドする必要がないので、古いアニメーションはデタッチしておく
		if (m_oldAnimationIndex != -1)
		{
			MV1DetachAnim(m_modelHandle, m_oldAnimationIndex);
			m_oldAnimationIndex = -1;
		}
	}
}