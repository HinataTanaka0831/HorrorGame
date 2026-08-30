#include "Collision.h"

// 2つの3D球体同士の包含・交差判定（三平方の定理による距離比較）
// 入力: centerPosA(中心A), radiusA(半径A), centerPosB(中心B), radiusB(半径B) / 出力: 接触していればtrue / 副作用: なし
bool Collision::CheckCircleToCircle(
	const VECTOR& centerPosA,
	const float& radiusA,
	const VECTOR& centerPosB,
	const float& radiusB)
{
	float distanceX = centerPosA.x - centerPosB.x;
	float distanceY = centerPosA.y - centerPosB.y;
	float distanceZ = centerPosA.z - centerPosB.z;

	// 平方根計算（sqrtf）の負荷を回避するため2乗距離で比較
	float distSq = distanceX * distanceX + distanceY * distanceY + distanceZ * distanceZ;
	float radiusSum = radiusA + radiusB;
	float radiusSq = radiusSum * radiusSum;

	return distSq <= radiusSq;
}

// 3D座標点と球体の包含判定
// 入力: pointPos(点の座標), centerPos(球体中心), radius(球体半径) / 出力: 点が球体内ならtrue / 副作用: なし
bool Collision::CheckPointToCircle(
	const VECTOR& pointPos,
	const VECTOR& centerPos,
	const float& radius
)
{
	float distanceX = centerPos.x - pointPos.x;
	float distanceY = centerPos.y - pointPos.y;
	float distanceZ = centerPos.z - pointPos.z;

	// 平方根計算（sqrtf）の負荷を回避するため2乗距離で比較
	float distSq = distanceX * distanceX + distanceY * distanceY + distanceZ * distanceZ;
	float radiusSq = radius * radius;

	return distSq <= radiusSq;
}