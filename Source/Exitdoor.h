#pragma once
#include "Object3D.h"
#include "DxLib.h"
#include <vector>

class Model;

// ゲームクリア目標地点となる脱出扉オブジェクトおよび当たり判定管理クラス
class Exitdoor : public Object3D
{
public:
	// ドア描画モデルおよび当たり判定専用コリジョンモデルのロード
    // 入力: fileName(描画モデルパス), exitdoorModelname(コリジョンモデルパス), initPosition(配置座標) / 出力: なし / 副作用: MV1SetupCollInfo実行
	Exitdoor(std::string fileName, std::string exitdoorModelname, VECTOR initPosition);
	~Exitdoor();

	void Update() override;

	// ドア3Dモデルの描画
	// 入力: なし / 出力: なし / 副作用: バックバッファへの描画
	void Draw() override;

	// カプセルとのポリゴン衝突判定
	// 入力: position1(カプセル始点), position2(カプセル終点), radius(半径) / 出力: 衝突時true / 副作用: normal法線ベクトルの更新
	bool CheckHit_Capsule(VECTOR position1, VECTOR position2, float radius);

	// 線分との交差判定
	// 入力: position1(始点), position2(終点) / 出力: 交差座標 / 副作用: なし
	VECTOR CheckHit_Line(VECTOR position1, VECTOR position2);

	VECTOR GetNormal() { return m_normal; }


private:
	Model* m_model;  // モデルクラスのポインタ
	int m_collisionHandle;  // 当たり判定モデル
	VECTOR m_normal;
};