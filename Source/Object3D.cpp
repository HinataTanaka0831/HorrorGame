#include "Object3D.h"
#include "Master.h"
#include "ObjectManager.h"
#include "Scene.h"

// コンストラクタ
Object3D::Object3D(VECTOR initPos)
	:mvPosition(initPos)
	, mvRotation(VGet(0.0f, 0.0f, 0.0f))
	, mbDeleteFlag(false)
	, mnTag(Tag3D::None3D)
{
	// 現在シーンのobjectManagerに自信(this)を追加する
	Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->AddObject(this);

}

// デストラクタ
Object3D::~Object3D()
{

}

// 更新
void Object3D::Update()
{

}

// 描画
void Object3D::Draw()
{

}