#pragma once

#include "Scene.h"
#include "Utility.h"
#include <memory>

class Button; // 前方宣言

// ゲームクリア時に祝福メッセージおよび次アクション（再プレイ/タイトル戻り）を提示するリザルト画面クラス
class ResultScene : public Scene
{
public:
	// リトライボタン・タイトルボタンの生成およびマウスロック解除
	// 入力: なし / 出力: なし / 副作用: Buttonインスタンスの生成
	ResultScene();
	~ResultScene();

	// リトライボタン・タイトルボタンの生成およびマウスロック解除
	// 入力: なし / 出力: なし / 副作用: Buttonインスタンスの生成
	void Initialize() override;

	// ボタン入力検知とシーン遷移要求
	// 入力: なし / 出力: なし / 副作用: SE再生、SceneManagerへの遷移予約
	void Update() override;

	// クリアメッセージおよびUIボタンの描画
	// 入力: なし / 出力: なし / 副作用: バックバッファへの描画
	void Draw() override;

	// リザルト画面の終了処理
	// 入力: なし / 出力: なし / 副作用: なし
	void Finalize() override;

private:
	std::unique_ptr<Button> m_retoryButton = nullptr;  // 再プレイボタン
	std::unique_ptr<Button> m_titleButton = nullptr;   // タイトル画面へ戻るボタン
	const int DrawX = Utility::SCREEN_WIDTH / 2 - 150; // ボタン配置基準X座標
	const int RetoryY = 600;                    // リトライボタンY座標
	const int TitleY = 750;                     // タイトルボタンY座標
};
