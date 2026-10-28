#pragma once
#include "DxLib.h"
#include <string>
#include <vector>

// AABB構造体
struct AABB
{
	VECTOR min; // AABBの最小座標
	VECTOR max; // AABBの最大座標
};

// 球体・点同士の幾何学的当たり判定を計算する静的ユーティリティクラス
class CollisionManager
{
public:
	// シングルトンインスタンスの取得
	// 入力: なし / 出力: CollisionManager参照 / 副作用: 初回呼び出し時に性的インスタンス生成
	static  CollisionManager& GetInstance();

	// 円と円の当たり判定をしてくれる関数
	// const VECTOR& centerPositionA ... Aの円の中心座標
	// const float& radiusA ... Aの円の半径
	// const VECTOR& centerPositionB ... Bの円の中心座標
	// const float& radiusB ... Bの円の半径
	bool CheckCircleToCircle(
		const VECTOR& centerPositionA,
		const float& radiusA,
		const VECTOR& centerPositionB,
		const float& radiusB
	);


	// 点と円の当たり判定
	// const VECTOR& pointPosition ... 点の中心座標
	// const VECTOR& centerPosition ... 円の中心座標
	// const float& radius ... 円の半径
	bool CheckPointToCircle(
		const VECTOR& pointPosition,
		const VECTOR& centerPosition,
		const float& radius
	);


	static bool CheckAABB(
		const AABB& a,
		const AABB& b
	);

	// CSVファイルを読み取りAABB判定を行う
	// 入力: fileName(CSVファイルの名前) / 出力: なし / 副作用: 読み込んだCSVファイルのデータを構造体(ColliderInfo)に格納
	void LoadCollisionCSV(const char* fileName);


	std::vector<AABB> GetWalls() const { return m_walls; }

	std::vector<AABB> GetFloors() const { return m_floors; }

	std::vector<AABB> GetClassrooms() const { return m_classrooms; }

	std::vector<AABB> GetStairs() const { return m_stairs; }

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

	std::vector<AABB> m_walls; // ステージの壁のAABB情報
	std::vector<AABB> m_floors; // ステージの床のAABB情報
	std::vector<AABB> m_classrooms; // 教室のAABB情報
	std::vector<AABB> m_stairs; // 階段のAABB情報

	struct ColliderInfo
	{
		std::string type = "";

		float minX = 0.0f;
		float minY = 0.0f;
		float minZ = 0.0f;

		float maxX = 0.0f;
		float maxY = 0.0f;
		float maxZ = 0.0f;
	};

};
