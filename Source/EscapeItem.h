#pragma once

#include "Item.h"
#include <string>
#include "ItemData.h"


// 校舎脱出に必要な収集対象アイテム（おふだ）クラス
class EscapeItem : public Item
{
public:
	ItemData escape = { 1, LoadGraph("Resource/UI/ui_escapeItem.png"), "おふだ" };  // EscapeItem のデータ
	static int m_nowNeedItem;  // 現在までに取得・消費された脱出アイテム数

public:
	// 脱出アイテム3Dモデルのロードおよび配置
	// 入力: fileName(モデルパス), initPosition(ワールド配置座標), isSeparateAnimation(分割モーションフラグ) / 出力: なし / 副作用: TagEscapeItem3D設定
	EscapeItem(const std::string& fileName, VECTOR initPosition, bool isSeparateAnimation = false);

	// 生成したクラスの削除
	// 入力: なし / 出力: なし / 副作用: m_modelの解放
	~EscapeItem() override;

	// アイテムの浮遊回転アニメーション設定、基底クラスの更新呼び出し
	// 入力: なし / 出力: なし / 副作用: 3Dモデルの回転設定、Item::Update()の呼び出し
	void Update() override;

	// アイテム3Dモデル描画
	// 入力: なし / 出力: なし / 副作用: Item::Draw()の呼び出し
	void Draw() override;

	// 収集数のインクリメントおよび4個達成時のクリアシーン遷移
	// 入力: なし / 出力: なし / 副作用: NowNeedItemの加算、クリア時のSceneManager遷移予約
	static void Escape();
};