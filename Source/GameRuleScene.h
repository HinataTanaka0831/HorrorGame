#pragma once

#include "Scene.h"
#include "Utility.h"
#include <memory>

class Button;

class GameRuleScene : public Scene
{
public:
	// コンストラクタ
	GameRuleScene();
	// デストラクタ
	~GameRuleScene();
	// 初期化
	void Initialize() override;
	// 更新
	void Update() override;
	// 描画
	void Draw() override;
	// 終了処理
	void Finalize() override;

private:
	std::unique_ptr<Button> mpBackButton = nullptr;
	const int StringX = Utility::SCREEN_WIDTH / 2 - 150;
	const int BackY = 900;
	int noise;
	int keyHandle = -1;        // 移動キーハンドル
	int key_R_Handle = -1;     // Rキーハンドル
	int key_F_Handle = -1;     // Fキーハンドル
	int key_E_Handle = -1;     // Eキーハンドル
	int mouseHandle = -1;      // マウス左クリックハンドル
	int mouseMoveHandle = -1;  // マウス移動ハンドル
};