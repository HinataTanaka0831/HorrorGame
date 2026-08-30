#include "SoundManagerh.h"
#include "DxLib.h"

SoundManager::SoundManager()
	: mnNowPlayingBgm((SOUND_BGM)-1)
	, mnNowPlayingSe((SOUND_SE)-1)
{
}

SoundManager::~SoundManager()
{
}

void SoundManager::Initialize()
{
	LoadBGM(SOUND_BGM::BGM_TITLE, "Resource/BGM/electrical_noise1.mp3");
	LoadBGM(SOUND_BGM::BGM_RESULT, "Resource/BGM/electrical_noise1.mp3");
	LoadBGM(SOUND_BGM::BGM_GAME, "Resource/BGM/Jumpscare.mp3");

	LoadSE(SOUND_SE::SE_DECIDE, "Resource/SE/maou_se_system49.mp3");
	LoadSE(SOUND_SE::SE_Walk, "Resource/SE/clear-bright-sound-of-step-in-shoes-on-the-floor.mp3");
}

void SoundManager::Finalize()
{
	for (auto it = mnBgmHandleList.begin(); it != mnBgmHandleList.end(); ++it)
	{
		DeleteSoundMem(it->second);
	}
	mnBgmHandleList.clear();

	for (auto it = mnSeHandleList.begin(); it != mnSeHandleList.end(); ++it)
	{
		DeleteSoundMem(it->second);
	}
	mnSeHandleList.clear();
}

void SoundManager::PlayBGM(SOUND_BGM bgm, bool isTop)
{
	// 同一BGMの継続再生要求時は二重再生を防ぐため処理をスキップ
	if (mnNowPlayingBgm == bgm && !isTop)
	{
		return;
	}

	for (auto it = mnBgmHandleList.begin(); it != mnBgmHandleList.end(); ++it)
	{
		if (it->first == bgm)
		{
			PlaySoundMem(it->second, DX_PLAYTYPE_LOOP, isTop);
			mnNowPlayingBgm = bgm;
			break;
		}
	}
}

void SoundManager::PlaySE(SOUND_SE se)
{
	for (auto it = mnSeHandleList.begin(); it != mnSeHandleList.end(); ++it)
	{
		if (it->first == se)
		{
			PlaySoundMem(it->second, DX_PLAYTYPE_BACK);
			mnNowPlayingSe = se;
			break;
		}
	}
}

void SoundManager::LoadBGM(SOUND_BGM bgm, std::string filename)
{
	// メモリリークおよび重複登録防止の存在チェック
	for (auto it = mnBgmHandleList.begin(); it != mnBgmHandleList.end(); ++it)
	{
		if (it->first == bgm)
		{
			return;
		}
	}

	int handle = LoadSoundMem(filename.c_str());
	if (handle == -1)
	{
		return;
	}

	mnBgmHandleList.push_back(std::pair<SOUND_BGM, int>(bgm, handle));
}

void SoundManager::LoadSE(SOUND_SE se, std::string filename)
{
	// メモリリークおよび重複登録防止の存在チェック
	for (auto it = mnSeHandleList.begin(); it != mnSeHandleList.end(); ++it)
	{
		if (it->first == se)
		{
			return;
		}
	}

	int handle = LoadSoundMem(filename.c_str());
	if (handle == -1)
	{
		return;
	}

	mnSeHandleList.push_back(std::pair<SOUND_SE, int>(se, handle));
}

void SoundManager::StopBGM()
{
	for (auto it = mnBgmHandleList.begin(); it != mnBgmHandleList.end(); ++it)
	{
		if (it->first == mnNowPlayingBgm)
		{
			if (CheckSoundMem(it->second))
			{
				StopSoundMem(it->second);
				break;
			}
		}
	}
}