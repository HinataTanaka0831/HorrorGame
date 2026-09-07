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
	// 入力: initPosition(初期スポーンワールド座標) / 出力: なし / 副作用: TagPlayer3D設定、MV1LoadModel実行
	Player3D(VECTOR initPosition);                     
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

	bool GetCrouching() { return m_isCrouching; }

	float GetHeight() { return m_playerHeight; } 

	void SetFreeze(bool freeze) { m_isFreeze = freeze; } 

	void SetUseStopItem(bool stop) { m_isUseStopItem = stop; }

	bool GetEscapeItem() const { return m_getEscapeItem; }
	bool GetTimeItem() const { return m_getTimeItem; }
	bool GetIsUseStopItem() { return m_isUseStopItem; }


	std::vector<VECTOR>& GetPlayerRecord() { return m_playerRecord; } // プレイヤーの座標を保持する構造体を取得

private:
	struct Player {
		VECTOR position;      // 中心座標
		float radius;    // 当たり判定の半径
	};

	VECTOR m_doorPos = VGet(0.0f, 0.0f, 0.0f);              // 脱出口の座標を取得するための変数
	VECTOR m_oldPosition = VGet(0.0f, 0.0f, 0.0f);          // 前の座標
	VECTOR m_oldPlayerPosition = m_position;                // プレイヤーの前回の座標
	VECTOR m_lightModelPosition = VGet(0.0f, 0.0f, 0.0f);   // ライトモデルの位置
							                            
	float m_angle = 0.0f;                                   // 現在の回転値
	float m_targetAngle = 0.0f;                             // 目標の回転値
	float m_playerHeight = 80.0f;                           // 高さ
	float m_speed = 70.0f;                                  // 速度
	float m_distance = 0.0f;                                // 距離
								                         
	bool m_isMove = false;                                  // 動いているかどうか
	bool m_isCrouching = false;                             // しゃがんでいるかどうか
	bool m_isFreeze = false;                                // フリーズさせる
	bool m_isActiveLight = true;                            // ライトを点けるかどうか
	int m_lightHandle = -1;                                 // ライトモデルハンドル
	int m_stamina = 100;                                    // スタミナ
	const int MaxStamina = 100;                                 // 最大スタミナ
	int m_staminaX = Utility::SCREEN_WIDTH / 2 - 260;       // スタミナゲージのX座標
	int m_staminaY = Utility::SCREEN_HEIGHT / 2 + 430;      // スタミナゲージのY座標
	int m_width = 490;                                      // 幅(スタミナゲージ)
	int m_height = 30;                                      // 高さ(スタミナゲージ)
	int m_gaugeWidth = 0;                                   // 現在値に応じたゲージの幅
	int m_count = 0;                                        // ステージの当たり判定用カウント
	int m_recordDis = 0;                                    // プレイヤーの保持した座標の距離
	int m_addItemID = 0;                                    // ID を背一定する変数
	int m_walkSETimer = 0;                                  // 歩いているときのSEを流す時間
	int m_runSETimer = 0;                                   // 走っているときのSEを流す時間
	int m_fontHandle = CreateFontToHandle(NULL, 40, -1, DX_FONTTYPE_ANTIALIASING);  // 画面に表示するフォントのハンドル


	std::vector<ItemData>m_items;             // アイテム関係変数

	bool m_getEscapeItem = false;                     // 脱出アイテムを所持しているかどうか
	bool m_getTimeItem = false;                       // 時間止めアイテムを所持しているかどうか
	bool m_isUseStopItem = false;                     // 時間止めアイテムを使用するかどうか

	// アイテム欄の変数
	int m_itemBoxX = 0;                  // アイテムをボックスに表示するためのX座標
	int m_itemBoxY = 0;                  // アイテムをボックスに表示するためのY座標
	const int ItemSize = 170;            // アイテムの大きさ
	const int ItemMargin = 10;           // アイテムの余白
	const int DrawX = Utility::SCREEN_WIDTH / 2 + 850 - (ItemSize + ItemMargin);   // アイテム画像を表示するためのX座標
	const int DrawY = Utility::SCREEN_HEIGHT / 2 + 550 - ItemSize - 20;            // アイテム画像を表示するためのY座標
	const int MaxSize = 1;                                                         // インベントリのサイズ

	enum MoveState        // 動いている状態の構造体
	{
		Walk,
		Run
	};

	MoveState m_state = Walk; // 最初は歩いている状態

	std::vector<VECTOR>m_playerRecord; // プレイヤーの座標を保持するVECTOR の構造体

	const float RotsteSpeed = 0.2f;   // 回転速度
	const int ITER = 20;              // 壁に当たる枚数
	const int SEframeWalk = 25;      // 歩いているときのSEフレーム
	const int SEframeRUN = 18;       // 走っているときのSEフレーム
	const int FrameLight = 25;       // ライトの点滅フレーム
};