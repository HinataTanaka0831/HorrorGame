#include "TimeItem.h"
#include "Model.h"
#include "Master.h"
#include "Scene.h"
#include "ObjectManager.h"
#include "Enemy3D.h"
#include "Player3D.h"
#include "Utility.h"

int  TimeItem::waitTimer       = 0;
bool TimeItem::isTimerActive   = false;
int  TimeItem::timerFontHandle = -1;

TimeItem::TimeItem(std::string filename, VECTOR initPos, bool isSeparateAnim)
	: Item(initPos)
	, mpModel(nullptr)
{
	SetTag(Object3D::TagTimeItem);
	mpModel = new Model(filename, initPos, isSeparateAnim);

	if (timerFontHandle == -1)
	{
		timerFontHandle = CreateFontToHandle(NULL, 50, -1, DX_FONTTYPE_ANTIALIASING);
	}
}

TimeItem::~TimeItem()
{
	if (mpModel != nullptr)
	{
		delete mpModel;
		mpModel = nullptr;
	}
}

// タイマー経過による敵AI停止制御およびモデル更新
// 入力: なし / 出力: なし / 副作用: StopTime実行、3Dモデル更新
void TimeItem::Update()
{
	StopTime();

	if (mpModel != nullptr)
	{
		mpModel->SetPosition(mvPosition);
		mpModel->Update();
	}
}

// アイテム3Dモデル描画
// 入力: なし / 出力: なし / 副作用: バックバッファへの描画
void TimeItem::Draw()
{
	if (mpModel != nullptr)
	{
		mpModel->Draw();
	}
}

// プレイヤーのアイテム使用を検知し全敵AIの停止フラグを20秒間制御
// 入力: なし / 出力: なし / 副作用: 全Enemy3DのisStopItem切り替え
void TimeItem::StopTime()
{
	auto player = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DByTag(Object3D::TagPlayer3D);
	Player3D* pPlayer = dynamic_cast<Player3D*>(player);

	if (pPlayer == nullptr)
	{
		return;
	}

	bool isUseStopItem = pPlayer->GetIsUseStopItem();

	// アイテム使用開始時に全敵AIの移動を無効化
	if (isUseStopItem && !isTimerActive)
	{
		isTimerActive = true;
		waitTimer = 0;

		auto eobjList = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DListByTag(Object3D::TagEnemy3D);
		for (auto eobj : eobjList)
		{
			Enemy3D* Enemy = dynamic_cast<Enemy3D*>(eobj);
			if (Enemy != nullptr)
			{
				Enemy->SetStopItem(true);
			}
		}
	}

	// 20秒（1200フレーム）経過で敵AIの移動を再開
	if (isTimerActive)
	{
		waitTimer++;
		if (waitTimer >= WaitFrame)
		{
			auto eobjList = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DListByTag(Object3D::TagEnemy3D);
			for (auto eobj : eobjList)
			{
				Enemy3D* Enemy = dynamic_cast<Enemy3D*>(eobj);
				if (Enemy != nullptr)
				{
					Enemy->SetStopItem(false);
				}
			}

			pPlayer->SetUseStopItem(false);
			waitTimer = 0;
			isTimerActive = false;
		}
	}
}

// 敵停止効果の残り秒数カウントダウンHUD描画
// 入力: なし / 出力: なし / 副作用: バックバッファへのUI描画
void TimeItem::DrawTimer()
{
	if (!isTimerActive)
	{
		return;
	}

	int remainingFrames = WaitFrame - waitTimer;
	int remainingSeconds = remainingFrames / 60;
	if (remainingSeconds < 0) remainingSeconds = 0;

	int drawX = Utility::SCREEN_WIDTH / 2 - 200;
	int drawY = 80;

	// 残り時間低下（5秒以下）時の警告色切り替え
	unsigned int color = (remainingSeconds <= 5) ? GetColor(255, 80, 80) : GetColor(255, 255, 255);

	DrawFormatStringToHandle(drawX, drawY, color, timerFontHandle, "敵停止中：残り %d 秒", remainingSeconds);
}