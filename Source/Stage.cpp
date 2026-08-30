#include "Stage.h"

Stage::Stage(std::string filename, std::string collisionFilename)
	: Object3D(VGet(0.0f, 0.0f, 0.0f))
	, normal(VGet(0.0f, 0.0f, 0.0f))
	, hitpos(VGet(0.0f, 0.0f, 0.0f))
{
	SetTag(Tag3D::TagStage);

	mnModelHandle = MV1LoadModel(filename.c_str());
	mnCollisionHandle = MV1LoadModel(collisionFilename.c_str());

	// 高速なポリゴン交差判定のためコリジョン情報ツリーを構築
	MV1SetupCollInfo(mnCollisionHandle);
}

Stage::~Stage()
{
	MV1DeleteModel(mnModelHandle);
	MV1DeleteModel(mnCollisionHandle);
}

void Stage::Update()
{
}

// ステージ3Dモデルの描画
// 入力: なし / 出力: なし / 副作用: バックバッファへの描画
void Stage::Draw()
{
	MV1DrawModel(mnModelHandle);
}

// カプセル形状（キャラクター）とステージ壁・床ポリゴンとの衝突判定
// 入力: pos1(カプセル始点), pos2(カプセル終点), r(半径) / 出力: 衝突していればtrue / 副作用: normal(法線), hitpos(衝突点)の更新
bool Stage::CheckHit_Capsule(VECTOR pos1, VECTOR pos2, float r)
{
	MV1_COLL_RESULT_POLY_DIM result = MV1CollCheck_Capsule(mnCollisionHandle, -1, pos1, pos2, r);

	if (result.HitNum >= 1)
	{
		float minDist = 1e9f;
		VECTOR bestNormal = VGet(0, 0, 0);
		VECTOR bestHitPos = VGet(0, 0, 0);

		// 最も近接しているポリゴンの法線と衝突点を探索
		for (int i = 0; i < result.HitNum; ++i)
		{
			float dist = VSize(VSub(result.Dim[i].HitPosition, pos1));
			if (dist < minDist)
			{
				minDist = dist;
				bestNormal = result.Dim[i].Normal;
				bestHitPos = result.Dim[i].HitPosition;
			}
		}

		normal = bestNormal;
		hitpos = bestHitPos;
	}

	MV1CollResultPolyDimTerminate(result);
	return (result.HitNum >= 1);
}

// 線分（視線・レイキャスト）とステージジオメトリとの交差判定
// 入力: pos1(始点), pos2(終点) / 出力: 交差座標（交差しなければ原点） / 副作用: なし
VECTOR Stage::CheckHit_Line(VECTOR pos1, VECTOR pos2)
{
	VECTOR ret = VGet(0.0f, 0.0f, 0.0f);
	auto result = MV1CollCheck_Line(mnCollisionHandle, -1, pos1, pos2);

	if (result.HitFlag)
	{
		ret = result.HitPosition;
	}

	return ret;
}