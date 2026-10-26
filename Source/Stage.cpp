#include "Stage.h"
#include "Player3D.h"
#include "Master.h"
#include "ObjectManager.h"
#include "Scene.h"
#include <fstream>
#include <sstream>
#include <string>


Stage::Stage(std::string stageModelName, std::string stageCollisionModelName)
	: Object3D(VGet(0.0f, 0.0f, 0.0f))  // 座標は原点としておく
	, m_modelHandle(-1)
	, m_collisionHandle(-1)
	, m_normal(VGet(0.0f, 0.0f, 0.0f))
	, m_hitpos(VGet(0.0f, 0.0f, 0.0f))

{
	SetTag(Object3D::TagStage);

	// ステージモデルの読み込み
	m_modelHandle = MV1LoadModel(stageModelName.c_str());
	
	// コリジョンモデル（当たり判定用モデル）の読み込み
	//m_collisionHandle = MV1LoadModel(stageCollisionModelName.c_str());

	// 当たり判定情報の作成
	//MV1SetupCollInfo(m_collisionHandle);

	LoadCollisionCSV("Resource/Data/collisionData_stage.csv");
}

Stage::~Stage()
{
	// 読み込んだモデルデータの破棄
	MV1DeleteModel(m_modelHandle);
	MV1DeleteModel(m_collisionHandle);
}

void Stage::Update()
{

}

void Stage::Draw()
{

	// ステージの描画
	MV1DrawModel(m_modelHandle);

	// コリジョンモデルの描画（ワイヤーフレームみたいな感じで描画）
	//MV1DrawModelDebug(m_collisionHandle, GetColor(255, 255, 255), 1, 10, 1, 0);

	for (const auto& walls : m_walls)
	{
		DrawCube3D(walls.min, walls.max, GetColor(255, 0, 0), GetColor(255, 0, 0), false);

	}

}

void Stage::LoadCollisionCSV(const char* fileName)
{
	std::ifstream file(fileName);

	if (!file.is_open())
	{
		return;
	}

	std::string line;

	// 1行目を捨てる
	std::getline(file, line);

	while (std::getline(file, line))
	{
		std::stringstream ss(line);

		std::string token;

		ColliderInfo info;

		std::getline(ss, info.type, ',');

		std::getline(ss, token, ',');
		info.minX = std::stof(token);

		std::getline(ss, token, ',');
		info.minY = std::stof(token);

		std::getline(ss, token, ',');
		info.minZ = std::stof(token);

		std::getline(ss, token, ',');
		info.maxX = std::stof(token);

		std::getline(ss, token, ',');
		info.maxY = std::stof(token);

		std::getline(ss, token, ',');
		info.maxZ = std::stof(token);

		AABB box;

		box.min = { info.minX, info.minY, info.minZ };
		box.max = { info.maxX, info.maxY, info.maxZ };

		if (info.type == "Wall")
		{
			m_walls.push_back(box);
		}
		else if (info.type == "ClassRooms")
		{
			m_classrooms.push_back(box);
		}
		else if (info.type == "Stairs")
		{
			m_stairs.push_back(box);
		}
	}
	file.close();
}

//bool Stage::CheckHit_Capsule(VECTOR pos1, VECTOR pos2, float r)
//{
//	// 生成しておいた当たり判定情報をもとに、カプセルとの当たり判定を行う
//	MV1_COLL_RESULT_POLY_DIM result = MV1CollCheck_Capsule(m_collisionHandle, -1, pos1, pos2, r);
//
//	// ポリゴンに1つ以上当たっている場合
//	if (result.HitNum >= 1)
//	{
//		float minDist = 1e9f;
//		VECTOR bestNormal = VGet(0, 0, 0);
//		VECTOR bestHitPos = VGet(0, 0, 0);
//
//		for (int i = 0; i < result.HitNum; i++)
//		{
//			float dist = VSize(VSub(result.Dim[i].HitPosition, pos1));
//
//			if (dist < minDist)
//			{
//				minDist = dist;
//				bestNormal = result.Dim[i].Normal;
//				bestHitPos = result.Dim[i].HitPosition;
//			}
//
//			//DrawTriangle3D(
//			//	result.Dim[i].Position[0],
//			//	result.Dim[i].Position[1],
//			//	result.Dim[i].Position[2],
//			//	GetColor(255, 0, 0),
//			//	0
//			//);
//
//		}
//
//		m_normal = bestNormal;
//		m_hitpos = bestHitPos;
//
//
//	}
//
//	// 当たり判定情報の後片付け
//	MV1CollResultPolyDimTerminate(result);
//
//	return (result.HitNum >= 1);
//}
//
//
//VECTOR Stage::CheckHit_Line(VECTOR pos1, VECTOR pos2)
//{
//	VECTOR ret = VGet(0.0f, 0.0f, 0.0f);
//
//	// 当たり判定情報と線分との当たり判定を行う
//	//MV1_COLL_RESULT_POLY_DIM result = MV1CollCheck_LineDim(m_collisionHandle, -1, pos1, pos2);
//	auto result = MV1CollCheck_Line(m_collisionHandle, -1, pos1, pos2);
//
//
//	// 当たっていた場合
//	if (result.HitFlag)
//	{		
//
//		// 当たった個所のポジションを return するように取得する
//		ret = result.HitPosition;
//
//	}
//
//	// 当たり判定情報の後片付け
//	//MV1CollResultPolyDimTerminate(result);
//	
//	return ret;
//}
//
