#pragma once
#include "DxLib.h"
#include <string>
#include <vector>


// カプセル判定用の構造体
struct Capsule
{
	VECTOR start = VGet(0.0f, 0.0f, 0.0f); // カプセルの始点座標
	VECTOR end = VGet(0.0f, 0.0f, 0.0f);   // カプセルの終点座標
	float radius = 0.0f; // カプセルの半径
};

// 階段判定用の構造体
struct Slope
{
	float minX = 0.0f;  // 階段の最小X座標
	float minY = 0.0f;  // 階段の最小Y座標
	float minZ = 0.0f;  // 階段の最小Z座標
	float maxX = 0.0f;  // 階段の最大X座標
	float maxY = 0.0f;  // 階段の最大Y座標
	float maxZ = 0.0f;  // 階段の最大Z座標
};


// 球体・点同士の幾何学的当たり判定を計算する静的ユーティリティクラス
class CollisionManager
{
public:
	// AABB構造体
	struct AABB
	{
		VECTOR min = VGet(0.0f, 0.0f, 0.0f); // AABBの最小座標
		VECTOR max = VGet(0.0f, 0.0f, 0.0f); // AABBの最大座標
	};

public:
	// シングルトンインスタンスの取得
	// 入力: なし / 出力: CollisionManager参照 / 副作用: 初回呼び出し時に静的インスタンス生成
	static  CollisionManager& GetInstance();

	// 円と円の当たり判定をしてくれる関数
	// 入力: centerPositionA（Aの円の中心座標）、radiusA（Aの円の半径）、centerPositionB（Bの円の中心座標）。radiusB（Bの円の半径）/ 出力: 円と円が当たっていたらtrue、それ以外ならfalse / 副作用: なし
	bool CheckCircleToCircle(
		const VECTOR& centerPositionA,
		const float& radiusA,
		const VECTOR& centerPositionB,
		const float& radiusB
	);


	// 点と円の当たり判定
	// 入力: pointPosition（点の中心座標）、centerPosition（円の中心座標）、radius（円の半径） / 出力: 点と円が当たったらtrue、それ以外ならfalse / 副作用: なし
	bool CheckPointToCircle(
		const VECTOR& pointPosition,
		const VECTOR& centerPosition,
		const float& radius
	);


	// AABBによる当たり判定
	// 入力: capsule（カプセル構造体の各情報）、aabb（AABB構造体の各情報） / 出力: カプセルとAABBが衝突していたらtrue、それ以外ならfalse / 副作用: なし
	bool CheckCapsuleAABB(
		const Capsule& capsule,
		const AABB& aabb
	);


	// CSVファイルを読み取り判定を行う
	// 入力: fileName(CSVファイルの名前) / 出力: なし / 副作用: 読み込んだCSVファイルのデータを構造体(ColliderInfo)に格納
	void LoadAABBCollisionCSV(const char* fileName);


	void Draw3DLine();

public:
	const std::vector<AABB>& GetWalls() const { return m_walls; }
	const std::vector<AABB>& GetFloors() const { return m_floors; }
	const std::vector<AABB>& GetExitDoor() const { return m_exitDoor; }
	const std::vector<Slope>& GetStairs() const { return m_stairs; }

private:
	// コンストラクタ
	CollisionManager();
	// デストラクタ
	~CollisionManager();

	CollisionManager(const CollisionManager&) = delete;             // コピー生成の禁止
	CollisionManager& operator=(const CollisionManager&) = delete;  // コピー代入の禁止

	CollisionManager(CollisionManager&&) = delete;                  // ムーブ生成の禁止
	CollisionManager&& operator=(CollisionManager&&) = delete;      // ムーブ代入の禁止

private:
	// CSVファイルから読み込んだ当たり判定情報を格納する構造体
	struct ColliderInfo
	{
		std::string type = "";    // 当たり判定の種類（wall, floor, exitDoor, stairsなど）

		float minX = 0.0f;        // AABBの最小X座標
		float minY = 0.0f;        // AABBの最小Y座標
		float minZ = 0.0f;        // AABBの最小Z座標

		float maxX = 0.0f;        // AABBの最大X座標
		float maxY = 0.0f;        // AABBの最大Y座標
		float maxZ = 0.0f;        // AABBの最大Z座標
	};

	// コンテナ関係
	std::vector<AABB> m_walls;    // ステージの壁のAABB情報
	std::vector<AABB> m_floors;   // ステージの床のAABB情報
	std::vector<AABB> m_exitDoor; // 脱出口のAABB情報
	std::vector<Slope> m_stairs;  // 階段のAABB情報
};
