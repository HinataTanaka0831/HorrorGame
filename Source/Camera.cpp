#include "Camera.h"
#include <cmath>
#include "Master.h"
#include "ObjectManager.h"
#include "Scene.h"
#include "Player3D.h"
#include "Enemy3D.h"

Camera::Camera()
{

}

Camera::~Camera()
{

}

void Camera::Initialize()
{
	m_position = VGet(m_position.x, m_position.y, m_position.z);

	// カメラのクリッピング距離の設定  
	SetCameraNearFar(20.0f, 50000.0f);

	// 背景色を設定（灰色）
	SetBackgroundColor(128, 128, 128);

	// カメラの位置を設定
	SetCameraPositionAndTarget_UpVecY(m_position, m_lookAtPosition);


	// 更新処理を一度行っておく
	Update();
}

void Camera::Update()
{

	// マウスによる回転処理
	UpdateRotation();

	if (m_isFreeze)
	{
		// 画面揺れの処理
		Shake();
	}

	if (m_target == nullptr)
	{
		m_target = Master::m_sceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DByTag(Object3D::TagPlayer3D);
	}

	if (m_target != nullptr)
	{
		// プレイヤーの座標にカメラを設定 // 160.0f
		m_position = VAdd(m_target->GetPosition(), VGet(0.0f, 160.0f, 0.0f));

		Player3D* player = dynamic_cast<Player3D*>(m_target);
		if (player != nullptr)
		{
			// しゃがみ状態かどうかを取得
			bool Crouching = player->GetCrouching();
			// プレイヤーの高さを取得
			float playerHeight = player->GetHeight();

			// しゃがみ状態ならばカメラの位置を変更する
			if (Crouching)
			{
				m_position.y -= playerHeight;
			}
		}

	

			// プレイヤーの位置に座標を設定
			m_lookAtPosition.x = m_position.x + cosf(m_verticalAngle / 5.0f * DX_PI_F) * sinf(m_horizontalAngle / 5.0f * DX_PI_F);
			m_lookAtPosition.y = m_position.y + sinf(m_verticalAngle / 5.0f * DX_PI_F);
			m_lookAtPosition.z = m_position.z + cosf(m_verticalAngle / 5.0f * DX_PI_F) * cosf(m_horizontalAngle / 5.0f * DX_PI_F);


			// カメラ設定を反映
			SetCameraPositionAndTarget_UpVecY(VAdd(m_position, m_shakePosition), VAdd(m_lookAtPosition, m_shakePosition));


	}
		
}

void Camera::UpdateRotation()
{

	// フリーズするなら終了
	if (m_isFreeze)
	{
		return;
	}

	// マウス移動による更新処理を呼ぶ
	InputManager::GetInstance().MouseRotationUpdate();

	// マウスで上下左右に動かせるようにする
	m_horizontalAngle += InputManager::GetInstance().GetDeltaX();
	m_verticalAngle -= InputManager::GetInstance().GetDeltaY();



	// 上下方向に制限をかける
	float MAX = DX_PI_F / 2.0f;
	if (m_verticalAngle >= MAX)
	{
		m_verticalAngle = MAX;
	}
	if (m_verticalAngle <= -MAX)
	{
		m_verticalAngle = -MAX;
	}


	// デバッグ表示
	//DrawFormatString(10, 50, GetColor(255, 255, 255), "VerticalAngle: %.2f", m_verticalAngle);
	//DrawFormatString(10, 70, GetColor(255, 255, 255), "HorizontalAngle: %.2f", m_horizontalAngle);
}

void Camera::Shake()
{
	if (m_shakeTimeCounter < m_shakeTime)
	{
		// sinf を利用して揺らし座標を算出
		m_shakePosition.y = sinf(m_shakeAngle) * (1.0f - (m_shakeTimeCounter / m_shakeTime) ) * m_shakeWidth;
		m_shakePosition.x = 0.0f;
		m_shakePosition.z = 0.0f;

		// 揺らし処理に使用する sinf に渡す角度の変更処理
		m_shakeAngle += m_shakeAngleSpeed * m_stepTime;

		// 揺らす時間を経過させる
		m_shakeTimeCounter += m_stepTime;

	}
	else
	{
		// 揺らされていない場合は揺らし処理による加算座標を０にする
		m_shakePosition = VGet(0.0f, 0.0f, 0.0f);
	}
}

void Camera::SetUpShake(float time, float width, float anglespeed, float stepTime)
{
	m_shakeTimeCounter = 0.0f;
	m_shakeTime = time;
	m_shakeWidth = width;
	m_shakeAngleSpeed = anglespeed;
	m_stepTime = stepTime;
}