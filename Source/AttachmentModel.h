#pragma once

#include "DxLib.h"
#include "Object3D.h"

// 親3Dモデルの特定ボーンフレームに追従して描画される装備品・小物モデルクラス
class AttachmentModel : public Object3D
{
public:
	// filename... モデルのファイルパス
	// parentModelHandle... くっつけたいモデル（親モデル）のハンドル
	// parentFrameIndex... くっつけたいモデル（親モデル）のフレーム番号

    // 親モデルフレームに追従するアタッチメントモデルのロード
	// 入力: filename(モデルパス), parentModelHandle(親モデルハンドル), parentFrameIndex(追従先フレーム番号) / 出力: なし / 副作用: MV1LoadModel実行
	AttachmentModel(std::string filename, int parentModelHandle, int parentFrameIndex);
	~AttachmentModel();

	// 親モデルボーンフレームのワールド変換行列を取得し自身に反映
	// 入力: なし / 出力: なし / 副作用: 自身の描画行列の同期
	void Update() override;

	// アタッチメントモデル描画
	// 入力: なし / 出力: なし / 副作用: バックバッファへの描画
	void Draw() override;

	// モデルハンドルの取得
	int GetHandle() { return mnHandle; }

private:
	int mnHandle;             // モデルハンドル
	int mnParentHandle;       // 親モデルのハンドル
	int mnParentFrameIndex;   // 親モデルのフレーム番号
};