#pragma once

#include "SceneManager.h"
#include "SoundManagerh.h"
#include "Camera.h"

//
// ゲーム内で1つだけしか存在しないものを管理するクラス
// 例えば、
// SceneManager だったり、（シーン管理クラス）
// SoundManager だったり、（サウンド管理クラス）
// ResourceManager だったり（リソース（画像やモデル）管理クラス）
// を、どこからでも参照できるようにしておきたいので、
// こういったクラスを作る。
//
class Master
{
public:
	static SceneManager* m_sceneManager;        // シーンマネージャーのポインタ
	static SoundManager* m_soundManager;        // サウンドマネージャーのポインタ
	static Camera* m_camera;                    // カメラポインタ
};