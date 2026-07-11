#include "Wall.h"

Wall::Wall(std::string filename, VECTOR centerPos, VECTOR topLeft, VECTOR bottomRight)
	:Object3D(centerPos)
{
	// タグ設定
	SetTag(Object3D::TagWall3D);

	// 画像読み込み
	mnGraphHandle = LoadGraph(filename.c_str());

	// 4頂点分のデータをセット

	// 左上
	mVertex[0].pos = VAdd(centerPos, topLeft);
	mVertex[0].norm = VGet(1.0f, 0.0f, 0.0f);     // 後で計算するので仮の値
	mVertex[0].dif = GetColorU8(255, 255, 255, 255);
	mVertex[0].spc = GetColorU8(0, 0, 0, 0);
	mVertex[0].u = 0.0f;
	mVertex[0].v = 0.0f;
	mVertex[0].su = 0.0f;
	mVertex[0].sv = 0.0f;

	// 右上
	mVertex[1].pos = VAdd(centerPos, VGet(bottomRight.x, topLeft.y, bottomRight.z));
	mVertex[1].norm = VGet(1.0f, 0.0f, 0.0f);     // 後で計算するので仮の値
	mVertex[1].dif = GetColorU8(255, 255, 255, 255);
	mVertex[1].spc = GetColorU8(0, 0, 0, 0);
	mVertex[1].u = 1.0f;
	mVertex[1].v = 0.0f;
	mVertex[1].su = 1.0f;
	mVertex[1].sv = 0.0f;

	// 左下
	mVertex[2].pos = VAdd(centerPos, VGet(topLeft.x, bottomRight.y, topLeft.z));
	mVertex[2].norm = VGet(1.0f, 0.0f, 0.0f);     // 後で計算するので仮の値
	mVertex[2].dif = GetColorU8(255, 255, 255, 255);
	mVertex[2].spc = GetColorU8(0, 0, 0, 0);
	mVertex[2].u = 0.0f;
	mVertex[2].v = 1.0f;
	mVertex[2].su = 0.0f;
	mVertex[2].sv = 1.0f;

	// 右下
	mVertex[3].pos = VAdd(centerPos, bottomRight);
	mVertex[3].norm = VGet(1.0f, 0.0f, 0.0f);     // 後で計算するので仮の値
	mVertex[3].dif = GetColorU8(255, 255, 255, 255);
	mVertex[3].spc = GetColorU8(0, 0, 0, 0);
	mVertex[3].u = 1.0f;
	mVertex[3].v = 1.0f;
	mVertex[3].su = 1.0f;
	mVertex[3].sv = 1.0f;




	// 法線の設定
	VECTOR norm = VCross(VSub(mVertex[0].pos, mVertex[1].pos), VSub(mVertex[0].pos, mVertex[2].pos));
	norm = VNorm(norm);   // 正規化（ベクトルの大きさを１にする）
	mVertex[0].norm = norm;
	mVertex[1].norm = norm;
	mVertex[2].norm = norm;
	mVertex[3].norm = norm;

}


Wall::~Wall()
{
	// 画像の破棄
	DeleteGraph(mnGraphHandle);
}

// 更新
void Wall::Update()
{


}

// 描画
void Wall::Draw()
{
	WORD index[6];

	// ２ポリゴン分のインデックスデータを設定
	// 右辺は、頂点データの配列番号
	index[0] = 0;
	index[1] = 1;
	index[2] = 2;
	index[3] = 3;
	index[4] = 2;
	index[5] = 1;

	SetUseLighting(false);

	// ２ポリゴンの描画
	DrawPolygonIndexed3D(mVertex, 4, index, 2,mnGraphHandle, true);

	SetUseLighting(true);
}