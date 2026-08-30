#pragma once

#include <list>
#include <vector>
#include "Object2D.h"
#include "Object3D.h"

//
// オブジェクトを管理するクラス
//
// 2Dや3Dのオブジェクトが生成されるとき（newされるとき）に必ず経由され、
// 生成されたオブジェクトを一括管理するクラス。
// このクラスを作ることで、どのオブジェクトからでも同様なアクセスが可能になり、
// より柔軟なゲーム制作がしやすくなる
//

// シーン内に存在する2Dおよび3Dオブジェクトの一括更新・描画・ライフサイクル管理クラス
class ObjectManager
{
public:
	// コンストラクタ
	ObjectManager();
	// デストラクタ
	~ObjectManager();

	// 管理下の全2D/3Dオブジェクトの一括更新
	// 入力: なし / 出力: なし / 副作用: 各オブジェクトのUpdate呼び出し
	void Update();

	// 管理下の全2D/3Dオブジェクトの一括描画
	// 入力: なし / 出力: なし / 副作用: 各オブジェクトのDraw呼び出し
	void Draw();


public:         // 2D系の関数宣言
	// 2Dオブジェクトのリスト登録
	// 入力: object2D(追加するオブジェクトポインタ) / 出力: なし / 副作用: mObject2DListへの追加
	void AddObject(Object2D* object2D);

	// 登録された全2Dオブジェクトのメモリ解放
	// 入力: なし / 出力: なし / 副作用: 全要素のdeleteおよびリストクリア
	void DeleteAll2D();

	// 破棄フラグが有効化された2Dオブジェクトの安全な遅延削除
	// 入力: なし / 出力: なし / 副作用: 対象オブジェクトのdeleteおよびイテレータ更新
	void DeleteAll2DIfNeeded();

	// 指定タグを持つ先頭の2Dオブジェクト検索取得
	// 入力: tag(検索タグ) / 出力: 一致するオブジェクトポインタ(見つからなければnullptr) / 副作用: なし
	Object2D* GetObject2DByTag(Object2D::Tag tag);

	// 指定タグを持つ全2Dオブジェクトのリスト取得
	// 入力: tag(検索タグ) / 出力: 一致オブジェクトのポインタ配列 / 副作用: なし
	std::vector<Object2D*> GetObject2DListByTag(Object2D::Tag tag);

public:      // 3D系の関数宣言
	// 3Dオブジェクトのリスト登録
	// 入力: object3D(追加するオブジェクトポインタ) / 出力: なし / 副作用: mObject3DListへの追加
	void AddObject(Object3D* object3D);

	// 登録された全3Dオブジェクトのメモリ解放
	// 入力: なし / 出力: なし / 副作用: 全要素のdeleteおよびリストクリア
	void DeleteAll3D();

	// 破棄フラグが有効化された3Dオブジェクトの安全な遅延削除
	// 入力: なし / 出力: なし / 副作用: 対象オブジェクトのdeleteおよびイテレータ更新
	void DeleteAll3DIfNeeded();

	// 指定タグを持つ先頭の3Dオブジェクト検索取得
	// 入力: tag(検索タグ) / 出力: 一致するオブジェクトポインタ(見つからなければnullptr) / 副作用: なし
	Object3D* GetObject3DByTag(Object3D::Tag3D tag);

	// 指定タグを持つ全3Dオブジェクトのリスト取得
	// 入力: tag(検索タグ) / 出力: 一致オブジェクトのポインタ配列 / 副作用: なし
	std::vector<Object3D*> GetObject3DListByTag(Object3D::Tag3D tag);



private:
	std::list<Object2D*> mObject2DList;         // 2Dオブジェクトを管理するリスト
	std::list<Object3D*> mObject3DList;         // 3Dオブジェクトを管理するリスト
};