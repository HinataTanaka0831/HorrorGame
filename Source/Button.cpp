#include "Button.h"

Button::Button(int x1, int y1, int x2, int y2, const std::string& name, int color, int changeColor, int fontHandle)
{
	this->x1 = x1;
	this->y1 = y1;
	this->x2 = x2;
	this->y2 = y2;
	this->name = name;
	this->Color = color;
	this->ChangeColor = changeColor;
	this->FontHandle = fontHandle;
	this->scale = 1.0f;
	this->stringColor = GetColor(255, 255, 255);
}

void Button::Update()
{
	if (!isActive)
	{
		mbisHover = false;
		return;
	}

	if (MouseManager::mouseX >= x1 && MouseManager::mouseY >= y1 && MouseManager::mouseX <= x2 && MouseManager::mouseY <= y2)
	{
		mbisHover = true;
	}
	else
	{
		mbisHover = false;
	}

}

void Button::Draw()
{
    unsigned int drawColor{};

    if (!isActive)
    {
        // ボタンが押せない状態なら暗くしている
        drawColor = GetColor(20, 20, 20);
        scale = 1.0f;
        stringColor = GetColor(80, 80, 80); // 文字の色も暗めに
    }
    else if (mbisHover)
    {
        scale = 1.1f; // マウスと重なっている時

        // 押されていたら
        if (MouseManager::isLeftDown)
        {
            drawColor = ChangeColor;
            scale = 1.0f; // クリックされた瞬間だけ小さく
        }
        else // 乗っているだけ
        {
            scale = 1.1f;
            drawColor = Color; // 色も通常時の
        }
    }
    else
    {
        scale = 1.0f; // 通常時
        drawColor = GetColor(0, 0, 0); // 色も通常時の
        stringColor = GetColor(255, 255, 255); // 文字色白
    }


    // 中心と幅　高さを出す
    float cx = (x1 + x2) / 2.0f; // 左端と右端を足して2で割って中心X
    float cy = (y1 + y2) / 2.0f; // 左上と右下を足して2で割って中心X
    float w = (float)x2 - x1;
    float h = (float)y2 - y1;

    float drawX1 = cx - (w * scale) / 2.0f; // 中心から幅の2分の1左に行く
    float drawY1 = cy - (h * scale) / 2.0f; // 中心から高さの2分の1上に行く
    float drawX2 = cx + (w * scale) / 2.0f; // 中心から幅の2分の1右に行く
    float drawY2 = cy + (h * scale) / 2.0f; // 中心から高さの2分の1下に行く

    int stringW = GetDrawFormatStringWidth("%s", name.c_str()); // 文字幅ゲット

    DrawBox((int)drawX1, (int)drawY1, (int)drawX2, (int)drawY2, drawColor, TRUE);
    DrawFormatStringToHandle((int)(cx - stringW / 2), (int)(cy - 10), stringColor, FontHandle, "%s", name.c_str());
}

