#pragma once

#include "Scene.h"

class Scene3D : public Scene
{
public:
	// コンストラクタ
	Scene3D();

	// デストラクタ
	~Scene3D();

	// 初期化
	void Initialize() override;

	// 更新
	void Update() override;

	// 描画
	void Draw() override;

	// 終了処理
	void Finalize() override;

private:
	int ItemFontHandle = CreateFontToHandle(NULL, 40, -1, DX_FONTTYPE_ANTIALIASING);  // 画面に表示するフォントのハンドル
	int mnGraph = -1;             // 画像ハンドル
	int mnGraph2 = -1;           // 画像ハンドル２

};