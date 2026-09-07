#include "SeparateModelAnimation.h"

SeparateModelAnimation::SeparateModelAnimation(int modelHandle)
    : m_modelHandle(modelHandle)
{
    // アニメーションで移動をしているフレーム番号を検索する
    int moveAnimFrameIndex = MV1SearchFrame(m_modelHandle, "mixamorig:Hips");
    //移動を無効にする
    MV1SetFrameUserLocalMatrix(
        m_modelHandle,
        moveAnimFrameIndex,
        MV1GetFrameLocalMatrix(m_modelHandle, moveAnimFrameIndex)
    );

    // 初期状態は待機モーションにしておく
    ChangeAnimation(AnimationState::AnimationNeutral);
}

SeparateModelAnimation::~SeparateModelAnimation()
{
    // 追加読み込みしたモーションの削除
    if (!m_animationInfoList.empty())
    {
        for (auto itr = m_animationInfoList.begin(); itr != m_animationInfoList.end(); )
        {
            auto temp = *itr;

            itr = m_animationInfoList.erase(itr);
            MV1DeleteModel(temp->m_animationHandle);

            delete temp;
            temp = nullptr;
        }
    }
}

void SeparateModelAnimation::Update()
{
    // モーションのブレンド率を進める
    if (m_animBlendRate < 1.0f)
    {
        m_animBlendRate += 0.1f;    // += 0.1f はブレンド速度。自由に変えてもok
        if (m_animBlendRate > 1.0f)
        {
            m_animBlendRate = 1.0f;
        }
    }


    // モーションの更新
    float fAnimTotalTime = 0.0f;
    if (m_animationIndex != -1)
    {
        // 総再生時間の取得
        fAnimTotalTime = MV1GetAttachAnimTotalTime(m_modelHandle, m_animationIndex);

        // モーションを進める
        m_animationTime += 0.5f;

        // ループさせる
        if (m_animationTime > fAnimTotalTime)
        {
            // ループしない設定であれば
            if (!m_loop)
            {
                // 次のモーションが設定されていない場合
                if (m_loopFinishState == AnimationMax)
                {
                    // モーションはこれ以上進めず、処理を中断させる
                    m_loopFinish = true;
                    return;
                }

                // ループ終了時のモーションへ変更
                ChangeAnimation(m_loopFinishState);
                // ブレンドはしない
                SetAnimationBlend(false);
                // 変更されたので改めて総再生時間をとっておく
                fAnimTotalTime = MV1GetAttachAnimTotalTime(m_modelHandle, m_animationIndex);
            }

            m_animationTime = 0.0f;
        }

        // モーションを反映
        MV1SetAttachAnimTime(m_modelHandle, m_animationIndex, m_animationTime);

        // ブレンド率を設定
        MV1SetAttachAnimBlendRate(m_modelHandle, m_animationIndex, m_animBlendRate);
    }

    // １つ前のモーションを更新
    if (m_oldAnimationIndex != -1)
    {
        // 総再生時間の取得
        fAnimTotalTime = MV1GetAttachAnimTotalTime(m_modelHandle, m_oldAnimationIndex);

        // モーションを進める
        m_oldAnimationTime += 0.5f;

        // ループさせる
        if (m_oldAnimationTime > fAnimTotalTime)
        {
            m_oldAnimationTime = 0.0f;
        }

        // モーションを反映
        MV1SetAttachAnimTime(m_modelHandle, m_oldAnimationIndex, m_oldAnimationTime);

        // ブレンド率を設定
        MV1SetAttachAnimBlendRate(m_modelHandle, m_oldAnimationIndex, 1.0f - m_animBlendRate);
    }
}

void SeparateModelAnimation::ChangeAnimation(AnimationState state, int index)
{
    // 切り替えようとしているモーションがすでに設定されている場合
    if (m_state == state)
    {
        return;     // 何もしない
    }

    // 切り替え先の番号を保持
    m_state = state;

    // ループ情報の初期化
    m_loop = true;  // 設定が特にない場合はループさせる
    m_loopFinishState = AnimationState::AnimationMax;  // ループ終了時のモーションは特になし
    m_loopFinish = false;

    // １つ前のモーションが有効状態であれば
    if (m_oldAnimationIndex != -1)
    {
        // モーションのデタッチ（取り外す）
        MV1DetachAnim(m_modelHandle, m_oldAnimationIndex);
        m_oldAnimationIndex = -1;
    }

    // 現在のモーション状態を保持する
    m_oldAnimationIndex = m_animationIndex;
    m_oldAnimationTime = m_animationTime;

    // モーションのアタッチ
    m_animationIndex = MV1AttachAnim(m_modelHandle, index, GetAnimationHandle(state), FALSE);

    // 再生時間の初期化
    m_animationTime = 0.0f;

    // ブレンド状態を初期化
    // ブレンド率は、古いモーションが有効でない場合は1.0f（ブレンドしない状態）にしておく
    m_animBlendRate = (m_oldAnimationIndex == -1 ? 1.0f : 0.0f);
}

void SeparateModelAnimation::SetAnimationBlend(bool isBlend)
{
    if (isBlend)    // ブレンドする場合
    {
        // ブレンド率は、古いモーションが有効でない場合は1.0f（ブレンドしない状態）にしておく
        m_animBlendRate = (m_oldAnimationIndex == -1 ? 1.0f : 0.0f);
    }
    else    // ブレンドしない場合
    {
        // ブレンドしない状態にする
        m_animBlendRate = 1.0f;

        // ブレンドする必要がないので、古いモーションはデタッチしておく
        if (m_oldAnimationIndex != -1)
        {
            MV1DetachAnim(m_modelHandle, m_oldAnimationIndex);
            m_oldAnimationIndex = -1;
        }
    }
}

void SeparateModelAnimation::AddAnimation(AnimationState state, std::string filename)
{
    // モーションモデル読み込み
    int handle = MV1LoadModel(filename.c_str());

    if (handle == -1)
    {
        return;
    }

    // AnimationState と読み込んだハンドルの紐づけ
    AnimationInfo* pInfo = new AnimationInfo();
    pInfo->m_state = state;
    pInfo->m_animationHandle = handle;
    m_animationInfoList.push_back(pInfo);

    // NEUTRALモーション（待機モーション）が追加されたらモーション変更処理をしておく
    if (state == AnimationState::AnimationNeutral)
    {
        // 初期状態は待機モーションにしておく
        ChangeAnimation(AnimationState::AnimationNeutral);
    }
}

int SeparateModelAnimation::GetAnimationHandle(AnimationState state)
{
    // そもそも空っぽの場合は探さない
    if (m_animationInfoList.empty())
    {
        return -1;
    }

    for (auto itr = m_animationInfoList.begin(); itr != m_animationInfoList.end(); itr++)
    {
        auto temp = *itr;

        // 対応するモーションハンドルがあればそれを返す
        if (temp->m_state == state)
        {
            return temp->m_animationHandle;
        }
    }

    // 見つからなかったら-1を返す
    return -1;
}