#pragma once

#include "Scene.h"
#include "Utility.h"


class SelectScene : public Scene
{

public:  // enum 定数の定義
	// 選択肢
public:
	enum SELECT_TYPE{
		select_salt,
		select_sauce,

		select_Num,
	};



public:
	// コンストラクタ
	SelectScene();
	// デストラクタ
	virtual~SelectScene();
	// 初期化
	virtual void Initialize() override;
	// 更新
	virtual void Update() override;
	// 描画
	virtual void Draw() override;
	// 終了処理
	virtual void Finalize() override;


protected:
	static int chag;  // 選択肢によって情報を分岐するための変数 //


private:
	static const int Salt_Y = 270;  // 選択肢（塩）のY座標
	static const int Sauce_Y= 320;  // 選択肢（ソース）のY座標
	int y = 0;           // 選択肢（アイコン）のY座標
	int NowSelect = select_salt;   // 選択肢（アイコン）の初期化
};