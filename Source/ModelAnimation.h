#pragma once

#include "DxLib.h"
#include <string>
#include <vector>
#include "ModelUtility.h"

// 内部アニメーションを持つ単一3Dモデルのアニメーションブレンド・ループ制御クラス
class ModelAnimation
{
public:
	// Mixamoのモデルアニメーションを使用するためここにあった定義を ModelUtility へ移動する
public:
	// 呼び出されたアニメーションで移動しているフレームがある場合、移動を無効にするためのフレーム番号を検索し無効にさせる。初期アニメーション状態は待機モーションにしておく
	// 入力: modelHandle(モデルハンドル) / 出力: なし / 副作用: モデルのアタッチアニメーションの移動を無効化、初期アニメーション状態を待機モーションに設定
	ModelAnimation(int modelHandle);     
	~ModelAnimation();

	// アニメーション再生時間の進行および前後モーションのクロスフェードブレンド率更新
	// 入力: なし / 出力: なし / 副作用: モデルのアタッチアニメーション時間・ブレンド率の変更
	void Update();

	// 指定アニメーションステートへの切り替えとブレンド初期化
	// 入力: state(ステート), index(アニメーションインデックス) / 出力: なし / 副作用: 旧アニメーション保持、新アニメーションのアタッチ
	void ChangeAnimation(AnimationState state, int index = 0);  // アニメーション切り替え処理

	// アニメーションをブレンドするかどうかの設定
	// 入力: isBlend(ブレンドするかどうか) / 出力: なし / 副作用: ブレンド率の初期化
	void SetAnimationBlend(bool isBlend);     // アニメーションのブレンド設定

	void SetLoop(bool isLoop) { m_loop = isLoop; }  // ループ設定
	void SetLoopFinishState(AnimationState state) { m_loopFinishState = state; }  // ループ終了時に再生するアニメーション

	AnimationState GetNowState() { return m_state; }     // 現在再生されているアニメーションの取得
	bool IsLoopFinish() { return m_loopFinish; }         // アニメーションのループが終了しているかどうか

private:
	int m_modelHandle;      // モデルのハンドル

	float m_animationTime = 0.0f;  // 再生しているアニメーションの現在の再生時間
	int m_animationIndex = -1;   // 再生しているアニメーションのインデックス

	float m_oldAnimationTime = 0.0f;   // 1つ前のアニメーション再生時間
	int m_oldAnimationIndex = -1;    // 1つ前のアニメーションのインデックス

	float m_animationBlendRate = 1.0f;      // モーションの切り替わり度合

	AnimationState m_state = AnimationState::AnimationMax; // 現在再生しているアニメーションの番号 

	bool m_loop = true;            // モーションをループさせるかどうか
	AnimationState m_loopFinishState = AnimationState::AnimationMax;   // ループが終わったときに再生したいアニメーション番号
	bool m_loopFinish = false;      // モーションループが終わったかどうか
};