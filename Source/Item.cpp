#include "Item.h"

Item::Item(VECTOR initPos)
	: Object3D(initPos)
{
	SetTag(Object3D::TagItem3D);
}

Item::~Item()
{
}

void Item::Update()
{
}

void Item::Draw()
{
}