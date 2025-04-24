#include "StateBase.h"
#include "CharacterBase.h"
#include <string>
#include <cassert>

#include "State/Player/PlayerStateAttack.h"
#include "State/Player/PlayerStateDamage.h"
#include "State/Player/PlayerStateDash.h"
#include "State/Player/PlayerStateDeath.h"
#include "State/Player/PlayerStateEnterBoss.h"
#include "State/Player/PlayerStateGuard.h"
#include "State/Player/PlayerStateIdle.h"
#include "State/Player/PlayerStateItem.h"
#include "State/Player/PlayerStateRoll.h"
#include "State/Player/PlayerStateStrongAttack.h"
#include "State/Player/PlayerStateTaking.h"
#include "State/Player/PlayerStateWalk.h"
#include "State/Player/PlayerStateJump.h"

StateBase::StateBase(std::shared_ptr<CharacterBase> chara) :
	m_pChara(chara),
	m_stageCol(0),
	m_nowState(StateKind::Null),
	m_nextState(StateKind::Null),
	m_targetState(false),
	m_hit(),
	m_leftX(0.0f),
	m_leftZ(0.0f),
	m_equipmentState(false)
{
}

StateBase::~StateBase()
{
}

void StateBase::ChangeState(StateKind kind)
{
	//�^�O���擾
	auto tag = m_pChara.lock()->GetTag();

	//�^�O���玝���傪�v���C���[���G���𔻒f���ď�����ς���
	if (tag == ObjectTag::Player)
	{
		//������̖��O���擾
		auto name = m_pChara.lock()->GetCharacterName();
		//�A�C�h�����
		if (kind == StateKind::Idle)
		{
			auto pNext = std::make_shared<PlayerStateIdle>(m_pChara.lock());
			pNext->Init(m_stageCol);

			m_pChara.lock()->ChangeState(pNext);
			return;
		}
		//�������
		else if (kind == StateKind::Walk)
		{
			auto pNext = std::make_shared<PlayerStateWalk>(m_pChara.lock());
			pNext->Init(m_stageCol);

			m_pChara.lock()->ChangeState(pNext);
			return;
		}
		//������
		else if (kind == StateKind::Roll)
		{
			auto pNext = std::make_shared<PlayerStateRoll>(m_pChara.lock());
			pNext->Init(m_stageCol);

			m_pChara.lock()->ChangeState(pNext);
			return;
		}
		//������
		else if (kind == StateKind::Dash)
		{
			auto pNext = std::make_shared<PlayerStateDash>(m_pChara.lock());
			pNext->Init(m_stageCol);

			m_pChara.lock()->ChangeState(pNext);
			return;
		}
		//�W�����v���
		else if (kind == StateKind::Jump)
		{
			auto pNext = std::make_shared<PlayerStateJump>(m_pChara.lock());
			pNext->Init(m_stageCol);

			m_pChara.lock()->ChangeState(pNext);
			return;
		}
		//�U�����
		else if (kind == StateKind::Attack)
		{
			auto pNext = std::make_shared<PlayerStateAttack>(m_pChara.lock());
			pNext->Init(m_stageCol);

			m_pChara.lock()->ChangeState(pNext);
			return;
		}
		//���U�����
		else if (kind == StateKind::StrongAttack)
		{
			auto pNext = std::make_shared<PlayerStateStrongAttack>(m_pChara.lock());
			pNext->Init(m_stageCol);

			m_pChara.lock()->ChangeState(pNext);
			return;
		}
		//�K�[�h���
		else if (kind == StateKind::Guard)
		{
			auto pNext = std::make_shared<PlayerStateGuard>(m_pChara.lock());
			pNext->Init(m_stageCol);

			m_pChara.lock()->ChangeState(pNext);
			return;
		}
		//�v���C���[�̃p���B���
		//else if (kind == StateKind::Parry)
		//{

		//}
		//�v���C���[�̃_���[�W���
		else if (kind == StateKind::Damage)
		{
			auto pNext = std::make_shared<PlayerStateDamage>(m_pChara.lock());
			pNext->Init(m_stageCol);

			m_pChara.lock()->ChangeState(pNext);
			return;
		}
		//�v���C���[�̎��S���
		else if (kind == StateKind::Death)
		{
			auto pNext = std::make_shared<PlayerStateDeath>(m_pChara.lock());
			pNext->Init(m_stageCol);

			m_pChara.lock()->ChangeState(pNext);
			return;
		}
		//�v���C���[�̃{�X�����ɓ�����
		else if (kind == StateKind::EnterBoss)
		{
			auto pNext = std::make_shared<PlayerStateEnterBoss>(m_pChara.lock());
			pNext->Init(m_stageCol);

			m_pChara.lock()->ChangeState(pNext);
			return;
		}
		//�v���C���[�̃A�C�e���g�p���
		else if (kind == StateKind::Item)
		{
			auto pNext = std::make_shared<PlayerStateItem>(m_pChara.lock());
			pNext->Init(m_stageCol);

			m_pChara.lock()->ChangeState(pNext);
			return;
		}
		//�v���C���[�̃A�C�e���擾���
		else if (kind == StateKind::Taking)
		{
			auto pNext = std::make_shared<PlayerStateTaking>(m_pChara.lock());
			pNext->Init(m_stageCol);

			m_pChara.lock()->ChangeState(pNext);
			return;
		}
	}

}

/// <summary>
/// �����傪�N�����f����
/// </summary>
/// <returns></returns>
bool StateBase::CheckState()
{
	//�v���C���[��������true��Ԃ�
	if (m_pChara.lock()->GetTag() == ObjectTag::Player)
	{
		return true;
	}

	return false;
}

const StateBase::eDir StateBase::GetDirection(float x, float y) const
{
	auto angle = atan2(y, x);
	if (angle < 0)
	{
		angle = angle + 2 * DX_PI_F;
	}
	angle = floor(angle * 360 / (2 * DX_PI_F));

	if (0 <= angle && angle <= 33)
	{
		return eDir::Right;
	}
	else if (34 <= angle && angle <= 67)
	{
		return eDir::BackRight;
	}
	else if (68 <= angle && angle <= 112)
	{
		return eDir::Back;
	}
	else if (113 <= angle && angle <= 157)
	{
		return eDir::ForwardLeft;
	}
	else if (158 <= angle && angle <= 202)
	{
		return eDir::Left;
	}
	else if (203 <= angle && angle <= 247)
	{
		return eDir::BackLeft;
	}
	else if (248 <= angle && angle <= 292)
	{
		return eDir::Forward;
	}
	else if (293 <= angle && angle <= 337)
	{
		return eDir::ForwardRight;

	}
	else
	{
		return Back;
	}
}
