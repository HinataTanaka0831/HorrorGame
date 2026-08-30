#pragma once

#include <string>
#include <vector>
#include "ModelUtility.h"

// 内部アニメーションを持つ単一3Dモデルのアニメーションブレンド・ループ制御クラス
class ModelAnimation
{
public:
	ModelAnimation(int modelHandle);
	~ModelAnimation();

	// アニメーション再生時間の進行および前後モーションのクロスフェードブレンド率更新
	// 入力: なし / 出力: なし / 副作用: モデルのアタッチアニメーション時間・ブレンド率の変更
	void Update();

	// 指定アニメーションステートへの切り替えとブレンド初期化
	// 入力: state(ステート), index(アニメーションインデックス) / 出力: なし / 副作用: 旧アニメーション保持、新アニメーションのアタッチ
	void ChangeAnimation(AnimationState state, int index = 0);

	void SetLoop(bool isLoop) { mbLoop = isLoop; }
	void SetLoopFinishState(AnimationState state) { mnLoopFinishState = state; }
	void SetAnimationBlend(bool isBlend);

	AnimationState GetNowState() const { return mnState; }
	bool IsLoopFinish() const { return mbLoopFinish; }

private:
	int mnModelHandle;
	float mfAnimationTime;
	int mnAnimationIndex;
	float mfOldAnimationTime;
	int mnOldAnimationIndex;
	float mfAnimBlendRate;
	AnimationState mnState;
	bool mbLoop;
	AnimationState mnLoopFinishState;
	bool mbLoopFinish;
};