#pragma once

#include <string>
#include <vector>

// BGM/SEリソースのロード、重複管理、ループ再生および停止を統括する音声管理クラス
class SoundManager
{
public:          // enum, struct などの定義
	// BGM の種類
	enum SOUND_BGM
	{
		BGM_TITLE = 0,  // タイトル画面BGM
		BGM_GAME,       // 探索・ホラー演出用BGM（ジャンプスケア等）
		BGM_RESULT,     // リザルト画面BGM
	};

	// SE の種類
	enum SOUND_SE
	{
		SE_DECIDE = 0,  // UI決定音
		SE_Walk = 1,    // プレイヤー歩行・足音
	};

public:
	// コンストラクタ
	SoundManager();
	// デストラクタ
	~SoundManager();

	// ゲーム内で使用する全BGM/SEの事前ロード
	// 入力: なし / 出力: なし / 副作用: 各種音声ファイルのメモリ読み込み
	void Initialize();

	// 確保済み音声メモリリソースの全解放
	// 入力: なし / 出力: なし / 副作用: サウンドハンドルの削除
	void Finalize();

public:        // サウンド再生系

	// 指定BGMのループ再生開始
	// 入力: bgm(再生するBGM種別), isTop(先頭から再生し直すか) / 出力: なし / 副作用: 音声再生および再生中BGM状態の更新
	void PlayBGM(SOUND_BGM bgm, bool isTop = true);

	// 指定SEのバックグラウンド単発再生
	// 入力: se(再生するSE種別) / 出力: なし / 副作用: 効果音再生
	void PlaySE(SOUND_SE se);

	// BGMファイルのメモリ読み込み（二重登録ガード付き）
	// 入力: bgm(BGM種別), filename(音声ファイルパス) / 出力: なし / 副作用: ハンドルリストへの登録
	void LoadBGM(SOUND_BGM bgm, std::string filename);

	// SEファイルのメモリ読み込み（二重登録ガード付き）
	// 入力: se(SE種別), filename(音声ファイルパス) / 出力: なし / 副作用: ハンドルリストへの登録
	void LoadSE(SOUND_SE se, std::string filename);

	// 現在再生中のBGMを停止
	// 入力: なし / 出力: なし / 副作用: BGM再生の停止
	void StopBGM();

private:    // メンバ変数
	SOUND_BGM mnNowPlayingBgm;       // 現在再生中のBGM種別（重複再生防止用）
	SOUND_SE mnNowPlayingSe;         // 直近に再生されたSE種別
	std::vector<std::pair<SOUND_BGM, int>> mnBgmHandleList;  // BGM種別とDXライブラリハンドルの対応表
	std::vector<std::pair<SOUND_SE, int>> mnSeHandleList;    // SE種別とDXライブラリハンドルの対応表
};