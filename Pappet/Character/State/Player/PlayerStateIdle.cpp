#include "PlayerStateIdle.h"
#include "Character/CharacterBase.h"
#include "External/CsvLoad.h"
#include "Input/Input.h"
#include "Character/Player.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="chara"></param>
PlayerStateIdle::PlayerStateIdle(std::shared_ptr<CharacterBase> chara) :
	StateBase(chara)
{
	//�V�[���h�𑕔����Ă��邩�ǂ���
	m_equipmentShield = chara->GetShield();

	//���݂̃X�e�[�g��ҋ@��Ԃɂ���
	m_nowState = StateKind::Idle;
	chara->ChangeStateAnim(CsvLoad::GetInstance().GetAnimData(chara->GetCharacterName(), "Idle"), false);
	chara->NotInitAnim(false);
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
PlayerStateIdle::~PlayerStateIdle()
{
}

/// <summary>
/// ����������
/// </summary>
/// <param name="md"></param>
void PlayerStateIdle::Init(int md)
{
	m_stageCol = md;
}

/// <summary>
/// �X�V����
/// </summary>
void PlayerStateIdle::Update()
{
	//�����傪�v���C���[���ǂ������`�F�b�N����
	if (!CheckState()) return;

	//�����Ă���L�����N�^�[�x�[�X�N���X���v���C���[�N���X�ɃL���X�g����(�_�E���L���X�g)
	auto own = std::dynamic_pointer_cast<Player>(m_pChara.lock());

	//�v���C���[�̑��x��0�ɂ���
	auto prevVel = own->GetRigidbody()->GetVelocity();
	own->GetRigidbody()->SetVelocity(MyLibrary::LibVec3(0.0f, prevVel.y, 0.0f));

	//�x�����ƃX�^�~�i���Ȃ����͓����Ȃ�����
	if (!own->GetRest() && !own->GetStaminaBreak())
	{
		//���X�e�B�b�N�����͂���Ă�����State��Walk��Dash�ɂ���
		if (Input::GetInstance().GetInputStick(false).first != 0.0f ||
			Input::GetInstance().GetInputStick(false).second != 0.0f)
		{
			//���j���[���J����Ă��Ȃ��ă_�b�V���{�^��������������Ă���_�b�V��
			if (Input::GetInstance().IsPushed("Input_Dash") && !own->GetMenu() && own->GetAction())
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

		//���j���[���J���Ă��Ȃ��ăW�����v�{�^����������Ă�����State��Jump�ɂ���
		if (Input::GetInstance().IsTriggered("Input_Jump") && !own->GetMenu() && own->GetAction())
		{
			ChangeState(StateKind::Jump);
			return;
		}

		//���j���[���J���Ă��Ȃ��čU���{�^����������Ă�����State���U���ɂ���
		if (Input::GetInstance().IsTriggered("Input_Attack") && !own->GetMenu())
		{
			ChangeState(StateKind::Attack);
			return;
		}

		//���U���{�^����������Ă�����State�����U���ɂ���
		if (Input::GetInstance().GetIsPushedTriggerButton(true) || Input::GetInstance().GetIsPushedTriggerButton(true))
		{
			//���j���[���J���Ă��Ȃ�������
			if (!own->GetMenu())
			{
				ChangeState(StateKind::StrongAttack);
				return;
			}

		}

		//���j���[���J����Ă��Ȃ�����{�^���������ꂽ��State������ɂ���
		if (Input::GetInstance().IsReleased("Input_Roll") && !own->GetMenu() && own->GetAction())
		{
			ChangeState(StateKind::Roll);
			return;
		}

		//���j���[���J����Ă��Ȃ��ăA�C�e���{�^���������ꂽ��A�C�e�����g�p����
		if (Input::GetInstance().IsTriggered("X") && !own->GetMenu())
		{
			ChangeState(StateKind::Item);
			return;
		}

		//���j���[���J����Ă��Ȃ��ăV�[���h�𑕔����Ă����ꍇ�K�[�h�{�^������������State���K�[�h�ɂ���
		if (Input::GetInstance().IsTriggered("Input_Shield") && m_equipmentShield && !own->GetMenu())
		{
			ChangeState(StateKind::Guard);
			return;
		}
	}
	//�X�^�~�i�؂�̎��͓���̃A�j���[�V���������ł���
	else if (!own->GetRest() && own->GetStaminaBreak())
	{
		//���X�e�B�b�N�����͂���Ă�����State��Walk���ɂ���
		if (Input::GetInstance().GetInputStick(false).first != 0.0f ||
			Input::GetInstance().GetInputStick(false).second != 0.0f)
		{
			ChangeState(StateKind::Walk);
			return;
		}

		//���j���[���J����Ă��Ȃ��ăA�C�e���{�^���������ꂽ��A�C�e�����g�p����
		if (Input::GetInstance().IsTriggered("X") && !own->GetMenu())
		{
			ChangeState(StateKind::Item);
			return;
		}

		//���j���[���J����Ă��Ȃ��ăV�[���h�𑕔����Ă����ꍇ�K�[�h�{�^������������State���K�[�h�ɂ���
		if (Input::GetInstance().IsTriggered("Input_Shield") && m_equipmentShield && !own->GetMenu())
		{
			ChangeState(StateKind::Guard);
			return;
		}
	}
}
