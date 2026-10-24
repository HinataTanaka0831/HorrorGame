#include "ModelCache.h"
#include "DxLib.h"

// ここで静的メンバを定義
std::unordered_map<std::string, ModelCache::CacheEntry> ModelCache::m_instanceMasters;
std::unordered_map<std::string, ModelCache::CacheEntry> ModelCache::m_sharedMasters;

int ModelCache::LoadMasterIfNeeded(CacheEntry& entry, const std::string& path)
{
	if (entry.masterHandle == -1)
	{
		entry.masterHandle = MV1LoadModel(path.c_str());
	}
	return entry.masterHandle;
}


int ModelCache::AcquireInstance(const std::string& path)
{
	CacheEntry& entry = m_instanceMasters[path];
	if (LoadMasterIfNeeded(entry, path) == -1)
	{
		m_instanceMasters.erase(path);
		return -1;
	}

	// 原本に座標やアタッチアニメを書くと全個体へ漏れるため、呼び出し側には複製だけ渡す
	const int instanceHandle = MV1DuplicateModel(entry.masterHandle);
	if (instanceHandle == -1)
	{
		return -1;
	}

	++entry.refCount;
	return instanceHandle;
}

void ModelCache::ReleaseInstance(const std::string& path, int instanceHandle)
{
	if (instanceHandle != -1)
	{
		MV1DeleteModel(instanceHandle);
	}

	auto it = m_instanceMasters.find(path);
	if (it == m_instanceMasters.end())
	{
		return;
	}

	--it->second.refCount;
	// 頂点・テクスチャは原本と共有されるため、複製が残る間は原本を残す
	if (it->second.refCount <= 0)
	{
		if (it->second.masterHandle != -1)
		{
			MV1DeleteModel(it->second.masterHandle);
		}
		m_instanceMasters.erase(it);
	}
}

int ModelCache::AcquireShared(const std::string& path)
{
	CacheEntry& entry = m_sharedMasters[path];
	if (LoadMasterIfNeeded(entry, path) == -1)
	{
		m_sharedMasters.erase(path);
		return -1;
	}

	++entry.refCount;
	return entry.masterHandle;
}

void ModelCache::ReleaseShared(const std::string& path)
{
	auto it = m_sharedMasters.find(path);
	if (it == m_sharedMasters.end())
	{
		return;
	}

	--it->second.refCount;
	if (it->second.refCount <= 0)
	{
		if (it->second.masterHandle != -1)
		{
			MV1DeleteModel(it->second.masterHandle);
		}
		m_sharedMasters.erase(it);
	}
}
