#pragma once

// 
// ★New★
// 新規追加ファイル
//


// アニメーションの切り替え番号
enum AnimationState
{
    AnimationNeutral = 0,  // 待機
    AnimationRun,          // 走り
    AnimationJumpIn,      // ジャンプ開始
    AnimationJumpLoop,    // ジャンプ中
    AnimationJumpOut,     // 着地時
    AnimationAttack,       // 攻撃
    // note: ↑ここから上には追加しない。
    //       Hero.mv1 がこの順番じゃないと機能しないので維持しておく。
    //       ただし、SeparateModelAnimation の方で使ってはいけない訳ではないので注意。

    // ★New★
    // 上にある定義以外で使いたいモーションがあれば、このように下に追加していけばOK。
    AnimationWalking,      // 歩き
    AnimationDying,        // 死亡時
    AnimationDropKick,    // ドロップキック
    AnimationLeftTurn,    // 左回転
    AnimationRightTTurn,   // 右回転

    AnimationMax
};

// ★New★
// 分割されているアニメーションのデータ
struct AnimationInfo
{
    AnimationState m_state;
    int m_animationHandle;
};