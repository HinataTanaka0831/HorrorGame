#pragma once

// 
// ★New★
// 新規追加ファイル
//

#include <string>
#include <vector>
#include "ModelUtility.h"

// 外部ファイル群から個別に読み込んだMixamo系モーションの動的アタッチ・ブレンド再生を管理するクラス
class SeparateModelAnimation
{
public:
    SeparateModelAnimation(int modelHandle);   // コンストラクタ
    ~SeparateModelAnimation();  // デストラクタ

    // モーション再生時間の進行および前後モーションのクロスフェードブレンド制御
    // 入力: なし / 出力: なし / 副作用: アタッチアニメーション再生時間・ブレンド率の更新
    void Update();  

    // 指定ステートへのモーション変更およびクロスフェード開始
    // 入力: state(切り替え先ステート), index(アタッチインデックス) / 出力: なし / 副作用: 旧モーションデタッチ予約、新モーションアタッチ
    void ChangeAnimation(AnimationState state, int index = 0); // モーション切り替え処理

    // 外部モーションファイルのロードとステート登録
    // 入力: state(ステート), filename(ファイルパス) / 出力: なし / 副作用: MV1LoadModel実行、mAnimationInfoListへの登録
    void AddAnimation(AnimationState state, std::string filename);

    // 登録済みモーションハンドルの検索取得
    // 入力: state(ステート) / 出力: モーションモデルハンドル(-1で見つからない) / 副作用: なし
    int GetAnimationHandle(AnimationState state);


    // ループ設定
    void SetLoop(bool isLoop) { mbLoop = isLoop; }
    // ループ終了時に再生するモーション
    void SetLoopFinishState(AnimationState state) { mnLoopFinishState = state; }
    // モーションのブレンド設定
    void SetAnimationBlend(bool isBlend);

    // 現在再生されているモーションの取得
    AnimationState GetNowState() { return mnState; }
    // モーションのループが終了しているかどうか
    bool IsLoopFinish() { return mbLoopFinish; }

private:
    int mnModelHandle;  // モデルのハンドル

    float mfAnimationTime;  // 再生しているモーションの現在の再生時間
    int mnAnimationIndex;   // 再生しているモーションのインデックス

    float mfOldAnimationTime;   // 1つ前のモーション再生時間
    int mnOldAnimationIndex;    // １つ前のモーションのインデックス

    float mfAnimBlendRate;      // モーションの切り替わり度合

    AnimationState mnState; // 現在再生しているモーションの番号

    bool mbLoop;            // モーションをループさせるかどうか
    AnimationState mnLoopFinishState;   // ループが終わった時に再生したいモーション番号
    bool mbLoopFinish;      // モーションループが終わったかどうか

    // ★New★
    // モーションデータのリスト
    std::vector<AnimationInfo*> mAnimationInfoList;
};