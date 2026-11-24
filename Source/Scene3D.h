#pragma once

#include "DxLib.h"
#include "Scene.h"
#include <vector>

// 前方宣言
class Player3D;
class Enemy3D;
class EscapeItem;
class TimeItem;
class Stage;
class Exitdoor;
class SkyBox;

// 3Dホラーゲーム本編のステージ構築、ローディング、HUD描画を統括するシーンクラス
class Scene3D : public Scene
{
public:
	Scene3D();

	// カメラのリセット、各ポインタやコンテナの初期化
	// 入力: なし / 出力: なし / 副作用: ResetTarget()の呼び出し、PlayerやEnemy、TimeItem、SkyBoxのポインタとコンテナを初期化
	~Scene3D() override;

	// ステージ・敵・アイテム生成タスクの実行およびFPSマウス拘束の有効化
	// 入力: なし / 出力: なし / 副作用: LoadingManagerによる非同期アセット生成、Cameraインスタンス生成
	void Initialize() override;

	// ステージ・敵・アイテム生成タスクの個別実行
	// 入力: なし / 出力: なし / 副作用: 各種3Dオブジェクトの生成および配置
	void InitializeCharacterData();
	void InitializeItemData();
	void InitializeSkyBoxData();
	void InitializeStageData();
	void InitializeExitDoorData();

	void SetPointer();

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
	// アイテム関係
	struct Point
	{
		VECTOR PointPosition = VGet(0.0f, 0.0f, 0.0f);         // 脱出用アイテムの出現座標
	};

	// プレイヤー関係
	int m_playerCrouchingHandle;         // プレイヤーしゃがみハンドル
	int m_playerMoveHandle;              // プレイヤー移動ハンドル
	Player3D* m_player;                  // プレイヤーのポインタ

	// 敵関係
	std::vector<Enemy3D*> m_enemies;     // 敵のポインタを管理する変数

	// アイテム関係
	std::vector<EscapeItem*> m_escapeItems; // 脱出アイテムのポインタを管理する変数
	std::vector<TimeItem*> m_timeItems;     // 時間止めアイテムのポインタを管理する変数

	// ステージ関係
	Stage* m_stage;                      // ステージのポインタ

	// 脱出口関係
	Exitdoor* m_exitDoor;                // 脱出口のポインタ

	// スカイボックス関係
	SkyBox* m_skybox;                    // スカイボックスのテクスチャと大きさを変更するためのポインタ

};