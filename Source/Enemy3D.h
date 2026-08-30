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

	bool GetScare() const { return isScare; }
	bool GetfoundPlayer() const { return foundPlayer; }
	void SetStopItem(bool stop) { isStopItem = stop; }

	// モーションデータの追加登録
	// 入力: state(ステート), filename(モーションパス) / 出力: なし / 副作用: mpModelへのモーション追加
	void AddAnimation(AnimationState state, std::string filename);

private:
	Model* mpModel;
	VECTOR playerPos;

	float mfAngle;
	float mfTargetAngle;

	float TargetX;
	float TargetY;
	float TargetZ;
	float dx;
	float dy;
	float dz;

	float mSpeed = 5.0f;
	float mAvoidanceDist = 0.5f;

	int ScareLight;
	int mncount;
	int currentPoint;
	int pointcount;
	int waitTimer;
	int enetype;
	int scareTimer;

	bool isWait;
	bool isback;
	bool isScare;
	bool foundPlayer;
	bool isRun;
	bool isStopItem;

	VECTOR oldPosition;
	VECTOR moveVec;
	VECTOR currentPos;

	struct Point
	{
		float pointX;
		float pointY;
		float pointZ;
	};

	const float ROTATE_SPEED = 0.2f;
	const float mnSpeed = 6.0f;
	const int waitFram = 60;
	const int ITER = 20;
};