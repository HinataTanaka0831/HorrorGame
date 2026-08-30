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

int InputManager::CheckDownKey(int KeyCode)
{
	int result = 0;
	int keyState = CheckHitKey(KeyCode);

	// 前フレームで非押下かつ現フレームで押下の場合のみトリガー成立
	if (mDownBuffer[KeyCode] == 0 && keyState == 1)
	{
		result = 1;
	}

	mDownBuffer[KeyCode] = keyState;
	return result;
}

// 指定されたキーが離された瞬間だけ 1 を返す関数
int InputManager::CheckUpKey(int KeyCode)
{
	int result = 0;
	int keyState = CheckHitKey(KeyCode);

	// 前フレームで押下中かつ現フレームで非押下の場合のみリリーストリガー成立
	if (mUpBuffer[KeyCode] == 1 && keyState == 0)
	{
		result = 1;
	}

	mUpBuffer[KeyCode] = keyState;
	return result;
}

// 指定されたキーを押し続けている間１を返す関数
int InputManager::CheckPressKey(int KeyCode)
{
	return CheckHitKey(KeyCode);
}

// FPS視点操作用のマウスカーソル固定および表示状態の切り替え
// 入力: enable(trueで固定/非表示、falseで解除/表示) / 出力: なし / 副作用: OSカーソル表示設定および画面中央への強制移動
void InputManager::EnableMouseLock(bool enable)
{
	m_isLocked = enable;
	if (enable) {
		// 画面外へのカーソル脱出とUI誤操作を防ぐため非表示かつ中央配置
		SetMouseDispFlag(FALSE);
		SetMousePoint(m_centerX, m_centerY);
	}
	else {
		SetMouseDispFlag(TRUE);
	}
}


// 画面外へのカーソル脱出を防ぎ連続回転を可能にするため中央へ再配置
// 入力: なし / 出力: なし / 副作用: OSマウスカーソル座標の変更
void InputManager::RecenterCursor()
{
	if (!m_isLocked) return;
	SetMousePoint(m_centerX, m_centerY);
}

// 毎フレームのマウス座標および入力状態のサンプリング
// 入力: なし / 出力: なし / 副作用: 内部の座標・入力ビットフラグを更新
void InputManager::MouseUpdate()
{
	int mx, my;
	GetMousePoint(&mx, &my);

	this->MouseX = (float)mx;
	this->MouseY = (float)my;

	PreviousMouseInput = CurrentMouseInput;
	CurrentMouseInput = GetMouseInput();
}

// マウスの相対移動量（デルタ）の算出およびFPS用中央復帰
// 入力: なし / 出力: なし / 副作用: DeltaX/DeltaYの更新およびカーソル再中央配置
void InputManager::MouseRotationUpdate()
{
	DeltaX = DeltaY = 0.0f;

	int mouseX, mouseY;
	GetMousePoint(&mouseX, &mouseY);

	// 中央座標からの差分を変位量（デルタ）として算出
	int dx = mouseX - m_centerX;
	int dy = mouseY - m_centerY;

	DeltaX = dx * MouseSensitivity;
	DeltaY = dy * MouseSensitivity;

	if (m_isLocked) {
		RecenterCursor();
	}
}

// 指定されたマウスがが押し続けられているかどうかを判定
// 入力: mouseCode(マウスコード) / 出力: 押下中ならtrue、それ以外はfalse / 副作用: なし
bool InputManager::CheckPressMouseClick(int mouseCode)
{
	return (CurrentMouseInput & mouseCode) != 0;
}

// 指定されたマウスが押された瞬間かどうかを判定
// 入力: mouseCode(マウスコード) / 出力: 押下瞬間ならtrue、それ以外はfalse / 副作用: なし
bool InputManager::CheckTriggerMouseClick(int mouseCode)
{
	return ((CurrentMouseInput & mouseCode) && !(PreviousMouseInput & mouseCode));
}

// 指定されたマウスが離された瞬間かどうかを判定
// 入力: mouseCode(マウスコード) / 出力: 離された瞬間ならtrue、それ以外はfalse / 副作用: なし
bool InputManager::CheckReleaseMouseClick(int mouseCode)
{
	return (!(CurrentMouseInput & mouseCode) && (PreviousMouseInput & mouseCode));
}