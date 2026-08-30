#pragma once

class Model;

#include "Object3D.h"

// 背景の全方位空間（天球/スカイドーム）を描画するスカイボックスクラス
class SkyBox : public Object3D
{
public:
	// スカイボックスモデルの生成
	// 入力: filename(モデルパス) / 出力: なし / 副作用: Modelインスタンス生成
	SkyBox(std::string filename);
	~SkyBox();					

	// モデルクラスの更新処理を呼び出す
	// 入力: なし / 出力: なし / 副作用: Modelクラスの更新呼び出し
	void Update() override;	

	// ライト計算を一時的に無効化してスカイボックスを描画（陰影による黒ずみ防止）
	// 入力: なし / 出力: なし / 副作用: バックバッファ描画、SetUseLightingフラグ変更
	void Draw() override;	

	void SetScale(float scale);									// 拡大値（スケール値）の設定（Modelクラスへの橋渡し）
	void SetModelTexture(std::string filename, int index = 0);	// モデルのテクスチャ変更（Modelクラスへの橋渡し）

private:
	Model* mpModel;			// モデルクラスのポインタ
};