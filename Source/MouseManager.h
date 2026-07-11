#pragma once
#pragma once

#include <DXLib.h>
#include <string>
#include "Utility.h"

class MouseManager {
public:
    MouseManager();

    /** Update mouse state – call once every frame before using any getter. */
    static void MouseRotationUpdate();

    void EnableMouseLock(bool enable);

	void ButtonSelect();

    bool IsMouseLocked() const { return m_isLocked; }

public:
    static float mouseX;
    static float mouseY;
    static float m_deltaX;
    static float m_deltaY;
    static bool m_isLocked;
    static bool isLeftDown; // 左クリックが押されているかどうかのフラグ
  static bool isLeftTrigger; // 左クリックが押された瞬間のフラグ
  static bool isLeftReleased; // 左クリックが離された瞬間のフラグ

private:
	int Input = 0; // 今の状態
    int islastLeftInput = 0; // 前フレームの左クリックの状態を保持するフラグ
    static const int m_centerX = Utility::SCREEN_WIDTH / 4;
    static const int m_centerY = Utility::SCREEN_HEIGHT / 4;
    static float MouseSensitivity; // マウスのスピード
    // 強制的にカーソルを画面の中心に持ってくる
    static void RecenterCursor();
};