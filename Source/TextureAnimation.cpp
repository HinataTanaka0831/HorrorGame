#include "TextureAnimation.h"

TextureAnimation::TextureAnimation(
	VECTOR position,
	std::string filename,
	int allNum,
	int xNum,
	int yNum,
	int interval
)
	: mvPosition(position)
	, mnCounter(0)
	, mnInterval(interval)
	, mnCurrentNum(0)
	, mnAllNum(allNum)
{
	mnHandleList = new int[allNum];

	int handle = LoadGraph(filename.c_str());
	if (handle == -1)
	{
		return;
	}

	int sizeX, sizeY;
	GetGraphSize(handle, &sizeX, &sizeY);
	DeleteGraph(handle);

	LoadDivGraph(
		filename.c_str(),
		allNum,
		xNum,
		yNum,
		sizeX / xNum,
		sizeY / yNum,
		mnHandleList
	);
}

TextureAnimation::~TextureAnimation()
{
	if (mnHandleList != nullptr)
	{
		for (int i = 0; i < mnAllNum; ++i)
		{
			DeleteGraph(mnHandleList[i]);
		}
		delete[] mnHandleList;
		mnHandleList = nullptr;
	}
}

// フレームインターバル経過によるコマ送り更新
// 入力: なし / 出力: なし / 副作用: mnCurrentNumおよびmnCounterの更新
void TextureAnimation::Update()
{
	mnCounter++;
	if (mnCounter % mnInterval == 0)
	{
		mnCounter = 0;
		mnCurrentNum = (mnCurrentNum + 1) % mnAllNum;
	}
}

// 現在のコマ画像描画
// 入力: なし / 出力: なし / 副作用: バックバッファへの描画
void TextureAnimation::Draw()
{
	if (mnHandleList != nullptr)
	{
		DrawGraph((int)mvPosition.x, (int)mvPosition.y, mnHandleList[mnCurrentNum], true);
	}
}