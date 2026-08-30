#include "Texture.h"
#include "DxLib.h"

Texture::Texture(std::string filename, VECTOR centerPosition, int transFlag)
	: mnHandle(-1)
	, mvPosition(centerPosition)
	, mnSizeX(0)
	, mnSizeY(0)
	, mnTransFlag(transFlag)
{
	mnHandle = LoadGraph(filename.c_str());
	GetGraphSize(mnHandle, &mnSizeX, &mnSizeY);
	// 円形当たり判定用に画像の横幅半分をデフォルト半径に設定
	mfRadius = (float)mnSizeX / 2.0f;
}

Texture::~Texture()
{
	if (mnHandle != -1)
	{
		DeleteGraph(mnHandle);
		mnHandle = -1;
	}
}

// 2D画像の中心基準描画
// 入力: なし / 出力: なし / 副作用: バックバッファへの描画
void Texture::Draw()
{
	DrawGraph((int)mvPosition.x - (mnSizeX / 2), (int)mvPosition.y - (mnSizeY / 2), mnHandle, mnTransFlag);
}

void Texture::Update()
{
}