#include "CharacterBase.h"

CharacterBase::CharacterBase(Priority priority, ObjectTag tag) :
	Collidable(priority, tag),
	m_modelHandle(-1),
	m_modelPos(),
	m_collisionPos(),
	m_status(),
	m_moveVec(),
	m_nowPos(),
	m_prevPos(),
	m_nowAnimNo(-1),
	m_equipAnimNo(-1),
	m_nowAnimIdx(-1),
	m_prevAnimNo(-1),
	m_animBlendRate(1.0f),
	m_nowFrame(0.0f),
	m_animTime(0.5f),
	m_isAnimationFinish(false),
	m_reset(false),
	m_angle(0.0f),
	m_moveflag(false),
	m_hit(false),
	m_animSpeed(0.0f)
{
}

CharacterBase::~CharacterBase()
{
}

/// <summary>
/// �A�j���[�V�����̍X�V
/// </summary>
/// <param name="attachNo">�i�s���������A�j���[�V�����ԍ�</param>
/// <param name="startTime">���[�v�������ǂ���</param>
/// <returns></returns>
bool CharacterBase::UpdateAnim(int attachNo, float startTime)
{
	//�A�j���[�V�������ݒ肳��Ă��Ȃ������瑁�����^�[��
	if (attachNo == -1) return false;

	//�A�j���[�V������i�s������
	m_nowFrame += m_animTime;

	//���ݍĐ����̃A�j���[�V�����̑��J�E���g���擾����
	float totalAnimFrame = MV1GetAttachAnimTotalTime(m_modelHandle, attachNo);
	bool isLoop = false;

	while (totalAnimFrame <= m_nowFrame)
	{
		m_nowFrame -= totalAnimFrame;
		m_nowFrame += startTime;
		isLoop = true;
	}

	m_animSpeed = m_nowFrame;

	//�i�߂����Ԃɐݒ�
	MV1SetAttachAnimTime(m_modelHandle, attachNo, m_nowFrame);


	return isLoop;
}

/// <summary>
/// �A�j���[�V�����̕ύX
/// </summary>
/// <param name="animIndex">�ύX��̃A�j���[�V�����ԍ�</param>
/// <param name="animSpeed">�A�j���[�V�����^�C��</param>
void CharacterBase::ChangeAnim(int animIndex, float animSpeed)
{
	//����ɌÂ��A�j���[�V�������A�^�b�`����Ă���ꍇ�����̎��_�ŏ���
	if (m_prevAnimNo != -1)
	{
		MV1DetachAnim(m_modelHandle, m_prevAnimNo);
	}
	//��񂾂����s
	if (!m_reset)
	{
		m_nowFrame = 0.0f;

		m_reset = true;
	}

	//���ݍĐ����̑ҋ@�A�j���[�V�����͕ύX�ڂ̃A�j���[�V�����̈����ɂ���
	m_prevAnimNo = m_nowAnimNo;

	//�ύX��̃A�j���[�V�����Ƃ��ĉ��߂Đݒ肷��
	m_nowAnimNo = MV1AttachAnim(m_modelHandle, animIndex);

	//�؂�ւ��̏u�Ԃ͕ύX�O�̃A�j���[�V�������Đ�������Ԃɂ���
	m_animBlendRate = 0.0f;

	m_animTime = animSpeed;

	//�ύX�O�̃A�j���[�V����100%
	MV1SetAttachAnimBlendRate(m_modelHandle, m_prevAnimNo, 1.0f - m_animBlendRate);
	//�ύX��̃A�j���[�V����0%
	MV1SetAttachAnimBlendRate(m_modelHandle, m_nowAnimNo, m_animBlendRate);
}
