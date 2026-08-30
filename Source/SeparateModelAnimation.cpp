#include "DxLib.h"
#include "SeparateModelAnimation.h"

SeparateModelAnimation::SeparateModelAnimation(int modelHandle)
    : mnModelHandle(modelHandle)
    , mfAnimationTime(0.0f)
    , mnAnimationIndex(-1)
    , mfOldAnimationTime(0.0f)
    , mnOldAnimationIndex(-1)
    , mfAnimBlendRate(1.0f)
    , mnState(AnimationState::ANIMATION_MAX)
    , mbLoop(true)
    , mnLoopFinishState(AnimationState::ANIMATION_MAX)
    , mbLoopFinish(false)
    , mAnimationInfoList()
{
    // Mixamoモデルのルートモーションによる予期せぬ位置ずれを防ぐためHipsフレームの移動を無効化
    int moveAnimFrameIndex = MV1SearchFrame(mnModelHandle, "mixamorig:Hips");
    MV1SetFrameUserLocalMatrix(
        mnModelHandle,
        moveAnimFrameIndex,
        MV1GetFrameLocalMatrix(mnModelHandle, moveAnimFrameIndex)
    );

    ChangeAnimation(AnimationState::ANIMATION_NEUTRAL);
}

SeparateModelAnimation::~SeparateModelAnimation()
{
    for (auto itr = mAnimationInfoList.begin(); itr != mAnimationInfoList.end(); )
    {
        auto temp = *itr;
        itr = mAnimationInfoList.erase(itr);
        MV1DeleteModel(temp->mnAnimationHandle);
        delete temp;
    }
}

// モーション再生時間の進行および前後モーションのクロスフェードブレンド制御
// 入力: なし / 出力: なし / 副作用: アタッチアニメーション再生時間・ブレンド率の更新
void SeparateModelAnimation::Update()
{
    if (mfAnimBlendRate < 1.0f)
    {
        mfAnimBlendRate += 0.1f;
        if (mfAnimBlendRate > 1.0f) mfAnimBlendRate = 1.0f;
    }

    float fAnimTotalTime = 0.0f;
    if (mnAnimationIndex != -1)
    {
        fAnimTotalTime = MV1GetAttachAnimTotalTime(mnModelHandle, mnAnimationIndex);
        mfAnimationTime += 0.5f;

        if (mfAnimationTime > fAnimTotalTime)
        {
            if (!mbLoop)
            {
                if (mnLoopFinishState == ANIMATION_MAX)
                {
                    mbLoopFinish = true;
                    return;
                }

                ChangeAnimation(mnLoopFinishState);
                SetAnimationBlend(false);
                fAnimTotalTime = MV1GetAttachAnimTotalTime(mnModelHandle, mnAnimationIndex);
            }
            mfAnimationTime = 0.0f;
        }

        MV1SetAttachAnimTime(mnModelHandle, mnAnimationIndex, mfAnimationTime);
        MV1SetAttachAnimBlendRate(mnModelHandle, mnAnimationIndex, mfAnimBlendRate);
    }

    if (mnOldAnimationIndex != -1)
    {
        fAnimTotalTime = MV1GetAttachAnimTotalTime(mnModelHandle, mnOldAnimationIndex);
        mfOldAnimationTime += 0.5f;

        if (mfOldAnimationTime > fAnimTotalTime)
        {
            mfOldAnimationTime = 0.0f;
        }

        MV1SetAttachAnimTime(mnModelHandle, mnOldAnimationIndex, mfOldAnimationTime);
        MV1SetAttachAnimBlendRate(mnModelHandle, mnOldAnimationIndex, 1.0f - mfAnimBlendRate);
    }
}

// 指定ステートへのモーション変更およびクロスフェード開始
// 入力: state(切り替え先ステート), index(アタッチインデックス) / 出力: なし / 副作用: 旧モーションデタッチ予約、新モーションアタッチ
void SeparateModelAnimation::ChangeAnimation(AnimationState state, int index)
{
    if (mnState == state) return;

    mnState = state;
    mbLoop = true;
    mnLoopFinishState = AnimationState::ANIMATION_MAX;
    mbLoopFinish = false;

    if (mnOldAnimationIndex != -1)
    {
        MV1DetachAnim(mnModelHandle, mnOldAnimationIndex);
        mnOldAnimationIndex = -1;
    }

    mnOldAnimationIndex = mnAnimationIndex;
    mfOldAnimationTime = mfAnimationTime;

    mnAnimationIndex = MV1AttachAnim(mnModelHandle, index, GetAnimationHandle(state), FALSE);
    mfAnimationTime = 0.0f;
    mfAnimBlendRate = (mnOldAnimationIndex == -1 ? 1.0f : 0.0f);
}

void SeparateModelAnimation::SetAnimationBlend(bool isBlend)
{
    if (isBlend)
    {
        mfAnimBlendRate = (mnOldAnimationIndex == -1 ? 1.0f : 0.0f);
    }
    else
    {
        mfAnimBlendRate = 1.0f;
        if (mnOldAnimationIndex != -1)
        {
            MV1DetachAnim(mnModelHandle, mnOldAnimationIndex);
            mnOldAnimationIndex = -1;
        }
    }
}

// 外部モーションファイルのロードとステート登録
// 入力: state(ステート), filename(ファイルパス) / 出力: なし / 副作用: MV1LoadModel実行、mAnimationInfoListへの登録
void SeparateModelAnimation::AddAnimation(AnimationState state, std::string filename)
{
    int handle = MV1LoadModel(filename.c_str());
    if (handle == -1) return;

    AnimationInfo* pInfo = new AnimationInfo();
    pInfo->mState = state;
    pInfo->mnAnimationHandle = handle;
    mAnimationInfoList.push_back(pInfo);

    if (state == AnimationState::ANIMATION_NEUTRAL)
    {
        ChangeAnimation(AnimationState::ANIMATION_NEUTRAL);
    }
}

// 登録済みモーションハンドルの検索取得
// 入力: state(ステート) / 出力: モーションモデルハンドル(-1で見つからない) / 副作用: なし
int SeparateModelAnimation::GetAnimationHandle(AnimationState state)
{
    if (mAnimationInfoList.empty()) return -1;

    for (auto itr = mAnimationInfoList.begin(); itr != mAnimationInfoList.end(); ++itr)
    {
        if ((*itr)->mState == state)
        {
            return (*itr)->mnAnimationHandle;
        }
    }
    return -1;
}