#include "TimeItem.h"
#include "Item.h"
#include "Model.h"
#include "Master.h"
#include "Scene.h"
#include "ObjectManager.h"
#include "Enemy3D.h"
#include "Player3D.h"
#include "Utility.h"

// =====================================================
// static変数の定義（クラスの外で1回だけ定義する）
// static変数は全てのTimeItemインスタンスで共有される
// =====================================================
int  TimeItem::m_waitTimer         = 0;     // 停止タイマー（0からカウントアップ）
bool TimeItem::m_isTimerActive     = false; // タイマーが動作中かどうか
int  TimeItem::m_timerFontHandle   = -1;    // フォントハンドル（未作成は-1）


TimeItem::TimeItem(std::string fileName, VECTOR initPosition, bool isSeparateAnimation)
    :Item(initPosition)
{
	SetTag(Object3D::TagTimeItem);

	m_model = new Model(fileName, initPosition, isSeparateAnimation);

	// フォントハンドルをまだ作っていなければ作成する
	// （最初のTimeItemが生成されたときに1回だけ作る）
	if (m_timerFontHandle == -1)
	{
		m_timerFontHandle = CreateFontToHandle(NULL, 50, -1, DX_FONTTYPE_ANTIALIASING);
	}
}

TimeItem::~TimeItem()
{
	if (m_model != nullptr)
	{
		delete m_model;
	}
}

void TimeItem::Update()
{
	// 敵の動きを止める処理
	StopTime();

	if (m_model != nullptr)
	{
		m_model->SetPosition(m_position);
		m_model->Update();
	}

}

void TimeItem::Draw()
{
	if (m_model != nullptr)
	{
		m_model->Draw();
	}
}

void TimeItem::StopTime()
{

	// -------------------------------------------------
	// プレイヤーを取得する
	// -------------------------------------------------
	auto player = Master::m_sceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DByTag(Object3D::TagPlayer3D);
	Player3D* pPlayer = dynamic_cast<Player3D*>(player);

	if (pPlayer == nullptr)
	{
		return;
	}

	// プレイヤーが時間停止アイテムを使用したかどうかを取得
	bool isUseStopItem = pPlayer->GetIsUseStopItem();

	// -------------------------------------------------
	// アイテム使用直後：タイマーを開始する
	// -------------------------------------------------
	if (isUseStopItem && !m_isTimerActive)
	{
		// タイマーを開始状態にする
		m_isTimerActive = true;

		// タイマーを0にリセット
		m_waitTimer = 0;

		// 全ての敵の動きを止める
		auto eobjList = Master::m_sceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DListByTag(Object3D::TagEnemy3D);
		for (int i = 0; i < eobjList.size(); i++)
		{
			auto eobj = eobjList[i];
			if (eobj != nullptr)
			{
				Enemy3D* Enemy = dynamic_cast<Enemy3D*>(eobj);
				if (Enemy != nullptr)
				{
					// 敵の動きを停止させる
					Enemy->SetStopItem(true);
				}
			}
		}
	}

	// -------------------------------------------------
	// タイマー動作中：カウントアップして残り時間を管理する
	// -------------------------------------------------
	if (m_isTimerActive)
	{
		// タイマーを1フレーム分進める
		m_waitTimer++;

		// 20秒（1200フレーム）経過したかチェック
		if (m_waitTimer >= WaitFrame)
		{
			// -----------------------------------------
			// 時間切れ！敵の動きを再開する
			// -----------------------------------------

			// 全ての敵の動きを再開する
			auto eobjList = Master::m_sceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DListByTag(Object3D::TagEnemy3D);
			for (int i = 0; i < (int)eobjList.size(); i++)
			{
				auto eobj = eobjList[i];
				if (eobj != nullptr)
				{
					Enemy3D* Enemy = dynamic_cast<Enemy3D*>(eobj);
					if (Enemy != nullptr)
					{
						// 敵の動きを再開させる
						Enemy->SetStopItem(false);
					}
				}
			}

			// アイテム使用フラグをリセット
			pPlayer->SetUseStopItem(false);

			// タイマーをリセット
			m_waitTimer = 0;

			// タイマーを停止状態にする
			m_isTimerActive = false;
		}
	}
}

void TimeItem::DrawTimer()
{
	// タイマーが動いていないなら何も表示しない
	if (!m_isTimerActive)
	{
		return;
	}

	// -------------------------------------------------
	// 残り時間を計算する（フレーム数 → 秒数に変換）
	// -------------------------------------------------
	// WaitFrame（1200）から現在のタイマー値を引いて、60で割ると残り秒数になる
	int remainingFrames = WaitFrame - m_waitTimer;       // 残りフレーム数
	int remainingSeconds = remainingFrames / 60;       // 残り秒数（小数切り捨て）

	// 0秒以下にならないようにする
	if (remainingSeconds < 0)
	{
		remainingSeconds = 0;
	}

	// -------------------------------------------------
	// 画面上部の中央に残り時間を表示する
	// -------------------------------------------------
	// 表示位置（画面上部の中央あたり）
	int drawX = Utility::SCREEN_WIDTH / 2 - 200;
	int drawY = 80;

	// 残り時間が5秒以下なら赤色、それ以外は白色で表示
	unsigned int color;
	if (remainingSeconds <= 5)
	{
		// 残り5秒以下：赤色で警告
		color = GetColor(255, 80, 80);
	}
	else
	{
		// 通常：白色
		color = GetColor(255, 255, 255);
	}

	// 「敵停止中：残り○○秒」と表示する
	DrawFormatStringToHandle(drawX, drawY, color, m_timerFontHandle,
		"敵停止中：残り %d 秒", remainingSeconds);
}