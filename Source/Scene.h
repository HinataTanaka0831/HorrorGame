#pragma once
#include "DxLib.h"
#include "Button.h"

// クラスの前方宣言
class ObjectManager;


// シーンの基底クラス

// タイトル、ゲーム、リザルト画面など、
// 画面を作る際は必ずこのクラスを継承する

// 各画面（タイトル・ゲーム本編・リザルト等）の共通インターフェースおよびオブジェクト管理基底クラス
class Scene
{

public:
	// シーン専任のObjectManagerインスタンス生成
	// 入力: なし / 出力: なし / 副作用: m_objectManagerのメモリ確保
	Scene();
	~Scene();

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


	// オブジェクトマネージャーの取得
	ObjectManager* GetObjectManager() { return m_objectManager; }

protected:
	// 画面に表示するフォントのハンドル
	int m_fontSize140;
	int m_fontSize130;
	int m_fontSize20;
	int m_fontSize50;
	int m_itemFontHandle;  

private:
	ObjectManager* m_objectManager;        // オブジェクト管理クラスのポインタ

};
