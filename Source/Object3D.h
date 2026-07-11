#pragma once

#include "DxLib.h"
#include <string>

class Object3D
{
public:
	// オブジェクトを見分けるためのタグ
	enum Tag3D
	{
		None3D = 0,   // 設定なし

		TagPlayer3D = 2000,
		TagWall3D = 2100,
		TagEnemy3D = 2200,
		TagStage = 2300,
		TagEscapeItem = 2400,
		TagTimeItem = 2500,
		TagExitdoor = 2600,

	};


public:
	Object3D(VECTOR initPos);   // コンストラクタ
	virtual ~Object3D();        // デストラクタ

	virtual void Update();     // 更新
	virtual void Draw();      // 描画

public:  // ゲッター・セッター

	// 座標
	void SetPosition(VECTOR pos) { mvPosition = pos; }
	VECTOR GetPosition() { return mvPosition; }

	// 回転
	void SetRotation(VECTOR rot) { mvRotation = rot; }
	VECTOR GetRotation() { return mvRotation; }

	// 削除フラグ
	void SetDeleteFlag(bool flag) { mbDeleteFlag = flag; }
	bool IsDeleteFlag() { return mbDeleteFlag; }

	// タグ
	void SetTag(Tag3D tag) { mnTag = tag; }
	Tag3D GetTag() { return mnTag; }


protected:
	VECTOR mvPosition;   // 座標
	VECTOR mvRotation;   // 回転


private:
	bool mbDeleteFlag;   // 削除フラグ
	Tag3D mnTag;         // タグ
	float mfCurrentCameraDistance;     // 現在のカメラとの距離
};