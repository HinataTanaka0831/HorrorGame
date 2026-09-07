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
	// 入力: fileName(モデルパス), initPosition(初期座標), enemyType(巡回ルート番号), isSeparateAnimation(外部分割モーションフラグ) / 出力: なし / 副作用: Model生成、ScareLight生成
	Enemy3D(std::string fileName, VECTOR initPosition, int enemyType, bool isSeparateAnimation = false);
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
	// 入力: type(ルート番号) / 出力: なし / 副作用: m_enemyTypeの更新
	void SetEnemyType(int type) { m_enemyType = type; } 


	bool GetScare() { return m_isScare; }              

	bool GetfoundPlayer() { return m_foundPlayer; }

	void SetStopItem(bool stop) { m_isStopItem = stop; }

	// モーションデータの追加登録
	// 入力: state(ステート), fileName(モーションパス) / 出力: なし / 副作用: m_modelへのモーション追加
	void AddAnimation(AnimationState state, std::string fileName);


	// 今後必要であれば move や Attack や Jump 関数などを作る

private:
	Model* m_model;  // モデルクラスのポインタ
	VECTOR m_playerPos = VGet(0.0f, 0.0f, 0.0f);

	float m_angle = 0.0f;              // 現在の回転値
	float m_targetAngle = 0.0f;        // 目標の回転値

	// ↓目標地点を設定するための変数
	float m_targetX = 0.0f;              // 目標X座標
	float m_targetY = 0.0f;              // 目標Y座標
	float m_targetZ = 0.0f;              // 目標Z座標
	float m_distanceX = 0.0f;            // 目標座標と敵の座標のX距離
	float m_distanceY = 0.0f;            // 目標座標と敵の座標のY距離
	float m_distanceZ = 0.0f;            // 目標座標と敵の座標のZ距離

	// 敵の移動速度（メートル/秒 などはゲーム設定に合わせて調整可）
	float m_speed = 5.0f;
	// 障害物回避時に横にずらす距離
	float m_avoidanceDist = 0.5f;

	int m_scareLight = 0;

	int m_count = 0;               // カウント変数
	int m_currentPoint = 0;          // 現在の目標座標を示す変数
	int m_pointcount = 28;            // 目標座標の最大数を表す変数
	int m_waitTimer = 0;             // 待機時間
	int m_enemyType = -1;               // 敵のモデルによって目標地点を変更するための変数
	int m_scareTimer = 0;            // ジャンプスケアを行う時間

	bool m_isWait = false;              // 待機状態にするかどうか
	bool m_isback = false;              // 目標地点を全部通ったら
	bool m_isScare = false;
	bool m_foundPlayer = false;         // プレイヤーを見つけたかどうか
	bool m_isRun = false;               // 走るかどうか
	bool m_isStopItem = false;          // 時間止めアイテムを使用したかどうか

	VECTOR m_oldPosition = VGet(0.0f, 0.0f, 0.0f);        // 前の座標
	VECTOR m_moveVec = VGet(0.0f, 0.0f, 0.0f);            // 移動ベクトル
	VECTOR m_currentPos = VGet(0.0f, 0.0f, 0.0f);         // 敵のモデルの位置を設定するため計算したものを保持する変数

	struct Point              // 目標座標を設定するための構造体
	{
		float pointX;        // 目標X座標
		float pointY;        // 目標Y座標
		float pointZ;        // 目標Z座標
	};


	const float RotateSpeed = 0.2f;     // 回転速度

	const float Speed = 6.0f; // 敵の移動スピード
	const int WaitFram = 60;  // 待機するフレーム数
	const int ITER = 20;
};