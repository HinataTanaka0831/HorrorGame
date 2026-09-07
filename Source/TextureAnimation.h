#pragma once

#include "DxLib.h"
#include <string>

// スプライトシートから分割読み込みした連番テクスチャのアニメーション制御クラス
class TextureAnimation
{
public:
	// 分割テクスチャの一括読み込みおよびアニメーションパラメータ初期化
	// 入力: position(表示座標), fileName(画像パス), allNum(総コマ数), xNum(横分割), yNum(縦分割), interval(コマ送り間隔フレーム) / 出力: なし / 副作用: LoadDivGraph実行
	TextureAnimation(
		VECTOR position,
		std::string fileName,
		int allNum,
		int xNum,
		int yNum,
		int interval
	);  

	~TextureAnimation(); 

	// フレームインターバル経過によるコマ送り更新
	// 入力: なし / 出力: なし / 副作用: m_currentNumおよびm_counterの更新
	void Update(); 

	// 現在のコマ画像描画
	// 入力: なし / 出力: なし / 副作用: バックバッファへの描画
	void Draw();   

private:
	VECTOR m_position;      // ポジション
	int m_counter = 0;      // アニメーションカウンタ
	int m_interval;         // テクスチャ切り替えのフレーム数
	int m_currentNum = 0;   // 何番目のテクスチャを表示するか
	int* m_handleList;      // 分割されたテクスチャのハンドルリスト
	int m_allNum;           // テクスチャ分割数
};
