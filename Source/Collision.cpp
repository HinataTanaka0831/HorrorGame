#include "Collision.h"


bool Collision::CheckCircleToCircle(
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


bool Collision::CheckPointToCircle(
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

bool Collision::CheckAABB(
	const AABB& a,
	const AABB& b
)
{

	return (a.min.x <= b.max.x && a.max.x >= b.min.x) &&
		(a.min.y <= b.max.y && a.max.y >= b.min.y) &&
		(a.min.z <= b.max.z && a.max.z >= b.min.z);

}


bool Collision::CheckCapsuleAABB(
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
