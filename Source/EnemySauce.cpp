#include "EnemySauce.h"
#include "Utility.h"


EnemySauce::EnemySauce(VECTOR initPos, std::string filename,int t)
	: Object2D(filename, initPos)
	,mvDirection(VGet(-1.0f, 0.0f, 0.0f))   // 最初はX軸の右方向へ動くようにしておく
{
	SetTag(Object2D::EnemySauce2D);

	SetHp(5);

	Settype(t);

}

EnemySauce::~EnemySauce()
{
	
}

void EnemySauce::Update()
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

void EnemySauce::Draw()
{
	Object2D::Draw();
}

void EnemySauce::Move()
{
	switch (type)
	{
	case 1:time = 200; break;
	case 2:time = 100; break;
	case 3:time = 400; break;
	case 4:time = 300; break;
	}

	if (GetNowCount() - StartTime > time)
	{
		// mvDirection.x の方向へ進むようにする
		mvPosition.x += (float)MOVE_SPEED * mvDirection.x;
	}

	// テクスチャに座標を伝える
	mpTexture->SetPosition(mvPosition);
}

bool EnemySauce::IsScreenOut()
{
	return(mvPosition.x + mpTexture->GetSizeX() / 2) < 0.0f;
}



void EnemySauce::Initialize()
{
	int work = rand() % 4;


	switch (work)
	{
	case 0:

		new EnemySauce(
			VGet((float)Utility::SCREEN_WIDTH + 115.0f, GetRand(380), 0.0f),
			"Resource/kusikatsu_gyu.png", 1
		);
		new EnemySauce(
			VGet((float)Utility::SCREEN_WIDTH + 115.0f, GetRand(380), 0.0f),
			"Resource/torimomoniku.png", 2
		);

		break;
		
	case 1:
		new EnemySauce(
			VGet((float)Utility::SCREEN_WIDTH + 115.0f, GetRand(380), 0.0f),
			"Resource/torimomoniku.png", 2
		);

		break;

	case 2:

		new EnemySauce(
			VGet((float)Utility::SCREEN_WIDTH + 130.0f, GetRand(380), 0.0f),
			"Resource/hamukatsu.png", 3
		);
		break;

	case 3:
		new EnemySauce(
			VGet((float)Utility::SCREEN_WIDTH + 140.0f, GetRand(380), 0.0f),
			"Resource/Rebakatsu.png", 4
		);
		break;

	default:
		break;
	}

}

