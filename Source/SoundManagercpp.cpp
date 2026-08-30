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

// ゲーム内で使用する全BGM/SEの事前ロード
// 入力: なし / 出力: なし / 副作用: 各種音声ファイルのメモリ読み込み
void SoundManager::Initialize()
{
	LoadBGM(SOUND_BGM::BGM_TITLE, "Resource/BGM/maou_14_shining_star.mp3");
	LoadSE(SOUND_SE::SE_DECIDE, "Resource/SE/maou_se_system49.mp3");
}

// 確保済み音声メモリリソースの全解放
// 入力: なし / 出力: なし / 副作用: サウンドハンドルの削除
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

// 指定BGMのループ再生開始
// 入力: bgm(再生するBGM種別), isTop(先頭から再生し直すか) / 出力: なし / 副作用: 音声再生および再生中BGM状態の更新
void SoundManager::PlayBGM(SOUND_BGM bgm, bool isTop)
{
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

// 指定SEのバックグラウンド単発再生
// 入力: se(再生するSE種別) / 出力: なし / 副作用: 効果音再生
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

// BGMファイルのメモリ読み込み（二重登録ガード付き）
// 入力: bgm(BGM種別), filename(音声ファイルパス) / 出力: なし / 副作用: ハンドルリストへの登録
void SoundManager::LoadBGM(SOUND_BGM bgm, std::string filename)
{
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

// SEファイルのメモリ読み込み（二重登録ガード付き）
// 入力: se(SE種別), filename(音声ファイルパス) / 出力: なし / 副作用: ハンドルリストへの登録
void SoundManager::LoadSE(SOUND_SE se, std::string filename)
{
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

// 現在再生中のBGMを停止
// 入力: なし / 出力: なし / 副作用: BGM再生の停止
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