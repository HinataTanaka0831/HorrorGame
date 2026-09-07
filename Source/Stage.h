#pragma once

#include "Object3D.h"
#include <vector>

// 校舎ステージの3Dグラフィックモデル描画およびDXライブラリ3Dコリジョン判定を管理するクラス
class Stage : public Object3D
{
public:
	// ステージモデルおよび当たり判定用コリジョンモデルのロードと情報構築
	// 入力: filename(描画用モデルパス), collisionFilename(コリジョン用モデルパス) / 出力: なし / 副作用: MV1LoadModelおよびMV1SetupCollInfo実行
	Stage(std::string stageModelName, std::string stageCollisionModelName);
	~Stage() override;

	void Update() override;

	// ステージ3Dモデルの描画
	// 入力: なし / 出力: なし / 副作用: バックバッファへの描画
	void Draw() override;

	// カプセル形状（キャラクター）とステージ壁・床ポリゴンとの衝突判定
	// 入力: position1(カプセル始点), position2(カプセル終点), radius(半径) / 出力: 衝突していればtrue / 副作用: normal(法線), hitpos(衝突点)の更新
	bool CheckHit_Capsule(VECTOR position1, VECTOR position2, float radius);

	// 線分（視線・レイキャスト）とステージジオメトリとの交差判定
	// 入力: position1(始点), position2(終点) / 出力: 交差座標（交差しなければ原点） / 副作用: なし
	VECTOR CheckHit_Line(VECTOR position1, VECTOR position2);

	VECTOR GetNormal() { return m_normal; }
	
	VECTOR GetHitPos() { return m_hitpos; }

	// MV1 の衝突判定関数が必要とするコリジョンハンドルを取得
	// これにより外部から MV1CollCheck_* 系の関数を直接呼び出す
	int GetCollisionHandle() const { return m_collisionHandle; }

private:
	int m_modelHandle;      // ステージモデルのハンドル
	int m_collisionHandle;  // ステージのコリジョンモデルのハンドル

	VECTOR m_normal;
	VECTOR m_hitpos;

};