#include "Stage.h"
#include "Player3D.h"
#include "Master.h"
#include "ObjectManager.h"
#include "Scene.h"
#include <fstream>
#include <sstream>
#include <algorithm>


Stage::Stage(std::string stageModelName)
	: Object3D(VGet(0.0f, 0.0f, 0.0f))  // 座標は原点としておく
	, m_modelHandle(-1)

{
	// タグを設定
	SetTag(Object3D::TagStage);

	// ステージモデルの読み込み
	m_modelHandle = MV1LoadModel(stageModelName.c_str());
	
	// ステージの当たり判定情報を取得
	LoadAABBCollisionCSV("Resource/Data/collisionData_stage.csv");
}

Stage::~Stage()
{
	// 読み込んだモデルデータの破棄
	MV1DeleteModel(m_modelHandle);
}


void Stage::Draw()
{
	// ステージの描画
	MV1DrawModel(m_modelHandle);

	Draw3DLine();
}

void Stage::LoadAABBCollisionCSV(const char* fileName)
{
	// CSVファイルを読み込む
	std::ifstream file(fileName);

	// CSVファイルが読み込めないなら終了
	if (!file.is_open())
	{
		return;
	}

	std::string line;

	// 1行目を捨てる
	std::getline(file, line);

	// CSVファイルを1行ずつ読み込む
	while (std::getline(file, line))
	{
		// 1行のCSVデータを解析するためのストリームを生成
		std::stringstream ss(line);

		std::string token;

		// CSVから読み込んだ当たり判定情報を一時的に保持するための構造体
		ColliderInfo info;

		// 当たり判定の種類を取得
		std::getline(ss, info.type, ',');

		// AABBの最小座標を取得
		std::getline(ss, token, ',');
		info.minX = std::stof(token);
		std::getline(ss, token, ',');
		info.minY = std::stof(token);
		std::getline(ss, token, ',');
		info.minZ = std::stof(token);

		// AABBの最大座標を取得
		std::getline(ss, token, ',');
		info.maxX = std::stof(token);
		std::getline(ss, token, ',');
		info.maxY = std::stof(token);
		std::getline(ss, token, ',');
		info.maxZ = std::stof(token);

		// CSVに記入されている「＋」か「－」のどちらかを取得
		std::getline(ss, info.direction, ',');

		// 当たり判定用のAABBを作成
		AABB box;
		box.min = { info.minX, info.minY, info.minZ };
		box.max = { info.maxX, info.maxY, info.maxZ };
		box.direction = info.direction;

		// 当たり判定の種類ごとにコンテナに情報を格納
		if (info.type == "Wall")
		{
			// 壁の当たり判定情報を追加
			m_walls.push_back(box);
		}
		else if (info.type == "Floor")
		{
			// 床の当たり判定情報を追加
			m_floors.push_back(box);
		}
		else if (info.type == "ExitDoor")
		{
			// 脱出口の当たり判定情報を追加
			m_exitDoor.push_back(box);
		}
		else if (info.type == "Stairs")
		{
			// 階段（坂）の当たり判定情報を追加
			m_stairs.push_back(box);
		}
	}
	// ファイルを閉じる
	file.close();

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