#pragma once
#include "Object3D.h"
#include "DxLib.h"
#include <vector>

class Model;

class Exitdoor : public Object3D
{
public:
	Exitdoor(std::string filename, std::string exitdoorModelname, VECTOR initPos);
	~Exitdoor();

	void Update() override;
	void Draw() override;

	bool CheckHit_Capsule(VECTOR pos1, VECTOR pos2, float r);

	VECTOR CheckHit_Line(VECTOR pos1, VECTOR pos2);

	VECTOR GetNormal() { return normal; }


private:
	Model* mpModel;  // モデルクラスのポインタ
	int mnCollisionHandle;  // 当たり判定モデル
	VECTOR normal;
};