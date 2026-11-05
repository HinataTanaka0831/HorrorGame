#include "Exitdoor.h"
#include "Model.h"
#include "Master.h"
#include "Scene.h"
#include "ObjectManager.h"

Exitdoor::Exitdoor(std::string fileName,VECTOR initPosition)
	: Object3D(initPosition)
	, m_model(nullptr)

{
	// ƒ^ƒOÝ’è
	SetTag(Object3D::TagExitdoor);

	// ƒ‚ƒfƒ‹‚Ìì¬
	m_model = new Model(fileName, initPosition);
}


Exitdoor::~Exitdoor()
{
	if (m_model != nullptr)
	{
		delete m_model;
	}
}


void Exitdoor::Update()
{
	if (m_model != nullptr)
	{
		m_model->Update();
	}
}


void Exitdoor::Draw()
{
	if (m_model != nullptr)
	{
		m_model->Draw();
	}
}