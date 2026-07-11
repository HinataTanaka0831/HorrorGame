#pragma once
#include "DxLib.h"
#include "Texture.h"
#include <string>
#include "Object2D.h"


class Enemy : public Object2D
{
public:
	// コンストラクタ
	Enemy(VECTOR initPos, std::string filename, int t,int s);

	//デストラクタ
	virtual ~Enemy();

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
	void ChangeDamage(int damage);  // enemytype 1の敵を倒したら //
	void ChangeDamage2(int damage); // enemytype 2の敵を倒したら //


public:

	void Settype(int a) { type = a; }

	// 敵の種類を分岐する変数を設定 //
	void SetEnemytype(int set) { enemytype = set; }

	int GetScore() { return score; }


private:
	VECTOR mvDirection;  // 移動方向
	int StartTime = GetNowCount();  // 時間指定
	static const int MOVE_SPEED = 4;
	int time = 0;
	int type;
	int mnHp = 4;  // HP(Enemyでしか使用しない変数)
	int enemytype; // 敵の種類を変更するための変数 //

};


