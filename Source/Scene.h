#pragma once
#include "DxLib.h"
#include "Button.h"

class ObjectManager;

// 各画面（タイトル・ゲーム本編・リザルト等）の共通インターフェースおよびオブジェクト管理基底クラス
class Scene
{
public:
	// シーン専任のObjectManagerインスタンス生成
	// 入力: なし / 出力: なし / 副作用: mpObjectManagerのメモリ確保
	Scene();
	virtual ~Scene();

	// シーン固有のリソース読み込みおよび初期オブジェクト配置
	// 入力: なし / 出力: なし / 副作用: 派生クラスによるグラフィック・モデル等の確保
	virtual void Initialize() = 0;

	// シーン内オブジェクトの一括更新
	// 入力: なし / 出力: なし / 副作用: 登録オブジェクトの状態更新
	virtual void Update();

	// シーン内オブジェクトの一括描画
	// 入力: なし / 出力: なし / 副作用: バックバッファへの描画
	virtual void Draw();

	// シーン終了時のリソース破棄
	// 入力: なし / 出力: なし / 副作用: 派生クラスによる確保リソースの全解放
	virtual void Finalize() = 0;

	ObjectManager* GetObjectManager() { return mpObjectManager; }

protected:
	int fontSize90 = CreateFontToHandle(NULL, 90, -1, -1);
	int fontSize130 = CreateFontToHandle(NULL, 130, -1, DX_FONTTYPE_ANTIALIASING);
	int fontSize20 = CreateFontToHandle(NULL, 20, -1, -1);
	int fontSize50 = CreateFontToHandle(NULL, 50, -1, -1);

private:
	ObjectManager* mpObjectManager;  // シーン所属オブジェクトの一元管理インスタンス
};