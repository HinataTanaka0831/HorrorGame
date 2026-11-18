#pragma once

#include "DxLib.h"
#include "Scene.h"

class Player3D;
class Enemy3D;
class TimeItem;
class SkyBox;

// 3Dホラーゲーム本編のステージ構築、ローディング、HUD描画を統括するシーンクラス
class Scene3D : public Scene
{
public:
	Scene3D();

	// カメラのリセット
	// 入力: なし / 出力: なし / 副作用: ResetTarget()の呼び出し
	~Scene3D() override;

	// ステージ・敵・アイテム生成タスクの実行およびFPSマウス拘束の有効化
	// 入力: なし / 出力: なし / 副作用: LoadingManagerによる非同期アセット生成、Cameraインスタンス生成
	void Initialize() override;

	// ステージ・敵・アイテム生成タスクの個別実行
	// 入力: なし / 出力: なし / 副作用: 各種3Dオブジェクトの生成および配置
	void InitializePlayerData();
	void InitializeEnemyData();
	void InitializeItemData();
	void InitializeSkyBoxData();
	void InitializeStageData();
	void InitializeExitDoorData();

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
	int m_playerCrouchingHandle;      // プレイヤーしゃがみハンドル
	int m_playerMoveHandle;           // プレイヤー移動ハンドル

	// 敵関係
	Player3D* m_player;               // プレイヤーのポインタ
	Enemy3D* m_enemy;                 // 敵キャラクターアニメーションを追加するためのポインタ
	Enemy3D* m_enemy2;                // 2対目の敵キャラクターアニメーションを追加するためのポインタ
	TimeItem* m_timeItem;             // 時間止めアイテムのポインタ

	// スカイボックス関係
	SkyBox* m_skybox;                 // スカイボックスのテクスチャと大きさを変更するためのポインタ

};