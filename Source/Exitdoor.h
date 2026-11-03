#pragma once
#include "Object3D.h"
#include "DxLib.h"
#include <vector>

class Model;

// ゲームクリア目標地点となる脱出扉オブジェクトおよび当たり判定管理クラス
class Exitdoor : public Object3D
{
public:
	// ドア描画モデルおよび当たり判定専用コリジョンモデルのロード
    // 入力: fileName(描画モデルパス), exitdoorModelname(コリジョンモデルパス), initPosition(配置座標) / 出力: なし / 副作用: MV1SetupCollInfo実行
	Exitdoor(std::string fileName, VECTOR initPosition);
	~Exitdoor() override;

	// ドアモデルの回転・座標更新および法線ベクトルの更新
	// 入力: なし / 出力: なし / 副作用: ドアモデルの回転・座標更新、法線ベクトルの更新
	void Update() override;

	// ドア3Dモデルの描画
	// 入力: なし / 出力: なし / 副作用: バックバッファへの描画
	void Draw() override;

private:
	Model* m_model;         // モデルクラスのポインタ
};