#pragma once
#include "DxLib.h"
#include "Texture.h"
#include <string>
#include "Object2D.h"
#include "Player.h"

class Player;



class EnemySalt : public Object2D
{
public:
	// コンストラクタ
	EnemySalt(VECTOR initPos, std::string filename, int t);

	//デストラクタ
	virtual ~EnemySalt();

	// 更新
	void Update() override;

	// 描画
	void Draw() override;

	// 移動処理
	void Move();

	// 画面外に出たら
	bool IsScreenOut();

	//// 初期化
	void Initialize();

	// ダメージ処理
	void Damage(int damage);


public:
	void Settype(int a) { type = a; }


private:
	VECTOR mvDirection;  // 移動方向
	int StartTime = GetNowCount();  // 時間指定
	static const int MOVE_SPEED = 5;
	int time = 0;
	int type;
	int mpHp = 6;  // HP(Enemyでしか使用しない変数)
	Player* lose;

};
