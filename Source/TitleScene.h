#pragma once

#include "Scene.h"
#include "Utility.h"
#include <memory>

class Button;

// タイトル画面のUI制御、砂嵐アニメーション描画およびシーン遷移を管理するクラス
class TitleScene : public Scene
{
public:
	TitleScene();
	~TitleScene();

	// タイトル画面のボタンUI生成、敵グラフィック読み込みおよびマウスロック解除
	// 入力: なし / 出力: なし / 副作用: 画像ハンドル読み込み、Buttonインスタンス生成
	void Initialize() override;

	// BGM再生の維持、ボタン入力の監視および各シーンへの遷移要求
	// 入力: なし / 出力: なし / 副作用: BGM再生、SE再生、SceneManagerへの遷移予約
	void Update() override;

	// ホラー演出の砂嵐アニメーション、敵キャラクターグラフィック、タイトルロゴ、UIボタンの描画
	// 入力: なし / 出力: なし / 副作用: バックバッファへの描画およびScreenFlip
	void Draw() override;

	// タイトル画面固有グラフィックリソースの解放およびBGM停止
	// 入力: なし / 出力: なし / 副作用: 画像ハンドルの削除、BGM停止
	void Finalize() override;

private:
	std::unique_ptr<Button> mpPlayButton = nullptr;      // ゲーム本編（3Dシーン）開始ボタン
	std::unique_ptr<Button> mpPlayRuleButton = nullptr;  // 操作方法画面遷移ボタン
	std::unique_ptr<Button> mpQuitButton = nullptr;      // ゲーム終了ボタン
	const int DrawX = Utility::SCREEN_WIDTH / 2 - 150;   // UIボタン配置基準X座標
	const int PlayY = 600;                               // プレイボタンY座標
	const int PlayRuleY = 750;                           // 操作方法ボタンY座標
	const int QuitY = 850;                               // 終了ボタンY座標
	int noise = -1;                                      // 砂嵐エフェクト画像ハンドル
	int enemyPictureHandle = -1;                         // 敵立ち絵グラフィックハンドル
};