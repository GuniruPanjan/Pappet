#include "CharacterBase.h"

namespace
{
	//アニメーションのマックス数を代入する変数
	int cAnimMax = 0;
}

CharacterBase::CharacterBase(Priority priority, ObjectTag tag) :
	Collidable(priority, tag),
	m_characterName(),
	m_anim(),
	m_modelHandle(-1),
	m_modelPos(),
	m_collisionPos(),
	m_status(),
	m_moveVec(),
	m_cameraAngle(0.0f),
	m_nowPos(VGet(0.0f,0.0f,0.0f)),
	m_nowAnimNo(-1),
	m_equipAnimNo(-1),
	m_nowAnimIdx(-1),
	m_prevAnimNo(-1),
	m_frameAnimNo(-1),
	m_animBlendRate(1.0f),
	m_nowFrame(0.0f),
	m_animTime(0.5f),
	m_isAnimationFinish(false),
	m_animInit(false),
	m_animReverse(false),
	m_sword(false),
	m_equipment(false),
	m_shield(false),
	m_fist(false),
	m_angle(0.0f),
	m_animSpeed(0.0f),
	m_attackRadius(0.0f),
	m_searchRadius(0.0f),
	m_heel(0),
	m_maxHeel(0),
	m_preAnimIdx(0),
	m_currentAnimNo(-1),
	m_pState(),
	m_capsuleY(0.0f),
	m_capsuleRadius(0.0f),
	m_jumpCan(false),
	m_guardTransition(false)
{
	for (int i = 0; i < ANIMATION_MAX; i++)
	{
		m_animOne[i] = false;
	}
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
bool CharacterBase::UpdateAnim(int attachNo, int max, float startTime)
{
	cAnimMax = max;

	//アニメーションが設定されていなかったら早期リターン
	if (attachNo == -1) return false;

	//現在再生中のアニメーションの総カウントを取得する
	float totalAnimFrame = MV1GetAttachAnimTotalTime(m_modelHandle, attachNo);
	bool isLoop = false;

	//アニメーションを進行させる
	if (!m_animReverse)
	{
		m_nowFrame += m_animTime;
	}
	//アニメーションを逆進行させる
	else if (m_animReverse)
	{
		m_nowFrame -= m_animTime;
	}

	//ここが問題になっている
	//アニメーションを再生させる時
	if (!m_animReverse)
	{
		while (totalAnimFrame <= m_nowFrame)
		{
			m_nowFrame -= totalAnimFrame;
			m_nowFrame += startTime;
			isLoop = true;
		}
	}
	//アニメーションを逆再生させる時
	else if (m_animReverse)
	{
		while (m_nowFrame <= 0.0f)
		{
			m_nowFrame += totalAnimFrame;
			m_nowFrame -= startTime;
			isLoop = true;
		}
	}

	if (!m_animInit)
	{
		//進めた時間に設定
		MV1SetAttachAnimTime(m_modelHandle, attachNo, m_nowFrame);
	}
	else if (m_animInit)
	{
		MV1SetAttachAnimTime(m_modelHandle, attachNo, m_animSpeed);
	}
	
	m_animSpeed = m_nowFrame;

	return isLoop;
}

bool CharacterBase::UpdateStateAnim(int attachNo, float startTime, float resetTime, bool loop)
{
	//アニメーションが設定されていなかったら早期リターン
	if (attachNo == -1) return false;

	//現在再生中のアニメーションの総カウントを取得する
	float totalAnimFrame = MV1GetAttachAnimTotalTime(m_modelHandle, attachNo);
	bool isLoop = false;

	//アニメーションを進行させる
	if (!m_animReverse)
	{
		m_nowFrame += m_animTime;
	}
	//アニメーションを逆進行させる
	else if (m_animReverse)
	{
		m_nowFrame -= m_animTime;
	}

	//ここが問題になっている
	//アニメーションを再生させる時
	if (!m_animReverse)
	{
		//一部分をループさせない
		if (!loop)
		{
			while (totalAnimFrame <= m_nowFrame)
			{
				m_nowFrame -= totalAnimFrame;
				m_nowFrame += startTime;
				isLoop = true;
			}
		}
		//一部分をループさせる
		else
		{
			while (resetTime <= m_nowFrame)
			{
				m_nowFrame -= resetTime;
				m_nowFrame += startTime;
				isLoop = true;
			}
		}
		
	}
	//アニメーションを逆再生させる時
	else if (m_animReverse)
	{
		while (m_nowFrame <= 0.0f)
		{
			m_nowFrame += totalAnimFrame;
			m_nowFrame -= startTime;
			isLoop = true;
		}
	}

	if (!m_animInit)
	{
		//進めた時間に設定
		MV1SetAttachAnimTime(m_modelHandle, attachNo, m_nowFrame);
	}
	else if (m_animInit)
	{
		MV1SetAttachAnimTime(m_modelHandle, attachNo, m_animSpeed);
	}

	m_animSpeed = m_nowFrame;

	return isLoop;
}

/// <summary>
/// アニメーションの変更
/// </summary>
/// <param name="animIndex">変更後のアニメーション番号</param>
/// <param name="animSpeed">アニメーションタイム</param>
void CharacterBase::ChangeAnim(int animIndex, bool& one, bool (&all)[30], float animSpeed, bool reverse, float resetTime)
{
	//一回だけ実行
	if (one == false)
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
	
		m_nowFrame = resetTime;

		//アニメーションを逆再生させるかどうか
		m_animReverse = reverse;

		m_animTime = animSpeed;

		//変更前のアニメーション100%
		MV1SetAttachAnimBlendRate(m_modelHandle, m_prevAnimNo, 1.0f - m_animBlendRate);

		//変更後のアニメーション0%
		MV1SetAttachAnimBlendRate(m_modelHandle, m_nowAnimNo, m_animBlendRate);
		

		//初期化する
		for (int i = 0; i < cAnimMax; i++)
		{
			all[i] = false;
		}

		one = true;
	}
	
}

void CharacterBase::ChangeStateAnim(int animIndex, bool init, float animSpeed, bool reverse, float resetTime)
{
	//さらに古いアニメーションがアタッチされている場合はこの時点で消しておく
	if (m_prevAnimNo != -1)
	{
		MV1DetachAnim(m_modelHandle, m_prevAnimNo);
	}

	m_preAnimIdx = m_nowAnimIdx;
	m_nowAnimIdx = animIndex;

	//現在再生中の待機アニメーションは変更目のアニメーションの扱いにする
	m_prevAnimNo = m_currentAnimNo;

	//変更後のアニメーションとして攻撃アニメーションを改めて設定する
	m_currentAnimNo = MV1AttachAnim(m_modelHandle, animIndex);

	//切り替えの瞬間は変更前のアニメーションが再生される状態にする
	m_animBlendRate = 0.0f;

	//アニメーションをタイムを初期化するかどうか
	if (init)
	{
		m_nowFrame = resetTime;
	}

	m_animTime = animSpeed;

	//アニメーションを逆再生させるかどうか
	m_animReverse = reverse;

	//変更前のアニメーション100%
	MV1SetAttachAnimBlendRate(m_modelHandle, m_prevAnimNo, 1.0f - m_animBlendRate);
	//変更後のアニメーション0%
	MV1SetAttachAnimBlendRate(m_modelHandle, m_currentAnimNo, m_animBlendRate);
}

void CharacterBase::FrameChangeAnim(int animIndex, bool& one, bool& two, int frame)
{
	//一回だけ実行
	if (one == false)
	{
		m_frameAnimNo = MV1AttachAnim(m_modelHandle, animIndex);
		
		one = true;
	}

	//フレームだけのアニメーション
	MV1SetAttachAnimBlendRate(m_modelHandle, m_frameAnimNo, 0.0f);
	//フレームだけアニメーション
	MV1SetAttachAnimBlendRateToFrame(m_modelHandle, m_nowAnimNo, frame, 0.0f);
	MV1SetAttachAnimBlendRateToFrame(m_modelHandle, m_frameAnimNo, frame, 1.0f);

	//進めた時間に設定
	MV1SetAttachAnimTime(m_modelHandle, m_frameAnimNo, m_nowFrame);

	two = false;
}

/// <summary>
/// ステートでのアニメーションフレームブレンド
/// </summary>
/// <param name="animIndex"></param>
/// <param name="frame"></param>
void CharacterBase::FrameStateChangeAnim(int animIndex, int frame, bool& one)
{
	if (one == true)
	{
		m_frameAnimNo = MV1AttachAnim(m_modelHandle, animIndex);

		one = false;
	}

	//フレームだけのアニメーション
	MV1SetAttachAnimBlendRate(m_modelHandle, m_frameAnimNo, 0.0f);
	//フレームだけアニメーション
	MV1SetAttachAnimBlendRateToFrame(m_modelHandle, m_currentAnimNo, frame, 0.0f);
	MV1SetAttachAnimBlendRateToFrame(m_modelHandle, m_frameAnimNo, frame, 1.0f);

	//進めた時間に設定
	MV1SetAttachAnimTime(m_modelHandle, m_frameAnimNo, m_nowFrame);
}

/// <summary>
/// ステートでのアニメーションフレームブレンド
/// </summary>
/// <param name="animIndex"></param>
/// <param name="frame"></param>
void CharacterBase::FrameEndStateAnim(int animIndex, int frame, bool& one)
{
	if (one == false)
	{
		MV1DetachAnim(m_modelHandle, animIndex);

		one = true;
	}

	//フレームだけアニメーション
	MV1SetAttachAnimBlendRateToFrame(m_modelHandle, m_currentAnimNo, frame, 1.0f);
	MV1SetAttachAnimBlendRateToFrame(m_modelHandle, m_frameAnimNo, frame, 0.0f);
}

void CharacterBase::FrameEndAnim(int animIndex, bool& one, bool& two, int frame)
{
	if (one || two)
	{
		MV1DetachAnim(m_modelHandle, animIndex);

		//フレームだけアニメーション
		MV1SetAttachAnimBlendRateToFrame(m_modelHandle, m_nowAnimNo, frame, 1.0f);
		MV1SetAttachAnimBlendRateToFrame(m_modelHandle, m_frameAnimNo, frame, 0.0f);

		one = false;
		two = false;
	}
}

/// <summary>
/// アニメーションタイムを初期化するかの判断
/// </summary>
/// <param name="init">初期化するかの判定</param>
void CharacterBase::NotInitAnim(bool init)
{
	m_animInit = init;
}

void CharacterBase::ChangeState(std::shared_ptr<StateBase> next)
{
	m_pState = next;
}
