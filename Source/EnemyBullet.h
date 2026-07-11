#pragma once
#include "DxLib.h"
#include "Texture.h"
#include <string>
#include "Object2D.h"

class EnemyBullet : public Object2D
{
public:
	// コンストラクタ
	EnemyBullet(VECTOR initPos, std::string filename);

	// デストラクタ
	~EnemyBullet();

	// 更新
	void Update() override;

	// 描画
	void Draw() override;

	// 移動処理
	void Move();

	// 画面外に出たら
	bool IsScreenOut();

	// 当たり判定
	void CalcCollision();

	// 進行方向
	void SetDirection(VECTOR dir) { mvDirection = dir; }

	// 速度設定
	void SetSpeed(float speed) { mfspeed = speed; }


};