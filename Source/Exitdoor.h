#pragma once
#include "Object3D.h"
#include "DxLib.h"
#include <string>

class Model;

// ゲームクリア目標地点となる脱出扉オブジェクトおよび当たり判定管理クラス
class Exitdoor : public Object3D
{
public:
	// ドア描画モデルおよび当たり判定専用コリジョンモデルのロード
	// 入力: filename(描画モデルパス), exitdoorModelname(コリジョンモデルパス), initPos(配置座標) / 出力: なし / 副作用: MV1SetupCollInfo実行
	Exitdoor(std::string filename, std::string exitdoorModelname, VECTOR initPos);
	~Exitdoor();

	void Update() override;

	// ドア3Dモデルの描画
	// 入力: なし / 出力: なし / 副作用: バックバッファへの描画
	void Draw() override;

	// カプセルとのポリゴン衝突判定
	// 入力: pos1(カプセル始点), pos2(カプセル終点), r(半径) / 出力: 衝突時true / 副作用: normal法線ベクトルの更新
	bool CheckHit_Capsule(VECTOR pos1, VECTOR pos2, float r);

	// 線分との交差判定
	// 入力: pos1(始点), pos2(終点) / 出力: 交差座標 / 副作用: なし
	VECTOR CheckHit_Line(VECTOR pos1, VECTOR pos2);

	VECTOR GetNormal() const { return normal; }

private:
	Model* mpModel;
	int mnCollisionHandle;
	VECTOR normal;
};