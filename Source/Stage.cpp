#include "Stage.h"
#include "Player3D.h"
#include "Master.h"
#include "ObjectManager.h"
#include "Scene.h"

Stage::Stage(std::string stageModelName, std::string stageCollisionModelName)
	: Object3D(VGet(0.0f, 0.0f, 0.0f))  // 座標は原点としておく
{
	SetTag(Object3D::TagStage);

	// ステージモデルの読み込み
	mnModelHandle = MV1LoadModel(stageModelName.c_str());
	
	// コリジョンモデル（当たり判定用モデル）の読み込み
	mnCollisionHandle = MV1LoadModel(stageCollisionModelName.c_str());

	// 当たり判定情報の作成
	MV1SetupCollInfo(mnCollisionHandle);

}

Stage::~Stage()
{
	// 読み込んだモデルデータの破棄
	MV1DeleteModel(mnModelHandle);
	MV1DeleteModel(mnCollisionHandle);
}

void Stage::Update()
{

}

void Stage::Draw()
{

	// ステージの描画
	MV1DrawModel(mnModelHandle);

	// コリジョンモデルの描画（ワイヤーフレームみたいな感じで描画）
	//MV1DrawModelDebug(mnCollisionHandle, GetColor(255, 255, 255), 1, 10, 1, 0);
}

bool Stage::CheckHit_Capsule(VECTOR pos1, VECTOR pos2, float r)
{
	// 生成しておいた当たり判定情報をもとに、カプセルとの当たり判定を行う
	MV1_COLL_RESULT_POLY_DIM result = MV1CollCheck_Capsule(mnCollisionHandle, -1, pos1, pos2, r);

	// ポリゴンに1つ以上当たっている場合
	if (result.HitNum >= 1)
	{
		float minDist = 1e9f;
		VECTOR bestNormal = VGet(0, 0, 0);
		VECTOR bestHitPos = VGet(0, 0, 0);

		for (int i = 0; i < result.HitNum; i++)
		{
			float dist = VSize(VSub(result.Dim[i].HitPosition, pos1));

			if (dist < minDist)
			{
				minDist = dist;
				bestNormal = result.Dim[i].Normal;
				bestHitPos = result.Dim[i].HitPosition;
			}

			//DrawTriangle3D(
			//	result.Dim[i].Position[0],
			//	result.Dim[i].Position[1],
			//	result.Dim[i].Position[2],
			//	GetColor(255, 0, 0),
			//	0
			//);

		}

		normal = bestNormal;
		hitpos = bestHitPos;


	}

	// 当たり判定情報の後片付け
	MV1CollResultPolyDimTerminate(result);

	return (result.HitNum >= 1);
}


VECTOR Stage::CheckHit_Line(VECTOR pos1, VECTOR pos2)
{
	VECTOR ret = VGet(0.0f, 0.0f, 0.0f);

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

