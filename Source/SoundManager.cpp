#include "SoundManagerh.h"

SoundManager::SoundManager()
	: m_nowPlayingBgm((SoundBGM)-1)           // 初期状態は何も再生されていない状態
	, m_nowPlayingSe((SoundSE)-1)             // 初期状態は何も再生されていない状態
{

}

SoundManager::~SoundManager()
{

}

void SoundManager::Initialize()
{
	// BGM の読み込み
	LoadBGM(SoundBGM::BGMTitle, "Resource/BGM/electrical_noise1.mp3");
	LoadBGM(SoundBGM::BGMResult, "Resource/BGM/electrical_noise1.mp3");
	LoadBGM(SoundBGM::BGMGame, "Resource/BGM/Jumpscare.mp3");

	// SE の読み込み
	LoadSE(SoundSE::SEDecide, "Resource/SE/maou_se_system49.mp3");
	LoadSE(SoundSE::SEWalk, "Resource/SE/clear-bright-sound-of-step-in-shoes-on-the-floor.mp3");
}

void SoundManager::Finalize()
{
	 // BGMの破棄
	for (auto it = m_bgmHandleList.begin(); it != m_bgmHandleList.end(); it++)
	{
		DeleteSoundMem(it->second);
	}

	// SEの破棄
	for (auto it = m_seHandleList.begin(); it != m_seHandleList.end(); it++)
	{
		DeleteSoundMem(it->second);
	}

}

void SoundManager::PlayBGM(SoundBGM bgm, bool isTop)
{
	if (m_nowPlayingBgm == bgm && !isTop)
	{
		return;
	}


	for (auto it = m_bgmHandleList.begin(); it != m_bgmHandleList.end(); it++)
	{
		// 一致した種類のBGMがあれば
		if (it->first == bgm)
		{
			// BGMをループ再生
			PlaySoundMem(it->second, DX_PLAYTYPE_LOOP, isTop);
			// 現在の再生種類を更新
			m_nowPlayingBgm = bgm;

			break;
		}
	}
}

void  SoundManager::PlaySE(SoundSE se)
{
	for (auto it = m_seHandleList.begin(); it != m_seHandleList.end(); it++)
	{
		// 一致した種類のSEがあれば
		if (it->first == se)
		{
			// SEをループ再生
			PlaySoundMem(it->second, DX_PLAYTYPE_BACK);
			// 現在の再生種類を更新
			m_nowPlayingSe = se;

			break;
		}
	}

}

void SoundManager::LoadBGM(SoundBGM bgm, std::string fileName)
{
	bool check = false;      // 重複して読み込んでいるかどうか
	for (auto it = m_bgmHandleList.begin(); it != m_bgmHandleList.end(); it++)
	{
		// 一致した種類のBGMがあれば
		if (it->first == bgm)
		{
			check = true;
			break;
		}

	}

	// 重複して読み込まれていたら何もしない
	if (check)
	{
		return;
	}

	// ファイル読み込み
	int handle = LoadSoundMem(fileName.c_str());
	if (handle == -1)
	{
		return; // 読み込み失敗していたら何もしない
	}

	// 読み込んだハンドルをリストに追加
	m_bgmHandleList.push_back(std::pair <SoundBGM, int> (bgm, handle) );
}


void SoundManager::LoadSE(SoundSE se, std::string fileName)
{
	bool check = false;      // 重複して読み込んでいるかどうか
	for (auto it = m_seHandleList.begin(); it != m_seHandleList.end(); it++)
	{
		// 一致した種類のBGMがあれば
		if (it->first == se)
		{
			check = true;
			break;
		}

	}

	// 重複して読み込まれていたら何もしない
	if (check)
	{
		return;
	}

	// ファイル読み込み
	int handle = LoadSoundMem(fileName.c_str());
	if (handle == -1)
	{
		return; // 読み込み失敗していたら何もしない
	}

	// 読み込んだハンドルをリストに追加
	m_seHandleList.push_back(std::pair <SoundSE, int>(se, handle));

}

void SoundManager::StopBGM()
{
	for (auto it = m_bgmHandleList.begin(); it != m_bgmHandleList.end(); it++)
	{
		
		if (it->first == m_nowPlayingBgm)
		{
			// BGMが再生されているか
			if (CheckSoundMem(it->second))
			{
				StopSoundMem(it->second);  // BGM停止
				break;
			}
		}

	}

}