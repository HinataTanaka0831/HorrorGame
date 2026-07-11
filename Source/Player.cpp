#include "Player.h"
#include "Collision.h"
#include "HomingBullet.h"
#include "Bullet.h"
#include "Master.h"
#include "ObjectManager.h"
#include "Scene.h"
#include "GameScene.h"
#include "Enemy.h"



Player::Player(VECTOR initPos, int sc)
	: Object2D("Resource/Player.png", initPos)
{
	SetTag(Object2D::Player2D);

	SetChange(sc);  	// 弾を分岐させるための変数を設定 // 

}

Player::~Player()
{

}

void Player::Update()
{
	// 移動処理
	Move();

	// 発射処理
	Shot();

	PCalcdamage();

	// 基底クラスの更新を呼ぶ
	Object2D::Update();

}

void Player::Draw()
{
	// 敵に当たった時に点滅表示する //
	if ((Hitframe / 10) % 2 == 0)
	{
		// 基底クラスの描画を呼ぶ
		Object2D::Draw();
	}
}

void Player::Move()
{

	if (CheckHitKey(KEY_INPUT_D))
	{
		mvPosition.x += (float)MOVE_SPEED;

	}

	if (CheckHitKey(KEY_INPUT_A))
	{
		mvPosition.x -= (float)MOVE_SPEED;


	}

	if (CheckHitKey(KEY_INPUT_S))
	{
		mvPosition.y += (float)MOVE_SPEED;


	}

	if (CheckHitKey(KEY_INPUT_W))
	{
		mvPosition.y -= (float)MOVE_SPEED;

	}

	// 左の画面外に出られないようにする //
	if (mvPosition.x < 0)
	{
		mvPosition.x = 0;
	}

	// 右の画面外に出られないようにする //
	if (mvPosition.x > 640 - 70)
	{
		mvPosition.x = 640 - 70;
	}

	// 上の画面外に出られないようにする //
	if (mvPosition.y < 0)
	{
		mvPosition.y = 0;
	}

	// 下の画面外に出られないようにする //
	if (mvPosition.y > 480 - 45)
	{
		mvPosition.y = 480 - 45;
	}

	// テクスチャに座標を伝える
	mpTexture->SetPosition(mvPosition);
}

void Player::Shot()
{
	// 間隔カウンタをインクリメント
	mnBulletShotCounter++;

	if (CheckHitKey(KEY_INPUT_SPACE))
	{
		if (mnBulletShotCounter > BULLET_SHOT_INTERVAL)
		{

			// 間隔カウンタ初期化
			mnBulletShotCounter = 0;

			// todo: ここで弾を打つ処理を書く
			if (CheckHitKey(KEY_INPUT_SPACE))
			{

				// 敵の種類によって弾を分岐 //
				switch (chg)
				{
				case 2:
					new Bullet(mvPosition, "Resource/bullet.png", 2);
					break;

				case 1:
					new Bullet(mvPosition, "Resource/bullet.png", 1);
					break;

				}
			}

		}
	}


}


// プレイヤーダメージ
void Player::PDamage(int damage)
{
	pHp -= damage;

	if (pHp <= 0)
	{
		Master::mpSceneManager->SetNextScene(SceneManager::SCENE_TYPE::SCENE_GAMEOVER);

		// ゲームオーバーになったらスコアを初期化 //
		score = 0;
	}

}

	

// 敵との当たり判定でのプレイヤーダメージ //
void Player::PCalcdamage()
{
	// 敵情報を取得
	auto pTargetList = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject2DListByTag(Object2D::Enemy2D);


	for (int i = 0; i < pTargetList.size(); i++)
	{
		auto pTarget = pTargetList[i];

		// 取得した情報が存在していれば
		if (pTarget != nullptr)
		{
			if (Hitframe == 0)
			{
				// 当たり判定
				if (Collision::CheckCircleToCircle(
					mvPosition,
					GetRadius(),
					pTarget->GetPosition(),
					pTarget->GetRadius())
					)
				{


					Enemy* pEnemy = dynamic_cast<Enemy*>(pTarget);


					if (pEnemy != nullptr)     // dynamic_cast をした後は必ずnullチェックを行う
					{
						// 当たったので点滅時間を設定
						Hitframe = 100;

						// プレイヤーダメージ
						PDamage(1);
					}
				}

				
			}

			
		}
		
	}

	// 点滅時間を減らしていく
	Hitframe--;
	// Hitframe が０より小さい場合０にする
	if (Hitframe < 0)
	{
		Hitframe = 0;
	}
}

