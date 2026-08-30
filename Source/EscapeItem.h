#pragma once

#include "Item.h"
#include "DxLib.h"
#include <string>

class Model;

// 校舎脱出に必要な収集対象アイテム（お札/鍵）クラス
class EscapeItem : public Item
{
public:
	static int NowNeedItem;  // 現在までに取得・消費された脱出アイテム数

public:
	// 脱出アイテム3Dモデルのロードおよび配置
	// 入力: filename(モデルパス), initPos(ワールド配置座標) / 出力: なし / 副作用: TagEscapeItem3D設定、Model生成
	EscapeItem(std::string filename, VECTOR initPos);
	~EscapeItem() override;

	// アイテムの浮遊回転アニメーション更新
	// 入力: なし / 出力: なし / 副作用: 3Dモデルの回転・座標更新
	void Update() override;

	// アイテム3Dモデル描画
	// 入力: なし / 出力: なし / 副作用: バックバッファへの描画
	void Draw() override;

	// 収集数のインクリメントおよび4個達成時のクリアシーン遷移
	// 入力: なし / 出力: なし / 副作用: NowNeedItemの加算、クリア時のSceneManager遷移予約
	static void Escape();

private:
	Model* mpModel;
};