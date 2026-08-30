/*

キー入力に関する補助関数
DXLibに存在しないので自作

*/

#include "InputManager.h"
#include "DxLib.h"

// 静的メンバ変数の初期化
//int InputManager::mDownBuffer[256] = { 0 };
//int InputManager::mUpBuffer[256] = { 0 };
//
//float InputManager::MouseX = 0.0f;
//float InputManager::MouseY = 0.0f;
//float InputManager::DeltaX = 0.0f;
//float InputManager::DeltaY = 0.0f;
//int InputManager::PreviousMouseInput = 0;
//int InputManager::CurrentMouseInput = 0;

InputManager& InputManager::GetInstance()
{
	static InputManager instance;
	return instance;
}


// コンストラクタ
InputManager::InputManager()
{
}

// デストラクタ
InputManager::~InputManager()
{

}


int InputManager::CheckDownKey(int KeyCode)
{
	// 戻り値用の変数を用意
	int result = 0;

	// 指定キーの現在の状態を取得
	int keyState = CheckHitKey(KeyCode);

	// 前回キーが押されておらず、かつ、現在キーが押されていたら「キーを押した瞬間」とする
	if(mDownBuffer[KeyCode] == 0 && keyState == 1)
	{
		result = 1;
	}

	// 現在のキー状態をバッファに格納
	mDownBuffer[KeyCode] = keyState;

	return result;
}

// 指定されたキーが離された瞬間だけ 1 を返す関数
int InputManager::CheckUpKey(int KeyCode)
{
	// 戻り値用の変数を用意
	int result = 0;

	// 指定キーの現在の状態を取得
	int keyState = CheckHitKey(KeyCode);

	// 前回キーが押されており、かつ、現在キーが押されていなかったら「キーを離した瞬間」とする
	if(mUpBuffer[KeyCode] == 1 && keyState == 0)
	{
		result = 1;
	}

	// 現在のキー状態をバッファに格納
	mUpBuffer[KeyCode] = keyState;

	return result;
}

// 指定されたキーを押し続けている間１を返す関数
int InputManager::CheckPressKey(int KeyCode)
{
	return CheckHitKey(KeyCode);
}




void InputManager::EnableMouseLock(bool enable)
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

void InputManager::RecenterCursor()
{
	if (!m_isLocked) return;
	// 画面中心へ強制移動 (DXLib の SetMousePoint が即座に反映される)
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

void InputManager::MouseRotationUpdate()
{
	// 前フレームの差分をリセット
	DeltaX = DeltaY = 0.0;

	// マウス座標取得 (ロック中は常にセンター付近になるはず)
	int MouseX, MouseY;
	GetMousePoint(&MouseX, &MouseY);

	// デルタ計算 (センターからのオフセット)
	int dx = MouseX - m_centerX;
	int dy = MouseY - m_centerY;


	// 感度を掛けて格納 (ピクセル → ラジアン等は Camera 側で変換)
	DeltaX = dx * MouseSensitivity;
	DeltaY = dy * MouseSensitivity;


	// FPS 方式なら毎フレームセンターへ戻す
	if (m_isLocked) {
		RecenterCursor();
	}
}

// 指定されたマウスがが押し続けられているかどうかを判定
// 入力: mouseCode(マウスコード) / 出力: 押下中ならtrue、それ以外はfalse / 副作用: なし
bool InputManager::CheckPressMouseClick(int mouseCode)
{
	bool result = false;

	if (CurrentMouseInput & mouseCode)
	{
		result = true;
	}

	return result;
}

// 指定されたマウスが押された瞬間かどうかを判定
// 入力: mouseCode(マウスコード) / 出力: 押下瞬間ならtrue、それ以外はfalse / 副作用: なし
bool InputManager::CheckTriggerMouseClick(int mouseCode)
{
	bool result = false;

	if ((CurrentMouseInput & mouseCode) && !(PreviousMouseInput & mouseCode))
	{
		result = true;
	}

	return result;
}

// 指定されたマウスが離された瞬間かどうかを判定
// 入力: mouseCode(マウスコード) / 出力: 離された瞬間ならtrue、それ以外はfalse / 副作用: なし
bool InputManager::CheckReleaseMouseClick(int mouseCode)
{
	bool result = false;

	if (!(CurrentMouseInput & mouseCode) && (PreviousMouseInput & mouseCode))
	{
		result = true;
	}

	return result;
}