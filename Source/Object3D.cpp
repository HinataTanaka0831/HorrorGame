#include "Object3D.h"
#include "Master.h"
#include "ObjectManager.h"
#include "Scene.h"

Object3D::Object3D(VECTOR initPos)
	: mvPosition(initPos)
	, mvRotation(VGet(0.0f, 0.0f, 0.0f))
	, mbDeleteFlag(false)
	, mnTag(Tag3D::None3D)
{
	// シーン管理下での一括更新・描画・破棄を可能にするため登録
	Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->AddObject(this);
}

Object3D::~Object3D()
{
}

void Object3D::Update()
{
}

void Object3D::Draw()
{
}