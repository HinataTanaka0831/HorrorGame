#pragma once
#include "Scene.h"
#include "Utility.h"
#include "Camera.h"
#include <memory>

class Button;

// プレイヤー死亡時にリトライまたはタイトル画面への復帰を提供するゲームオーバー画面クラス
class GameOverScene : public Scene
{
public:
	// コンストラクタ
	GameOverScene();
	// デストラクタ
	~GameOverScene();

	// リトライボタン・タイトルボタンの生成およびマウスロック解除
	// 入力: なし / 出力: なし / 副作用: Buttonインスタンスの生成
	void Initialize() override;

	// ボタン入力の検知と再挑戦(3Dシーン)/タイトルへの遷移制御
	// 入力: なし / 出力: なし / 副作用: BGM・SE再生、SceneManagerへの遷移予約
	void Update() override;

	// ゲームオーバータイトル、砂嵐演出および選択ボタンの描画
	// 入力: なし / 出力: なし / 副作用: バックバッファへの描画およびScreenFlip
	void Draw() override;

	// 確保済み画像ハンドルの破棄およびBGM停止
	// 入力: なし / 出力: なし / 副作用: 画像削除、BGM停止
	void Finalize() override;
private:
	std::unique_ptr<Button> mpRetoryButton = nullptr;  // リトライ（3Dシーン再開始）ボタン
	std::unique_ptr<Button> mpTitleButton = nullptr;   // タイトル画面へ戻るボタン
	int noise = -1;                                    // 砂嵐背景画像ハンドル

	const int DrawX = Utility::SCREEN_WIDTH / 2 - 150;       // ボタン配置基準X座標
	static const int RetoryY = 650;                    // リトライボタンY座標
	static const int TitleY = 750;                     // タイトルボタンY座標
};