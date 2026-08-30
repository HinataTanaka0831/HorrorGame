#pragma once

#include "DxLib.h"
#include <string>
#include <vector>
#include "ItemData.h"
#include "Object3D.h"
#include "Utility.h"

class Item;
class EscapeItem;
class TimeItem;

// プレイヤーの移動、スタミナ消費、懐中電灯、しゃがみ、アイテムインベントリおよび敵AI追跡用ログを管理するクラス
class Player3D : public Object3D
{
public:
	// プレイヤー初期化および懐中電灯モデルのロード
	// 入力: initPos(初期スポーンワールド座標) / 出力: なし / 副作用: TagPlayer3D設定、MV1LoadModel実行
	Player3D(VECTOR initPos);                     
	~Player3D() override;                         
			
	// 入力・移動・壁衝突押し出し・ライト同期・足音・スタミナ更新の一括実行
	// 入力: なし / 出力: なし / 副作用: 座標・スタミナ・ライト状態・SE再生の更新
	void Update() override;                       

	// 懐中電灯3DモデルおよびアイテムHUD枠の描画
	// 入力: なし / 出力: なし / 副作用: バックバッファへの描画
	void Draw() override;                         
					
	// カメラ視線方向を基準とした前後左右移動ベクトルの算出およびステージ壁コリジョン押し出し
	// 入力: なし / 出力: なし / 副作用: プレイヤー座標mvPositionの更新
	void MoveEx();                                

	// しゃがみ/直立姿勢の切り替えおよび視線高・移動速度の補正
	// 入力: なし / 出力: なし / 副作用: isCrouchingフラグおよびplayerHeight/playerSpeedの変更
	void PlayerSquat();                        

	// 懐中電灯の点灯トグルおよびカメラ視線へのライトモデル・ディレクショナル光源同期
	// 入力: なし / 出力: なし / 副作用: DXライブラリライト有効無効切り替え
	void HaveLight();                          

	// 周囲の脱出アイテムおよび時間延長アイテムとの近接インタラクション判定
	// 入力: なし / 出力: なし / 副作用: Rキー押下時のアイテム取得
	void ItemCollision();   	               

	// 保持中アイテムの消費実行（時間停止発動または脱出ドア解錠フラグセット）
	// 入力: なし / 出力: なし / 副作用: インベントリからの消費および各ギミック状態の更新
	void UseItem();

	// アイテムスロット枠および所持アイテムアイコンのHUD描画
	// 入力: なし / 出力: なし / 副作用: バックバッファへのUI描画
	void ItemBox();                         	

	// インベントリへのアイテム追加
	// 入力: item(取得アイテムデータ) / 出力: 格納成功ならtrue / 副作用: items配列へのpush_back
	bool AddItem(const ItemData item);   	   

	// インベントリからの先頭アイテム削除
	// 入力: なし / 出力: なし / 副作用: items配列のpop
	void LoseItem();                           

	// スタミナゲージバーの画面描画
	// 入力: なし / 出力: なし / 副作用: バックバッファへのHUD描画
	void DrawStamina();

	// ダッシュ入力時のスタミナ消費および非ダッシュ時の自然回復処理
	// 入力: isKeyProssese(シフトキー押下中か) / 出力: 走行可能状態ならtrue / 副作用: stamina現在値の増減
	bool StaminaUpdate(bool isKeyProssese);    

	bool GetCrouching() { return isCrouching; }

	float GetHeight() { return playerHeight; } 

	void SetFreeze(bool freeze) { isFreeze = freeze; } 

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

	VECTOR DoorPos;                                       // 脱出口の座標を取得するための変数
	VECTOR oldPosition;                                   // 前の座標
	VECTOR oldPlayerPosition;                             // プレイヤーの前回の座標
	VECTOR lightModelPosition;                            // ライトモデルの位置
							                            
	float mfAngle;                                       // 現在の回転値
	float mfTargetAngle;                                 // 目標の回転値
	float playerHeight = 80.0f;                          // 高さ
	float playerSpeed = 70.0f;                           // 速度
	float dis;                                           // 距離
								                         
	bool isMove;                                         // 動いているかどうか
	bool isCrouching;                                   // しゃがんでいるかどうか
	bool isFreeze;                                      // フリーズさせる
	bool isActiveLight;                                 // ライトを点けるかどうか

	int lightHandle;                                     // ライトモデルハンドル
	int stamina = 100;                                   // スタミナ
	int staminaMAX = 100;                                // 最大スタミナ
	int stamina_X = Utility::SCREEN_WIDTH / 2 - 260;    // スタミナゲージのX座標
	int stamina_Y = Utility::SCREEN_HEIGHT / 2 + 430;   // スタミナゲージのY座標
	int width = 490;                                    // 幅(スタミナゲージ)
	int height = 30;                                    // 高さ(スタミナゲージ)
	int gaugeWidth;                                     // 現在値に応じたゲージの幅
	int mncount;                                        // ステージの当たり判定用カウント
	int RecordDis;                                     // プレイヤーの保持した座標の距離
	int AddItemID;                                     // ID を背一定する変数
	int WalkSETimer;                                   // 歩いているときのSEを流す時間
	int RunSETimer;                                    // 走っているときのSEを流す時間
	int FontHandle = CreateFontToHandle(NULL, 40, -1, DX_FONTTYPE_ANTIALIASING);  // 画面に表示するフォントのハンドル


	std::vector<ItemData>items;             // アイテム関係変数//

	int maxSize = 1;                        // インベントリのサイズ

	bool getEscapeItem;                     // 脱出アイテムを使用したかどうか
	bool getTimeItem;                       // 時間止めアイテムを使用したかどうか
	bool isUseStopItem;                     // 時間止めアイテムを使用するかどうか

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