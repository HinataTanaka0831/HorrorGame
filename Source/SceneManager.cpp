#include "SceneManager.h"
#include "Scene.h"
#include "TitleScene.h"
#include "ResultScene.h"
#include "GameRuleScene.h"
#include "GameOverScene.h"
#include "Scene3D.h"

SceneManager::SceneManager()
	: mnSceneType(SCENE_TYPE::SCENE_NONE)
	, mnNextSceneType(SCENE_TYPE::SCENE_NONE)
{
}

SceneManager::~SceneManager()
{
}

void SceneManager::Initialize()
{
	mnNextSceneType = SCENE_TYPE::SCENE_TITLE;
	ChangeSceneIfNeeded();
}

void SceneManager::Update()
{
	if (mpCurrentScene != nullptr)
	{
		mpCurrentScene->Update();
	}
}

void SceneManager::Draw()
{
	if (mpCurrentScene != nullptr)
	{
		mpCurrentScene->Draw();
	}
}

void SceneManager::Finalize()
{
}

void SceneManager::ChangeSceneIfNeeded()
{
	if (mnSceneType == mnNextSceneType)
	{
		return;
	}

	if (mpCurrentScene != nullptr)
	{
		mpCurrentScene->Finalize();
		delete mpCurrentScene;
		mpCurrentScene = nullptr;
	}

	mnSceneType = mnNextSceneType;

	switch (mnSceneType)
	{
	case SCENE_TYPE::SCENE_TITLE:
		mpCurrentScene = new TitleScene();
		break;

	case SCENE_TYPE::SCENE_RESULT:
		mpCurrentScene = new ResultScene();
		break;

	case SCENE_TYPE::SCENE_GAME_RULE:
		mpCurrentScene = new GameRuleScene();
		break;

	case SCENE_TYPE::SCENE_GAMEOVER:
		mpCurrentScene = new GameOverScene();
		break;

	case SCENE_TYPE::SCENE_3D:
		mpCurrentScene = new Scene3D();
		break;

	default:
		break;
	}

	if (mpCurrentScene != nullptr)
	{
		mpCurrentScene->Initialize();
	}
}

void SceneManager::RequestQuit()
{
	mbQuitRequest = true;
}