#pragma once

#include <string>
#include <vector>
#include "ModelUtility.h"

// 内部アニメーションを持つ単一3Dモデルのアニメーションブレンド・ループ制御クラス
class ModelAnimation
{
public:
	// Mixamoのモデルアニメーションを使用するためここにあった定義を ModelUtility へ移動する
public:
	ModelAnimation(int modelHandle);     // コンストラクタ
	~ModelAnimation();    // デストラクタ

	// アニメーション再生時間の進行および前後モーションのクロスフェードブレンド率更新
	// 入力: なし / 出力: なし / 副作用: モデルのアタッチアニメーション時間・ブレンド率の変更
	void Update();

	// 指定アニメーションステートへの切り替えとブレンド初期化
	// 入力: state(ステート), index(アニメーションインデックス) / 出力: なし / 副作用: 旧アニメーション保持、新アニメーションのアタッチ
	void ChangeAnimation(AnimationState state, int index = 0);  // アニメーション切り替え処理

	void SetLoop(bool isLoop) { mbLoop = isLoop; }  // ループ設定
	void SetLoopFinishState(AnimationState state) { mnLoopFinishState = state; }  // ループ終了時に再生するアニメーション
	void SetAnimationBlend(bool isBlend);     // アニメーションのブレンド設定

	AnimationState GetNowState() { return mnState; }     // 現在再生されているアニメーションの取得
	bool IsLoopFinish() { return mbLoopFinish; }         // アニメーションのループが終了しているかどうか

private:
	int mnModelHandle;      // モデルのハンドル

	float mfAnimationTime;  // 再生しているアニメーションの現在の再生時間
	int mnAnimationIndex;   // 再生しているアニメーションのインデックス

	float mfOldAnimationTime;   // 1つ前のアニメーション再生時間
	int mnOldAnimationIndex;    // 1つ前のアニメーションのインデックス

	float mfAnimBlendRate;      // モーションの切り替わり度合

	AnimationState mnState; // 現在再生しているアニメーションの番号 

	bool mbLoop;            // モーションをループさせるかどうか
	AnimationState mnLoopFinishState;   // ループが終わったときに再生したいアニメーション番号
	bool mbLoopFinish;      // モーションループが終わったかどうか
};