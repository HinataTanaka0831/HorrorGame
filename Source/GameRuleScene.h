#pragma once

#include "Scene.h"

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
	int noise;
	int keyHandle = -1;        // 移動キーハンドル
	int key_R_Handle = -1;     // Rキーハンドル
	int key_F_Handle = -1;     // Fキーハンドル
	int key_E_Handle = -1;     // Eキーハンドル
	int mouseHandle = -1;      // マウス左クリックハンドル
	int mouseMoveHandle = -1;  // マウス移動ハンドル
};