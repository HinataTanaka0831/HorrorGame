#pragma once

#include "DxLib.h"
#include "Object3D.h"
#include <string>
#include <vector>
#include "ItemData.h"

class Item : public Object3D
{
public:
	// コンストラクタ
	Item(VECTOR initPos);
	// デストラクタ
	virtual ~Item() override;

	// 更新
	virtual void Update() override;

	// 描画
	virtual void Draw() override;
};

