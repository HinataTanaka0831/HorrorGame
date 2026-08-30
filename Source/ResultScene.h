#pragma once

#include "Scene.h"
#include "Utility.h"
#include <memory>

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
	std::unique_ptr<Button> mpRetoryButton = nullptr;  // リトライボタン
	std::unique_ptr<Button> mpTitleButton = nullptr;   // タイトルボタン
	int StringX = Utility::SCREEN_WIDTH / 2 - 150;	   // 文字列のX座標
	static const int RetoryY = 600;                 //リトライボタンの高さ
	static const int TitleY = 750;                  // タイトルボタンの高さ
};
