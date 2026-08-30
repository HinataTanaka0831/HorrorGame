#pragma once

#include "Scene.h"
#include "Utility.h"
#include <memory>

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
	std::unique_ptr<Button> mpPlayButton = nullptr;
	std::unique_ptr<Button> mpPlayRuleButton = nullptr;
	std::unique_ptr<Button> mpQuitButton = nullptr;
	const int StringX = Utility::SCREEN_WIDTH / 2 - 150;
	const int PlayY = 600;        // プレイボタンの高さ
	const int PlayRuleY = 750;	  // 操作方法ボタンの高さ
	const int QuitY = 850;
	int Noise;
	int EnemyPictureHandle = -1;

	int MouceX = 0;                                   // マウスのX座標を取得
	int MouceY = 0;                                  // マウスのY座標を取得

};
