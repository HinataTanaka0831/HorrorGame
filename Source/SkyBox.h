#pragma once

#include "Object3D.h"
#include <string>

class Model;

// 背景の全方位空間（天球/スカイドーム）を描画するスカイボックスクラス
class SkyBox : public Object3D
{
public:
	// スカイボックスモデルの生成
	// 入力: filename(モデルパス) / 出力: なし / 副作用: Modelインスタンス生成
	SkyBox(std::string filename);
	~SkyBox();

	void Update() override;

	// ライト計算を一時的に無効化してスカイボックスを描画（陰影による黒ずみ防止）
	// 入力: なし / 出力: なし / 副作用: バックバッファ描画、SetUseLightingフラグ変更
	void Draw() override;

	void SetScale(float scale);
	void SetModelTexture(std::string filename, int index = 0);

private:
	Model* mpModel;
};