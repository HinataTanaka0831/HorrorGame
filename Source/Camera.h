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
	// 入力: なし / 出力: なし / 副作用: m_horizontalAngle/m_verticalAngleの更新
	void UpdateRotation(); 

	// ジャンプスケア時等の視点操作固定フラグ設定
	// 入力: isFreeze(固定フラグ) / 出力: なし / 副作用: m_isFreezeの更新
	void SetFreeze(bool isFreeze) { m_isFreeze = isFreeze; }       

	// 正弦波衰減関数による画面振動オフセットの計算
	// 入力: なし / 出力: なし / 副作用: m_shakePositionの更新
	void Shake();                                

	// 画面揺れ演出パラメータ（継続時間・振幅・周期速度）の設定
	// 入力: time(継続秒), width(振幅), angleSpeed(角速度), stepTime(更新刻み) / 出力: なし / 副作用: 揺れカウンタのリセット
	void SetUpShake(float time, float width, float angleSpeed, float stepTime = 1.0f); 

	// 座標取得
	VECTOR GetPosition() { return m_position; } 

	// 注視点取得
	VECTOR GetLookAtPosition() { return m_lookAtPosition; } 



private:
	float m_horizontalAngle = DX_PI_F / 1.2f;  // ヨー角（水平回転角度・ラジアン）
	float m_verticalAngle = 0.0f;    // ピッチ角（垂直見渡し角度・ラジアン）

	//↓ 画面揺れで使用する変数
	float m_shakeAngle = 0.0f;        // 画面揺れ正弦波用アングル
	float m_shakeTimeCounter = 0.0f;  // 画面揺れ経過時間
	float m_shakeTime = 0.0f;         // 画面揺れ総継続時間
	float m_shakeWidth = 0.0f;        // 画面揺れ振幅
	float m_shakeAngleSpeed = 0.0f;   // 画面揺れ振動周波数
	float m_stepTime = 0.0f;          // 画面揺れ更新刻み時間

	bool m_isFreeze = false;             // 視点操作拘束フラグ

	VECTOR m_position = VGet(0.0f, 0.0f, 0.0f);         // カメラワールド座標
	VECTOR m_lookAtPosition = VGet(0.0f, 0.0f, 0.0f);   // カメラ注視点ワールド座標
	VECTOR m_shakePosition = VGet(0.0f, 0.0f, 0.0f);                             // 画面揺れによる加算オフセット

	Object3D* m_target = nullptr;                       // 追従対象（プレイヤーオブジェクト）
};