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
	for (int i = 0; i < 10; i++)
	{
		// カプセルの始点と終点の間を10分割して、各点がAABB内にあるかをチェックする
		float t = static_cast<float>(i) / 10.0f; // 0.0fから1.0fまでの範囲で10分割

		VECTOR pointInCapsule = VGet((capsule.start.x + (capsule.end.x - capsule.start.x) * t),
									 (capsule.start.y + (capsule.end.y - capsule.start.y) * t),
									 (capsule.start.z + (capsule.end.z - capsule.start.z) * t));

		VECTOR closest;
		closest.x = (std::max)(aabb.min.x, (std::min)(pointInCapsule.x, aabb.max.x));
		closest.y = (std::max)(aabb.min.y, (std::min)(pointInCapsule.y, aabb.max.y));
		closest.z = (std::max)(aabb.min.z, (std::min)(pointInCapsule.z, aabb.max.z));

		VECTOR difference = VSub(pointInCapsule, closest);
		float distanceSquared = difference.x * difference.x + difference.y * difference.y + difference.z * difference.z;

		if (distanceSquared < capsule.radius * capsule.radius)
		{
			return true; // カプセルとAABBが衝突している
		}
	}

	return false; // カプセルとAABBが衝突していない
}


void CollisionManager::LoadAABBCollisionCSV(const char* fileName)
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

		Slope slope;
		slope.minX = info.minX;
		slope.minY = info.minY;
		slope.minZ = info.minZ;
		slope.maxX = info.maxX;
		slope.minY = info.maxY;
		slope.maxZ = info.maxZ;


		if (info.type == "Wall")
		{
			m_walls.push_back(box);
		}
		else if (info.type == "ExitDoor")
		{
			m_exitDoor.push_back(box);
		}
		else if (info.type == "Stairs")
		{
			m_stairs.push_back(slope);
		}
	}
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
}