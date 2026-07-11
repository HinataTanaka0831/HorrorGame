#pragma once
#include "DxLib.h"
#include "Texture.h"
#include <string>
#include "Object2D.h"

class Player;



class Bullet : public Object2D
{


public:
	// コンストラクタ
	Bullet(VECTOR initPos, std::string filename, int sc);

	// デストラクタ
	virtual ~Bullet();

	// 更新
	virtual void Update() override;

	// 描画
	virtual void Draw() override;

	// 移動処理
	void Move();

	// 画面外に出たかどうか
	bool IsScreenOut();

	// 当たり判定
	void CalcCollision();

	// プレイヤーのダメージを分岐させる変数を設定 //
	void SetChange(int c) { change = c; }



protected:
	VECTOR mvDirection;  // 移動方向
	float mfAngle;       // 現在の目標角度
	float mfspeed;
	int change; // ダメージを分岐させるための変数 //
	int Attack = 1;
};
