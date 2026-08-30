#pragma once

#include "DxLib.h"
#include "Object3D.h"
#include <string>
#include <vector>
#include "ItemData.h"

// ステージ上に配置される3Dアイテムオブジェクトの共通基底クラス
class Item : public Object3D
{
public:
	// アイテムの配置座標初期化およびTagItem3D設定
	// 入力: initPos(ワールド配置座標) / 出力: なし / 副作用: Object3D初期化
	Item(VECTOR initPos);
	virtual ~Item() override;


	virtual void Update() override;


	virtual void Draw() override;
};

