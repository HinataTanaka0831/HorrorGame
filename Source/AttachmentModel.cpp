#include "AttachmentModel.h"

AttachmentModel::AttachmentModel(std::string filename, int parentModelHandle, int parentFrameIndex)
	: Object3D(VGet(0.0f, 0.0f, 0.0f))
	, mnParentHandle(parentModelHandle)
	, mnParentFrameIndex(parentFrameIndex)
{
	mnHandle = MV1LoadModel(filename.c_str());
}

AttachmentModel::~AttachmentModel()
{
	MV1DeleteModel(mnHandle);
}

// 親モデルボーンフレームのワールド変換行列を取得し自身に反映
// 入力: なし / 出力: なし / 副作用: 自身の描画行列の同期
void AttachmentModel::Update()
{
	MATRIX matrix = MV1GetFrameLocalWorldMatrix(mnParentHandle, mnParentFrameIndex);
	MV1SetMatrix(mnHandle, matrix);
}

// アタッチメントモデル描画
// 入力: なし / 出力: なし / 副作用: バックバッファへの描画
void AttachmentModel::Draw()
{
	MV1DrawModel(mnHandle);
}