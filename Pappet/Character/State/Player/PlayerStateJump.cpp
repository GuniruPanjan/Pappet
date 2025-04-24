#include "PlayerStateJump.h"
#include "Character/CharacterBase.h"
#include "External/CsvLoad.h"
#include "Input/Input.h"
#include "Character/Player.h"

namespace
{
	//�W�����v��
	constexpr float cJumpPower = 7.0f;

	//�W�����v�t���[����
	constexpr int cJumpFrame = 26;

	//���n�A�j���[�V�����̍Đ����x
	constexpr float cLandingAnimSpeed = 1.0f;
}

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="chara"></param>
PlayerStateJump::PlayerStateJump(std::shared_ptr<CharacterBase> chara) :
	StateBase(chara),
	m_updateFund(),
	m_jumpFrame(0)
{
	//���݂̃X�e�[�g���W�����v��Ԃɂ���
	m_nowState = StateKind::Jump;

	chara->ChangeStateAnim(CsvLoad::GetInstance().GetAnimData(chara->GetCharacterName(), "JumpUp"), true, 1.0f, false, 22.0f);

	//���̃X�e�[�g�ɓ������u�ԃW�����v�͂𑫂�
	auto vel = chara->GetRigidbody()->GetVelocity();
	vel = MyLibrary::LibVec3(vel.x / 2, vel.y + cJumpPower, vel.z / 2);
	chara->GetRigidbody()->SetVelocity(vel);
	//�W�����v��Ԃɂ���
	chara->GetRigidbody()->SetJump(true);
	chara->GetRigidbody()->SetJumpNow(true);
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
PlayerStateJump::~PlayerStateJump()
{
}

/// <summary>
/// ������
/// </summary>
/// <param name="md"></param>
void PlayerStateJump::Init(int md)
{
	m_updateFund = &PlayerStateJump::UpUpdate;

	m_stageCol = md;
}

/// <summary>
/// �X�V
/// </summary>
void PlayerStateJump::Update()
{
	//�����傪�v���C���[���ǂ������`�F�b�N����
	if (!CheckState()) return;

	//�����o�֐��|�C���^�̍X�V
	(this->*m_updateFund)();
}

/// <summary>
/// �W�����v�㏸���
/// </summary>
void PlayerStateJump::UpUpdate()
{
	//�W�����v�t���[�����㏸�����A�j���[�V�����̏I���t���[���ȏ�Ȃ�W�����v����Ԃɂ���
	if (m_jumpFrame >= 5)
	{
		//�A�j���[�V�����ύX
		m_pChara.lock()->ChangeStateAnim(CsvLoad::GetInstance().GetAnimData("Player", "JumpUp"), true, 0.01f, false, 31.0f);
		//�W�����v�t���[��������������
		m_jumpFrame = 0;
		//�W�����v����Ԃɂ���
		m_updateFund = &PlayerStateJump::LoopUpdate;
	}

	//�W�����v�t���[�����X�V����
	m_jumpFrame++;

}

/// <summary>
/// �W�����v�����
/// </summary>
void PlayerStateJump::LoopUpdate()
{
	//���g�̑������W�ƈړ����x�ŃJ�v�Z�������
	auto own = std::dynamic_pointer_cast<Player>(m_pChara.lock());

	//�W�����v�t���[���K�萔�ȏ�Ȃ�
	if (m_jumpFrame > cJumpFrame)
	{
		
		auto pos = m_pChara.lock()->GetRigidbody()->GetPos();
		auto vel = m_pChara.lock()->GetRigidbody()->GetVelocity();
		auto modelBottomPos = pos;
		modelBottomPos.y -= own->GetCapsuleY();
		auto underPos = modelBottomPos;
		underPos.y -= (own->GetCapsuleRadius() + own->GetCapsuleY()) * 2.0f * (0.4f - vel.y);

		//�X�e�[�W�Ƃ̃J�v�Z���Ƃœ����蔻������
		auto hit = MV1CollCheck_Line(m_stageCol, -1, modelBottomPos.ConversionToVECTOR(), underPos.ConversionToVECTOR());

		own->SetStart(31.0f);
		own->SetReset(33.0f);
		own->SetLoop(true);

		//�W�����v�I��
		own->GetRigidbody()->SetJump(false);

		//�X�e�[�W�ƃJ�v�Z�����������Ă�����W�����v���~��Ԃɂ���
		if (hit.HitFlag)
		{
			//�A�j���[�V�����ύX
			m_pChara.lock()->ChangeStateAnim(CsvLoad::GetInstance().GetAnimData("Player", "JumpUp"), true, 1.2f, false, 33.0f);
			//�W�����v�t���[��������������
			m_jumpFrame = 0;
			own->SetStart(0.0f);
			own->SetReset(0.0f);
			//���[�v���Ȃ�����
			own->SetLoop(false);
			//�W�����v���~��Ԃɂ���
			m_updateFund = &PlayerStateJump::DownUpdate;
		}
	
	}

	//���U���{�^����������Ă�����State�����U���ɂ���
	if (Input::GetInstance().GetIsPushedTriggerButton(true) || Input::GetInstance().GetIsPushedTriggerButton(true))
	{
		//�X�^�~�i�؂ꂶ��Ȃ������ꍇ
		if (!own->GetStaminaBreak())
		{
			//�W�����v�I��
			own->GetRigidbody()->SetJump(false);
			//�t���[��������������
			own->SetStart(0.0f);
			own->SetReset(0.0f);
			//�A�j���[�V�������[�v����߂�
			own->SetLoop(false);

			ChangeState(StateKind::StrongAttack);
			return;
		}
	}


	//�t���[���X�V
	m_jumpFrame++;
}

/// <summary>
/// �W�����v���~���
/// </summary>
void PlayerStateJump::DownUpdate()
{
	//�_�E���L���X�g
	auto own = std::dynamic_pointer_cast<Player>(m_pChara.lock());

	if (m_jumpFrame >= 15.0f)
	{

		//�W�����v�I��
		own->GetRigidbody()->SetJump(false);
		own->GetRigidbody()->SetJumpNow(false);

		//���X�e�B�b�N�����͂���Ă��Ȃ�������State��Idle�ɂ���
		if (Input::GetInstance().GetInputStick(false).first == 0.0f &&
			Input::GetInstance().GetInputStick(false).second == 0.0f)
		{
			ChangeState(StateKind::Idle);
			return;
		}

		//���X�e�B�b�N�����͂���Ă�����State��Walk��Dash�ɂ���
		if (Input::GetInstance().GetInputStick(false).first != 0.0f ||
			Input::GetInstance().GetInputStick(false).second != 0.0f)
		{
			//�_�b�V���{�^��������������Ă���_�b�V��
			if (Input::GetInstance().IsPushed("Input_Dash") && !own->GetStaminaBreak())
			{
				ChangeState(StateKind::Dash);
				return;
			}
			//������Ă��Ȃ�������Walk
			else
			{
				ChangeState(StateKind::Walk);
				return;
			}
		}

		//�W�����v�{�^������������W�����v�֑J�ڂ���
		if (Input::GetInstance().IsPushed("Input_Jump") && !own->GetStaminaBreak())
		{
			ChangeState(StateKind::Jump);
			return;
		}
	}
	else
	{
		//���U���{�^����������Ă�����State�����U���ɂ���
		if (Input::GetInstance().GetIsPushedTriggerButton(true) || Input::GetInstance().GetIsPushedTriggerButton(true))
		{
			//�X�^�~�i�؂ꂶ��Ȃ������ꍇ
			if (!own->GetStaminaBreak())
			{
				ChangeState(StateKind::StrongAttack);
				return;
			}
		}
	}

	//�W�����v�t���[�����X�V����
	m_jumpFrame++;
}
