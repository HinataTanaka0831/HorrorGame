#pragma once

#include "DxLib.h"
#include <string>
#include <vector>
#include "Object3D.h"
#include "Collision.h"
#include "ModelUtility.h"

// 前方宣言
class Player3D;
class Model;

// 徘徊巡回・視界索敵・プレイヤー追跡および捕獲時ジャンプスケア演出を実行する3D敵AIクラス
class Enemy3D : public Object3D
{
public:
	// 敵AIモデルのロード、パトロールルート初期化、ジャンプスケア用光源の生成
	// 入力: fileName(モデルパス), initPosition(初期座標), enemyType(巡回ルート番号), isSeparateAnimation(外部分割モーションフラグ) / 出力: なし / 副作用: Model生成、ScareLight生成
	Enemy3D(std::string fileName, VECTOR initPosition, int enemyType, bool isSeparateAnimation = false);

	// ジャンプすケア用のライトやポインタを解放
	// 入力: なし / 出力: なし / 副作用: 生成したライトハンドルやModelクラスのポインタを解放
	~Enemy3D() override;

	// 索敵・移動・旋回・ステージ壁押し出し・ジャンプスケア発動の一括更新
	// 入力: なし / 出力: なし / 副作用: 敵座標mvPosition、アニメーションステートの更新
	void Update() override;

	// 敵3Dモデルの描画
	// 入力: なし / 出力: なし / 副作用: バックバッファへの描画
	void Draw() override;

	// 敵が動けるかどうかを判定
    // 入力: なし / 出力: スケア中や時間停止中ならfalse / 副作用: 敵の移動の可否
	bool CanMove();

	// 高低差と視界判定でプレイヤーを捜索
	// 入力: なし / 出力: なし / 副作用: m_foundPlayer を更新
	void UpdatePlayerSearch();

	// プレイヤーの追跡
	// 入力: なし / 出力: なし / 副作用: 近ければ直接追跡、遠ければ足跡をたどるようにして移動方向と速度を更新
	void ChasePlayer();

	// CSVファイル情報の追加を行う
	// 入力: なし / 出力: なし / 副作用: CsvLoader で読み込んだCSVファイルのデータを構造体(WayPoint)に格納
	void WayPoint();

	// ステージの目標地点を巡回する
	// 入力: なし / 出力: なし / 副作用: WayPoint() の情報を用いて目標地点まで m_moveVec 、m_position を更新
	void Patrol();

	// 移動による更新
    // 入力: なし / 出力: なし / 副作用: 移動状態に応じて m_moveVec の正規化、m_targetAngle とアニメーションの更新
	void UpdateByMove();

	// パトロールウェイポイント巡回またはプレイヤー追跡による移動ベクトル算出
	// 入力: なし / 出力: なし / 副作用: mvPositionの更新およびアニメーション切り替え
	void Move();

	// 進行方向ベクトルに向けた滑らかなY軸旋回補間
	// 入力: なし / 出力: なし / 副作用: 敵モデルの回転角度更新
	void RotationByMove();

	// プレイヤー捕獲時のカメラ強制注視、赤色ライティング演出およびゲームオーバーシーン遷移
	// 入力: なし / 出力: なし / 副作用: カメラFreeze・Shake、ScareLight有効化、シーン遷移予約
	void CollScare();

public:
	// プレイヤークラスのポインタを設定
	Player3D* SetPlayer(Player3D* pPlayer) { m_player = pPlayer; }
	// ジャンプスケアのフラグを取得
	bool GetScare() const { return m_isScare; }              
	// プレイヤーの発見フラグを取得
	bool GetfoundPlayer() const { return m_foundPlayer; }
	// 時間止めアイテム使用フラグを設定
	void SetStopItem(bool stop) { m_isStopItem = stop; }
	// モーションデータの追加登録
	// 入力: state(ステート), fileName(モーションパス) / 出力: なし / 副作用: m_modelへのモーション追加
	void AddAnimation(AnimationState state, std::string fileName);


private:
	// 定数
	const float RotateSpeed;                             // 回転速度
	const int WaitFrame;                                  // 待機するフレーム数
	const int MaxPointCount;                             // 目標座標の最大数を表す変数

	// Transform情報
	VECTOR m_oldPosition;                                // 前の座標
	float m_angle;                                       // 現在の回転値
	float m_targetAngle;                                 // 目標の回転値

	// 移動・捜索情報
	float m_speed;                                         // 敵の移動スピード
	VECTOR m_moveVec;                                    // 移動ベクトル
	VECTOR m_playerPosition;                             // プレイヤーの座標を取得するためのベクトル
	VECTOR toPlayer;                                     // 敵からプレイヤーまでのベクトル
	float m_smoothHeight;                                // 滑らかな高さを示す変数
	float distanceToPlayer;                              // プレイヤーまでの長さ
	float m_targetX;                                     // 目標X座標
	float m_targetY;                                     // 目標Y座標
	float m_targetZ;                                     // 目標Z座標
	float m_distanceX;                                   // 目標座標と敵の座標のX距離
	float m_distanceY;                                   // 目標座標と敵の座標のY距離
	float m_distanceZ;                                   // 目標座標と敵の座標のZ距離
	int m_currentPoint;                                  // 現在の目標座標を示す変数
	int m_waitTimer;                                     // 待機時間
	size_t m_trackIndex;                                 // 足跡のインデックスを追跡するための変数
	struct WayPoints                                     // 目標座標を設定するための構造体
	{
		float pointX = 0.0f;                             // 目標X座標
		float pointY = 0.0f;                             // 目標Y座標
		float pointZ = 0.0f;                             // 目標Z座標
	};

	// ハンドル関係
	int m_scareLightHandle;                              // ジャンプスケア用のライトハンドル

	// ジャンプスケア関係
	bool m_foundPlayer;                                  // プレイヤーを見つけたかどうか
	bool m_isScare;                                      // ジャンプスケアを行うかどうか
	int m_scareTimer;                                    // ジャンプスケアを行う時間

	// 状態関係
	bool m_isRun;                                        // 走るかどうか
	bool m_isWait;                                       // 待機状態にするかどうか
	bool m_isback;                                       // 目標地点を全部通ったら
	bool m_isStopItem;                                   // 時間止めアイテムを使用したかどうか

	// プレイヤー関係
	Player3D* m_player;                                  // プレイヤークラスのポインタ

	// モデル関係
	Model* m_model;                                      // モデルクラスのポインタ
	int m_enemyType;                                     // 複数の敵を異なる目標地点に移動させるための変数

	// コンテナ関係
	std::vector<std::vector<std::string>> m_wayPointCsv; // ウェイポイントのコンテナを格納する
	std::vector<VECTOR> m_wayPointA;                     // ウェイポイントAのコンテナ
	std::vector<VECTOR> m_wayPointB;                     // ウェイポイントBのコンテナ
};