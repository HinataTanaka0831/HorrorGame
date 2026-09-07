#include "SceneManager.h"
#include "Scene.h"
#include "TitleScene.h"
#include "ResultScene.h"
#include "GameRuleScene.h"
#include "GameOverScene.h"
#include "Scene3D.h"


SceneManager::SceneManager()
{

}

SceneManager::~SceneManager()
{

}

void SceneManager::Initialize()
{
	 // 初期シーンの設定
	m_nextSceneType = SceneType::SceneTitle;

	// シーン遷移をさせる
	ChangeSceneIfNeeded();
}

void SceneManager::Update()
{
	// シーンの更新
	m_currentScene->Update();
}

void SceneManager::Draw()
{
	// シーンの描画
	m_currentScene->Draw();
}

void SceneManager::Finalize()
{

}

void SceneManager::ChangeSceneIfNeeded()
{

	// 現在シーンと次シーンが一緒であれば何もしない
	if (m_sceneType == m_nextSceneType)
	{
		return;
	}
	

	if (m_currentScene != nullptr)
	{
		// 現在シーンの終了処理をする
		m_currentScene->Finalize();

		// 一旦シーン自体も破棄しておく
		delete m_currentScene;
		m_currentScene = nullptr;
	}


	// 次シーンにするためシーンタイプを更新
	m_sceneType = m_nextSceneType;

	// m_sceneType に応じてシーンを生成する
	switch (m_sceneType)
	{
	case SceneType::SceneTitle:
		m_currentScene = new TitleScene();
		break;

	case SceneType::SceneResult:
		m_currentScene = new ResultScene();
		break;

	case SceneType::SceneGameRule:
		m_currentScene = new GameRuleScene();
		break;

	case SceneType::SceneGameOver:
		m_currentScene = new GameOverScene();
		break;

	case SceneType::SceneGame3D:
		m_currentScene = new Scene3D();
		break;

	default:
		break;
	}


	// シーンの生成がされているはずなので、初期化処理を読んでおく
	if (m_currentScene != nullptr)
	{
		m_currentScene->Initialize();
	}

}

void SceneManager::RequestQuit()
{
	m_quitRequest = true;
}
