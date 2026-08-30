#pragma once

#include "DxLib.h"
#include <string>
#include "ModelUtility.h"
#include "ModelAnimation.h"
#include "SeparateModelAnimation.h"

class AttachmentModel;

// 3Dモデルアセットのロード、トランスフォーム、アニメーション制御、アタッチメントを統括するクラス
class Model
{
public:
	// 3Dモデルのロードおよび通常/分割アニメーションコントローラの生成
	// 入力: filename(モデルファイルパス), initPos(初期配置座標), isSeparateAnimation(外部分割アニメーションを使用するか) / 出力: なし / 副作用: MV1LoadModelおよびアニメーションクラス生成
	Model(std::string filename, VECTOR initPos, bool isSeparateAnimation = false);
	~Model();

	// アニメーション再生時間の進行および座標・回転の3Dモデルへの反映
	// 入力: なし / 出力: なし / 副作用: DXライブラリモデル変換行列の更新
	void Update();

	// 3Dモデル描画
	// 入力: なし / 出力: なし / 副作用: バックバッファへの3D描画
	void Draw();

	// 指定アニメーションステートへの切り替え
	// 入力: state(変更先ステート) / 出力: なし / 副作用: アニメーションコントローラのステート変更
	void ChangeAnimation(AnimationState state);

	void SetLoop(bool loop);
	void SetLoopFinishState(AnimationState state);
	void SetAnimationBlend(bool isBlend);
	AnimationState GetNowState();
	bool IsAnimationLoopFinish();

	// 親モデル特定ボーンへのアタッチメントモデル追加
	// 入力: filename(追加モデルパス), attachFrameName(ボーンフレーム名) / 出力: なし / 副作用: AttachmentModel生成
	void AddAttachment(std::string filename, std::string attachFrameName);

	// アタッチされたモデルのワールド座標を取得
	// 入力: なし / 出力: ワールド座標 / 副作用: なし
	VECTOR GetAttachmentPosition();

	VECTOR GetPosition() const { return mvPosition; }
	void SetPosition(VECTOR pos) { mvPosition = pos; }

	VECTOR GetRotation() const { return mvRotation; }
	void SetRotation(VECTOR rot) { mvRotation = rot; }

	void SetScale(float scale);
	void SetTexture(std::string filename, int index = 0);

	// 外部モーションファイルの追加登録（Mixamo分割ファイル用）
	// 入力: state(紐づけるステート), filename(アニメーションモデルパス) / 出力: なし / 副作用: SeparateModelAnimationへの登録
	void AddAnimation(AnimationState state, std::string filename);

private:
	int mnHandle;
	VECTOR mvPosition;
	VECTOR mvRotation;
	float mfScale;
	int mnChangeTextureHandle;

	ModelAnimation* mpAnimation;
	SeparateModelAnimation* mpSeparateAnimation;
	AttachmentModel* mpAttachment;
};