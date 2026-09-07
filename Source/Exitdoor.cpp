#include "Exitdoor.h"
#include "Model.h"
#include "Master.h"
#include "Scene.h"
#include "ObjectManager.h"

Exitdoor::Exitdoor(std::string fileName, std::string exitCallModelname, VECTOR initPosition)
	:Object3D(initPosition)
{
	// タグ設定
	SetTag(Object3D::TagExitdoor);

	m_collisionHandle = MV1LoadModel(exitCallModelname.c_str());

	MV1SetPosition(m_collisionHandle, GetPosition());

	MV1SetupCollInfo(m_collisionHandle);

	// モデルの作成
	m_model = new Model(fileName, initPosition);
}


Exitdoor::~Exitdoor()
{
	MV1DeleteModel(m_collisionHandle);

	if (m_model != nullptr)
	{
		delete m_model;
	}
}


void Exitdoor::Update()
{
	if (m_model != nullptr)
	{
		m_model->Update();
	}
}


void Exitdoor::Draw()
{

	if (m_model != nullptr)
	{
		m_model->Draw();
	}


}


// ステージとカプセル型との当たり判定
bool Exitdoor::CheckHit_Capsule(VECTOR position1, VECTOR position2, float radius)
{
	// 生成しておいた当たり判定情報をもとに、カプセルとの当たり判定を行う
	MV1_COLL_RESULT_POLY_DIM result = MV1CollCheck_Capsule(m_collisionHandle, -1, position1, position2, radius);

	// ポリゴンに1つ以上当たっている場合
	if (result.HitNum >= 1)
	{
		m_normal = VGet(0.0f, 0.0f, 0.0f);

		for (int i = 0; i < result.HitNum; i++)
		{
			m_normal = result.Dim[i].Normal;

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
VECTOR Exitdoor::CheckHit_Line(VECTOR position1, VECTOR position2)
{
	VECTOR ret = GetPosition();

	// 当たり判定情報と線分との当たり判定を行う
	//MV1_COLL_RESULT_POLY_DIM result = MV1CollCheck_LineDim(mnCollisionHandle, -1, pos1, pos2);
	auto result = MV1CollCheck_Line(m_collisionHandle, -1, position1, position2);

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

