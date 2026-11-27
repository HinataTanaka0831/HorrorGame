#include "Player3D.h"
#include "Master.h"
#include "Scene.h"
#include "ObjectManager.h"
#include "Stage.h"
#include "Enemy3D.h"
#include "EscapeItem.h"
#include "TimeItem.h"
#include "Exitdoor.h"


Player3D::Player3D(VECTOR initPosition)
	: Object3D(initPosition)
	, SEframeWalk(25)
	, SEframeRUN(18)
	, ItemSize(170)
	, ItemMargin(10)
	, DrawX(Utility::SCREEN_WIDTH / 2 + 850 - (ItemSize + ItemMargin))
	, DrawY(Utility::SCREEN_HEIGHT / 2 + 550 - ItemSize - 20)
	, MaxSize(1)
	, MaxStamina(100)
	, SplitCount(4)
	, m_moveVec(VGet(0.0f, 0.0f, 0.0f))
	, m_oldPosition(VGet(0.0f, 0.0f, 0.0f))
	, m_oldFootprintPosition(VGet(0.0f, 0.0f, 0.0f))
	, m_playerHeight(80.0f)
	, m_distance(0.0f)
	, m_speed(70.0f)
	, m_capsule{ VGet(0.0f, 0.0f, 0.0f), VGet(0.0f, 0.0f, 0.0f), 0.0f }
	, m_stamina(MaxStamina)
	, m_staminaX(Utility::SCREEN_WIDTH / 2 - 260)
	, m_staminaY(Utility::SCREEN_HEIGHT / 2 + 430)
	, m_width(490)
	, m_height(30)
	, m_gaugeWidth(0)
	, m_walkSETimer(0)
	, m_runSETimer(0)
	, m_state(MoveState::Walk)
	, m_isMove(false)
	, m_isCrouching(false)
	, m_isFreeze(false)
	, m_isActiveLight(true)
	, m_isGetEscapeItem(false)
	, m_isGetTimeItem(false)
	, m_isUseStopItem(false)
	, m_lightHandle(-1)
	, m_fontHandle(CreateFontToHandle(NULL, 40, -1, DX_FONTTYPE_ANTIALIASING))
	, m_itemBoxX(0)
	, m_itemBoxY(0)
	, m_addItemID(0)
	, m_escapeItem(nullptr)
	, m_timeItem(nullptr)
	, m_enemy(nullptr)
	, m_exitDoor(nullptr)
	, m_stage(nullptr)

{
	// タグ設定
	SetTag(Object3D::TagPlayer3D);

	// 懐中電灯のモデルを生成
	m_lightHandle = MV1LoadModel("Resource/Object/light/object_light.mv1");

	// 足跡記録用のコンテナの容量を大体の個数(300個)事前に確保しておくことで、大体の個数(300個)以下までは再確保が発生しないようにする
	m_footprintsRecord.reserve(300);
}

Player3D::~Player3D()
{
	// 懐中電灯のモデルを削除
	MV1DeleteModel(m_lightHandle);
}

void Player3D::Update()
{
	// 移動処理
	Move();

	// アイテムとの当たり判定処理
	ItemCollision();

	// アイテムの使用処理
	UseItem();

	// ライトの処理
	//HaveLight();

	// ライトのON/OFFを変更
	//SetLightEnable(m_isActiveLight);

}

void Player3D::Draw()
{
	// 懐中電灯のモデルを描画
	MV1DrawModel(m_lightHandle);

	// デバッグ表示
	DrawFormatStringToHandle(0, 80, GetColor(255, 255, 255), m_fontHandle, "m_position.x : %.2f, m_position.y : %.2f, m_position.z : %.2f", m_position.x, m_position.y, m_position.z);

}

void Player3D::UpdateAABB()
{
	// AABBのサイズをプレイヤーの幅と高さに基づいて更新
	float width = 40.0f;
	float height = 80.0f;

	m_aabb.min = { m_position.x - width, m_position.y, m_position.z - width };
	m_aabb.max = { m_position.x + width, m_position.y + height, m_position.z + width };
}

void Player3D::UpdateCapsule()
{
	// カプセルの始点と終点をプレイヤーの座標に基づいて更新
	m_capsule.start = m_position;
	m_capsule.end = VAdd(m_position, VGet(0.0f, m_playerHeight, 0.0f));
	m_capsule.radius = 30.0f;
}

void Player3D::GetInputMoveDirection()
{
	m_moveVec = VGet(0.0f, 0.0f, 0.0f);
	VECTOR upMoveVector = VGet(0.0f, 0.0f, 0.0f);    // カメラの上方向（奥方向）ベクトル
	VECTOR leftMoveVector = VGet(0.0f, 0.0f, 0.0f);  // カメラの左方向ベクトル
	VECTOR downMoveVector = VGet(0.0f, 0.0f, 0.0f);  // カメラの下方向ベクトル
	VECTOR rightMoveVector = VGet(0.0f, 0.0f, 0.0f); // カメラの右方向ベクトル
	// カメラの向きから移動ベクトルを求める。上方向への移動ベクトルは、カメラの視線方向からY成分を抜いたものとする
	upMoveVector = VSub(Master::m_camera->GetLookAtPosition(), Master::m_camera->GetPosition());
	upMoveVector.y = 0.0f;
	// 左方向への移動ベクトルは、上方向への移動ベクトルと、Y軸のプラス方向へのベクトルに垂直な方向（外積）
	leftMoveVector = VCross(upMoveVector, VGet(0.0f, 1.0f, 0.0f));
	leftMoveVector.y = 0.0f;
	// 移動ベクトルは移動量を加味しないので、正規化しておく（ベクトルの長さを１にすること）
	upMoveVector = VNorm(upMoveVector);
	leftMoveVector = VNorm(leftMoveVector);
	// 左方向
	if (Master::m_input->CheckPressKey(KEY_INPUT_A)) m_moveVec = VAdd(m_moveVec, leftMoveVector);
	// 右方向
	if (Master::m_input->CheckPressKey(KEY_INPUT_D)) m_moveVec = VAdd(m_moveVec, VScale(leftMoveVector, -1.0f)); // VScale...掛け算　-1.0fを掛けて反転している
	// 奥方向
	if (Master::m_input->CheckPressKey(KEY_INPUT_W)) m_moveVec = VAdd(m_moveVec, upMoveVector);
	// 手前方向
	if (Master::m_input->CheckPressKey(KEY_INPUT_S)) m_moveVec = VAdd(m_moveVec, VScale(upMoveVector, -1.0f));
	// 移動している状態であれば
	m_isMove = (m_moveVec.x != 0.0f || m_moveVec.z != 0.0f);
	if (m_isMove) m_moveVec = VNorm(m_moveVec);  // 移動方向を正規化しておく
}

void Player3D::PlayerSquat()
{
	// Eキーを押したらしゃがむ
	if (Master::m_input->CheckPressKey(KEY_INPUT_E)) {
		m_isCrouching = true;
	}
	else {
		m_isCrouching = false;
	}
	if (m_isCrouching) {
		m_playerHeight = 60.0f;  	// プレイヤーの高さを変更
		m_speed = 3.0f;       		// プレイヤーのスピードを変更
	}
	else {  // 初期値と同じ
		m_playerHeight = 80.0f;
		m_speed = 6.0f;
	}
}

void Player3D::UpdateSpeedAndFootsteps()
{
	PlayerSquat();
	if (StaminaUpdate(Master::m_input->CheckPressKey(KEY_INPUT_LSHIFT))) {
		if (m_state == MoveState::Run && m_isMove) {
			m_speed = m_speed * 3.0f;  // 速度を走った時の速度にする
			m_runSETimer++;
			// 足音を走った時の速度に合わせるように再生する
			if (m_runSETimer >= SEframeRUN) {
				Master::m_soundManager->PlaySE(SoundManager::SeFootSteps);
				m_runSETimer = 0;
			}
		}
		else {
			return;
		}
	}
	else {
		if (m_state == MoveState::Walk && m_isMove) {
			m_walkSETimer++;
			// 足音を歩いた時の速度に合わせるように再生する
			if (m_walkSETimer >= SEframeWalk) {
				Master::m_soundManager->PlaySE(SoundManager::SeFootSteps);
				m_walkSETimer = 0;
			}
		}
	}
}

void Player3D::RecordFootprint()
{
    // 足跡リストが200より大きくなったら古いものから消していく
	if (m_footprintsRecord.size() > 200) m_footprintsRecord.erase(m_footprintsRecord.begin());
	m_distance = VSize(VSub(m_position, m_oldFootprintPosition)); 	// ベクトルのサイズを取得・距離を計測
	// プレイヤーの座標を保存して、古い足跡座標を更新する
	if (m_distance >= 15.0f) {
		m_footprintsRecord.push_back(m_position);
		m_oldFootprintPosition = m_position;
	}
}

void Player3D::ResolveWalls(int& wallNum)
{
	if (m_stage == nullptr) return;
	// ステージにある「壁」との当たり判定処理
	for (const auto& wall : m_stage->GetWalls()) {
		if (Collision::CheckAABB(m_aabb, wall)) {
			wallNum++;    // 当たった壁の枚数を数える
			float overlapX = min(m_aabb.max.x, wall.max.x) - max(m_aabb.min.x, wall.min.x);   // プレイヤーが壁にどれだけめり込んだのかを求める
			float overlapZ = min(m_aabb.max.z, wall.max.z) - max(m_aabb.min.z, wall.min.z);
			// 最も少ない移動で壁の外に出られる方向を求めるために、X軸方向の侵入量とZ軸方向の侵入量を比較して、少ない方の法線を取得する
			VECTOR normal = VGet(0.0f, 0.0f, 0.0f);
			if (overlapX < overlapZ) {
				normal.x = (m_moveVec.x > 0.0f) ? 1.0f : -1.0f;
			}
			else {
				normal.z = (m_moveVec.z > 0.0f) ? 1.0f : -1.0f;
			}
			VECTOR wallSlider = VGet(0.0f, 0.0f, 0.0f);
			float dot = VDot(m_moveVec, normal);  	            // 移動方向ベクトルと取得した衝突法線との内積を求める
			wallSlider = VSub(m_moveVec, VScale(normal, dot));  //　壁沿いベクトルの計算
			wallSlider = VNorm(wallSlider); 			        // 壁沿いベクトルを正規化する(ベクトルの長さを1にする)
			wallSlider.y = 0.0f;                                // Y成分はなくす
			// 当たった壁の枚数が2枚以上なら移動を止めて、それ以外ならスライド処理を行う
			if (wallNum > SplitCount) {
				m_position = m_oldPosition;
			}
			else {
				m_position = VAdd(m_oldPosition, VScale(wallSlider, m_speed));
			}
			UpdateAABB();  	 // もう一度プレイヤーのAABB判定を更新させる
		}
	}
}

void Player3D::ResolveStairs()
{
	// ステージにある「階段」との当たり判定処理（坂による当たり判定）
	for (const auto& stairs : m_stage->GetStairs()) {
		if (Collision::CheckCapsuleAABB(m_capsule, stairs)) {
			float t = 0.0f;
			// 階段の方向に応じて、プレイヤーの座標を階段の傾斜に沿って補正する
			if (stairs.direction == "+") {
				t = (m_position.z - stairs.min.z) / (stairs.max.z - stairs.min.z);
			}
			else {
				t = (stairs.max.z - m_position.z) / (stairs.max.z - stairs.min.z);
			}
			// tの値を0.0fから1.0fの範囲に制限する
			if (t < 0.0f) t = 0.0f;
			if (t > 1.0f) t = 1.0f;
			// 階段の範囲内にプレイヤーがいる場合のみ、Y座標を補正する
			if (m_position.x >= stairs.min.x &&
				m_position.x <= stairs.max.x &&
				m_position.z >= stairs.min.z &&
				m_position.z <= stairs.max.z) 
			{
				float slopeY = stairs.min.y + (stairs.max.y - stairs.min.y) * t;
				m_position.y = slopeY;
			}
			UpdateCapsule(); 	// もう一度プレイヤーのカプセル判定を更新させる
		}
	}
}

void Player3D::ResolveFloors()
{
	// ステージの「床」との当たり判定処理
	for (const auto& floors : m_stage->GetFloors()) {
		if (Collision::CheckCapsuleAABB(m_capsule, floors)) {
			// プレイヤーの座標が床の設定した幅上にいるかどうかを確認
			bool insideX = m_position.x >= floors.min.x && m_position.z <= floors.max.x;
			bool insideY = m_position.y >= floors.min.y && m_position.y <= floors.max.y;
			bool insideZ = m_position.z >= floors.min.z && m_position.z <= floors.max.z;
			// プレイヤーの足元座標を出力
			float footY = m_position.y - m_playerHeight;
			// プレイヤーが床の幅上におり、足元のY座標が当たり判定のY座標以下ならプレイヤーのY座標を床に合わせる
			if (insideX && insideZ && insideY && footY <= floors.max.y) m_position.y = floors.max.y + m_playerHeight;
			UpdateCapsule();  	// もう一度プレイヤーのカプセル判定を更新させる
		}
	}
}

void Player3D::ResolveExitDoor()
{
	// ステージにある「脱出口」との当たり判定処理
	for (const auto& exitDoor : m_stage->GetExitDoor()) {
		if (Collision::CheckAABB(m_aabb, exitDoor)) {
			// 脱出口にどれだけめり込んだのかを求める
			float overlapX = min(m_aabb.max.x, exitDoor.max.x) - max(m_aabb.min.x, exitDoor.min.x);
			float overlapZ = min(m_aabb.max.z, exitDoor.max.z) - max(m_aabb.min.z, exitDoor.min.z);
			// 最も少ない移動で当たり判定の外に出られる方向を求めるために侵入量を比較して、値が少ないほうの法線を取得する。
			VECTOR normal = VGet(0.0f, 0.0f, 0.0f);
			if (overlapX < overlapZ) {
				normal.x = (m_moveVec.x > 0.0f) ? 1.0f : -1.0f;
			}
			else {
				normal.z = (m_moveVec.z > 0.0f) ? 1.0f : -1.0f;
			}
			VECTOR doorSlider = VGet(0.0f, 0.0f, 0.0f);
			float dot = VDot(m_moveVec, normal);  			    // 移動方向ベクトルと取得した衝突法線との内積を求める
			doorSlider = VSub(m_moveVec, VScale(normal, dot)); 	// 壁沿いベクトルの計算
			doorSlider = VNorm(doorSlider);        			    // 壁沿いベクトルを正規化する(ベクトルの長さを1にする)
			doorSlider.y = 0.0f;                                // Y成分をなくす
			m_position = VAdd(m_oldPosition, VScale(doorSlider, m_speed));
			UpdateAABB(); 		// もう一度プレイヤーのAABB判定を更新させる
		}
	}
}

void Player3D::MoveWithCollision()
{
	// ステージとの当たり判定処理
	int wallNum = 0;
	// 移動処理を分割して行うことで、壁にめり込むのを防ぐ
	for (int i = 0; i < SplitCount; i++)
	{
		m_position = VAdd(m_position, VScale(m_moveVec, m_speed / SplitCount));  // プレイヤーの座標を更新
		UpdateAABB();  		// 移動処理の後にプレイヤーのAABB判定を更新させる
		UpdateCapsule();    // 移動処理の後にプレイヤーのカプセル判定を更新させる
		ResolveWalls(wallNum);
		ResolveStairs();
		ResolveFloors();
		ResolveExitDoor();
	}
}

void Player3D::Move()
{
	if (m_isFreeze) return;
	m_oldPosition = m_position;  	//  前回の座標を更新

	GetInputMoveDirection();
	UpdateSpeedAndFootsteps();
	RecordFootprint();
	MoveWithCollision();
}

void Player3D::HaveLight()
{
	VECTOR lightPosition = MV1GetFramePosition(m_lightHandle, 0);  	// ライトモデルの光らせる位置を取得
	VECTOR camPos = Master::m_camera->GetPosition();    	        // カメラの座標を取得
	VECTOR worldUp = VGet(0.0f, 1.0f, 0.0f);            	        // ワールド座標の上方向
	VECTOR camLook = Master::m_camera->GetLookAtPosition();      	// カメラの注視点を取得
	VECTOR camDirection = VNorm(VSub(camLook, camPos));       	    // カメラの向きを取得
	VECTOR camRight = VNorm(VCross(worldUp, camDirection));      	// カメラの右方向を取得
	VECTOR camUp = VNorm(VCross(camDirection, camRight));        	// カメラの上方向を取得
	// ライトのモデルの位置を設定
	VECTOR lightModelPosition = camPos;
	lightModelPosition = VAdd(lightModelPosition, VScale(camDirection, 100.0f));
	lightModelPosition = VAdd(lightModelPosition, VScale(camRight, 85.0f));
	lightModelPosition = VAdd(lightModelPosition, VScale(camUp, -310.0f));
	// 懐中電灯のモデルの位置を設定
	MV1SetPosition(m_lightHandle, lightModelPosition);
	// ライトモデルを視点を動かしたら同じ方向に回転させる
	float angle = atan2f(camDirection.x, camDirection.z);
	float angleUpDown = atanf(camDirection.y);
	MV1SetRotationXYZ(m_lightHandle, VGet(-angleUpDown, angle, 0.0f));
	SetLightDifColor(GetColorF(1.0f, 0.8f, 0.4f, 0.0f)); 	// ディフューズカラー（オレンジっぽい光）
	// ライトをスポットライトに変更する
	ChangeLightTypeSpot(
	lightPosition,      // ライト位置
	camDirection,       // カメラの向き
	DX_PI_F / 3.5f, 
	DX_PI_F / 2.0f, 
	2000.0f,            // 有効距離
	0.05f,              // 減衰係数1
	0.00020f,           // 減衰係数2
	0.0f                // 減衰係数3
	);

	if (m_enemy == nullptr) return;
	bool getScare = m_enemy->GetScare();
	//  左クリックした場合ライトを点ける
	if (Master::m_input->CheckTriggerMouseClick(MOUSE_INPUT_LEFT)) m_isActiveLight = true;
	// ライトが照らされており、左クリックした場合ライトを消す
	if (GetLightEnable() == TRUE && !getScare) {
		if (Master::m_input->CheckTriggerMouseClick(MOUSE_INPUT_LEFT)) m_isActiveLight = false;
	}
	// ジャンプスケア状態の場合プレイヤーのライトを消す
	if (getScare) m_isActiveLight = false;
}

void Player3D::ItemCollision()
{
	// 脱出アイテム取得
	if (m_escapeItem == nullptr) return;
	if (Collision::CheckCircleToCircle(
		m_position,
		60.0f,
		m_escapeItem->GetPosition(),
		90.0f)
		) {
		// 近づいたら表示
		if (!m_isGetEscapeItem && !m_isGetTimeItem) DrawStringToHandle(Utility::SCREEN_WIDTH / 2 - 100, 900, "R : 取得", GetColor(255, 255, 255), m_fontHandle);
		if (Master::m_input->CheckPressKey(KEY_INPUT_R)) {
			m_isGetEscapeItem = true;
			AddItem(m_escapeItem->escape);  	 // 脱出アイテムをインベントリに追加
			m_escapeItem->SetDeleteFlag(true);
			m_addItemID = 1;                     // ItemIDを1にする
		}
	}
	// 時間止めアイテム取得
	if (m_timeItem == nullptr) return;
	if (Collision::CheckCircleToCircle(
		m_position,
		60.0f,
		m_timeItem->GetPosition(),
		90.0f)
		) {
		// 近づいたら表示
		if (!m_isGetTimeItem && !m_isGetEscapeItem) DrawStringToHandle(Utility::SCREEN_WIDTH / 2 - 100, 900, "R : 取得", GetColor(255, 255, 255), m_fontHandle);
		if (Master::m_input->CheckPressKey(KEY_INPUT_R)) {
			m_isGetTimeItem = true;
			AddItem(m_timeItem->stoptime);    	// 時間止めアイテムをインベントリに追加
			m_timeItem->SetDeleteFlag(true);
			m_addItemID = 2;           			// ItemIDを2にする
		}
	}
}

void Player3D::UseItem()
{
	// 脱出アイテムを持っていたら
	if (m_addItemID == 1) {
		if (m_exitDoor == nullptr) return;
		if (Collision::CheckCircleToCircle(
			m_position,
			100.0f,
			m_exitDoor->GetPosition(),
			100.0f)
			) {
			// 近づいたら表示
			if (m_isGetEscapeItem) DrawStringToHandle(Utility::SCREEN_WIDTH / 2 - 100, 900, "F : 使用", GetColor(255, 255, 255), m_fontHandle);
			if (Master::m_input->CheckPressKey(KEY_INPUT_F)) {
				LoseItem();
				m_isGetEscapeItem = false;  // 脱出アイテムを持っていない
			}
		}
	}
	// 時間止めアイテムを持っていたら
	if (m_isGetTimeItem) DrawStringToHandle(Utility::SCREEN_WIDTH / 2 - 100, 900, "F : 使用", GetColor(255, 255, 255), m_fontHandle);
	if (m_addItemID == 2) {
		if (Master::m_input->CheckPressKey(KEY_INPUT_F)) {
			if (!m_isUseStopItem) LoseItem();
			m_isGetTimeItem = false;  	// 時間止めアイテムを持っていない
		}
	}
}

void Player3D::ItemBox()
{
	// アイテムインベントリにアイテムがある場合
	for (size_t i = 0; i < m_items.size(); i++) {
		m_itemBoxX = DrawX + (int)i * (ItemSize + ItemMargin);
		m_itemBoxY = DrawY;
		// アイテムボックスの枠
		DrawBox(m_itemBoxX, m_itemBoxY, m_itemBoxX + ItemSize, m_itemBoxY + ItemSize, GetColor(200, 200, 200), false);
		// アイテム名の表示
		DrawStringToHandle(m_itemBoxX + 5, m_itemBoxY - 45, m_items[i].name, GetColor(255, 255, 255), m_fontHandle);
		// アイテム画像の表示
		DrawGraph(m_itemBoxX + 50, m_itemBoxY, m_items[i].image, true);
	}
}

bool Player3D::AddItem(const ItemData& item)
{
	// アイテムのサイズが最大数より少ない場合構造体に情報を追加する
	if (m_items.size() < MaxSize) {
		m_items.push_back(item);
		return true;
	}
	// それ以外なら何もしない
	return false;
}

void Player3D::LoseItem()
{
	// アイテムのサイズが最大数より小さい場合何もしない
	if (m_items.size() < MaxSize) return;

	for (size_t i = 0; i < m_items.size(); i++) {
		// アイテム番号が1の場合アイテムを脱出口で使用したときの処理を呼び出す
		if (m_items[i].id == 1) EscapeItem::Escape();
		// アイテム番号が 2 の場合のアイテムを使用したときの処理を呼び出す
		if (m_items[i].id == 2) m_isUseStopItem = true;
		m_items.clear();  	// アイテムの情報を削除する
	}
}

void Player3D::DrawStamina()
{
	int r = 20, g = 150, b = 20;
	// スタミナゲージが少なくなったら揺れて点滅表示されるようにする
	if ((float)m_stamina <= 0.5f) {
		int noise = GetRand(30) - 20;
		r = max(0, min(255, r + noise));
		int shake = 2 + (int)((0.2f - (float)m_stamina) * 20);
		m_staminaX += GetRand(shake * 2) - shake;
		m_staminaY += GetRand(shake * 2) - shake;
		float flicker = (float)sin(GetNowCount() * 0.02f);
		int alpha = 180 + (int)(75 * abs(flicker));
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
	}
	else {
		// スタミナゲージのX座標
		m_staminaX = Utility::SCREEN_WIDTH / 2 - 260; 
		// スタミナゲージのY座標
		m_staminaY = Utility::SCREEN_HEIGHT / 2 + 430; 
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	}
	// スタミナゲージの枠
	DrawBox(m_staminaX, m_staminaY, m_staminaX + m_width, m_staminaY + m_height, GetColor(0, 0, 0), true);
	// スタミナゲージ
	DrawBox(m_staminaX, m_staminaY, m_staminaX + m_gaugeWidth, m_staminaY + m_height, GetColor(r, g, b), true);
	// 枠線
	DrawBox(m_staminaX, m_staminaY, m_staminaX + m_width, m_staminaY + m_height, GetColor(255, 255, 255), false);
}

bool Player3D::StaminaUpdate(bool isDashKeyPressed)
{
	m_gaugeWidth = (int)((float)m_stamina / MaxStamina * m_width);   // 現在値に応じたスタミナゲージの幅
	// 走りキーを押して移動しているならならスタミナゲージを減らす
	if (isDashKeyPressed && m_stamina > 0 && m_isMove)
	{
		m_state = MoveState::Run;   // 走り状態にする
		m_stamina--;          		// スタミナゲージを減らす
		// 0以下なら歩き状態にする
		if (m_stamina <= 0) m_state = MoveState::Walk;
		return true;
	}
	else {
		m_state = MoveState::Walk;  // 歩き状態にする
		// 現在のスタミナゲージが最大値より少ない場合スタミナを増やす
		if (m_stamina < MaxStamina) m_stamina++;
		return false;
	}
}