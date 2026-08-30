#pragma once

#include <string>
#include <vector>
#include "ModelUtility.h"

// 外部ファイル群から個別に読み込んだMixamo系モーションの動的アタッチ・ブレンド再生を管理するクラス
class SeparateModelAnimation
{
public:
	SeparateModelAnimation(int modelHandle);
	~SeparateModelAnimation();

	// モーション再生時間の進行および前後モーションのクロスフェードブレンド制御
	// 入力: なし / 出力: なし / 副作用: アタッチアニメーション再生時間・ブレンド率の更新
	void Update();

	// 指定ステートへのモーション変更およびクロスフェード開始
	// 入力: state(切り替え先ステート), index(アタッチインデックス) / 出力: なし / 副作用: 旧モーションデタッチ予約、新モーションアタッチ
	void ChangeAnimation(AnimationState state, int index = 0);

	// 外部モーションファイルのロードとステート登録
	// 入力: state(ステート), filename(ファイルパス) / 出力: なし / 副作用: MV1LoadModel実行、mAnimationInfoListへの登録
	void AddAnimation(AnimationState state, std::string filename);

	// 登録済みモーションハンドルの検索取得
	// 入力: state(ステート) / 出力: モーションモデルハンドル(-1で見つからない) / 副作用: なし
	int GetAnimationHandle(AnimationState state);

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

	std::vector<AnimationInfo*> mAnimationInfoList;
};