#include "Camera.h"
#include <cmath>
#include "Master.h"
#include "ObjectManager.h"
#include "Scene.h"
#include "Player3D.h"
#include "Enemy3D.h"

// コンストラクタ
Camera::Camera()
	: mfHorizontalAngle(DX_PI_F / 1.2f)
	, mfVerticalAngle(0.0f)
	, mvPosition(VGet(0.0f, 0.0f, 0.0f))
	, mvLookAtPosition(VGet(0.0f, 0.0f, 0.0f))
	, mpTarget(nullptr)
	, isFreeze(false)
{

}

// デストラクタ
Camera::~Camera()
{

}

// 初期化
void Camera::Initialize()
{
	this->mvPosition = VGet(mvPosition.x, mvPosition.y, mvPosition.z);

	// カメラのクリッピング距離の設定  
	SetCameraNearFar(20.0f, 50000.0f);

	// 背景色を設定（灰色）
	SetBackgroundColor(128, 128, 128);

	// カメラの位置を設定
	SetCameraPositionAndTarget_UpVecY(mvPosition, mvLookAtPosition);


	// 更新処理を一度行っておく
	Update();
}

// 更新
void Camera::Update()
{

	// マウスによる回転処理
	UpdateRotation();

	if (isFreeze)
	{
		// 画面揺れの処理
		Shake();
	}

	if (mpTarget == nullptr)
	{
		mpTarget = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DByTag(Object3D::TagPlayer3D);
	}

	if (mpTarget != nullptr)
	{
		// プレイヤーの座標にカメラを設定 // 160.0f
		mvPosition = VAdd(mpTarget->GetPosition(), VGet(0.0f, 160.0f, 0.0f));

		Player3D* player = dynamic_cast<Player3D*>(mpTarget);
		if (player != nullptr)
		{
			// しゃがみ状態かどうかを取得
			bool Crouching = player->GetCrouching();
			// プレイヤーの高さを取得
			float playerHeight = player->GetHeight();

			// しゃがみ状態ならばカメラの位置を変更する
			if (Crouching)
			{
				mvPosition.y -= playerHeight;
			}
		}

	

			// プレイヤーの位置に座標を設定
			mvLookAtPosition.x = mvPosition.x + cosf(mfVerticalAngle / 5.0f * DX_PI_F) * sinf(mfHorizontalAngle / 5.0f * DX_PI_F);
			mvLookAtPosition.y = mvPosition.y + sinf(mfVerticalAngle / 5.0f * DX_PI_F);
			mvLookAtPosition.z = mvPosition.z + cosf(mfVerticalAngle / 5.0f * DX_PI_F) * cosf(mfHorizontalAngle / 5.0f * DX_PI_F);


			// カメラ設定を反映
			SetCameraPositionAndTarget_UpVecY(VAdd(mvPosition, mvShakePosition), VAdd(mvLookAtPosition, mvShakePosition));


	}
		
}

// マウス操作での視点回転
void Camera::UpdateRotation()
{

	// フリーズするなら終了
	if (isFreeze)
	{
		return;
	}

	// マウス移動による更新処理を呼ぶ
	InputManager::GetInstance().MouseRotationUpdate();

	// マウスで上下左右に動かせるようにする
	mfHorizontalAngle += InputManager::GetInstance().GetDeltaX();
	mfVerticalAngle -= InputManager::GetInstance().GetDeltaY();



	// 上下方向に制限をかける
	float MAX = DX_PI_F / 2.0f;
	if (mfVerticalAngle >= MAX)
	{
		mfVerticalAngle = MAX;
	}
	if (mfVerticalAngle <= -MAX)
	{
		mfVerticalAngle = -MAX;
	}


	// デバッグ表示
	//DrawFormatString(10, 10, GetColor(255, 255, 255), "MoveX: %d", MoveX);
	//DrawFormatString(10, 30, GetColor(255, 255, 255), "MoveY: %d", MoveY);
	//DrawFormatString(10, 50, GetColor(255, 255, 255), "VerticalAngle: %.2f", mfVerticalAngle);
	//DrawFormatString(10, 70, GetColor(255, 255, 255), "HorizontalAngle: %.2f", mfHorizontalAngle);
	//DrawFormatString(10, 90, GetColor(255, 255, 255), "MouseY: %d", MouseY);
	//DrawFormatString(10, 110, GetColor(255, 255, 255), "CENTER_Y: %d", CENTER_Y);
}

// 画面揺れ
void Camera::Shake()
{
	if (mfShakeTimeCounter < mfShakeTime)
	{
		// sinf を利用して揺らし座標を算出
		mvShakePosition.y = sinf(mfShakeAngle) * (1.0f - (mfShakeTimeCounter / mfShakeTime) ) * mfShakeWidth;
		mvShakePosition.x = 0.0f;
		mvShakePosition.z = 0.0f;

		// 揺らし処理に使用する sinf に渡す角度の変更処理
		mfShakeAngle += mfShakeAngleSpeed * mfStepTime;

		// 揺らす時間を経過させる
		mfShakeTimeCounter += mfStepTime;

	}
	else
	{
		// 揺らされていない場合は揺らし処理による加算座標を０にする
		mvShakePosition = VGet(0.0f, 0.0f, 0.0f);
	}
}

// 画面揺れ設定
void Camera::SetUpShake(float time, float width, float anglespeed, float stepTime)
{
	mfShakeTimeCounter = 0.0f;
	mfShakeTime = time;
	mfShakeWidth = width;
	mfShakeAngleSpeed = anglespeed;
	mfStepTime = stepTime;
}