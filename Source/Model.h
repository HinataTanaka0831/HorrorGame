#pragma once

#include "DxLib.h"
#include <string>
#include "ModelUtility.h"

// 前方宣言
class AttachmentModel;
class ModelAnimation;
class SeparateModelAnimation;


// 3Dモデルアセットのロード、トランスフォーム、アニメーション制御、アタッチメントを統括するクラス
class Model
{
public:
	// Mixamo用処理
	// note: 分割アニメーションを使うかどうかの設定を追加。

	// 3Dモデルのロードおよび通常/分割アニメーションコントローラの生成
	// 入力: fileName(モデルファイルパス), initPosition(初期配置座標), isSeparateAnimation(外部分割アニメーションを使用するか) / 出力: なし / 副作用: MV1LoadModelおよびアニメーションクラス生成
	Model(std::string fileName, VECTOR initPosition, bool isSeparateAnimation = false);
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
	// 入力: fileName(追加モデルパス), attachFrameName(ボーンフレーム名) / 出力: なし / 副作用: AttachmentModel生成	
	void AddAttachment(std::string fileName, std::string attachFrameName);

	// アタッチされたモデルのワールド座標を取得
	// 入力: なし / 出力: ワールド座標 / 副作用: なし
	VECTOR GetAttachmentPosition();


	VECTOR GetPosition() { return m_position; }  // 座標取得
	void SetPosition(VECTOR position) { m_position = position; }  // 座標設定

	VECTOR GetRotation() { return m_rotation; }  // 回転取得
	void SetRotation(VECTOR rotation) { m_rotation = rotation; }  // 回転設定

	// スケール取得・設定
	void SetScale(float scale);
	// 読み込んだモデルのテクスチャを変更
	// 入力: fileName(テクスチャパス), index(変更するテクスチャ番号) / 出力: なし / 副作用: MV1SetTextureGraphHandleの実行
	void SetTexture(std::string fileName, int index = 0);

	// 外部モーションファイルの追加登録（Mixamo分割ファイル用）
	// 入力: state(紐づけるステート), fileName(アニメーションモデルパス) / 出力: なし / 副作用: SeparateModelAnimationへの登録
	void AddAnimation(AnimationState state, std::string fileName);

private:
	int m_handle;  // 読み込んだモデルのハンドル
	VECTOR m_position;  // 座標
	VECTOR m_rotation;  // 回転
	float m_scale;
	int m_changeTextureHandle;

	ModelAnimation* m_animation;    // モデルアニメーションクラスのポインタ

	// Mixamo用
	// 分割読み込みバージョンのモデルアニメーションクラスのポインタ
	SeparateModelAnimation* m_separateAnimation;

	AttachmentModel* m_attachment;  // アタッチモデル（複数持たせたい場合は std::vector や配列で管理すると良い）

};