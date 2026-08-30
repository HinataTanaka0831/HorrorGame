#include "Exitdoor.h"
#include "Model.h"
#include "Master.h"
#include "Scene.h"
#include "ObjectManager.h"

Exitdoor::Exitdoor(std::string filename, std::string exitCallModelname, VECTOR initPos)
	: Object3D(initPos)
	, mpModel(nullptr)
	, normal(VGet(0.0f, 0.0f, 0.0f))
{
	SetTag(Object3D::TagExitdoor);

	mnCollisionHandle = MV1LoadModel(exitCallModelname.c_str());
	MV1SetPosition(mnCollisionHandle, GetPosition());
	MV1SetupCollInfo(mnCollisionHandle);

	mpModel = new Model(filename, initPos);
}

Exitdoor::~Exitdoor()
{
	MV1DeleteModel(mnCollisionHandle);
	if (mpModel != nullptr)
	{
		delete mpModel;
		mpModel = nullptr;
	}
}

void Exitdoor::Update()
{
	if (mpModel != nullptr)
	{
		mpModel->Update();
	}
}

// ドア3Dモデルの描画
// 入力: なし / 出力: なし / 副作用: バックバッファへの描画
void Exitdoor::Draw()
{
	if (mpModel != nullptr)
	{
		mpModel->Draw();
	}
}

// カプセルとのポリゴン衝突判定
// 入力: pos1(カプセル始点), pos2(カプセル終点), r(半径) / 出力: 衝突時true / 副作用: normal法線ベクトルの更新
bool Exitdoor::CheckHit_Capsule(VECTOR pos1, VECTOR pos2, float r)
{
	MV1_COLL_RESULT_POLY_DIM result = MV1CollCheck_Capsule(mnCollisionHandle, -1, pos1, pos2, r);

	if (result.HitNum >= 1)
	{
		normal = VGet(0.0f, 0.0f, 0.0f);
		for (int i = 0; i < result.HitNum; ++i)
		{
			normal = result.Dim[i].Normal;
		}
	}

	MV1CollResultPolyDimTerminate(result);
	return (result.HitNum >= 1);
}

// 線分との交差判定
// 入力: pos1(始点), pos2(終点) / 出力: 交差座標 / 副作用: なし
VECTOR Exitdoor::CheckHit_Line(VECTOR pos1, VECTOR pos2)
{
	VECTOR ret = GetPosition();
	auto result = MV1CollCheck_Line(mnCollisionHandle, -1, pos1, pos2);

	if (result.HitFlag)
	{
		ret = result.HitPosition;
	}

	return ret;
}