#pragma once

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

	// 2D画像の中心基準描画
	// 入力: なし / 出力: なし / 副作用: バックバッファへの描画
	void Draw();

	void Update();

	void SetPosition(VECTOR centerPosition) { mvPosition = centerPosition; }
	VECTOR GetPosition() const { return mvPosition; }
	int GetSizeX() const { return mnSizeX; }
	int GetSizeY() const { return mnSizeY; }
	float GetRadius() const { return mfRadius; }

private:
	int mnHandle;
	VECTOR mvPosition;
	int mnSizeX;
	int mnSizeY;
	int mnTransFlag;
	float mfRadius;
};