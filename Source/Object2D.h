#pragma once

#include "DxLib.h"
#include <string>

class Texture;

// 2Dスプライトオブジェクトの基底クラス
class Object2D
{
public:
	enum Tag
	{
		None2D = 0,
		Player2D,
		Enemy2D,
		Bullet2D,
	};

public:
	// 2Dテクスチャ生成およびObjectManagerへの自動登録
	// 入力: filename(画像パス), initPos(初期座標) / 出力: なし / 副作用: Texture生成、シーンObjectManagerへの登録
	Object2D(std::string filename, VECTOR initPos);
	virtual ~Object2D();

	// 座標同期およびスプライト更新
	// 入力: なし / 出力: なし / 副作用: 内部Texture座標の更新
	virtual void Update();

	// スプライト描画
	// 入力: なし / 出力: なし / 副作用: バックバッファへの描画
	virtual void Draw();

	Tag GetTag() const { return mnTag; }
	void SetTag(Tag tag) { mnTag = tag; }

	VECTOR GetPosition() const { return mvPosition; }
	void SetPosition(VECTOR pos) { mvPosition = pos; }

	float GetRadius();

	bool IsDeleteFlag() const { return mbDeleteFlag; }
	void SetDeleteFlag(bool f) { mbDeleteFlag = f; }

protected:
	Texture* mpTexture;
	VECTOR mvPosition;
	bool mbDeleteFlag;
	float mfAngle;
	float mfspeed;
	VECTOR mvDirection;
	Tag mnTag;

public:
	static int score;
};