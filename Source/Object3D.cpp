#include "Object3D.h"
#include "Master.h"
#include "ObjectManager.h"
#include "Scene.h"

Object3D::Object3D(VECTOR initPosition)
	:m_position(initPosition)
	, m_rotation(VGet(0.0f, 0.0f, 0.0f))
	, m_deleteFlag(false)
	, m_tag(Tag3D::None3D)
{
	// Œ»ÝƒV[ƒ“‚ÌobjectManager‚ÉŽ©M(this)‚ð’Ç‰Á‚·‚é
	Master::m_sceneManager->GetCurrentScene()->GetObjectManager()->AddObject(this);

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