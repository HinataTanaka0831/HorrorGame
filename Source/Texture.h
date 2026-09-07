#pragma once

#include <string>
#include "DxLib.h"

// 2Dテクスチャ画像のロード、座標管理、透過描画を行うラッパークラス
class Texture
{
public:
	// 画像読み込みとサイズ・当たり判定半径の算出
	// 入力: fileName(画像パス), centerPosition(中心座標), transFlag(透過フラグ) / 出力: なし / 副作用: LoadGraph実行
	Texture(std::string fileName, VECTOR centerPosition, int transFlag);     
	~Texture();    

	void Update();  

	// 2D画像の中心基準描画
    // 入力: なし / 出力: なし / 副作用: バックバッファへの描画
	void Draw();

	// セッター関数 //
	// ポジション設定
	void SetPosition(VECTOR centerPosition) { m_position = centerPosition; }

	// ゲッター関数
	// ポジション取得
	VECTOR GetPosition() { return m_position; }
	// サイズ取得
	int GetSizeX() { return m_sizeX; }
	int GetSizeY() { return m_sizeY; }
	float GetRadius() { return m_radius; }
	
private:
	int m_handle = -1;       // 読み込んだ画像のハンドル
	VECTOR m_position;       // ポジション
	int m_sizeX = 0;         // 画像の幅
	int m_sizeY = 0;         // 画像の高さ
	int m_transFlag;         // 画像の透過を有効にするか
	float m_radius = 0.0f;   // 半径
	

};
