#pragma once
#include "DxLib.h"
#include <string>
#include "MouseManager.h"


class Button {
public:
    Button(int x1, int y1, int x2, int y2, const std::string& name, int color, int changeColor, int fontHandle);

    void Update();
    void Draw();

    bool IsClicked() const { return mbisHover && MouseManager::isLeftTrigger; }

private:
    int x1, y1, x2, y2;
    std::string name;
    int Color; // ボタンの色
    int ChangeColor; // ボタンが選択されているときの色
    int stringColor; // 文字の色
    int FontHandle; // 文字のフォントハンドル
    float scale; // ボタンの拡大率


    bool mbisHover = false;
    bool isActive = true;
};
