#pragma once
#include "DxLib.h"
#include <string>
#include "Item.h"
#include "Model.h"
#include "ItemData.h"

// 使用時に全敵AIの活動を20秒間停止させる砂時計アイテムクラス
class TimeItem : public Item
{
public:
	ItemData stoptime = { 2, LoadGraph("Resource/UI/ui_timeItem.png"), "時間停止" }; // TimeItem のデータ

public:
	// 時間停止アイテム3DモデルのロードおよびHUDフォント初期化
	// 入力: fileName(モデルパス), initPosition(配置座標), isSeparateAnimation(分割モーションフラグ) / 出力: なし / 副作用: Model生成、TagTimeItem設定
	TimeItem(std::string fileName, VECTOR initPosition, bool isSeparateAnimation = false);
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
	Model* m_model;

	static int m_waitTimer;           // 停止タイマー（フレーム単位でカウントアップ）
	static bool m_isTimerActive;      // タイマーが動作中かどうか
	static int m_lastProcessedFrame;  // 最後に処理したフレーム番号（1フレーム1回処理用）

	// 20秒間 × 60FPS = 1200フレーム
	static const int WaitFrame = 1200;  // 20秒間（60FPS × 20秒）

	// 残り時間表示用のフォントハンドル
	static int m_timerFontHandle;

};