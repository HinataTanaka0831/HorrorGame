#include "ModelAnimation.h"

ModelAnimation::ModelAnimation(int modelHandle)
	: m_modelHandle(modelHandle)
	, m_animationTime(0.0f)    
	, m_oldAnimationTime(0.0f)
	, m_animationIndex(-1)
    , m_oldAnimationIndex(-1)
    , m_animationBlendRate(1.0f)
	, m_loop(true)
	, m_loopFinish(false)
	, m_state(AnimationState::AnimationMax)
	, m_loopFinishState(AnimationState::AnimationMax)

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
	if (m_animationBlendRate < 1.0f) {
		m_animationBlendRate += 0.1f;     // += 0.1f はブレンド速度。自由に変えてもok
		if (m_animationBlendRate > 1.0f) m_animationBlendRate = 1.0f;
	}
	// アニメーションの更新
	float fAnimTotalTime = 0.0f;
	if (m_animationIndex != -1) {
		fAnimTotalTime = MV1GetAttachAnimTotalTime(m_modelHandle, m_animationIndex);  // 総再生時間の取得
		m_animationTime += 0.5f;       // アニメーションを進める
		// ループさせる
		if (m_animationTime > fAnimTotalTime) {

			if (!m_loop) {
				// 次のアニメーションが設定されていない場合アニメーションはこれ以上進めず、処理を中断させる
				if (m_loopFinishState == AnimationMax) {
					m_loopFinish = true;
					return;
				}
				ChangeAnimation(m_loopFinishState);   // ループ終了時のアニメーションへ変更
				SetAnimationBlend(false);     		  // ブレンドはしない
				fAnimTotalTime = MV1GetAttachAnimTotalTime(m_modelHandle, m_animationIndex);  // 変更されたので改めて総再生時間を取っておく
			}
			m_animationTime = 0.0f;
		}
		MV1SetAttachAnimTime(m_modelHandle, m_animationIndex, m_animationTime);  		  // アニメーションを反映
		MV1SetAttachAnimBlendRate(m_modelHandle, m_animationIndex, m_animationBlendRate); // ブレンド率を設定
	}
	// 1つ前のアニメーションを更新
	if (m_oldAnimationIndex != -1) {
		fAnimTotalTime = MV1GetAttachAnimTotalTime(m_modelHandle, m_oldAnimationIndex);
		m_oldAnimationTime += 0.5f;
		// ループさせる
		if (m_oldAnimationTime > fAnimTotalTime) m_oldAnimationTime = 0.0f;
		MV1SetAttachAnimTime(m_modelHandle, m_oldAnimationIndex, m_oldAnimationTime);
		MV1SetAttachAnimBlendRate(m_modelHandle, m_oldAnimationIndex, 1.0f - m_animationBlendRate);
	}
}

void ModelAnimation::ChangeAnimation(AnimationState state, int index)
{
	// 切り替えようとしているアニメーションが既に設定されている場合
	if (m_state == state) return;   // 何もしない
	// 切り替え先の番号を保持
	m_state = state;
	// ループ情報の初期化
	m_loop = true;    // 設定が特にない場合はループさせる
	m_loopFinishState = AnimationState::AnimationMax;  // ループ終了時のアニメーションは特になし
	m_loopFinish = false;
	// 1つ前のアニメーションが有効状態であれば
	if (m_oldAnimationIndex != -1) {
		// アニメーションのデタッチ（取り外す）
		MV1DetachAnim(m_modelHandle, m_oldAnimationIndex);
		m_oldAnimationIndex = -1;
	}
	// 現在のアニメーション状態を保持する
	m_oldAnimationIndex = m_animationIndex;
	m_oldAnimationTime = m_animationTime;
	m_animationIndex = MV1AttachAnim(m_modelHandle, (int)state);  	// アニメーションのアタッチ
	m_animationTime = 0.0f;  	// 再生時間の初期化
	// ブレンド状態を初期化
	// ブレンド率は、古いモーションが有効でない場合は1.0f（ブレンドしない状態）にしておく
	m_animationBlendRate = (m_oldAnimationIndex == -1 ? 1.0f : 0.0f);
}

void ModelAnimation::SetAnimationBlend(bool isBlend)
{
	if (isBlend) { // ブレンドする場合
		// ブレンド率は、古いモーションが有効でない場合は1.0f（ブレンドしない状態）にしておく
		m_animationBlendRate = (m_oldAnimationIndex == -1 ? 1.0f : 0.0f);
	}
	else  {  // ブレンドしない場合
		// ブレンドしない状態にする
		m_animationBlendRate = 1.0f;
		// ブレンドする必要がないので、古いアニメーションはデタッチしておく
		if (m_oldAnimationIndex != -1) {
			MV1DetachAnim(m_modelHandle, m_oldAnimationIndex);
			m_oldAnimationIndex = -1;
		}
	}
}