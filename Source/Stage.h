#pragma once

#include "Object3D.h"
#include <vector>

class Stage : public Object3D
{
public:
	Stage(std::string stageModelName, std::string stageCollisionModelName);
	~Stage() override;

	void Update() override;
	void Draw() override;

	// ステージとカプセル型との当たり判定
	bool CheckHit_Capsule(VECTOR pos1, VECTOR pos2, float r);

	// ステージと線分との当たり判定
	VECTOR CheckHit_Line(VECTOR pos1, VECTOR pos2);

	VECTOR GetNormal() { return normal; }
	
	VECTOR GetHitPos() { return hitpos; }

	// ★ 追加 ★
	// MV1 の衝突判定関数が必要とするコリジョンハンドルを取得します
	// これにより外部から MV1CollCheck_* 系の関数を直接呼び出せます
	int GetCollisionHandle() const { return mnCollisionHandle; }

private:
	int mnModelHandle;      // ステージモデルのハンドル
	int mnCollisionHandle;  // ステージのコリジョンモデルのハンドル

	VECTOR normal;
	VECTOR hitpos;

};