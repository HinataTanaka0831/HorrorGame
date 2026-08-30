#include "Exitdoor.h"
#include "Model.h"
#include "Master.h"
#include "Scene.h"
#include "ObjectManager.h"

Exitdoor::Exitdoor(std::string filename, std::string exitCallModelname, VECTOR initPos)
	:Object3D(initPos)
{
	// タグ設定
	SetTag(Object3D::TagExitdoor);

	mnCollisionHandle = MV1LoadModel(exitCallModelname.c_str());

	MV1SetPosition(mnCollisionHandle, GetPosition());

	MV1SetupCollInfo(mnCollisionHandle);

	// モデルの作成
	mpModel = new Model(filename, initPos);
}


Exitdoor::~Exitdoor()
{
	MV1DeleteModel(mnCollisionHandle);

	if (mpModel != nullptr)
	{
		delete mpModel;
	}
}


void Exitdoor::Update()
{
	if (mpModel != nullptr)
	{
		mpModel->Update();
	}
}


void Exitdoor::Draw()
{

	if (mpModel != nullptr)
	{
		mpModel->Draw();
	}


}


// ステージとカプセル型との当たり判定
bool Exitdoor::CheckHit_Capsule(VECTOR pos1, VECTOR pos2, float r)
{
	// 生成しておいた当たり判定情報をもとに、カプセルとの当たり判定を行う
	MV1_COLL_RESULT_POLY_DIM result = MV1CollCheck_Capsule(mnCollisionHandle, -1, pos1, pos2, r);

	// ポリゴンに1つ以上当たっている場合
	if (result.HitNum >= 1)
	{
		normal = VGet(0.0f, 0.0f, 0.0f);

		for (int i = 0; i < result.HitNum; i++)
		{
			normal = result.Dim[i].Normal;

			//DrawTriangle3D(
			//	result.Dim[i].Position[0],
			//	result.Dim[i].Position[1],
			//	result.Dim[i].Position[2],
			//	GetColor(255, 0, 0),
			//	0
			//);


		}


	}

	// 当たり判定情報の後片付け
	MV1CollResultPolyDimTerminate(result);


	return (result.HitNum >= 1);
}


// ステージと線分との当たり判定
VECTOR Exitdoor::CheckHit_Line(VECTOR pos1, VECTOR pos2)
{
	VECTOR ret = GetPosition();

	// 当たり判定情報と線分との当たり判定を行う
	//MV1_COLL_RESULT_POLY_DIM result = MV1CollCheck_LineDim(mnCollisionHandle, -1, pos1, pos2);
	auto result = MV1CollCheck_Line(mnCollisionHandle, -1, pos1, pos2);

	// 当たっていた場合
	if (result.HitFlag)
	{
		// 当たった個所のポジションを return するように取得する
		ret = result.HitPosition;
	}

	// 当たり判定情報の後片付け
	//MV1CollResultPolyDimTerminate(result);


	return ret;
}

