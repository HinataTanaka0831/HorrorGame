#pragma once

#include "SelectScene.h"


class GameScene : public SelectScene
{


public:
	// コンストラクタ
	GameScene();
	// デストラクタ
	virtual~GameScene();

	// 初期化
	virtual void Initialize() override;
	// 更新
	virtual void Update() override;
	// 描画
	virtual void Draw() override;
	// 終了処理
	virtual void Finalize() override;




private:
	char font[32];
};
