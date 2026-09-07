#pragma once

#include "DxLib.h"
#include <string>
#include <vector>
#include "ModelUtility.h"

// 外部ファイル群から個別に読み込んだMixamo系モーションの動的アタッチ・ブレンド再生を管理するクラス
class SeparateModelAnimation
{
public:
    // 呼び出されたアニメーションで移動しているフレームがある場合、移動を無効にするためのフレーム番号を検索し無効にさせる。初期アニメーション状態は待機モーションにしておく
    // 入力: modelHandle(モデルハンドル) / 出力: なし / 副作用: モデルのアタッチアニメーションの移動を無効化、初期アニメーション状態を待機モーションに設定
    SeparateModelAnimation(int modelHandle);   
    ~SeparateModelAnimation();  

    // モーション再生時間の進行および前後モーションのクロスフェードブレンド制御
    // 入力: なし / 出力: なし / 副作用: アタッチアニメーション再生時間・ブレンド率の更新
    void Update();  

    // 指定ステートへのモーション変更およびクロスフェード開始
    // 入力: state(切り替え先ステート), index(アタッチインデックス) / 出力: なし / 副作用: 旧モーションデタッチ予約、新モーションアタッチ
    void ChangeAnimation(AnimationState state, int index = 0); // モーション切り替え処理

    // 外部モーションファイルのロードとステート登録
    // 入力: state(ステート), fileName(ファイルパス) / 出力: なし / 副作用: MV1LoadModel実行、mAnimationInfoListへの登録
    void AddAnimation(AnimationState state, std::string fileName);

    // 登録済みモーションハンドルの検索取得
    // 入力: state(ステート) / 出力: モーションモデルハンドル(-1で見つからない) / 副作用: なし
    int GetAnimationHandle(AnimationState state);


    // ループ設定
    void SetLoop(bool isLoop) { m_loop = isLoop; }
    // ループ終了時に再生するモーション
    void SetLoopFinishState(AnimationState state) { m_loopFinishState = state; }
    // モーションのブレンド設定
    void SetAnimationBlend(bool isBlend);

    // 現在再生されているモーションの取得
    AnimationState GetNowState() { return m_state; }
    // モーションのループが終了しているかどうか
    bool IsLoopFinish() { return m_loopFinish; }

private:
    int m_modelHandle;  // モデルのハンドル

    float m_animationTime = 0.0f;  // 再生しているモーションの現在の再生時間
    int m_animationIndex = -1;   // 再生しているモーションのインデックス

    float m_oldAnimationTime = 0.0f;   // 1つ前のモーション再生時間
    int m_oldAnimationIndex = -1;    // １つ前のモーションのインデックス

    float m_animBlendRate = 1.0f;      // モーションの切り替わり度合

    AnimationState m_state = AnimationState::AnimationMax; // 現在再生しているモーションの番号  最初は最大値にしておく

    bool m_loop = true;            // モーションをループさせるかどうか
    AnimationState m_loopFinishState = AnimationState::AnimationMax;   // ループが終わった時に再生したいモーション番号
    bool m_loopFinish = false;      // モーションループが終わったかどうか

    // モーションデータのリスト
    std::vector<AnimationInfo*> m_animationInfoList = {};
};