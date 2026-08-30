#include "Player3D.h"
#include "Master.h"
#include "InputManager.h"
#include "Scene.h"
#include "ObjectManager.h"
#include "Enemy3D.h"
#include "Stage.h"
#include "Collision.h"
#include "Exitdoor.h"
#include "EscapeItem.h"
#include "TimeItem.h"

Player3D::Player3D(VECTOR initPos)
	: Object3D(initPos)
	, isMove(false)
	, isCrouching(false)
	, isFreeze(false)
	, isActiveLight(true)
	, isUseStopItem(false)
	, getEscapeItem(false)
	, getTimeItem(false)
	, oldPosition(VGet(0.0f, 0.0f, 0.0f))
	, oldPlayerPosition(mvPosition)
	, lightModelPosition(VGet(0.0f, 0.0f, 0.0f))
	, DoorPos(VGet(0.0f, 0.0f, 0.0f))
	, dis(0.0f)
	, mfAngle(0.0f)
	, mfTargetAngle(0.0f)
	, playerHeight(0.0f)
	, playerSpeed(0.0f)
	, gaugeWidth(0)
	, mncount(0)
	, RecordDis(0)
	, AddItemID(0)
	, WalkSETimer(0)
	, RunSETimer(0)
	, currentItemIndex(0)
	, x(0)
	, y(0)
	, selX(0)
	, selY(0)
{
	SetTag(Object3D::TagPlayer3D);
	lightHandle = MV1LoadModel("Resource/3D/Linterna/Linterna.mv1");
}

Player3D::~Player3D()
{
	if (lightHandle != -1)
	{
		MV1DeleteModel(lightHandle);
	}
}

// 入力・移動・壁衝突押し出し・ライト同期・足音・スタミナ更新の一括実行
// 入力: なし / 出力: なし / 副作用: 座標・スタミナ・ライト状態・SE再生の更新
void Player3D::Update()
{
	if (isFreeze)
	{
		return;
	}

	MoveEx();
	PlayerSquat();
	HaveLight();
	ItemCollision();
	UseItem();
}

// 懐中電灯3DモデルおよびアイテムHUD枠の描画
// 入力: なし / 出力: なし / 副作用: バックバッファへの描画
void Player3D::Draw()
{
	if (lightHandle != -1)
	{
		MV1DrawModel(lightHandle);
	}

	ItemBox();
}

// カメラ視線方向を基準とした前後左右移動ベクトルの算出およびステージ壁コリジョン押し出し
// 入力: なし / 出力: なし / 副作用: プレイヤー座標mvPositionの更新
void Player3D::MoveEx()
{
	isMove = false;
	oldPosition = mvPosition;

	VECTOR camPos = Master::mpCamera->GetPosition();
	VECTOR camLook = Master::mpCamera->GetLookAtPosition();

	VECTOR forward = VSub(camLook, camPos);
	forward.y = 0.0f;
	forward = VNorm(forward);

	VECTOR right = VCross(VGet(0.0f, 1.0f, 0.0f), forward);
	right = VNorm(right);

	VECTOR moveDir = VGet(0.0f, 0.0f, 0.0f);

	if (CheckHitKey(KEY_INPUT_W)) { moveDir = VAdd(moveDir, forward); isMove = true; }
	if (CheckHitKey(KEY_INPUT_S)) { moveDir = VSub(moveDir, forward); isMove = true; }
	if (CheckHitKey(KEY_INPUT_D)) { moveDir = VSub(moveDir, right); isMove = true; }
	if (CheckHitKey(KEY_INPUT_A)) { moveDir = VAdd(moveDir, right); isMove = true; }

	// ダッシュ可否とスタミナ消費判定
	bool isRunKey = CheckHitKey(KEY_INPUT_LSHIFT) != 0;
	bool canRun = StaminaUpdate(isRunKey && isMove && !isCrouching);

	if (canRun && isRunKey && !isCrouching)
	{
		state = RUN;
		playerSpeed = 120.0f;
	}
	else
	{
		state = WALK;
		playerSpeed = isCrouching ? 35.0f : 70.0f;
	}

	if (isMove && VSize(moveDir) > 0.0f)
	{
		moveDir = VNorm(moveDir);
		mvPosition = VAdd(mvPosition, VScale(moveDir, playerSpeed * (1.0f / 60.0f) * 10.0f));
	}

	// 敵AI追跡用の移動履歴サンプリング（一定距離移動毎に記録）
	float moveDist = VSize(VSub(mvPosition, oldPlayerPosition));
	if (moveDist > 100.0f)
	{
		playerRecord.push_back(mvPosition);
		oldPlayerPosition = mvPosition;
		if (playerRecord.size() > 50)
		{
			playerRecord.erase(playerRecord.begin());
		}
	}

	// ステージ壁面ポリゴンとの衝突判定およびスライド押し出し
	auto pStageObj = Master::mpSceneManager->GetCurrentScene()->GetObjectManager()->GetObject3DByTag(Object3D::TagStage);
	Stage* pStage = dynamic_cast<Stage*>(pStageObj);
	if (pStage != nullptr)
	{
		VECTOR p1 = VAdd(mvPosition, VGet(0.0f, 30.0f, 0.0f));
		VECTOR p2 = VAdd(mvPosition, VGet(0.0f, 150.0f, 0.0f));
		if (pStage->CheckHit_Capsule(p1, p2, 40.0f))
		{
			VECTOR hitNormal = pStage->GetNormal();
			hitNormal.y = 0.0f;
			if (VSize(hitNormal) > 0.0f)
			{
				hitNormal = VNorm(hitNormal);
				mvPosition = VAdd(mvPosition, VScale(hitNormal, 2.0f));
			}
		}
	}

	// 移動状態に応じた足音SEの周期的再生
	if (isMove)
	{
		if (state == RUN)
		{
			RunSETimer++;
			if (RunSETimer >= SEframe_RUN)
			{
				Master::mpSoundManager->PlaySE(SoundManager::SE_Walk);
				RunSETimer = 0;
			}
		}
		else
		{
			WalkSETimer++;
			if (WalkSETimer >= SEframe_Walk)
			{
				Master::mpSoundManager->PlaySE(SoundManager::SE_Walk);
				WalkSETimer = 0;
			}
		}
	}
	else
	{
		WalkSETimer = 0;
		RunSETimer = 0;
	}
}

// しゃがみ/直立姿勢の切り替えおよび視線高・移動速度の補正
// 入力: なし / 出力: なし / 副作用: isCrouchingフラグおよびplayerHeight/playerSpeedの変更
void Player3D::PlayerSquat()
{
	if (InputManager::GetInstance().CheckDownKey(KEY_INPUT_E))
	{
		isCrouching = !isCrouching;
	}
	playerHeight = isCrouching ? 50.0f : 0.0f;
}

// 懐中電灯の点灯トグルおよびカメラ視線へのライトモデル・ディレクショナル光源同期
// 入力: なし / 出力: なし / 副作用: DXライブラリライト有効無効切り替え
void Player3D::HaveLight()
{
	if (InputManager::GetInstance().CheckDownKey(KEY_INPUT_F))
	{
		isActiveLight = !isActiveLight;
	}

	VECTOR camPos = Master::mpCamera->GetPosition();
	VECTOR camLook = Master::mpCamera->GetLookAtPosition();
	VECTOR dir = VNorm(VSub(camLook, camPos));

	// 懐中電灯モデルを画面右下にオフセット配置してFPS手元を表現
	VECTOR right = VNorm(VCross(VGet(0.0f, 1.0f, 0.0f), dir));
	lightModelPosition = VAdd(camPos, VScale(dir, 30.0f));
	lightModelPosition = VAdd(lightModelPosition, VScale(right, 15.0f));
	lightModelPosition.y -= 10.0f;

	MV1SetPosition(lightHandle, lightModelPosition);
	MV1SetRotationXYZ(lightHandle, VGet(0.0f, atan2f(dir.x, dir.z), 0.0f));
}

// 周囲の脱出アイテムおよび時間延長アイテムとの近接インタラクション判定
// 入力: なし / 出力: なし / 副作用: Rキー押下時のアイテム取得
void Player3D::ItemCollision()
{
	if (!InputManager::GetInstance().CheckDownKey(KEY_INPUT_R))
	{
		return;
	}

	auto objMgr = Master::mpSceneManager->GetCurrentScene()->GetObjectManager();

	// 脱出アイテム（鍵）の取得判定
	auto escapeList = objMgr->GetObject3DListByTag(Object3D::TagEscapeItem3D);
	for (auto obj : escapeList)
	{
		EscapeItem* item = dynamic_cast<EscapeItem*>(obj);
		if (item && !item->IsDeleteFlag())
		{
			if (Collision::CheckPointToCircle(item->GetPosition(), mvPosition, 150.0f))
			{
				ItemData data{ 1, "鍵", LoadGraph("Resource/3D_UI/escapeItem.png") };
				if (AddItem(data))
				{
					item->SetDeleteFlag(true);
					EscapeItem::NowNeedItem++;
					break;
				}
			}
		}
	}

	// 時間延長アイテムの取得判定
	auto timeList = objMgr->GetObject3DListByTag(Object3D::TagTimeItem);
	for (auto obj : timeList)
	{
		TimeItem* item = dynamic_cast<TimeItem*>(obj);
		if (item && !item->IsDeleteFlag())
		{
			if (Collision::CheckPointToCircle(item->GetPosition(), mvPosition, 150.0f))
			{
				ItemData data{ 2, "砂時計", LoadGraph("Resource/3D_UI/TimeItem.png") };
				if (AddItem(data))
				{
					item->SetDeleteFlag(true);
					break;
				}
			}
		}
	}
}

// 保持中アイテムの消費実行（時間停止発動または脱出ドア解錠フラグセット）
// 入力: なし / 出力: なし / 副作用: インベントリからの消費および各ギミック状態の更新
void Player3D::UseItem()
{
	if (InputManager::GetInstance().CheckDownKey(KEY_INPUT_SPACE) && !items.empty())
	{
		if (items[0].id == 2)
		{
			isUseStopItem = true;
			LoseItem();
		}
	}
}

// アイテムスロット枠および所持アイテムアイコンのHUD描画
// 入力: なし / 出力: なし / 副作用: バックバッファへのUI描画
void Player3D::ItemBox()
{
	// アイテム枠HUD
	DrawBox(HUD_X, HUD_Y, HUD_X + ItemSize, HUD_Y + ItemSize, GetColor(50, 50, 50), TRUE);
	DrawBox(HUD_X, HUD_Y, HUD_X + ItemSize, HUD_Y + ItemSize, GetColor(255, 255, 255), FALSE);

	if (!items.empty() && items[0].imageHandle != -1)
	{
		DrawExtendGraph(HUD_X + 10, HUD_Y + 10, HUD_X + ItemSize - 10, HUD_Y + ItemSize - 10, items[0].imageHandle, TRUE);
	}
}

// インベントリへのアイテム追加
// 入力: item(取得アイテムデータ) / 出力: 格納成功ならtrue / 副作用: items配列へのpush_back
bool Player3D::AddItem(const ItemData item)
{
	if (items.size() < (size_t)maxSize)
	{
		items.push_back(item);
		return true;
	}
	return false;
}

// インベントリからの先頭アイテム削除
// 入力: なし / 出力: なし / 副作用: items配列のpop
void Player3D::LoseItem()
{
	if (!items.empty())
	{
		if (items[0].imageHandle != -1)
		{
			DeleteGraph(items[0].imageHandle);
		}
		items.erase(items.begin());
	}
}

// スタミナゲージバーの画面描画
// 入力: なし / 出力: なし / 副作用: バックバッファへのHUD描画
void Player3D::DrawStamina()
{
	gaugeWidth = (int)((float)width * ((float)stamina / (float)staminaMAX));
	DrawBox(stamina_X, stamina_Y, stamina_X + width, stamina_Y + height, GetColor(30, 30, 30), TRUE);
	DrawBox(stamina_X, stamina_Y, stamina_X + gaugeWidth, stamina_Y + height, GetColor(50, 200, 50), TRUE);
	DrawBox(stamina_X, stamina_Y, stamina_X + width, stamina_Y + height, GetColor(255, 255, 255), FALSE);
}

// ダッシュ入力時のスタミナ消費および非ダッシュ時の自然回復処理
// 入力: isKeyProssese(シフトキー押下中か) / 出力: 走行可能状態ならtrue / 副作用: stamina現在値の増減
bool Player3D::StaminaUpdate(bool isKeyProssese)
{
	if (isKeyProssese)
	{
		if (stamina > 0)
		{
			stamina -= 1;
			return true;
		}
		return false;
	}
	else
	{
		if (stamina < staminaMAX)
		{
			stamina += 1;
		}
		return false;
	}
}