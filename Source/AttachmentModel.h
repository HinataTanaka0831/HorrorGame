#pragma once

#include "DxLib.h"
#include "Object3D.h"

// 親3Dモデルの特定ボーンフレームに追従して描画される装備品・小物モデルクラス
class AttachmentModel : public Object3D
{
public:
    // 親モデルフレームに追従するアタッチメントモデルのロード
	// 入力: fileName(モデルパス), parentModelHandle(くっつけたいモデル、親モデルハンドル), parentFrameIndex(くっつけたいモデル(追従先)のフレーム番号) / 出力: なし / 副作用: MV1LoadModel実行
	AttachmentModel(const std::string& fileName, int parentModelHandle, int parentFrameIndex);
	~AttachmentModel() override;

	// 親モデルボーンフレームのワールド変換行列を取得し自身に反映
	// 入力: なし / 出力: なし / 副作用: 自身の描画行列の同期
	void Update() override;

	// アタッチメントモデル描画
	// 入力: なし / 出力: なし / 副作用: バックバッファへの描画
	void Draw() override;

public:
	// モデルハンドルの取得
	int GetHandle() { return m_handle; }

private:
	int m_handle;             // モデルハンドル
	int m_parentHandle;       // 親モデルのハンドル
	int m_parentFrameIndex;   // 親モデルのフレーム番号
};