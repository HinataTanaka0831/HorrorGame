#pragma once

#include "Object3D.h"
#include <string>
#include <vector>

// 前方宣言
class Model;

// ステージ上に配置される3Dアイテムオブジェクトの共通基底クラス
class Item : public Object3D
{
public:
	// アイテムの配置座標初期化およびTagItem3D設定
	// 入力: initPosition(ワールド配置座標) / 出力: なし / 副作用: Object3D初期化
	Item(const std::string& fileName, VECTOR initPosition, bool isSeparateAnimation = false);
	virtual ~Item() override;

	// アイテムの浮遊回転アニメーション設定、基底クラスの更新呼び出し
    // 入力: なし / 出力: なし / 副作用: 3Dモデルの回転・座標更新
	virtual void Update() override;

	// アイテム3Dモデル描画
    // 入力: なし / 出力: なし / 副作用: バックバッファへの描画
	virtual void Draw() override;

public:
	void SetRotationZ(float setRotationZ) { m_rotationZ = setRotationZ; }

private:
	Model* m_model;
	float m_rotationZ;
};

