#pragma once

#include "Utility.h"

// キーボードおよびマウスのトリガー・プレス判定、FPS視点制御用マウスロックを管理するシングルトンクラス
class InputManager
{
public:
	// シングルトンインスタンスの取得
	// 入力: なし / 出力: InputManager参照 / 副作用: 初回呼び出し時に静的インスタンス生成
	static InputManager& GetInstance();

	// キーが押された瞬間（立ち上がりエッジ）の検知
	// 入力: KeyCode(DXライブラリのキーコード) / 出力: 押下瞬間なら1、それ以外は0 / 副作用: 前フレーム状態バッファを更新
	int CheckDownKey(int KeyCode);

	// キーが離された瞬間（立ち下がりエッジ）の検知
	// 入力: KeyCode(DXライブラリのキーコード) / 出力: 離された瞬間なら1、それ以外は0 / 副作用: 前フレーム状態バッファを更新
	int CheckUpKey(int KeyCode);

	// キーが押下され続けている状態の検知
	// 入力: KeyCode(DXライブラリのキーコード) / 出力: 押下中なら1、それ以外は0 / 副作用: なし
	int CheckPressKey(int KeyCode);

	// FPS視点操作用のマウスカーソル固定および表示状態の切り替え
	// 入力: enable(trueで固定/非表示、falseで解除/表示) / 出力: なし / 副作用: OSカーソル表示設定および画面中央への強制移動
	void EnableMouseLock(bool enable);

	// 毎フレームのマウス絶対座標および入力状態のサンプリング
	// 入力: なし / 出力: なし / 副作用: 内部座標および前フレーム・現フレーム入力ビットフラグの更新
	void MouseUpdate();

	// マウスの相対移動量（デルタ）の算出およびFPS用中央復帰
	// 入力: なし / 出力: なし / 副作用: DeltaX/DeltaYの更新およびカーソル再中央配置
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

	InputManager(const InputManager&) = delete;              // コピー生成の禁止
	InputManager& operator=(const InputManager&) = delete;   // コピー代入の禁止

	InputManager(InputManager&&) = delete;                   // ムーブ生成の禁止
	InputManager&& operator=(InputManager&&) = delete;       // ムーブ代入の禁止

private:
	int mDownBuffer[256] = { 0 };      // 前フレームのキー押下状態（立ち上がりエッジ検知用）
	int mUpBuffer[256] = { 0 };        // 前フレームのキー押下状態（立ち下がりエッジ検知用）

	float MouseX = 0.0f;       // マウスのX移動量
	float MouseY = 0.0f;       // マウスのY移動量
	float DeltaX = 0.0f;       // フレーム間のX軸移動差分（感度適用済み）
	float DeltaY = 0.0f;       // フレーム間のY軸移動差分（感度適用済み）
	int CurrentMouseInput = 0;     // 現フレームのマウス入力ビットフラグ
	int PreviousMouseInput = 0;    // 前フレームのマウス入力ビットフラグ
	const int m_centerX = Utility::SCREEN_WIDTH / 4;
	const int m_centerY = Utility::SCREEN_HEIGHT / 4;
	float MouseSensitivity = 0.004f; // マウス移動量から回転角への変換係数
	bool m_isLocked = false;

	// 画面外へのカーソル脱出を防ぎ連続回転を可能にするため中央へ再配置
	// 入力: なし / 出力: なし / 副作用: OSマウスカーソル座標の変更
	void RecenterCursor();

};
