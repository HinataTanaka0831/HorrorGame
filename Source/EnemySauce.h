#pragma once
#include "DxLib.h"
#include "Texture.h"
#include <string>
#include "Object2D.h"


class EnemySauce : public Object2D
{
public:
	// コンストラクタ
	EnemySauce(VECTOR initPos, std::string filename, int t);

	//デストラクタ
	virtual ~EnemySauce();

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




	// HPの取得・設定処理
	int GetHp() { return mnHp; }
	void SetHp(int hp) { mnHp = hp; }

	void Settype(int a) { type = a; }


private:
	VECTOR mvDirection;  // 移動方向
	int StartTime = GetNowCount();  // 時間指定
	int mnHp;  // HP(Enemyでしか使用しない変数)
	static const int MOVE_SPEED = 5;
	int time = 0;
	int type;
	
};
