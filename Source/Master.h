#pragma once

#include "SceneManager.h"
#include "SoundManagerh.h"
#include "Camera.h"

// ゲーム全体で共有する主要マネージャーへのグローバルアクセスを提供する静的サービスクラス
class Master
{
public:
	static SceneManager* mpSceneManager;  // 画面遷移・進行制御
	static SoundManager* mpSoundManager;  // 音声再生・停止制御
	static Camera* mpCamera;              // メイン視点・カメラ演出制御
};