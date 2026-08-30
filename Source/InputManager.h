/*
キー入力に関する補助関数
DXLibに存在しないので自作
*/
#pragma once

#include "Utility.h"

class InputManager
{
public:
	static InputManager& GetInstance();

	// 指定されたキーが押された瞬間だけ 1 を返す関数
	int CheckDownKey(int KeyCode);

	// 指定されたキーが離された瞬間だけ 1 を返す関数
	int CheckUpKey(int KeyCode);

	// 指定されたキーを押し続けている間１を返す関数
	int CheckPressKey(int KeyCode);


	void EnableMouseLock(bool enable);

	// 毎フレームのマウス座標および入力状態のサンプリング
	// 入力: なし / 出力: なし / 副作用: 内部の座標・入力ビットフラグを更新
	void MouseUpdate();

	void MouseRotationUpdate();

	// 指定されたマウスがが押し続けられているかどうかを判定
	// 入力: mouseCode(マウスコード) / 出力: 押下中ならtrue、それ以外はfalse / 副作用: なし
	bool CheckPressMouseClick(int mouseCode);

	// 指定されたマウスが押された瞬間かどうかを判定
	// 入力: mouseCode(マウスコード) / 出力: 押下瞬間ならtrue、それ以外はfalse / 副作用: なし
	bool CheckTriggerMouseClick(int mouseCode);

	// 指定されたマウスが離された瞬間かどうかを判定
	// 入力: mouseCode(マウスコード) / 出力: 離された瞬間ならtrue、それ以外はfalse / 副作用: なし
	bool CheckReleaseMouseClick(int mouseCode);


	bool IsMouseLocked() const { return m_isLocked; }

	float GetMouseX() { return MouseX; }
	float GetMouseY() { return MouseY; }
	float GetDeltaX() { return DeltaX; }
	float GetDeltaY() { return DeltaY; }

private:
	// コンストラクタ
	InputManager();
	// デストラクタ
	~InputManager();

	InputManager(const InputManager&) = delete;
	InputManager& operator=(const InputManager&) = delete;

	InputManager(InputManager&&) = delete;
	InputManager&& operator=(InputManager&&) = delete;


private:
	int mDownBuffer[256];	// CheckDownKey用のキーバッファ
	int mUpBuffer[256];		// CheckUpKey用のキーバッファ

	float MouseX = 0.0f;                 // 現在のマウスX座標
	float MouseY = 0.0f;                 // 現在のマウスY座標
	float DeltaX = 0.0f;
	float DeltaY = 0.0f;
	int CurrentMouseInput;     // 現フレームのマウス入力ビットフラグ
	int PreviousMouseInput;    // 前フレームのマウス入力ビットフラグ（エッジ検出用）
	const int m_centerX = Utility::SCREEN_WIDTH / 4;
	const int m_centerY = Utility::SCREEN_HEIGHT / 4;
	float MouseSensitivity = 0.004f; // マウスのスピード
	bool m_isLocked = false;
	// 強制的にカーソルを画面の中心に持ってくる
	void RecenterCursor();

};
