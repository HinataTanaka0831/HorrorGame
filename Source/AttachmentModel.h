#pragma once

#include "DxLib.h"
#include "Object3D.h"

class AttachmentModel : public Object3D
{
public:
	// filename... モデルのファイルパス
	// parentModelHandle... くっつけたいモデル（親モデル）のハンドル
	// parentFrameIndex... くっつけたいモデル（親モデル）のフレーム番号

	AttachmentModel(std::string filename, int parentModelHandle, int parentFrameIndex);
	~AttachmentModel();

	void Update() override;
	void Draw() override;

	// モデルハンドルの取得
	int GetHandle() { return mnHandle; }

private:
	int mnHandle;    // モデルハンドル
	int mnParentHandle;       // 親モデルのハンドル
	int mnParentFrameIndex;   // 親モデルのフレーム番号
};