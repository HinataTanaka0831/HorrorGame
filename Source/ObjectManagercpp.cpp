#include "ObjectManager.h"
#include "Item.h"
#include "Master.h"

ObjectManager::ObjectManager()
{

}

ObjectManager::~ObjectManager()
{

}

void ObjectManager::Update()
{
	// 2Dオブジェクトの更新
	// mObject2Dlist.begin() ... リストの先頭の要素へのイテレーターを取得する
	// mObject2Dlist.end() ... リストの末尾の要素へのイテレーターを取得する
	// イテレーターとは？ ... std::list の要素のことを指す。
	// リストの利点 ... 要素と要素の間に新しい要素を差し込むことが容易に出来る
	// std::list ... 双方向連結リスト（要素の前後の要素にアクセスすることが簡単）
	for (std::list<Object2D*>::iterator itr = m_object2DList.begin(); itr != m_object2DList.end(); itr++)
	{
		(*itr)->Update();
	}

	// 3D
	for (std::list<Object3D*>::iterator itr = m_object3DList.begin(); itr != m_object3DList.end(); itr++)
	{
		(*itr)->Update();
	}


}

void ObjectManager::Draw()
{

	// auto ... 型推論。 = より右側の型を推測してくれる便利なやつ
	for (auto itr = m_object2DList.begin(); itr != m_object2DList.end(); itr++)
	{
		(*itr)->Draw();
	}

	// 3D
	for (auto itr = m_object3DList.begin(); itr != m_object3DList.end(); itr++)
	{
		(*itr)->Draw();

	}

}

void ObjectManager::AddObject(Object2D* object2D)
{
	m_object2DList.push_back(object2D);
}

void ObjectManager::DeleteAll2D()
{
	for (auto itr = m_object2DList.begin(); itr != m_object2DList.end(); /*ここは空っぽなので注意*/)
	{
		Object2D* temp = *itr;

		// リストから削除
		itr = m_object2DList.erase(itr);

		// オブジェクトそのものを削除
		delete temp;
		temp = nullptr;
	}
}


void ObjectManager::DeleteAll2DIfNeeded()
{
	for (auto itr = m_object2DList.begin(); itr != m_object2DList.end(); /*ここは空っぽなので注意*/)
	{
		// 破棄フラグが立っていれば削除する
		if ( (*itr)->IsDeleteFlag() )
		{
			Object2D *temp = *itr;

			// リストから削除
			// erase() は、削除した itr の次の要素を返却してくれる
			itr = m_object2DList.erase(itr);

			// オブジェクトそのものを削除
			delete temp;
			temp = nullptr;
		}
		else
		{
			// 次の要素へ進める
			itr++;
		}
	}

}

Object2D* ObjectManager::GetObject2DByTag(Object2D::Tag tag)
{
	// std::find を利用して対象のオブジェクトを探す
	auto itr = std::find_if(
		m_object2DList.begin(),
		m_object2DList.end(),
		[&](Object2D* obj) { return obj->GetTag() == tag; }   // ラムダ式
		// [&] ... 今回の場合、mObject2Dlist の要素を[参照]するという意味合い
		// (Object2D *obj) ... 参照したオブジェクトの型と引数名
		// { ... } ... 処理内容（今回は条件式）
		// [](){ ... } この形がラムダ式の基本
	);


	// 見つかったかどうか判定
	if (itr != m_object2DList.end())
	{
		return (*itr);    // オブジェクトが見つかった
	}

	return nullptr;      // オブジェクトが見つからなかった
}


std::vector<Object2D*> ObjectManager::GetObject2DListByTag(Object2D::Tag tag)
{
	std::vector<Object2D*> ret;

	for (auto itr = m_object2DList.begin(); itr != m_object2DList.end(); itr++)
	{
		// tag と同じタグを持っているオブジェクトがあればvectorに入れる
		if ( (*itr)->GetTag() == tag )
		{
			ret.push_back((*itr));
		}
	}


	return ret;
}


// ---------3D---------- ///

void ObjectManager::AddObject(Object3D* object3D)
{
	m_object3DList.push_back(object3D);
}

// 3Dオブジェクトの全削除
void ObjectManager::DeleteAll3D()
{
	for (auto itr = m_object3DList.begin(); itr != m_object3DList.end(); /*ここは空っぽなので注意*/)
	{
		Object3D* temp = *itr;

		// リストから削除
		itr = m_object3DList.erase(itr);

		// オブジェクトそのものを削除
		delete temp;
		temp = nullptr;
	}
}


void ObjectManager::DeleteAll3DIfNeeded()
{
	for (auto itr = m_object3DList.begin(); itr != m_object3DList.end(); /*ここは空っぽなので注意*/)
	{
		// 破棄フラグが立っていれば削除する
		if ((*itr)->IsDeleteFlag())
		{
			Object3D* temp = *itr;

			// リストから削除
			// erase() は、削除した itr の次の要素を返却してくれる
			itr = m_object3DList.erase(itr);

			// オブジェクトそのものを削除
			delete temp;
			temp = nullptr;
		}
		else
		{
			// 次の要素へ進める
			itr++;
		}
	}

}

Object3D* ObjectManager::GetObject3DByTag(Object3D::Tag3D tag)
{
	// std::find を利用して対象のオブジェクトを探す
	auto itr = std::find_if(
		m_object3DList.begin(),
		m_object3DList.end(),
		[&](Object3D* obj) { return obj->GetTag() == tag; }   // ラムダ式
		// [&] ... 今回の場合、mObject3Dlist の要素を[参照]するという意味合い
		// (Object3D *obj) ... 参照したオブジェクトの型と引数名
		// { ... } ... 処理内容（今回は条件式）
		// [](){ ... } この形がラムダ式の基本
	);


	// 見つかったかどうか判定
	if (itr != m_object3DList.end())
	{
		return (*itr);    // オブジェクトが見つかった
	}

	return nullptr;      // オブジェクトが見つからなかった
}


std::vector<Object3D*> ObjectManager::GetObject3DListByTag(Object3D::Tag3D tag)
{
	std::vector<Object3D*> ret;

	for (auto itr = m_object3DList.begin(); itr != m_object3DList.end(); itr++)
	{
		// tag と同じタグを持っているオブジェクトがあればvectorに入れる
		if ((*itr)->GetTag() == tag)
		{
			ret.push_back((*itr));
		}
	}


	return ret;
}
