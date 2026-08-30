#pragma once
// #ifndef _TEXTURE_H_
// #define _TEXTURE_H_
#include <string>
#include "DxLib.h"

// 2Dテクスチャ画像のロード、座標管理、透過描画を行うラッパークラス
class Texture
{
public:
	// 画像読み込みとサイズ・当たり判定半径の算出
	// 入力: filename(画像パス), centerPosition(中心座標), transFlag(透過フラグ) / 出力: なし / 副作用: LoadGraph実行
	Texture(std::string filename, VECTOR centerPosition, int transFlag);     
	~Texture();    

	void Update();  

	// 2D画像の中心基準描画
    // 入力: なし / 出力: なし / 副作用: バックバッファへの描画
	void Draw();

	// セッター関数 //
	// ポジション設定
	void SetPosition(VECTOR centerPosition) { mvPosition = centerPosition; }

	// ゲッター関数
	// ポジション取得
	VECTOR GetPosition() { return mvPosition; }
	// サイズ取得
	int GetSizeX() { return mnSizeX; }
	int GetSizeY() { return mnSizeY; }
	float GetRadius() { return mfRadius; }
	
private:
	int mnHandle;      // 読み込んだ画像のハンドル
	VECTOR mvPosition; // ポジション
	int mnSizeX;       // 画像の幅
	int mnSizeY;       // 画像の高さ
	int mnTransFlag;  // 画像の透過を有効にするか
	float mfRadius;   // 半径
	

};
