#pragma once

#include "DxLib.h"
#include <string>
#include <vector>
#include "Object3D.h"
#include "Model.h"

// 徘徊巡回・視界索敵・プレイヤー追跡および捕獲時ジャンプスケア演出を実行する3D敵AIクラス
class Enemy3D : public Object3D
{
public:
	// 敵AIモデルのロード、パトロールルート初期化、ジャンプスケア用光源の生成
	// 入力: filename(モデルパス), initPos(初期座標), enemytype(巡回ルート番号), isSeparateAnim(外部分割モーションフラグ) / 出力: なし / 副作用: Model生成、ScareLight生成
	Enemy3D(std::string filename, VECTOR initPos, int enemytype, bool isSeparateAnim = false);
	~Enemy3D() override;

	// 索敵・移動・旋回・ステージ壁押し出し・ジャンプスケア発動の一括更新
	// 入力: なし / 出力: なし / 副作用: 敵座標mvPosition、アニメーションステートの更新
	void Update() override;

	// 敵3Dモデルの描画
	// 入力: なし / 出力: なし / 副作用: バックバッファへの描画
	void Draw() override;

	// パトロールウェイポイント巡回またはプレイヤー追跡による移動ベクトル算出
	// 入力: なし / 出力: なし / 副作用: mvPositionの更新およびアニメーション切り替え
	void Move();             

	// 進行方向ベクトルに向けた滑らかなY軸旋回補間
	// 入力: なし / 出力: なし / 副作用: 敵モデルの回転角度更新
	void RotationByMove();   

	// プレイヤー捕獲時のカメラ強制注視、赤色ライティング演出およびゲームオーバーシーン遷移
	// 入力: なし / 出力: なし / 副作用: カメラFreeze・Shake、ScareLight有効化、シーン遷移予約
	void CollScare();        

	// 巡回パトロールルートの初期設定
	// 入力: type(ルート番号) / 出力: なし / 副作用: enetypeの更新
	void SetEnemyType(int type) { enetype = type; } 


	bool GetScare() { return isScare; }              

	bool GetfoundPlayer() { return foundPlayer; }

	void SetStopItem(bool stop) { isStopItem = stop; }

	// モーションデータの追加登録
	// 入力: state(ステート), filename(モーションパス) / 出力: なし / 副作用: mpModelへのモーション追加
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