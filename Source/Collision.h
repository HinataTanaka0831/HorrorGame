#pragma once
#include "DxLib.h"

// 球体・点同士の幾何学的当たり判定を計算する静的ユーティリティクラス
class Collision
{
public:

	// 円と円の当たり判定をしてくれる関数
	// const VECTOR& centerPositionA ... Aの円の中心座標
	// const float& radiusA ... Aの円の半径
	// const VECTOR& centerPositionB ... Bの円の中心座標
	// const float& radiusB ... Bの円の半径
	static bool CheckCircleToCircle(
		const VECTOR& centerPositionA,
		const float& radiusA,
		const VECTOR& centerPositionB,
		const float& radiusB
	);


	// 点と円の当たり判定
	// const VECTOR& pointPosition ... 点の中心座標
	// const VECTOR& centerPosition ... 円の中心座標
	// const float& radius ... 円の半径
	static bool CheckPointToCircle(
		const VECTOR& pointPosition,
		const VECTOR& centerPosition,
		const float& radius
	);
};
