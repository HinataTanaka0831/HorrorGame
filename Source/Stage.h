#pragma once

#include "Object3D.h"

// 校舎ステージの3Dグラフィックモデル描画およびDXライブラリ3Dコリジョン判定を管理するクラス
class Stage : public Object3D
{
public:
	// ステージモデルおよび当たり判定用コリジョンモデルのロードと情報構築
	// 入力: filename(描画用モデルパス), collisionFilename(コリジョン用モデルパス) / 出力: なし / 副作用: MV1LoadModelおよびMV1SetupCollInfo実行
	Stage(std::string stageModelName);
	~Stage() override;

	// ステージ3Dモデルの描画
	// 入力: なし / 出力: なし / 副作用: バックバッファへの描画
	void Draw() override;

private:
	int m_modelHandle;      // ステージモデルのハンドル
};