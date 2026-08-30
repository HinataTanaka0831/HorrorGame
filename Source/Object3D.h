#pragma once

#include "DxLib.h"
#include <string>

// 3Dアクターおよびギミックオブジェクトの共通基底クラス
class Object3D
{
public:
	enum Tag3D
	{
		None3D = 0,
		TagPlayer3D,       // プレイヤー
		TagEnemy3D,        // 敵キャラクター
		TagItem3D,         // 一般アイテム
		TagEscapeItem3D,   // 脱出用アイテム
		TagTimeItem,       // 制限時間アイテム
		TagStage,          // ステージマップ
		TagExitdoor,       // 脱出ドア
	};

public:
	// 座標初期化およびシーンObjectManagerへの自動登録
	// 入力: initPos(初期3Dワールド座標) / 出力: なし / 副作用: シーンObjectManagerへの登録
	Object3D(VECTOR initPos);
	virtual ~Object3D();

	// 毎フレームの状態更新
	// 入力: なし / 出力: なし / 副作用: 派生クラスによる座標・AI・入力状態等の変更
	virtual void Update();

	// 3Dモデル描画
	// 入力: なし / 出力: なし / 副作用: 派生クラスによるDXライブラリ3D描画
	virtual void Draw();

	Tag3D GetTag() const { return mnTag; }
	void SetTag(Tag3D tag) { mnTag = tag; }

	VECTOR GetPosition() const { return mvPosition; }
	void SetPosition(VECTOR pos) { mvPosition = pos; }

	VECTOR GetRotation() const { return mvRotation; }
	void SetRotation(VECTOR rot) { mvRotation = rot; }

	bool IsDeleteFlag() const { return mbDeleteFlag; }
	void SetDeleteFlag(bool f) { mbDeleteFlag = f; }

protected:
	VECTOR mvPosition;
	VECTOR mvRotation;
	bool mbDeleteFlag;
	Tag3D mnTag;
};