#include "PlayerStateTaking.h"
#include "Character/CharacterBase.h"
#include "External/CsvLoad.h"
#include "Input/Input.h"
#include "Character/Player.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="chara"></param>
PlayerStateTaking::PlayerStateTaking(std::shared_ptr<CharacterBase> chara) :
	StateBase(chara),
	m_animEnd(false)
{
	m_equipmentShield = chara->GetShield();

	//���݂̃X�e�[�g���A�C�e���擾��Ԃɂ���
	m_nowState = StateKind::Taking;
	chara->ChangeStateAnim(CsvLoad::GetInstance().GetAnimData(chara->GetCharacterName(), "Taking"), true);
	chara->NotInitAnim(false);
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
PlayerStateTaking::~PlayerStateTaking()
{
}

/// <summary>
/// ������
/// </summary>
/// <param name="md"></param>
void PlayerStateTaking::Init(int md)
{
	m_stageCol = md;
}

/// <summary>
/// �X�V
/// </summary>
void PlayerStateTaking::Update()
{
	//�����傪�v���C���[���ǂ������`�F�b�N����
	if (!CheckState()) return;

	//�����Ă���L�����N�^�[�x�[�X�N���X���v���C���[�N���X�ɃL���X�g����(�_�E���L���X�g)
	auto own = std::dynamic_pointer_cast<Player>(m_pChara.lock());

	//�v���C���[�̑��x��0�ɂ���
	auto prevVel = own->GetRigidbody()->GetVelocity();
	own->GetRigidbody()->SetVelocity(MyLibrary::LibVec3(0.0f, prevVel.y, 0.0f));

	if (own->GetFrame() >= 60.0f)
	{
		m_animEnd = true;
	}

	//�A�j���[�V�������I��������
	if (m_animEnd)
	{
		//�X�^�~�i�؂ꂶ��Ȃ������ꍇ
		if (!own->GetStaminaBreak())
		{
			//���X�e�B�b�N�����͂���Ă�����State��Walk�ɂ���
			if (Input::GetInstance().GetInputStick(false).first != 0.0f ||
				Input::GetInstance().GetInputStick(false).second != 0.0f)
			{
				//�_�b�V���{�^���������ꂽ��State���_�b�V���ɂ���
				if (Input::GetInstance().IsPushed("Input_Dash"))
				{
					ChangeState(StateKind::Dash);
					m_animEnd = false;
					return;
				}
				//������Ă��Ȃ�������walk�ɂ���
				else
				{
					ChangeState(StateKind::Walk);
					m_animEnd = false;
					return;
				}

			}

			//���X�e�B�b�N�����͂���ĂȂ�������State��Idle�ɂ���
			if (Input::GetInstance().GetInputStick(false).first == 0.0f ||
				Input::GetInstance().GetInputStick(false).second == 0.0f)
			{
				ChangeState(StateKind::Idle);
				m_animEnd = false;
				return;
			}

			//�W�����v�{�^����������Ă�����State��Jump�ɂ���
			if (Input::GetInstance().IsTriggered("Input_Jump"))
			{
				ChangeState(StateKind::Jump);
				m_animEnd = false;
				return;
			}

			//�U���{�^����������Ă�����State���U���ɂ���
			if (Input::GetInstance().IsTriggered("Input_Attack"))
			{
				ChangeState(StateKind::Attack);
				m_animEnd = false;
				return;
			}

			//���U���{�^����������Ă�����State�����U���ɂ���
			if (Input::GetInstance().GetIsPushedTriggerButton(true) || Input::GetInstance().GetIsPushedTriggerButton(true))
			{
				ChangeState(StateKind::StrongAttack);
				m_animEnd = false;
				return;
			}

			//����{�^���������ꂽ��State������ɂ���
			if (Input::GetInstance().IsTriggered("Input_Roll"))
			{
				ChangeState(StateKind::Roll);
				m_animEnd = false;
				return;
			}

			//�A�C�e���{�^���������ꂽ��A�C�e�����g�p����
			if (Input::GetInstance().IsTriggered("X"))
			{
				ChangeState(StateKind::Item);
				m_animEnd = false;
				return;
			}

			//�V�[���h�𑕔����Ă����ꍇ�K�[�h�{�^������������State���K�[�h�ɂ���
			if (Input::GetInstance().IsTriggered("Input_Shield") && m_equipmentShield)
			{
				ChangeState(StateKind::Guard);
				m_animEnd = false;
				return;
			}
		}
		//�X�^�~�i�؂ꂾ�����ꍇ
		else
		{
			//���X�e�B�b�N�����͂���Ă�����State��Walk�ɂ���
			if (Input::GetInstance().GetInputStick(false).first != 0.0f ||
				Input::GetInstance().GetInputStick(false).second != 0.0f)
			{
				ChangeState(StateKind::Walk);
				m_animEnd = false;
				return;

			}

			//���X�e�B�b�N�����͂���ĂȂ�������State��Idle�ɂ���
			if (Input::GetInstance().GetInputStick(false).first == 0.0f ||
				Input::GetInstance().GetInputStick(false).second == 0.0f)
			{
				ChangeState(StateKind::Idle);
				m_animEnd = false;
				return;
			}

			//�A�C�e���{�^���������ꂽ��A�C�e�����g�p����
			if (Input::GetInstance().IsTriggered("X"))
			{
				ChangeState(StateKind::Item);
				m_animEnd = false;
				return;
			}

			//�V�[���h�𑕔����Ă����ꍇ�K�[�h�{�^������������State���K�[�h�ɂ���
			if (Input::GetInstance().IsTriggered("Input_Shield") && m_equipmentShield)
			{
				ChangeState(StateKind::Guard);
				m_animEnd = false;
				return;
			}
		}

		
	}
}
