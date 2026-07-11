#pragma once

#include "DxLib.h"
#include <string>
#include <vector>
#include "ItemData.h"
#include "Object3D.h"
#include "Utility.h"
#include "MouseManager.h"

class Item;
class EscapeItem;
class TimeItem;

class Player3D : public Object3D
{
public:
	Player3D(VECTOR initPos);                     	// コンストラクタ
	~Player3D() override;                           // デストラクタ
								                  
	void Update() override;                         // 更新
	void Draw() override;                           // 描画
								                  
	void MoveEx();                                 // 移動処理（ステージとの当たり判定用）

	void PlayerSquat();                          // プレイヤーのしゃがみ状態の変更処理

	void HaveLight();                           // ライトの処理

	void ItemCollision();   	                  // アイテムとの当たり判定

	void UseItem();

	void ItemBox();                         	// アイテムボックス

	bool AddItem(const ItemData item);   	   // アイテムをインベントリに追加（取得処理）

	void LoseItem();                           	// アイテムをインベントリから削除（使用処理

	void DrawStamina();

	bool StaminaUpdate(bool isKeyProssese);      // スタミナゲージの更新処理

	bool GetCrouching() { return isCrouching; } // しゃがんでいるかどうかを取得

	float GetHeight() { return playerHeight; }  // プレイヤーの高さを取得

	void SetFreeze(bool freeze) { isFreeze = freeze; } // フリーズを設定

	// 時間止めアイテムを使用するかを設定
	void SetUseStopItem(bool stop) { isUseStopItem = stop; }

	bool GetEscapeItem() const { return getEscapeItem; }
	bool GetTimeItem() const { return getTimeItem; }
	bool GetIsUseStopItem() { return isUseStopItem; }


	std::vector<VECTOR>& GetPlayerRecord() { return playerRecord; } // プレイヤーの座標を保持する構造体を取得

private:
	struct Player {
		VECTOR pos;      // 中心座標
		float radius;    // 当たり判定の半径
	};

	MouseManager mMouseMgr;         // マウスマネージャー

	VECTOR DoorPos;               // 脱出口の座標を取得するための変数
	VECTOR oldPosition;           // 前の座標
	VECTOR oldplayerPos;          // プレイヤーの前回の座標
	VECTOR lightModelPosition;    // ライトモデルの位置

	float mfAngle;               // 現在の回転値
	float mfTargetAngle;        // 目標の回転値
	float playerHeight = 80.0f; // 高さ
	float playerSpeed = 70.0f;  // 速度
	float dis;                  // 距離

	bool isMove;               // 動いているかどうか
	bool isCrouching;         // しゃがんでいるかどうか
	bool isFreeze;            // フリーズさせる
	bool isActiveLight;       // ライトを点けるかどうか

	int lightHandle;        // ライトモデルハンドル
	int stamina = 100;      // スタミナ
	int staminaMAX = 100;   // 最大スタミナ
	int stamina_X = Utility::SCREEN_WIDTH / 2 - 260; // スタミナゲージのX座標
	int stamina_Y = Utility::SCREEN_HEIGHT / 2 + 430; // スタミナゲージのY座標
	int width = 490;       // 幅(スタミナゲージ)
	int height = 30;       // 高さ(スタミナゲージ)
	int gaugeWidth;        // 現在値に応じたゲージの幅
	int mncount;           // ステージの当たり判定用カウント
	int RecordDis;        // プレイヤーの保持した座標の距離
	int AddItemID;        // ID を背一定する変数
	int WalkSETimer;      // 歩いているときのSEを流す時間
	int RunSETimer;       // 走っているときのSEを流す時間
	int FontHandle = CreateFontToHandle(NULL, 40, -1, DX_FONTTYPE_ANTIALIASING);  // 画面に表示するフォントのハンドル


	// アイテム関係変数//
	std::vector<ItemData>items;

	int maxSize = 1;     // インベントリのサイズ

	bool getEscapeItem;       // 脱出アイテムを使用したかどうか
	bool getTimeItem;         // 時間止めアイテムを使用したかどうか
	bool isUseStopItem;    // 時間止めアイテムを使用するかどうか

	// アイテム欄の変数
	int x;                  // アイテムをボックスに表示するためのX座標
	int y;                  // アイテムをボックスに表示するためのY座標
	int selX;               // 選択で表示するX線
	int selY;               // 選択で表示するY線
	int currentItemIndex;   // 現在のアイテムインデックス

	const int ItemSize = 170;   // アイテムの大きさ
	const int ItemMargin = 10; // アイテムの余白
	const int HUD_X = Utility::SCREEN_WIDTH / 2 + 850 - (ItemSize + ItemMargin);  // アイテム画像を表示するためのX座標
	const int HUD_Y = Utility::SCREEN_HEIGHT / 2 + 550 - ItemSize - 20;             // アイテム画像を表示するためのY座標
	//---------------//


	enum MoveState        // 動いている状態の構造体
	{
		WALK,
		RUN
	};

	MoveState state = WALK; // 最初は歩いている状態

	std::vector<VECTOR>playerRecord; // プレイヤーの座標を保持するVECTOR の構造体

	const float ROTATE_SPEED = 0.2f; // 回転速度
	const int ITER = 20;              // 壁に当たる枚数
	const int SEframe_Walk = 25;          // 歩いているときのSEフレーム
	const int SEframe_RUN = 18;      // 走っているときのSEフレーム
	const int Frame_Light = 25;     // ライトの点滅フレーム
};