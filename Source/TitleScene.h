#pragma once

#include "Scene.h"
#include "Utility.h"

class Button;

class TitleScene : public Scene
{
public:
	enum {
		select_None,
		select_Play,
		select_PlayRule,

		select_Now3,
	};

public:
	// コンストラクタ
	TitleScene();
	// デストラクタ
	~TitleScene();


	// 初期化
	void Initialize() override;
	// 更新
	void Update() override;
	// 描画
	void Draw() override;
	// 終了処理
	void Finalize() override;



private:
	Button* mpPlay = nullptr;
	Button* mpRule = nullptr;
	int Noise;
	int EnemyPictureHandle = -1;
	// ↓マウスで選択肢を選択するための変数
	int MouceX = 0;                                   // マウスのX座標を取得
	int MouceY = 0;                                  // マウスのY座標を取得
	int StringX = Utility::SCREEN_WIDTH / 2 - 150;   // 文字列のX座標
	int StringX_RightDown = StringX + 250;          // 文字列の右下X座標
	int StringY_RightDown = Play_Y + 60;            // 文字列の右下Y座標（プレイボタン）
	int PRStringY_RightDown = PlayRule_Y + 60;     // 文字列の右下Y座標（操作方法ボタン）
	int NowSelect3 = select_None;                  // 現在選択されているボタン


	RECT playBtn = { StringX, Play_Y -10, StringX_RightDown, StringY_RightDown };            // プレイボタンを選択できる範囲
	RECT playRule = { StringX, PlayRule_Y - 10, StringX_RightDown, PRStringY_RightDown };    // 操作方法ボタンを選択できる範囲

	static const int Play_Y = 600;       // プレイボタンの高さ
	static const int PlayRule_Y = 750;   // 操作方法ボタンの高さ


};
