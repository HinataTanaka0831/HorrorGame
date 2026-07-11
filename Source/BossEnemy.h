#pragma once
#include "DxLib.h"
#include "Texture.h"
#include "Object2D.h"

class Player;

class BossEnemy : public Object2D
{
public:
	// コンストラクタ
	BossEnemy(VECTOR initPos);
	// デストラクタ
	~BossEnemy();

	// 更新
	void Update();
	// 描画
	void Draw();
	// 移動処理
	void Move();
	// ダメージ処理
	void BDamage(int damage);
	// 弾の発射処理
	void Shot();

public:
	// 弾の作成
	void CreateBullet(VECTOR initPos,VECTOR Dire, float speed);
	// 弾の処理
	void RandomBullet();

private:      // メンバ変数
	static const int MOVE_SPEED = 6;  // 移動スピード
	VECTOR mvDirection;  // 移動方向
	// 弾を打つ間隔カウンタ
	int mnShotCounter;
	static const int BULLET_MAX = 10;   // 弾の個数
	static const int Boss_SHOT_INTERVAL = 10;   // 弾を打てる間隔（単位：フレーム）
	int bHp = 30;
};