#pragma once

#include "DxLib.h"
#include <string>
#include <vector>
#include "Object3D.h"
#include "Model.h"


class Enemy3D : public Object3D
{
public:
	// コンストラクタ
	// Mixamo用処理
	// 分割アニメーションを使用するかの設定を追加（デフォルトはfalse（使用しない））
	Enemy3D(std::string filename, VECTOR initPos, int enemytype, bool isSeparateAnim = false);
	// デストラクタ
	~Enemy3D() override;

	void Update() override; // 更新
	void Draw() override;   // 描画

	void Move();             // 移動処理
	void RotationByMove();   // 移動による回転処理
	void CollScare();        // ゲームオーバー処理（ジャンプスケア処理）

	void SetEnemyType(int type) { enetype = type; }  // 敵のモデルによって目標地点を変更する

	bool GetScare() { return isScare; }              // ジャンプスケア処理

	bool GetfoundPlayer() { return foundPlayer; }

	void SetStopItem(bool stop) { isStopItem = stop; }

	// Mixamo用処理
	// アニメーション追加
	// Modelクラスへの橋渡し関数
	void AddAnimation(AnimationState state, std::string filename);


	// 今後必要であれば move や Attack や Jump 関数などを作る

private:
	Model* mpModel;  // モデルクラスのポインタ
	VECTOR playerPos;

	float mfAngle;              // 現在の回転値
	float mfTargetAngle;        // 目標の回転値

	// ↓目標地点を設定するための変数
	float TargetX;              // 目標X座標
	float TargetY;              // 目標Y座標
	float TargetZ;              // 目標Z座標
	float dx;                   // 目標座標と敵の座標のX距離
	float dy;                   // 目標座標と敵の座標のY距離
	float dz;                   // 目標座標と敵の座標のZ距離

	// 敵の移動速度（メートル/秒 などはゲーム設定に合わせて調整可）
	float mSpeed = 5.0f;
	// 障害物回避時に横にずらす距離
	float mAvoidanceDist = 0.5f;

	int ScareLight;

	int mncount;               // カウント変数
	int currentPoint;          // 現在の目標座標を示す変数
	int pointcount;            // 目標座標の最大数を表す変数
	int waitTimer;             // 待機時間
	int enetype;               // 敵のモデルによって目標地点を変更するための変数
	int scareTimer;            // ジャンプスケアを行う時間

	bool isWait;               // 待機状態にするかどうか
	bool isback;               // 目標地点を全部通ったら
	bool isScare;
	bool foundPlayer;         // プレイヤーを見つけたかどうか
	bool isRun;               // 走るかどうか
	bool isStopItem;          // 時間止めアイテムを使用したかどうか

	VECTOR oldPosition;        // 前の座標
	VECTOR moveVec;            // 移動ベクトル
	VECTOR currentPos;         // 敵のモデルの位置を設定するため計算したものを保持する変数

	struct Point              // 目標座標を設定するための構造体
	{
		float pointX;        // 目標X座標
		float pointY;        // 目標Y座標
		float pointZ;        // 目標Z座標
	};


	const float ROTATE_SPEED = 0.2f;     // 回転速度

	const float mnSpeed = 6.0f; // 敵の移動スピード
	const int waitFram = 60;  // 待機するフレーム数
	const int ITER = 20;
};