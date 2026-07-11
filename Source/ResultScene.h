#pragma once

#include "Scene.h"

class Button; // 前方宣言

class ResultScene : public Scene
{
public:
	enum {
		select_None,
		select_Retory,
		select_Title,

		select_Now4,
	};


public:
	// コンストラクタ
	ResultScene();
	// デストラクタ
	~ResultScene();

	// 初期化
	void Initialize() override;
	// 更新
	void Update() override;
	// 描画
	void Draw() override;
	// 終了処理
	void Finalize() override;

private:
	Button* mpRetoryBtn = nullptr;  // リトライボタン
	Button* mpTitleBtn = nullptr;   // タイトルボタン

	int StringX = Utility::SCREEN_WIDTH / 2 - 150;	   // 文字列のX座標
	int StringX_RightDown = StringX + 250;			   // 文字列の右下X座標
	int StringY_RightDown = Retory_Y + 60;			   // 文字列の右下Y座標（リトライボタン）
	int PRStringY_RightDown = Title_Y + 60;			  // 文字列の右下Y座標（タイトルボタン）
	int NowSelect = select_None;                    // 現在選択されているボタン

	RECT RetoryBtn = { StringX, Retory_Y - 10, StringX_RightDown, StringY_RightDown };     // リトライボタンの選択範囲
	RECT TitleBtn = { StringX, Title_Y - 10, StringX_RightDown, PRStringY_RightDown };     // タイトルボタンの選択範囲

	static const int Retory_Y = 600;                 //リトライボタンの高さ
	static const int Title_Y = 750;                  // タイトルボタンの高さ


};
