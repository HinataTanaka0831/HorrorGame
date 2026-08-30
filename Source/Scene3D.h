#pragma once

#include "Scene.h"

// 3Dホラーゲーム本編のステージ構築、ローディング、HUD描画を統括するシーンクラス
class Scene3D : public Scene
{
public:
	Scene3D();
	~Scene3D();

	// ステージ・敵・アイテム生成タスクの実行およびFPSマウス拘束の有効化
	// 入力: なし / 出力: なし / 副作用: LoadingManagerによる非同期アセット生成、Cameraインスタンス生成
	void Initialize() override;

	// シーン内オブジェクトの一括更新
	// 入力: なし / 出力: なし / 副作用: ObjectManager内の全3Dオブジェクト更新
	void Update() override;

	// 3Dワールド描画後のZバッファ一時無効化による2D HUD（残りアイテム数・スタミナ・制限時間）の最前面描画
	// 入力: なし / 出力: なし / 副作用: バックバッファ描画、Zバッファフラグ切り替え
	void Draw() override;

	// シーン固有HUD画像リソースの解放
	// 入力: なし / 出力: なし / 副作用: 画像ハンドルの破棄
	void Finalize() override;

private:
	int mnItemFontHandle = CreateFontToHandle(NULL, 40, -1, DX_FONTTYPE_ANTIALIASING);  // 画面に表示するフォントのハンドル
	int mnGraph = -1;             // 画像ハンドル
	int mnGraph2 = -1;           // 画像ハンドル２

};