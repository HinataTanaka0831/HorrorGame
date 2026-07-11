#pragma once

#include "Item.h"
#include <string>
#include "DxLib.h"
#include "Model.h"
#include "ItemData.h"


class EscapeItem : public Item
{
public:
	ItemData escape = { 1, LoadGraph("Resource/3D_UI/escapeItem.png"), "お札" };  // EscapeItem のデータ
	static int NowNeedItem; // 残りの脱出のために必要なアイテム数

public:
	// コンストラクタ
	EscapeItem(std::string filename, VECTOR initPos);
	// デストラクタ
	~EscapeItem() override;

	// 更新
	void Update() override;
	// 描画
	void Draw() override;

	// アイテムでの脱出処理
	static void Escape();

private:
	Model* mpModel;  // モデルクラスのポインタ
};