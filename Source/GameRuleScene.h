#pragma once

#include "Scene.h"
#include "Utility.h"
#include <memory>

class Button;

// 操作説明およびキーバインド一覧を表示するルール解説画面クラス
class GameRuleScene : public Scene
{
public:
	GameRuleScene();
	~GameRuleScene();

	// 操作キーUIグラフィックの読み込みおよび戻るボタンの生成
	// 入力: なし / 出力: なし / 副作用: キーバインド画像ハンドルのロード、Button生成
	void Initialize() override;

	// タイトルへ戻るボタン入力の監視と遷移要求
	// 入力: なし / 出力: なし / 副作用: SE再生、SceneManagerへの遷移予約
	void Update() override;

	// 操作説明画像、対応キーアイコン、解説テキストおよび戻るボタンの描画
	// 入力: なし / 出力: なし / 副作用: バックバッファへの描画およびScreenFlip
	void Draw() override;

	// ロードした全操作キー画像リソースの解放
	// 入力: なし / 出力: なし / 副作用: 画像ハンドルの破棄、BGM停止
	void Finalize() override;

private:
	std::unique_ptr<Button> m_backButton = nullptr;      // タイトルへ戻るボタン
	const int DrawX = Utility::SCREEN_WIDTH / 2 - 150;   // ボタン配置基準X座標
	const int BackY = 900;                               // 戻るボタンY座標
	int m_noiseHandle = -1;                                      // 砂嵐背景画像ハンドル
	int m_keyHandle = -1;                                  // WASD移動キーアイコン
	int m_keyRHandle = -1;                               // Rキーアイコン（アイテム取得）
	int m_keyFHandle = -1;                               // Fキーアイコン（アイテム使用）
	int m_keyEHandle = -1;                               // Eキーアイコン（しゃがみ）
	int m_mouseHandle = -1;                                // マウス左クリックアイコン（ライトON/OFF）
	int m_mouseMoveHandle = -1;                            // マウス移動アイコン（視点回転）
};