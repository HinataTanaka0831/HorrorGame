#pragma once
#include "DxLib.h"
#include <string>
#include "Item.h"
#include "Model.h"
#include "ItemData.h"


class TimeItem : public Item
{
public:
	ItemData stoptime = { 2, LoadGraph("Resource/3D_UI/TimeItem.png"), "時間停止" }; // TimeItem のデータ

public:
	// コンストラクタ
	TimeItem(std::string filename, VECTOR initPos, bool isSeparateAnim = false);
	
	// デストラクタ
	~TimeItem() override;

	// 更新
	void Update() override;

	// 描画
	void Draw() override;

	// 使用したら敵の動きを停止する処理
	void StopTime();

	// 残り時間を画面に表示する処理
	void DrawTimer();

private:
	Model* mpModel;

	// =====================================================
	// static変数（全てのTimeItemインスタンスで共有する）
	// → TimeItemが複数あっても、タイマーは1つだけ動く
	// =====================================================
	static int waitTimer;           // 停止タイマー（フレーム単位でカウントアップ）
	static bool isTimerActive;      // タイマーが動作中かどうか
	static int lastProcessedFrame;  // 最後に処理したフレーム番号（1フレーム1回処理用）

	// 20秒間 × 60FPS = 1200フレーム
	static const int WaitFrame = 1200;

	// 残り時間表示用のフォントハンドル
	static int timerFontHandle;

};