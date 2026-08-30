#pragma once

#include "DxLib.h"
#include <string>

// 3Dアクターおよびギミックオブジェクトの共通基底クラス
class Object3D
{
public:
	// オブジェクトを見分けるためのタグ
	enum Tag3D
	{
		None3D = 0,        // 設定なし
		TagPlayer3D,       // プレイヤー
		TagEnemy3D,        // 敵キャラクター
		TagEscapeItem,   // 脱出用アイテム
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