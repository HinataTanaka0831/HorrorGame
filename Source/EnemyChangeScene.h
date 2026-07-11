#pragma once
#include "SelectScene.h"

class EnemyChangeScene : public SelectScene
{
public:
	// コンストラクタ
	EnemyChangeScene();
	// デストラクタ
	~EnemyChangeScene();

	// 初期化
	virtual void Initialize() override;
	// 更新
	virtual void Update() override;
	// 描画
	virtual void Draw() override;
	// 終了処理
	virtual void Finalize() override;

};