#pragma once

// 3Dキャラクターアニメーションのステート定義
enum AnimationState
{
    ANIMATION_NEUTRAL = 0,  // 待機モーション
    ANIMATION_RUN,          // 走行モーション
    ANIMATION_JUMP_IN,      // ジャンプ開始予備動作
    ANIMATION_JUMP_LOOP,    // 滞空中ループ動作
    ANIMATION_JUMP_OUT,     // 着地硬直動作
    ANIMATION_ATTACK,       // 攻撃アクション
    ANIMATION_WALKING,      // 徘徊・歩行動作
    ANIMATION_DYING,        // 死亡・ダウン動作
    ANIMATION_DROP_KICK,    // 特殊攻撃動作
    ANIMATION_LEFT_TURN,    // 左旋回
    ANIMATION_RIGHT_TURN,   // 右旋回

    ANIMATION_MAX
};

// 外部ファイルから個別読み込みされたアニメーションハンドルとステートの対応構造体
struct AnimationInfo
{
    AnimationState mState;
    int mnAnimationHandle;
};