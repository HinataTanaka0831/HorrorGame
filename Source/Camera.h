#pragma once

#include "DxLib.h"
#include "Utility.h"

class Object3D;

// FPS視点の追従カメラ、マウス入力による水平/垂直回転、ジャンプスケア時画面揺れを制御するクラス
class Camera
{
public:
	Camera();   // コンストラクタ
	~Camera();  // デストラクタ

	// カメラクリップ距離・背景色・初期注視点の初期設定
	// 入力: なし / 出力: なし / 副作用: SetCameraNearFar, SetBackgroundColor実行
	void Initialize();      

	// 追従ターゲット（プレイヤー）座標の取得、しゃがみ時の視線高調整、注視点算出およびDXライブラリカメラへの反映
	// 入力: なし / 出力: なし / 副作用: SetCameraPositionAndTarget_UpVecY実行
	void Update();          

	// マウス変位量に応じた視点回転角の加算およびピッチ角（上下見渡し）の上下90度制限
	// 入力: なし / 出力: なし / 副作用: mfHorizontalAngle/mfVerticalAngleの更新
	void UpdateRotation(); 

	// ジャンプスケア時等の視点操作固定フラグ設定
	// 入力: f(固定フラグ) / 出力: なし / 副作用: isFreezeの更新
	void SetFreeze(bool f) { isFreeze = f; }       

	// 正弦波衰減関数による画面振動オフセットの計算
	// 入力: なし / 出力: なし / 副作用: mvShakePositionの更新
	void Shake();                                

	// 画面揺れ演出パラメータ（継続時間・振幅・周期速度）の設定
	// 入力: time(継続秒), width(振幅), anglespeed(角速度), stepTime(更新刻み) / 出力: なし / 副作用: 揺れカウンタのリセット
	void SetUpShake(float time, float width, float anglespeed, float stepTime = 1.0f); 

	// 座標取得
	VECTOR GetPosition() { return mvPosition; } 

	// 注視点取得
	VECTOR GetLookAtPosition() { return mvLookAtPosition; } 



private:
	float mfHorizontalAngle;  // ヨー角（水平回転角度・ラジアン）
	float mfVerticalAngle;    // ピッチ角（垂直見渡し角度・ラジアン）

	//↓ 画面揺れで使用する変数
	float mfShakeAngle;        // 画面揺れ正弦波用アングル
	float mfShakeTimeCounter;  // 画面揺れ経過時間
	float mfShakeTime;         // 画面揺れ総継続時間
	float mfShakeWidth;        // 画面揺れ振幅
	float mfShakeAngleSpeed;   // 画面揺れ振動周波数
	float mfStepTime;

	bool isFreeze;             // 視点操作拘束フラグ

	VECTOR mvPosition;         // カメラワールド座標
	VECTOR mvLookAtPosition;   // カメラ注視点ワールド座標
	VECTOR mvShakePosition;    // 画面揺れによる加算オフセット

	Object3D* mpTarget;        // 追従対象（プレイヤーオブジェクト）
};