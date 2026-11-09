#include "Stage.h"
#include "Player3D.h"
#include "Master.h"
#include "ObjectManager.h"
#include "Scene.h"
#include "CollisionManager.h"


Stage::Stage(std::string stageModelName)
	: Object3D(VGet(0.0f, 0.0f, 0.0f))  // 座標は原点としておく
	, m_modelHandle(-1)

{
	SetTag(Object3D::TagStage);

	// ステージモデルの読み込み
	m_modelHandle = MV1LoadModel(stageModelName.c_str());
	
}

Stage::~Stage()
{
	// 読み込んだモデルデータの破棄
	MV1DeleteModel(m_modelHandle);
}


void Stage::Draw()
{
	// ステージの描画
	MV1DrawModel(m_modelHandle);
}
