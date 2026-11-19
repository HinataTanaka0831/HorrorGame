#pragma once
#include "DxLib.h"
#include <string>

// AABB構造体
struct AABB
{
	VECTOR min = VGet(0.0f, 0.0f, 0.0f); // AABBの最小座標
	VECTOR max = VGet(0.0f, 0.0f, 0.0f); // AABBの最大座標
	std::string direction = "";          // 階段用の上る方向
};

// カプセル判定用の構造体
struct Capsule
{
	VECTOR start = VGet(0.0f, 0.0f, 0.0f); // カプセルの始点座標
	VECTOR end = VGet(0.0f, 0.0f, 0.0f);   // カプセルの終点座標
	float radius = 0.0f; // カプセルの半径
};


// 球体・点同士の幾何学的当たり判定を計算する静的クラス
class Collision
{
public:
	// コンストラクタ
	Collision()
	{
	}

	// デストラクタ
	~Collision()
	{
	}

	// 円と円の当たり判定をしてくれる関数
	// 入力: centerPositionA（Aの円の中心座標）、radiusA（Aの円の半径）、centerPositionB（Bの円の中心座標）。radiusB（Bの円の半径）/ 出力: 円と円が当たっていたらtrue、それ以外ならfalse / 副作用: なし
	static bool CheckCircleToCircle(
		const VECTOR& centerPositionA,
		const float& radiusA,
		const VECTOR& centerPositionB,
		const float& radiusB
	);


	// 点と円の当たり判定
	// 入力: pointPosition（点の中心座標）、centerPosition（円の中心座標）、radius（円の半径） / 出力: 点と円が当たったらtrue、それ以外ならfalse / 副作用: なし
	static bool CheckPointToCircle(
		const VECTOR& pointPosition,
		const VECTOR& centerPosition,
		const float& radius
	);


	// AABBによる当たり判定
	static bool CheckAABB(
		const AABB& a,
		const AABB& b
	);


	// AABBによる当たり判定
	// 入力: capsule（カプセル構造体の各情報）、aabb（AABB構造体の各情報） / 出力: カプセルとAABBが衝突していたらtrue、それ以外ならfalse / 副作用: なし
	static bool CheckCapsuleAABB(
		const Capsule& capsule,
		const AABB& aabb
	);

};
