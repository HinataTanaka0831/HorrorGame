#include "Camera.h"
#include <cmath>
#include "Master.h"
#include "ObjectManager.h"
#include "Scene.h"
#include "Player3D.h"
#include "Enemy3D.h"

Camera::Camera()
	: mfHorizontalAngle(DX_PI_F / 1.2f)
	, mfVerticalAngle(0.0f)
	, mvPosition(VGet(0.0f, 0.0f, 0.0f))
	, mvLookAtPosition(VGet(0.0f, 0.0f, 0.0f))
	, mvShakePosition(VGet(0.0f, 0.0f, 0.0f))
	, mpTarget(nullptr)
	, isFreeze(false)
	, NoSetMouse(false)
	, mfShakeAngle(0.0f)
	, mfShakeTimeCounter(0.0f)
	, mfShakeTime(0.0f)
	, mfShakeWidth(0.0f)
	, mfShakeAngleSpeed(0.0f)
	, mfStepTime(1.0f)
{
}

Camera::~Camera()
{
}

// カメラクリップ距離・背景色・初期注視点の初期設定
// 入力: なし / 出力: なし / 副作用: SetCameraNearFar, SetBackgroundColor実行
void Camera::Initialize()
{
	this->mvPosition = VGet(mvPosition.x, mvPosition.y, mvPosition.z);

	// 屋内ステージの近接ポリゴン欠け防止と遠景描画のためクリップ距離を設定
	SetCameraNearFar(20.0f, 50000.0f);
	SetBackgroundColor(128, 128, 128);
	SetCameraPositionAndTarget_UpVecY(mvPosition, mvLookAtPosition);

	Update();
}

// 追従ターゲット（プレイヤー）座標の取得、しゃがみ時の視線高調整、注視点算出およびDXライブラリカメラへの反映
// 入力: なし / 出力: なし / 副作用: SetCameraPositionAndTarget_UpVecY実行
void Camera::Update()
{
	UpdateRotation();

	if (isFreeze)
	{
		Shake();
	}

	if (mpTarget == nullptr)
	{
		mpTarget = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DByTag(Object3D::TagPlayer3D);
	}

	if (mpTarget != nullptr)
	{
		// プレイヤーの頭部高さ（160.0f）にカメラをオフセット配置
		mvPosition = VAdd(mpTarget->GetPosition(), VGet(0.0f, 160.0f, 0.0f));

		Player3D* player = dynamic_cast<Player3D*>(mpTarget);
		if (player != nullptr)
		{
			// しゃがみ中は頭部高さを引き下げて低い視点を再現
			if (player->GetCrouching())
			{
				mvPosition.y -= player->GetHeight();
			}
		}

		// 球面座標系（ヨー・ピッチ）から視線注視点ベクトルを算出
		mvLookAtPosition.x = mvPosition.x + cosf(mfVerticalAngle / 5.0f * DX_PI_F) * sinf(mfHorizontalAngle / 5.0f * DX_PI_F);
		mvLookAtPosition.y = mvPosition.y + sinf(mfVerticalAngle / 5.0f * DX_PI_F);
		mvLookAtPosition.z = mvPosition.z + cosf(mfVerticalAngle / 5.0f * DX_PI_F) * cosf(mfHorizontalAngle / 5.0f * DX_PI_F);

		// 画面揺れオフセットを加算してビュー行列を設定
		SetCameraPositionAndTarget_UpVecY(VAdd(mvPosition, mvShakePosition), VAdd(mvLookAtPosition, mvShakePosition));
	}
}

// マウス変位量に応じた視点回転角の加算およびピッチ角（上下見渡し）の上下90度制限
// 入力: なし / 出力: なし / 副作用: mfHorizontalAngle/mfVerticalAngleの更新
void Camera::UpdateRotation()
{
	if (isFreeze)
	{
		return;
	}

	InputManager::GetInstance().MouseRotationUpdate();

	mfHorizontalAngle += InputManager::GetInstance().GetDeltaX();
	mfVerticalAngle -= InputManager::GetInstance().GetDeltaY();

	// 視界の天地逆転（ジンバルロック的破綻）を防ぐためピッチ角を上下90度に制限
	float MAX = DX_PI_F / 2.0f;
	if (mfVerticalAngle >= MAX) mfVerticalAngle = MAX;
	if (mfVerticalAngle <= -MAX) mfVerticalAngle = -MAX;
}

// 正弦波衰減関数による画面振動オフセットの計算
// 入力: なし / 出力: なし / 副作用: mvShakePositionの更新
void Camera::Shake()
{
	if (mfShakeTimeCounter < mfShakeTime)
	{
		// 時間経過とともに振幅を線形減衰させながら正弦波で上下に揺らす
		mvShakePosition.y = sinf(mfShakeAngle) * (1.0f - (mfShakeTimeCounter / mfShakeTime)) * mfShakeWidth;
		mvShakePosition.x = 0.0f;
		mvShakePosition.z = 0.0f;

		mfShakeAngle += mfShakeAngleSpeed * mfStepTime;
		mfShakeTimeCounter += mfStepTime;
	}
	else
	{
		mvShakePosition = VGet(0.0f, 0.0f, 0.0f);
	}
}

// 画面揺れ演出パラメータ（継続時間・振幅・周期速度）の設定
// 入力: time(継続秒), width(振幅), anglespeed(角速度), stepTime(更新刻み) / 出力: なし / 副作用: 揺れカウンタのリセット
void Camera::SetUpShake(float time, float width, float anglespeed, float stepTime)
{
	mfShakeTimeCounter = 0.0f;
	mfShakeTime = time;
	mfShakeWidth = width;
	mfShakeAngleSpeed = anglespeed;
	mfStepTime = stepTime;
}