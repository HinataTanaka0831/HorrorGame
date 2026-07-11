#pragma once
#include "DxLib.h"
#include "Texture.h"
#include "Object2D.h"


class Player : public Object2D
{
private:  // Playerクラスで使用する定数の定義
	static const int MOVE_SPEED = 5;    // 移動速度
	static const int BULLET_MAX = 10;   // 弾の個数
	static const int BULLET_SHOT_INTERVAL = 10;   // 弾を打てる間隔（単位：フレーム）

public:
	// コンストラクタ
	Player(VECTOR initPos, int sc);
	// デストラクタ
	virtual ~Player();


	// 更新
	void Update() override;

	// 描画
	void Draw() override;

	// 移動処理
	void Move();

	// 弾の発射処理
	void Shot();

	// ダメージ処理
	void PDamage(int damage);

	// 敵との当たり判定でのプレイヤーダメージ //
	void PCalcdamage();

	// HPの取得
	int GetHp() { return pHp; }

	// 弾を分岐させるための変数を設定 //
	void SetChange(int hg) { chg = hg; }


private:      // メンバ変数
	// 弾を打つ間隔カウンタ
	int mnBulletShotCounter;
	int pHp = 5;  // HP
	int chg; // 弾を分岐させる変数 //
	int Hitframe = 0;  // 当たった時の点滅時間 //

};
