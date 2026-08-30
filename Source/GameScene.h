#pragma once

#include "SelectScene.h"


class GameScene : public SelectScene
{


public:
	// �R���X�g���N�^
	GameScene();
	// �f�X�g���N�^
	virtual~GameScene();

	// ������
	virtual void Initialize() override;
	// �X�V
	virtual void Update() override;
	// �`��
	virtual void Draw() override;
	// �I������
	virtual void Finalize() override;




private:
	char font[32];
};
