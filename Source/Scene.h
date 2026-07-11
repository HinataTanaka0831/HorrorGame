#pragma once
#include "DxLib.h"
#include "MouseManager.h"
#include "Button.h"

// クラスの前方宣言
class ObjectManager;


// シーンの基底クラス

// タイトル、ゲーム、リザルト画面など、
// 画面を作る際は必ずこのクラスを継承する

class Scene
{

public:
	// コンストラクタ
	Scene();

	// デストラクタ
	~Scene();

	// 初期化
	virtual void Initialize() = 0;

	// 更新
	virtual void Update();

	// 描画
	virtual void Draw();

	// 終了処理
	virtual void Finalize() = 0;



	// オブジェクトマネージャーの取得
	ObjectManager* GetObjectManager() { return mpObjectManager; }

protected:
	int FontSize = CreateFontToHandle(NULL, 140, -1, -1);
	int FontSize130 = CreateFontToHandle(NULL, 130, -1, DX_FONTTYPE_ANTIALIASING);
	int FontSize20 = CreateFontToHandle(NULL, 20, -1, -1);
	int FontSize50 = CreateFontToHandle(NULL, 50, -1, -1);
	MouseManager g_MouseMgr;
private:
	ObjectManager* mpObjectManager;        // オブジェクト管理クラスのポインタ

};
