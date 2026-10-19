#pragma once

#include <string>
#include <unordered_map>

// 同一パスの MV1LoadModel 重複を避け、描画用は必ず複製ハンドルを返すキャッシュ
class ModelCache
{
public:
	struct CacheEntry
	{
		int masterHandle = -1;
		int refCount = 0;
	};

	// 描画・姿勢・アニメアタッチ用の独立ハンドルを返す
	// 入力: path(モデルパス) / 出力: 複製ハンドル(-1は失敗) / 副作用: 未ロードなら原本を読み、参照カウント増加
	static int AcquireInstance(const std::string& path);

	// 複製ハンドルの破棄。参照が尽きたら原本も破棄する
	// 入力: path(モデルパス), instanceHandle(AcquireInstanceの戻り値) / 出力: なし / 副作用: MV1DeleteModel、参照カウント減少
	static void ReleaseInstance(const std::string& path, int instanceHandle);

	// モーションソース用。複製せず同じハンドルを共有する
	// 入力: path(モーションモデルパス) / 出力: 共有ハンドル(-1は失敗) / 副作用: 未ロードなら読み込み、参照カウント増加
	static int AcquireShared(const std::string& path);

	// 共有モーションハンドルの参照を1つ減らす
	// 入力: path(モーションモデルパス) / 出力: なし / 副作用: 参照が0なら MV1DeleteModel
	static void ReleaseShared(const std::string& path);

	static int LoadMasterIfNeeded(CacheEntry& entry, const std::string& path);

private:
	ModelCache() = delete;

	static std::unordered_map<std::string, CacheEntry> m_instanceMasters;
	static std::unordered_map<std::string, CacheEntry> m_sharedMasters;

};
