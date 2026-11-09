#include "CollisionManager.h"
#include <fstream>
#include <sstream>
#include <algorithm>


CollisionManager& CollisionManager::GetInstance()
{
	static CollisionManager instance;
	return instance;
}


CollisionManager::CollisionManager()
{
	LoadAABBCollisionCSV("Resource/Data/collisionData_stage.csv");
}


CollisionManager::~CollisionManager()
{

}


bool CollisionManager::CheckCircleToCircle(
	const VECTOR& centerPositionA,
	const float& radiusA,
	const VECTOR& centerPositionB,
	const float& radiusB)
{
	// 当たり判定を書く　三平方の定理(a^2 + b^2 = c^2)

	// X軸の距離（緑の横線）
	float distanceX = centerPositionA.x - centerPositionB.x;
	// Y軸の距離（緑の縦線）
	float distanceY = centerPositionA.y - centerPositionB.y;
	// Z軸の距離（緑の奥の線）
	float distanceZ = centerPositionA.z - centerPositionB.z;

	// 条件
	float powX = distanceX * distanceX; // 横線の2乗
	float powY = distanceY * distanceY; // 縦線の２乗
	float powZ = distanceZ * distanceZ; // 奥の線の2乗
	float powR = (radiusA + radiusB) * (radiusA + radiusB);  // それぞれの半径を足したものの２乗
	if ((powX + powY + powZ) <= powR)
	{
		return true;       // 成功
	}

	// 条件を満たさないのであれば失敗
	return false;
}


bool CollisionManager::CheckPointToCircle(
	const VECTOR& pointPosition,
	const VECTOR& centerPosition,
	const float& radius
)
{
	// X軸の距離
	float distanceX = centerPosition.x - pointPosition.x;
	// Y軸の距離
	float distanceY = centerPosition.y - pointPosition.y;
	// Z軸の距離
	float distanceZ = centerPosition.z - pointPosition.z;

	// 判定
	float powX = distanceX * distanceX;   // 横線の２乗
	float powY = distanceY * distanceY;   // 縦線の２乗
	float powZ = distanceZ * distanceZ;   // 奥の線の2乗
	float powR = radius * radius;         // 半径の２乗

	if ((powX + powY + powZ) <= powR)
	{
		return true;    // 当たっている
	}

	// 条件を満たさないのであれば失敗
	return false;
}


bool CollisionManager::CheckCapsuleAABB(
	const Capsule& capsule,
	const AABB& aabb
)
{
	// カプセルの始点と終点の間を10分割して、各点がAABB内にあるかをチェックする
	for (int i = 0; i < 10; i++)
	{
		float t = static_cast<float>(i) / 10.0f; // 0.0fから1.0fまでの範囲で10分割

		// カプセルの始点と終点の間の点（中心線上の点）を計算
		VECTOR pointInCapsule = VGet((capsule.start.x + (capsule.end.x - capsule.start.x) * t),
									 (capsule.start.y + (capsule.end.y - capsule.start.y) * t),
									 (capsule.start.z + (capsule.end.z - capsule.start.z) * t));

		// AABBの最も近い点(最近接点)を求める
		VECTOR closest;
		closest.x = (std::max)(aabb.min.x, (std::min)(pointInCapsule.x, aabb.max.x));
		closest.y = (std::max)(aabb.min.y, (std::min)(pointInCapsule.y, aabb.max.y));
		closest.z = (std::max)(aabb.min.z, (std::min)(pointInCapsule.z, aabb.max.z));

		// 最近接点とカプセルの中心線上の点との距離を計算
		VECTOR difference = VSub(pointInCapsule, closest);
		// 距離の長さの2乗を求める
		float distanceSquared = difference.x * difference.x + difference.y * difference.y + difference.z * difference.z;

		// 距離の長さがカプセルの半径より小さい場合、カプセルとAABBが衝突していると判断する
		if (distanceSquared < capsule.radius * capsule.radius)
		{
			return true;
		}
	}

	return false; // カプセルとAABBが衝突していない
}


void CollisionManager::LoadAABBCollisionCSV(const char* fileName)
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

		// 当たり判定用のAABBを作成
		AABB box;
		box.min = { info.minX, info.minY, info.minZ };
		box.max = { info.maxX, info.maxY, info.maxZ };

		// 階段（坂）用の情報を作成
		Slope slope;
		slope.minX = info.minX;
		slope.minY = info.minY;
		slope.minZ = info.minZ;
		slope.maxX = info.maxX;
		slope.minY = info.maxY;
		slope.maxZ = info.maxZ;

		// 当たり判定の種類ごとにコンテナに情報を格納
		if (info.type == "Wall")
		{
			// 壁の当たり判定情報を追加
			m_walls.push_back(box);
		}
		else if (info.type == "ExitDoor")
		{
			// 脱出口の当たり判定情報を追加
			m_exitDoor.push_back(box);
		}
		else if (info.type == "Stairs")
		{
			// 階段（坂）の当たり判定情報を追加
			m_stairs.push_back(slope);
		}
	}
	// ファイルを閉じる
	file.close();

}


void CollisionManager::Draw3DLine()
{
	for(const auto& walls : m_walls)
	{
		DrawCube3D(walls.min, walls.max, GetColor(255, 0, 0), GetColor(255, 0, 0), false);

	}

	for (const auto& exitDoor : m_exitDoor)
	{
		DrawCube3D(exitDoor.min, exitDoor.max, GetColor(0, 0, 255), GetColor(0, 0, 255), false);
	}

	for (const auto& stair : m_stairs)
	{
		VECTOR min = VGet(stair.minX, stair.minY, stair.minZ);
		VECTOR max = VGet(stair.maxX, stair.maxY, stair.maxZ);
		DrawCube3D(min, max, GetColor(0, 255, 0), GetColor(0, 255, 0), false);
	}
}