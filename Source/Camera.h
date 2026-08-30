#pragma once

#include "DxLib.h"
#include "Utility.h"

class Object3D;

class Camera
{
public:
	Camera();   // コンストラクタ
	~Camera();  // デストラクタ

	void Initialize();      // 初期化

	void Update();          // 更新

	void UpdateRotation();  // 回転処理

	void SetFreeze(bool f) { isFreeze = f; }        // カメラを動かせなくなるように設定

	void Shake();                                   // 画面揺れの処理

	void SetUpShake(float time, float width, float anglespeed, float stepTime = 1.0f);  // 画面揺れを設定

	VECTOR GetPosition() { return mvPosition; }  // 座標取得
	VECTOR GetLookAtPosition() { return mvLookAtPosition; }  // 注視点取得

	void SetMousePointFlag(bool MP) { NoSetMouse = MP; }    // マウス座標を中央に戻すかどうかを設定


private:
	float mfHorizontalAngle;  // 水平方向アングル
	float mfVerticalAngle;    // 垂直方向アングル

	//↓ 画面揺れで使用する変数
	float mfShakeAngle;        // 画面を揺らすためのアングル
	float mfShakeTimeCounter;  // 画面揺れの時間カウント
	float mfShakeTime;         // 画面揺れの時間
	float mfShakeWidth;        // 画面揺れの幅
	float mfShakeAngleSpeed;   // 画面揺れのスピード
	float mfStepTime;          // 画面揺れの時間を経過させる

	bool isFreeze;            // ジャンプスケアが起きたら止める
	bool NoSetMouse;          // マウス座標を中央に戻すかどうか

	VECTOR mvPosition;        // カメラ座標
	VECTOR mvLookAtPosition;  // カメラの注視点座標
	VECTOR mvShakePosition;

	Object3D* mpTarget;      // カメラを向ける対象

};