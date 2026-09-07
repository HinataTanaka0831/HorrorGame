#include "TextureAnimation.h"

TextureAnimation::TextureAnimation(
	VECTOR position,
	std::string fileName,
	int allNum,
	int xNum,
	int yNum,
	int interval
)
	: m_position(position)
	, m_interval(interval)
	, m_allNum(allNum)
{
	m_handleList = new int[allNum];

	// 画像ファイル読み込み
	int handle = LoadGraph(fileName.c_str());
	if (handle == -1)
	{
		return;   // 読み込み失敗していたら以降は処理しない
	}

	// サイズ取得
	int sizeX, sizeY;
	GetGraphSize(handle, &sizeX, &sizeY);

	// テクスチャの分割読み込み
	int success = LoadDivGraph(
		fileName.c_str(),
		allNum,
		xNum,
		yNum,
		sizeX / xNum,
		sizeY / yNum,
		m_handleList
	);
}

TextureAnimation::~TextureAnimation()
{

}

void TextureAnimation::Update()
{
	// カウンタをインクリメント
	m_counter++;
	if (m_counter % m_interval == 0)
	{
		m_counter = 0;   // カウンタを戻す
		m_currentNum++;  // テクスチャ番号を進める
		if (m_currentNum >= m_allNum)  // 分割数を超えるならループさせる
		{
			m_currentNum = 0;   // ループさせる
		}
	}
}

void TextureAnimation::Draw()
{
	DrawGraph((int)m_position.x, (int)m_position.y, m_handleList[m_currentNum], true);
}
