#include "TitleScene.h"
#include "DxLib.h"
#include "Utility.h"
#include "Master.h"
#include "InputManager.h"



TitleScene::TitleScene() 
: Scene()     // ���N���X�̃R���X�g���N�^��Ăяo��
{

}

TitleScene::~TitleScene()
{
	
}

void TitleScene::Initialize()
{
	// �^�C�g�����S�̃N���X�̍쐬
	// �v���C���[�̐���
	// �Ȃǂ�����ōs��
	// ->�^�C�g����ʂŕK�v�ȃI�u�W�F�N�g������Ő�������

	// BGM�Đ�
	Master::mpSoundManager->PlayBGM(SoundManager::BGM_TITLE);

}

void TitleScene::Update()
{
	// S�L�[�������ꂽ�牺�ɉ�����
	if (InputManager::CheckDownKey(KEY_INPUT_S))
	{
		// SE�Đ�
		 Master::mpSoundManager->PlaySE(SoundManager::SE_DECIDE);

		NowSelect = (NowSelect + 1) % select_Now;
	}
	// W�L�[�������ꂽ���ɏオ��
	if (InputManager::CheckDownKey(KEY_INPUT_W))
	{
		// SE�Đ�
		Master::mpSoundManager->PlaySE(SoundManager::SE_DECIDE);

		NowSelect = (NowSelect + (select_Now - 1)) % select_Now;
	}
	// �G���^�[�L�[�������ꂽ���ʂ̐؂�ւ�����
	if (InputManager::CheckDownKey(KEY_INPUT_RETURN))
	{
		// SE�Đ�
		Master::mpSoundManager->PlaySE(SoundManager::SE_DECIDE);

		switch (NowSelect)
		{
		case select_Play:  // �v���C��ʂ�
			Master::mpSceneManager->SetNextScene(SceneManager::SCENE_SELECT);
			break;

		case select_PlayRule:  // �V�ѕ���
			Master::mpSceneManager->SetNextScene(SceneManager::SCENE_GAMERULE);
			break;
		}

	}

	// ���N���X�̍X�V������Ăяo��
	Scene::Update();
}

void TitleScene::Draw()
{
	// �w�i�̕\��
	DrawGraph(0, 0, grHandle, false);
	// �����̃t�H���g�T�C�Y�ύX
	SetFontSize(40);
	// ������̕\��
	DrawString(Utility::SCREEN_WIDTH / 2 - 120, Utility::SCREEN_HEIGHT / 2 - 100, "2D shooting", GetColor(255, 255, 255));

	SetFontSize(20);

	DrawString(Utility::SCREEN_WIDTH / 2 - 100, 200, "W:��@S:���@ENTER:����", GetColor(255, 255, 255));

	DrawString(Utility::SCREEN_WIDTH / 2 - 60, Play_Y, "�@�v���C�@", GetColor(255, 255, 255));

	DrawString(Utility::SCREEN_WIDTH / 2 - 60, PlayRule_Y, "�@�V�ѕ��@", GetColor(255, 255, 255));


	switch (NowSelect)
	{
	case select_Play:  // �I����i�v���C�j���I�����Ă���ꍇ�̓v���C��Y���W��ݒ肷��
		y = Play_Y;
		break;

	case select_PlayRule:  // �I����i�V�ѕ��j���I�����Ă���ꍇ�ɂ͗V�ѕ���Y���W��ݒ肷��
		y = PlayRule_Y;
		break;
	}

	// �摜�̕\��
	DrawGraph(Utility::SCREEN_WIDTH / 2 - 70, y, icHandle, false);

	// ���N���X�̕`�揈����Ăяo��
	Scene::Draw();
}

void TitleScene::Finalize()
{
	// BGM��~
	Master::mpSoundManager->StopBGM();
}


		// SE�Đ�
//Master::mpSoundManager->PlaySE(SoundManager::SE_DECIDE);
//
//Master::mpSceneManager->SetNextScene(SceneManager::SCENE_TYPE::SCENE_SELECT);
