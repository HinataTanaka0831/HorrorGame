#include "CollisionManager.h"
#include <fstream>
#include <sstream>


CollisionManager& CollisionManager::GetInstance()
{
	static CollisionManager instance;
	return instance;
}


CollisionManager::CollisionManager()
{
	LoadCollisionCSV("Resource/Data/collisionData_stage.csv");
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
	// “–‚½‚è”»’è‚ğ‘‚­@O•½•û‚Ì’è—(a^2 + b^2 = c^2)

	// X²‚Ì‹——£i—Î‚Ì‰¡üj
	float distanceX = centerPositionA.x - centerPositionB.x;
	// Y²‚Ì‹——£i—Î‚Ìcüj
	float distanceY = centerPositionA.y - centerPositionB.y;
	// Z²‚Ì‹——£i—Î‚Ì‰œ‚Ìüj
	float distanceZ = centerPositionA.z - centerPositionB.z;

	// ğŒ
	float powX = distanceX * distanceX; // ‰¡ü‚Ì2æ
	float powY = distanceY * distanceY; // cü‚Ì‚Qæ
	float powZ = distanceZ * distanceZ; // ‰œ‚Ìü‚Ì2æ
	float powR = (radiusA + radiusB) * (radiusA + radiusB);  // ‚»‚ê‚¼‚ê‚Ì”¼Œa‚ğ‘«‚µ‚½‚à‚Ì‚Ì‚Qæ
	if ((powX + powY + powZ) <= powR)
	{
		return true;       // ¬Œ÷
	}

	// ğŒ‚ğ–‚½‚³‚È‚¢‚Ì‚Å‚ ‚ê‚Î¸”s
	return false;
}


bool CollisionManager::CheckPointToCircle(
	const VECTOR& pointPosition,
	const VECTOR& centerPosition,
	const float& radius
)
{
	// X²‚Ì‹——£
	float distanceX = centerPosition.x - pointPosition.x;
	// Y²‚Ì‹——£
	float distanceY = centerPosition.y - pointPosition.y;
	// Z²‚Ì‹——£
	float distanceZ = centerPosition.z - pointPosition.z;

	// ”»’è
	float powX = distanceX * distanceX;   // ‰¡ü‚Ì‚Qæ
	float powY = distanceY * distanceY;   // cü‚Ì‚Qæ
	float powZ = distanceZ * distanceZ;   // ‰œ‚Ìü‚Ì2æ
	float powR = radius * radius;         // ”¼Œa‚Ì‚Qæ

	if ((powX + powY + powZ) <= powR)
	{
		return true;    // “–‚½‚Á‚Ä‚¢‚é
	}

	// ğŒ‚ğ–‚½‚³‚È‚¢‚Ì‚Å‚ ‚ê‚Î¸”s
	return false;
}


bool CollisionManager::CheckAABB(
	const AABB& a,
	const AABB& b
)
{

	return (a.min.x <= b.max.x && a.max.x >= b.min.x) &&
		(a.min.y <= b.max.y && a.max.y >= b.min.y) &&
		(a.min.z <= b.max.z && a.max.z >= b.min.z);

}


void CollisionManager::LoadCollisionCSV(const char* fileName)
{
	std::ifstream file(fileName);

	if (!file.is_open())
	{
		return;
	}

	std::string line;

	// 1s–Ú‚ğÌ‚Ä‚é
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
