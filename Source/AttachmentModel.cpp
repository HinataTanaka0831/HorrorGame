#include "AttachmentModel.h"

AttachmentModel::AttachmentModel(std::string fileName, int parentModelHandle, int parentFrameIndex)
	: Object3D(VGet(0.0f, 0.0f, 0.0f))  // 座標はアタッチ先によって変わるので初期値は(0,0,0)にしておく
	, m_parentHandle(parentModelHandle)
	, m_parentFrameIndex(parentFrameIndex)
{
	// 3Dモデルの読み込み
	m_handle = MV1LoadModel(fileName.c_str());
}

AttachmentModel::~AttachmentModel()
{
	// モデルの破棄
	MV1DeleteModel(m_handle);
}

void AttachmentModel::Update()
{
	// アタッチ先のモデルのフレームの行列情報を取得
	MATRIX matrix = MV1GetFrameLocalWorldMatrix(m_parentHandle, m_parentFrameIndex);

	// 上で取得した行列情報を自身に設定する
	MV1SetMatrix(m_handle, matrix);
}

void AttachmentModel::Draw()
{
	// モデルの描画
	MV1DrawModel(m_handle);
}