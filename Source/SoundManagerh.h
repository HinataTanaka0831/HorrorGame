#pragma once

#include "DxLib.h"
#include <string>
#include <vector>

// BGM/SEリソースのロード、重複管理、ループ再生および停止を統括する音声管理クラス
class SoundManager
{
public:          // enum, struct などの定義
	// BGM の種類
	enum SoundBGM
	{
		BGMTitle = 0,  // タイトル画面BGM
		BGMGame,       // 探索・ホラー演出用BGM（ジャンプスケア等）
		BGMResult,     // リザルト画面BGM
	};

	// SE の種類
	enum SoundSE
	{
		SEDecide = 0,  // UI決定音
		SEWalk = 1,    // プレイヤー歩行・足音
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
	void PlayBGM(SoundBGM bgm, bool isTop = true);

	// 指定SEのバックグラウンド単発再生
	// 入力: se(再生するSE種別) / 出力: なし / 副作用: 効果音再生
	void PlaySE(SoundSE se);

	// BGMファイルのメモリ読み込み（二重登録ガード付き）
	// 入力: bgm(BGM種別), fileName(音声ファイルパス) / 出力: なし / 副作用: ハンドルリストへの登録
	void LoadBGM(SoundBGM bgm, std::string fileName);

	// SEファイルのメモリ読み込み（二重登録ガード付き）
	// 入力: se(SE種別), fileName(音声ファイルパス) / 出力: なし / 副作用: ハンドルリストへの登録
	void LoadSE(SoundSE se, std::string fileName);

	// 現在再生中のBGMを停止
	// 入力: なし / 出力: なし / 副作用: BGM再生の停止
	void StopBGM();

private:    // メンバ変数
	SoundBGM m_nowPlayingBgm;       // 現在再生中のBGM種別（重複再生防止用）
	SoundSE m_nowPlayingSe;         // 直近に再生されたSE種別
	std::vector<std::pair<SoundBGM, int>> m_bgmHandleList;  // BGM種別とDXライブラリハンドルの対応表
	std::vector<std::pair<SoundSE, int>> m_seHandleList;    // SE種別とDXライブラリハンドルの対応表
};