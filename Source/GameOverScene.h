#pragma once
#include "Scene.h"
#include "Utility.h"
#include "Camera.h"
#include <memory>

class Button;
class TextureAnimation;

// プレイヤー死亡時にリトライまたはタイトル画面への復帰を提供するゲームオーバー画面クラス
class GameOverScene : public Scene
{
public:
	// コンストラクタ
	GameOverScene();
	// デストラクタ
	~GameOverScene();

	// リトライボタン・タイトルボタン、背景アニメーションの生成およびマウスロック解除
	// 入力: なし / 出力: なし / 副作用: Button、TextureAnimationインスタンスの生成
	void Initialize() override;

	// ボタン入力の検知と再挑戦(3Dシーン)/タイトルへの遷移制御と背景の更新処理
	// 入力: なし / 出力: なし / 副作用: BGM・SE再生、SceneManagerへの遷移予約、TextureAnimationの更新
	void Update() override;

	// ゲームオーバータイトル、砂嵐演出および選択ボタンの描画
	// 入力: なし / 出力: なし / 副作用: バックバッファへの描画
	void Draw() override;

	// BGM停止
	// 入力: なし / 出力: なし / 副作用: BGM停止
	void Finalize() override;
private:
	std::unique_ptr<Button> m_retoryButton = nullptr;                // リトライ（3Dシーン再開始）ボタン
	std::unique_ptr<Button> m_titleButton = nullptr;                 // タイトル画面へ戻るボタン
	std::unique_ptr<TextureAnimation> m_textureAnimation = nullptr;  // 砂嵐アニメーション

	const int DrawX = Utility::SCREEN_WIDTH / 2 - 150;               // ボタン配置基準X座標
	const int RetoryY = 650;                                        // リトライボタンY座標
	const int TitleY = 750;                                         // タイトルボタンY座標
};