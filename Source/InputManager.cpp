/*

キー入力に関する補助関数
DXLibに存在しないので自作

*/

#include "InputManager.h"
#include "DxLib.h"


InputManager& InputManager::GetInstance()
{
	static InputManager instance;
	return instance;
}


InputManager::InputManager()
{
}

InputManager::~InputManager()
{

}


int InputManager::CheckDownKey(int keyCode)
{
	// 戻り値用の変数を用意
	int result = 0;

	// 指定キーの現在の状態を取得
	int keyState = CheckHitKey(keyCode);

	// 前回キーが押されておらず、かつ、現在キーが押されていたら「キーを押した瞬間」とする
	if(m_downBuffer[keyCode] == 0 && keyState == 1)
	{
		result = 1;
	}

	// 現在のキー状態をバッファに格納
	m_downBuffer[keyCode] = keyState;

	return result;
}

int InputManager::CheckUpKey(int keyCode)
{
	// 戻り値用の変数を用意
	int result = 0;

	// 指定キーの現在の状態を取得
	int keyState = CheckHitKey(keyCode);

	// 前回キーが押されており、かつ、現在キーが押されていなかったら「キーを離した瞬間」とする
	if(m_upBuffer[keyCode] == 1 && keyState == 0)
	{
		result = 1;
	}

	// 現在のキー状態をバッファに格納
	m_upBuffer[keyCode] = keyState;

	return result;
}

int InputManager::CheckPressKey(int keyCode)
{
	return CheckHitKey(keyCode);
}


// ----FPS視点操作用のマウスカーソル固定および表示状態の切り替え----

void InputManager::EnableMouseLock(bool enable)
{
	m_isLocked = enable;
	if (enable) {
		// カーソル非表示 & 中央へ移動
		SetMouseDispFlag(FALSE);          // DXLib: カーソル非表示
		SetMousePoint(CenterX, CenterY); // 現在位置をセンターに合わせる
	}
	else {
		SetMouseDispFlag(TRUE);           // カーソル表示
	}
}

void InputManager::RecenterCursor()
{
	if (!m_isLocked)
	{
		return;
	}

	// 画面中心へ強制移動 (DXLib の SetMousePoint が即座に反映される)
	SetMousePoint(CenterX, CenterY);
}


void InputManager::MouseUpdate()
{
	int mouseX, mouseY;
	GetMousePoint(&mouseX, &mouseY);

	m_mouseX = (float)mouseX;
	m_mouseY = (float)mouseY;

	m_previousMouseInput = m_currentMouseInput;
	m_currentMouseInput = GetMouseInput();
}

void InputManager::MouseRotationUpdate()
{
	// 前フレームの差分をリセット
	m_deltaX = m_deltaY = 0.0;

	// マウス座標取得 (ロック中は常にセンター付近になるはず)
	int mouseX, mouseY;
	GetMousePoint(&mouseX, &mouseY);

	// デルタ計算 (センターからのオフセット)
	int dx = mouseX - CenterX;
	int dy = mouseY - CenterY;


	// 感度を掛けて格納 (ピクセル → ラジアン等は Camera 側で変換)
	m_deltaX = dx * m_mouseSensitivity;
	m_deltaY = dy * m_mouseSensitivity;


	// FPS 方式なら毎フレームセンターへ戻す
	if (m_isLocked) {
		RecenterCursor();
	}
}

bool InputManager::CheckPressMouseClick(int mouseCode)
{
	bool result = false;

	if (m_currentMouseInput & mouseCode)
	{
		result = true;
	}

	return result;
}

bool InputManager::CheckTriggerMouseClick(int mouseCode)
{
	bool result = false;

	if ((m_currentMouseInput & mouseCode) && !(m_previousMouseInput & mouseCode))
	{
		result = true;
	}

	return result;
}

bool InputManager::CheckReleaseMouseClick(int mouseCode)
{
	bool result = false;

	if (!(m_currentMouseInput & mouseCode) && (m_previousMouseInput & mouseCode))
	{
		result = true;
	}

	return result;
}