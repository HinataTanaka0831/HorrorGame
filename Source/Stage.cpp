#include "Stage.h"
#include "Player3D.h"
#include "Master.h"
#include "ObjectManager.h"
#include "Scene.h"
#include "CsvLoader.h"


Stage::Stage(const std::string& stageModelName)
	: Object3D(VGet(0.0f, 0.0f, 0.0f))  // 座標は原点としておく
	, m_modelHandle(-1)

{
	SetTag(Object3D::TagStage); 	// タグを設定
	m_modelHandle = MV1LoadModel(stageModelName.c_str()); 	// ステージモデルの読み込み
	m_aabbCsv = CsvLoader::Load("Resource/Data/data_stagecollision.csv");  	// ステージの当たり判定情報を取得
	AABBCollision();
}

Stage::~Stage()
{
	// 読み込んだモデルデータの破棄
	MV1DeleteModel(m_modelHandle);
}

void Stage::Update()
{

}

void Stage::Draw()
{
	// ステージの描画
	MV1DrawModel(m_modelHandle);

	Draw3DLine();
}

void Stage::AABBCollision()
{
	for (auto& aabb : m_aabbCsv) {
		ColliderInfo info;              // CSVから読み込んだ当たり判定情報を一時的に保持するための構造体
		info.type = aabb[0];             // 当たり判定の種類を取得
		// AABBの最小座標を取得
		info.minX = std::stof(aabb[1]);
		info.minY = std::stof(aabb[2]);
		info.minZ = std::stof(aabb[3]);
		// AABBの最大座標を取得
		info.maxX = std::stof(aabb[4]);
		info.maxY = std::stof(aabb[5]);
		info.maxZ = std::stof(aabb[6]);
		// CSVに記入されている「＋」か「－」のどちらかを取得
		info.direction = "";
		if (aabb.size() >= 8) info.direction = std::stof(aabb[7]);
		// 当たり判定用のAABBを作成
		AABB box;  
		box.min = { info.minX, info.minY, info.minZ };
		box.max = { info.maxX, info.maxY, info.maxZ };
		box.direction = info.direction;
		if (info.type == "Wall") {
			m_walls.push_back(box);      	// 壁の当たり判定情報をコンテナに追加
		}
		else if (info.type == "Floor") {
			m_floors.push_back(box); 		// 床の当たり判定情報をコンテナに追加
		}
		else if (info.type == "ExitDoor") {
			m_exitDoor.push_back(box); 		// 脱出口の当たり判定情報をコンテナに追加
		}
		else if (info.type == "Stairs") {
			m_stairs.push_back(box); 		// 階段（坂）の当たり判定情報をコンテナに追加
		}
	}
}

void Stage::Draw3DLine()
{
	
	for (const auto& walls : m_walls)
	{
		DrawCube3D(walls.min, walls.max, GetColor(255, 0, 0), GetColor(255, 0, 0), false);
	}

	for (const auto& floors : m_floors)
	{
		DrawCube3D(floors.min, floors.max, GetColor(255, 230, 200), GetColor(255, 230, 200), false);
	}

	for (const auto& exitDoor : m_exitDoor)
	{
		DrawCube3D(exitDoor.min, exitDoor.max, GetColor(0, 0, 255), GetColor(0, 0, 255), false);
	}

	for (const auto& stair : m_stairs)
	{
		DrawCube3D(stair.min, stair.max, GetColor(0, 255, 0), GetColor(0, 255, 0), false);
	}
}