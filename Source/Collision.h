#pragma once
#include "DxLib.h"

// 球体・点同士の幾何学的当たり判定を計算する静的ユーティリティクラス
class Collision
{
public:
	Collision() {}
	~Collision() {}

	// 2つの3D球体同士の包含・交差判定（三平方の定理による距離比較）
	// 入力: centerPosA(中心A), radiusA(半径A), centerPosB(中心B), radiusB(半径B) / 出力: 接触していればtrue / 副作用: なし
	static bool CheckCircleToCircle(
		const VECTOR& centerPosA,
		const float& radiusA,
		const VECTOR& centerPosB,
		const float& radiusB
	);

	// 3D座標点と球体の包含判定
	// 入力: pointPos(点の座標), centerPos(球体中心), radius(球体半径) / 出力: 点が球体内ならtrue / 副作用: なし
	static bool CheckPointToCircle(
		const VECTOR& pointPos,
		const VECTOR& centerPos,
		const float& radius
	);
};