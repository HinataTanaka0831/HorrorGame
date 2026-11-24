#pragma once

#include "Object3D.h"
#include "Collision.h"
#include <string>
#include <vector>

// 校舎ステージの3Dグラフィックモデル描画およびDXライブラリ3Dコリジョン判定を管理するクラス
class Stage : public Object3D
{
public:
	// ステージモデルおよび当たり判定用コリジョンモデルのロードと情報構築
	// 入力: filename(描画用モデルパス), collisionFilename(コリジョン用モデルパス) / 出力: なし / 副作用: MV1LoadModelおよびMV1SetupCollInfo実行
	Stage(const std::string& stageModelName);
	~Stage() override;

	void Update() override;

	// ステージ3Dモデルの描画
	// 入力: なし / 出力: なし / 副作用: バックバッファへの描画
	void Draw() override;

	// CSVファイル情報の追加を行う
    // 入力: なし / 出力: なし / 副作用: CsvLoaderで読み込んだCSVファイルのデータを構造体(ColliderInfo)に格納
	void AABBCollision();


	void Draw3DLine();


public:
	// 壁（当たり判定）の情報を取得
	const std::vector<AABB>& GetWalls() const { return m_walls; }
	// 床（当たり判定）の情報を取得 
	const std::vector<AABB>& GetFloors() const { return m_floors; }
	// 脱出口（当たり判定）の情報を取得
	const std::vector<AABB>& GetExitDoor() const { return m_exitDoor; }
	// 階段（当たり判定）の情報を取得
	const std::vector<AABB>& GetStairs() const { return m_stairs; }


private:
	// ハンドル関係
	int m_modelHandle;      // ステージモデルのハンドル

	// CSVファイルから読み込んだ当たり判定情報を格納する構造体
	struct ColliderInfo
	{
		std::string type = "";       // 当たり判定の種類（wall, floor, exitDoor, stairsなど）

		float minX = 0.0f;           // AABBの最小X座標
		float minY = 0.0f;           // AABBの最小Y座標
		float minZ = 0.0f;           // AABBの最小Z座標

		float maxX = 0.0f;           // AABBの最大X座標
		float maxY = 0.0f;           // AABBの最大Y座標
		float maxZ = 0.0f;           // AABBの最大Z座標

		std::string direction = "";  // 階段の上る向き
	};

	// コンテナ関係
	std::vector<std::vector<std::string>> m_aabbCsv;  // ステージ当たり判定のcsv情報
	std::vector<AABB> m_walls;                  // ステージの壁のAABB情報
	std::vector<AABB> m_floors;                 // ステージの床のAABB情報
	std::vector<AABB> m_exitDoor;               // 脱出口のAABB情報
	std::vector<AABB> m_stairs;                 // 階段のAABB情報

};