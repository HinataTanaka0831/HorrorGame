#pragma once

#include "Scene.h"
#include "Utility.h"
#include <memory>

class Button;
class TextureAnimation;

// タイトル画面のUI制御、砂嵐アニメーション描画およびシーン遷移を管理するクラス
class TitleScene : public Scene
{
public:
	// コンストラクタ
	TitleScene();
	// デストラクタ
	~TitleScene();


	// タイトル画面のボタンUI、背景アニメーション生成およびマウスロック解除
	// 入力: なし / 出力: なし / 副作用: Button、TextureAnimationインスタンス生成
	void Initialize() override;

	// BGM再生の維持、ボタン入力の監視および各シーンへの遷移要求、砂嵐アニメーションの更新
	// 入力: なし / 出力: なし / 副作用: BGM再生、SE再生、SceneManagerへの遷移予約、TextureAnimationの更新
	void Update() override;

	// ホラー演出の砂嵐アニメーション、タイトルロゴ、UIボタンの描画
	// 入力: なし / 出力: なし / 副作用: バックバッファへの描画
	void Draw() override;

	// タイトル画面BGM停止
	// 入力: なし / 出力: なし / 副作用: BGM停止
	void Finalize() override;



private:
	std::unique_ptr<Button> m_playButton = nullptr;                  // ゲーム本編（3Dシーン）開始ボタン
	std::unique_ptr<Button> m_playRuleButton = nullptr;              // 操作方法画面遷移ボタン
	std::unique_ptr<Button> m_quitButton = nullptr;                  // ゲーム終了ボタン
	std::unique_ptr<TextureAnimation> m_textureAnimation = nullptr;  // 砂嵐アニメーション
	const int DrawX = Utility::SCREEN_WIDTH / 2 - 150;               // UIボタン配置基準X座標
	const int PlayY = 650;                                           // プレイボタンY座標
	const int PlayRuleY = 750;                                       // 操作方法ボタンY座標
	const int QuitY = 850;                                           // 終了ボタンY座標
};