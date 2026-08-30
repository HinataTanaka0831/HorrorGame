#include "ObjectManager.h"
#include "Item.h"
#include "Master.h"
#include <algorithm>

ObjectManager::ObjectManager()
{
}

ObjectManager::~ObjectManager()
{
	DeleteAll2D();
	DeleteAll3D();
}

void ObjectManager::Update()
{
	for (auto itr = mObject2DList.begin(); itr != mObject2DList.end(); ++itr)
	{
		(*itr)->Update();
	}

	for (auto itr = mObject3DList.begin(); itr != mObject3DList.end(); ++itr)
	{
		(*itr)->Update();
	}
}

void ObjectManager::Draw()
{
	for (auto itr = mObject2DList.begin(); itr != mObject2DList.end(); ++itr)
	{
		(*itr)->Draw();
	}

	for (auto itr = mObject3DList.begin(); itr != mObject3DList.end(); ++itr)
	{
		(*itr)->Draw();
	}
}

void ObjectManager::AddObject(Object2D* object2D)
{
	mObject2DList.push_back(object2D);
}

void ObjectManager::DeleteAll2D()
{
	for (auto itr = mObject2DList.begin(); itr != mObject2DList.end();)
	{
		Object2D* temp = *itr;
		itr = mObject2DList.erase(itr);
		delete temp;
	}
}

void ObjectManager::DeleteAll2DIfNeeded()
{
	for (auto itr = mObject2DList.begin(); itr != mObject2DList.end();)
	{
		if ((*itr)->IsDeleteFlag())
		{
			Object2D* temp = *itr;
			itr = mObject2DList.erase(itr);
			delete temp;
		}
		else
		{
			++itr;
		}
	}
}

Object2D* ObjectManager::GetObject2DByTag(Object2D::Tag tag)
{
	auto itr = std::find_if(
		mObject2DList.begin(),
		mObject2DList.end(),
		[&](Object2D* obj) { return obj->GetTag() == tag; }
	);

	return (itr != mObject2DList.end()) ? (*itr) : nullptr;
}

std::vector<Object2D*> ObjectManager::GetObject2DListByTag(Object2D::Tag tag)
{
	std::vector<Object2D*> ret;
	for (auto itr = mObject2DList.begin(); itr != mObject2DList.end(); ++itr)
	{
		if ((*itr)->GetTag() == tag)
		{
			ret.push_back(*itr);
		}
	}
	return ret;
}

void ObjectManager::AddObject(Object3D* object3D)
{
	mObject3DList.push_back(object3D);
}

void ObjectManager::DeleteAll3D()
{
	for (auto itr = mObject3DList.begin(); itr != mObject3DList.end();)
	{
		Object3D* temp = *itr;
		itr = mObject3DList.erase(itr);
		delete temp;
	}
}

void ObjectManager::DeleteAll3DIfNeeded()
{
	for (auto itr = mObject3DList.begin(); itr != mObject3DList.end();)
	{
		if ((*itr)->IsDeleteFlag())
		{
			Object3D* temp = *itr;
			itr = mObject3DList.erase(itr);
			delete temp;
		}
		else
		{
			++itr;
		}
	}
}

Object3D* ObjectManager::GetObject3DByTag(Object3D::Tag3D tag)
{
	auto itr = std::find_if(
		mObject3DList.begin(),
		mObject3DList.end(),
		[&](Object3D* obj) { return obj->GetTag() == tag; }
	);

	return (itr != mObject3DList.end()) ? (*itr) : nullptr;
}

std::vector<Object3D*> ObjectManager::GetObject3DListByTag(Object3D::Tag3D tag)
{
	std::vector<Object3D*> ret;
	for (auto itr = mObject3DList.begin(); itr != mObject3DList.end(); ++itr)
	{
		if ((*itr)->GetTag() == tag)
		{
			ret.push_back(*itr);
		}
	}
	return ret;
}