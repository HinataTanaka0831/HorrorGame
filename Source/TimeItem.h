#pragma once
#include "DxLib.h"
#include "Item.h"
#include <string>

class Model;

// 使用時に全敵AIの活動を20秒間停止させる砂時計アイテムクラス
class TimeItem : public Item
{
public:
	// 時間停止アイテム3DモデルのロードおよびHUDフォント初期化
	// 入力: filename(モデルパス), initPos(配置座標), isSeparateAnim(分割モーションフラグ) / 出力: なし / 副作用: Model生成、TagTimeItem設定
	TimeItem(std::string filename, VECTOR initPos, bool isSeparateAnim = false);
	~TimeItem() override;

	// タイマー経過による敵AI停止制御およびモデル更新
	// 入力: なし / 出力: なし / 副作用: StopTime実行、3Dモデル更新
	void Update() override;

	// アイテム3Dモデル描画
	// 入力: なし / 出力: なし / 副作用: バックバッファへの描画
	void Draw() override;

	// プレイヤーのアイテム使用を検知し全敵AIの停止フラグを20秒間制御
	// 入力: なし / 出力: なし / 副作用: 全Enemy3DのisStopItem切り替え
	void StopTime();

	// 敵停止効果の残り秒数カウントダウンHUD描画
	// 入力: なし / 出力: なし / 副作用: バックバッファへのUI描画
	void DrawTimer();

private:
	Model* mpModel;

	static int waitTimer;
	static bool isTimerActive;
	static int timerFontHandle;
	static const int WaitFrame = 1200;  // 20秒間（60FPS × 20秒）
};