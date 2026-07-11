#include "MouseManager.h"

float MouseManager::mouseX = 0.0f;
float MouseManager::mouseY = 0.0f;
float MouseManager::m_deltaX = 0.0f;
float MouseManager::m_deltaY = 0.0f;
bool MouseManager::m_isLocked = false;
float MouseManager::MouseSensitivity = 0.004f;
bool MouseManager::isLeftDown = false;
bool MouseManager::isLeftTrigger = false;
bool MouseManager::isLeftReleased = false;

MouseManager::MouseManager()
{

}

void MouseManager::EnableMouseLock(bool enable)
{
    m_isLocked = enable;
    if (enable) {
        // カーソル非表示 & 中央へ移動
        SetMouseDispFlag(FALSE);          // DXLib: カーソル非表示
        SetMousePoint(m_centerX, m_centerY); // 現在位置をセンターに合わせる
    }
    else {
        SetMouseDispFlag(TRUE);           // カーソル表示
    }
}

void MouseManager::ButtonSelect()
{
	int mX, mY;
	GetMousePoint(&mX, &mY);

    mouseX = (float)mX;
    mouseY = (float)mY;
	
    islastLeftInput = Input; // 前フレームの左クリックの状態を保持するフラグ

	Input = GetMouseInput();

	isLeftTrigger = (Input & MOUSE_INPUT_LEFT)  && !(islastLeftInput & MOUSE_INPUT_LEFT); // 左クリックが押された瞬間

	isLeftDown = (Input & MOUSE_INPUT_LEFT); // 左クリックが押されているかどうかのフラグ

	isLeftReleased = (Input & MOUSE_INPUT_LEFT) == 0 && (islastLeftInput & MOUSE_INPUT_LEFT); // 左クリックが離された瞬間

}


void MouseManager::RecenterCursor()
{
    if (!m_isLocked) return;
    // 画面中心へ強制移動 (DXLib の SetMousePoint が即座に反映される)
    SetMousePoint(m_centerX, m_centerY);
}

void MouseManager::MouseRotationUpdate()
{
    // 前フレームの差分をリセット
    m_deltaX = m_deltaY = 0.0;

    // マウス座標取得 (ロック中は常にセンター付近になるはず)
    int MouseX, MouseY;
    GetMousePoint(&MouseX, &MouseY);

    // デルタ計算 (センターからのオフセット)
    int dx = MouseX - m_centerX;
    int dy = MouseY - m_centerY;


    // 感度を掛けて格納 (ピクセル → ラジアン等は Camera 側で変換)
    m_deltaX = dx * MouseSensitivity;
    m_deltaY = dy * MouseSensitivity;


    // FPS 方式なら毎フレームセンターへ戻す
    if (m_isLocked) {
        RecenterCursor();
    }
}