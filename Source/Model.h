#pragma once

#include "DxLib.h"
#include <string>
#include "ModelUtility.h"
#include "ModelAnimation.h"
#include "SeparateModelAnimation.h"

// 前方宣言
class AttachmentModel;

// 3Dモデルアセットのロード、トランスフォーム、アニメーション制御、アタッチメントを統括するクラス
class Model
{
public:
	// Mixamo用処理
	// note: 分割アニメーションを使うかどうかの設定を追加。

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
	void ChangeAnimation(AnimationState state);	// ループ設定

	void SetLoop(bool loop);
	void SetLoopFinishState(AnimationState state);
	// アニメーションのブレンド設定
	void SetAnimationBlend(bool isBlend);
	// 現在再生されているアニメーションの取得
	AnimationState GetNowState();
	
	// アニメーションのループが終了しているかどうか
	bool IsAnimationLoopFinish();

	// 親モデル特定ボーンへのアタッチメントモデル追加
	// 入力: filename(追加モデルパス), attachFrameName(ボーンフレーム名) / 出力: なし / 副作用: AttachmentModel生成	
	void AddAttachment(std::string filename, std::string attachFrameName);

	// アタッチされたモデルのワールド座標を取得
	// 入力: なし / 出力: ワールド座標 / 副作用: なし
	VECTOR GetAttachmentPosition();


	VECTOR GetPosition() { return mvPosition; }  // 座標取得
	void SetPosition(VECTOR pos) { mvPosition = pos; }  // 座標設定

	VECTOR GetRotation() { return mvRotation; }  // 回転取得
	void SetRotation(VECTOR rot) { mvRotation = rot; }  // 回転設定

	void SetScale(float scale);
	void SetTexture(std::string filename, int index = 0);

	// 外部モーションファイルの追加登録（Mixamo分割ファイル用）
	// 入力: state(紐づけるステート), filename(アニメーションモデルパス) / 出力: なし / 副作用: SeparateModelAnimationへの登録
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