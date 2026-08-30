#pragma once
#include "DxLib.h"
#include <string>

// スプライトシートから分割読み込みした連番テクスチャのアニメーション制御クラス
class TextureAnimation
{
public:
	// 分割テクスチャの一括読み込みおよびアニメーションパラメータ初期化
	// 入力: position(表示座標), filename(画像パス), allNum(総コマ数), xNum(横分割), yNum(縦分割), interval(コマ送り間隔フレーム) / 出力: なし / 副作用: LoadDivGraph実行
	TextureAnimation(
		VECTOR position,
		std::string filename,
		int allNum,
		int xNum,
		int yNum,
		int interval
	);  

	~TextureAnimation(); 

	// フレームインターバル経過によるコマ送り更新
	// 入力: なし / 出力: なし / 副作用: mnCurrentNumおよびmnCounterの更新
	void Update(); 

	// 現在のコマ画像描画
	// 入力: なし / 出力: なし / 副作用: バックバッファへの描画
	void Draw();   

private:
	VECTOR mvPosition;  // ポジション
	int mnCounter;      // アニメーションカウンタ
	int mnInterval;     // テクスチャ切り替えのフレーム数
	int mnCurrentNum;   // 何番目のテクスチャを表示するか
	int* mnHandleList;  // 分割されたテクスチャのハンドルリスト
	int mnAllNum;       // テクスチャ分割数
};
