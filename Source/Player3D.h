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

	bool GetCrouching() const { return isCrouching; }
	float GetHeight() const { return playerHeight; }
	void SetFreeze(bool freeze) { isFreeze = freeze; }
	void SetUseStopItem(bool stop) { isUseStopItem = stop; }
	bool GetEscapeItem() const { return getEscapeItem; }
	bool GetTimeItem() const { return getTimeItem; }
	bool GetIsUseStopItem() const { return isUseStopItem; }

	std::vector<VECTOR>& GetPlayerRecord() { return playerRecord; }

private:
	struct Player {
		VECTOR pos;
		float radius;
	};

	VECTOR DoorPos;
	VECTOR oldPosition;
	VECTOR oldPlayerPosition;
	VECTOR lightModelPosition;

	float mfAngle;
	float mfTargetAngle;
	float playerHeight = 80.0f;
	float playerSpeed = 70.0f;
	float dis;

	bool isMove;
	bool isCrouching;
	bool isFreeze;
	bool isActiveLight;

	int lightHandle;
	int stamina = 100;
	int staminaMAX = 100;
	int stamina_X = Utility::SCREEN_WIDTH / 2 - 260;
	int stamina_Y = Utility::SCREEN_HEIGHT / 2 + 430;
	int width = 490;
	int height = 30;
	int gaugeWidth;
	int mncount;
	int RecordDis;
	int AddItemID;
	int WalkSETimer;
	int RunSETimer;
	int FontHandle = CreateFontToHandle(NULL, 40, -1, DX_FONTTYPE_ANTIALIASING);

	std::vector<ItemData> items;
	int maxSize = 1;

	bool getEscapeItem;
	bool getTimeItem;
	bool isUseStopItem;

	int x;
	int y;
	int selX;
	int selY;
	int currentItemIndex;

	const int ItemSize = 170;
	const int ItemMargin = 10;
	const int HUD_X = Utility::SCREEN_WIDTH / 2 + 850 - (ItemSize + ItemMargin);
	const int HUD_Y = Utility::SCREEN_HEIGHT / 2 + 550 - ItemSize - 20;

	enum MoveState
	{
		WALK,
		RUN
	};

	MoveState state = WALK;
	std::vector<VECTOR> playerRecord;

	const float ROTATE_SPEED = 0.2f;
	const int ITER = 20;
	const int SEframe_Walk = 25;
	const int SEframe_RUN = 18;
	const int Frame_Light = 25;
};