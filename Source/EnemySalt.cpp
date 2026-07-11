#include "EnemySalt.h"
#include "Utility.h"
#include "GameScene.h"


EnemySalt::EnemySalt(VECTOR initPos, std::string filename, int t )
	: Object2D(filename, initPos)
	, mvDirection(VGet(-1.0f, 0.0f, 0.0f))   // 最初はX軸の右方向へ動くようにしておく
{
	SetTag(Object2D::EnemySalt2D);

	Settype(t);

	lose = new Player(initPos);
}

EnemySalt::~EnemySalt()
{

}

void EnemySalt::Update()
{

	// 移動処理
	Move();

	if (IsScreenOut())
	{
		SetDeleteFlag(true);

		Initialize();

	}

	Object2D::Update();
}

void EnemySalt::Draw()
{
	Object2D::Draw();
}

void EnemySalt::Move()
{
	switch (type)
	{
	case 1:time = 300; break;
	case 2:time = 400; break;
	case 3:time = 200; break;
	case 4:time = 500; break;

	}

	if (GetNowCount() - StartTime > time)
	{
		// mvDirection.x の方向へ進むようにする
		mvPosition.x += (float)MOVE_SPEED * mvDirection.x;
	}

	// テクスチャに座標を伝える
	mpTexture->SetPosition(mvPosition);
}

bool EnemySalt::IsScreenOut()
{
	return(mvPosition.x + mpTexture->GetSizeX() / 2) < 0.0f;
}



void EnemySalt::Initialize()
{
	int work = rand() % 4;


	switch (work)
	{
	case 0:

		new EnemySalt(
			VGet((float)Utility::SCREEN_WIDTH + 115.0f, GetRand(380), 0.0f),
			"Resource/kusikatsu_ton.png", 1
		);
		break;

	case 1:
		new EnemySalt(
			VGet((float)Utility::SCREEN_WIDTH + 115.0f, GetRand(380), 0.0f),
			"Resource/tamanegi.png", 2
		);
		break;

	case 2:

		new EnemySalt(
			VGet((float)Utility::SCREEN_WIDTH + 130.0f, GetRand(380), 0.0f),
			"Resource/renkon.png", 3
		);
		break;

	case 3:
		new EnemySalt(
			VGet((float)Utility::SCREEN_WIDTH + 150.0f, GetRand(380), 0.0f),
			"Resource/torisasami.png", 4
		);
		break;

	default:
		new EnemySalt(
			VGet((float)Utility::SCREEN_WIDTH + 130.0f, GetRand(380), 0.0f),
			"Resource/renkon.png", 3
		);

		break;
	}

}




void EnemySalt::Damage(int damage)
{

	mpHp -= damage;

	if (mpHp <= 0)
	{
		SetDeleteFlag(true);

		int c = da->select_salt;

		if (c)
		{
			lose->Damage();
		}

	}

}

