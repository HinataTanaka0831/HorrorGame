#pragma once

#include "DxLib.h"
#include <string>
#include "ModelUtility.h"
#include "ModelAnimation.h"
#include "SeparateModelAnimation.h"

// 前方宣言
class AttachmentModel;


class Model
{
public:
	// Mixamo用処理
	// note: 分割アニメーションを使うかどうかの設定を追加。
	Model(std::string filename, VECTOR initPos, bool isSeparateAnimation = false);   // コンストラクタ
	~Model();  // デストラクタ

	void Update();   // 更新
	void Draw();     // 描画

	// アニメーション切り替え
	void ChangeAnimation(AnimationState state);
	// ループ設定
	void SetLoop(bool loop);
	void SetLoopFinishState(AnimationState state);
	// アニメーションのブレンド設定
	void SetAnimationBlend(bool isBlend);
	// 現在再生されているアニメーションの取得
	AnimationState GetNowState();
	
	// アニメーションのループが終了しているかどうか
	bool IsAnimationLoopFinish();

	// アタッチモデル関連
	// アタッチメントを追加
	void AddAttachment(std::string filename, std::string attachFrameName);
	// アタッチモデルの座標取得
	VECTOR GetAttachmentPosition();


	VECTOR GetPosition() { return mvPosition; }  // 座標取得
	void SetPosition(VECTOR pos) { mvPosition = pos; }  // 座標設定

	VECTOR GetRotation() { return mvRotation; }  // 回転取得
	void SetRotation(VECTOR rot) { mvRotation = rot; }  // 回転設定

	void SetScale(float scale);
	void SetTexture(std::string filename, int index = 0);

	// Mixamo用
	// アニメーションデータの追加
	// note: SeparateModelAnimation クラスへの橋渡し関数
	void AddAnimation(AnimationState state, std::string filename);

private:
	int mnHandle;  // 読み込んだモデルのハンドル
	VECTOR mvPosition;  // 座標
	VECTOR mvRotation;  // 回転
	float mfScale;
	int mnChangeTextureHandle;

	ModelAnimation* mpAnimation;    // モデルアニメーションクラスのポインタ

	// Mixamo用
	// 分割読み込みバージョンのモデルアニメーションクラスのポインタ
	SeparateModelAnimation* mpSeparateAnimation;

	AttachmentModel* mpAttachment;  // アタッチモデル（複数持たせたい場合は std::vector や配列で管理すると良い）

};