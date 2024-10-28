#include "CharacterBase.h"

CharacterBase::CharacterBase(Priority priority, ObjectTag tag) :
	Collidable(priority, tag),
	m_modelHandle(-1),
	m_modelPos(),
	m_collisionPos(),
	m_status(),
	m_moveVec(),
	m_nowAnimNo(-1),
	m_equipAnimNo(-1),
	m_nowAnimIdx(-1),
	m_prevAnimNo(-1),
	m_animBlendRate(1.0f),
	m_animTime(0.5f),
	m_isAnimationFinish(false),
	m_angle(0.0f),
	m_moveflag(false),
	m_hit(false)
{
}

CharacterBase::~CharacterBase()
{
}

/// <summary>
/// アニメーションの更新
/// </summary>
/// <param name="attachNo">進行させたいアニメーション番号</param>
/// <param name="startTime">ループしたかどうか</param>
/// <returns></returns>
bool CharacterBase::UpdateAnim(int attachNo, float startTime)
{
	//アニメーションが設定されていなかったら早期リターン
	if (attachNo == -1) return false;

	//アニメーションを進行させる
	float nowFrame = MV1GetAttachAnimTime(m_modelHandle, attachNo);
	nowFrame += m_animTime;

	//現在再生中のアニメーションの総カウントを取得する
	float totalAnimFrame = MV1GetAttachAnimTime(m_modelHandle, attachNo);
	bool isLoop = false;

	while (totalAnimFrame >= nowFrame)
	{
		nowFrame -= totalAnimFrame;
		nowFrame += startTime;
		isLoop = true;
	}

	//進めた時間に設定
	MV1SetAttachAnimTime(m_modelHandle, attachNo, nowFrame);


	return isLoop;
}

/// <summary>
/// アニメーションの変更
/// </summary>
/// <param name="animIndex">変更後のアニメーション番号</param>
/// <param name="animSpeed">アニメーションタイム</param>
void CharacterBase::ChangeAnim(int animIndex, float animSpeed)
{
	//さらに古いアニメーションがアタッチされている場合がこの時点で消す
	if (m_prevAnimNo != -1)
	{
		MV1DetachAnim(m_modelHandle, m_prevAnimNo);
	}

	//現在再生中の待機アニメーションは変更目のアニメーションの扱いにする
	m_prevAnimNo = m_nowAnimNo;

	//変更後のアニメーションとして改めて設定する
	m_nowAnimNo = MV1AttachAnim(m_modelHandle, animIndex);

	//切り替えの瞬間は変更前のアニメーションが再生される状態にする
	m_animBlendRate = 0.0f;

	m_animTime = animSpeed;

	//変更前のアニメーション100%
	MV1SetAttachAnimBlendRate(m_modelHandle, m_prevAnimNo, 1.0f - m_animBlendRate);
	//変更後のアニメーション0%
	MV1SetAttachAnimBlendRate(m_modelHandle, m_nowAnimNo, m_animBlendRate);
}
