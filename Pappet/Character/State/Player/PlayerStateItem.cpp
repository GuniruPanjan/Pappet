#include "PlayerStateItem.h"
#include "Character/CharacterBase.h"
#include "External/CsvLoad.h"
#include "Input/Input.h"
#include "Character/Player.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="chara"></param>
PlayerStateItem::PlayerStateItem(std::shared_ptr<CharacterBase> chara) :
	StateBase(chara)
{
	m_equipmentShield = chara->GetShield();

	//���݂̃X�e�[�g���A�C�e���g�p��Ԃɂ���
	m_nowState = StateKind::Item;
	chara->ChangeStateAnim(CsvLoad::GetInstance().GetAnimData(chara->GetCharacterName(), "Recovery"), true);
	chara->NotInitAnim(false);
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
PlayerStateItem::~PlayerStateItem()
{
}

/// <summary>
/// ������
/// </summary>
/// <param name="md"></param>
void PlayerStateItem::Init(int md)
{
	m_stageCol = md;
}

/// <summary>
/// �X�V
/// </summary>
void PlayerStateItem::Update()
{
	//�����傪�v���C���[���ǂ������`�F�b�N����
	if (!CheckState()) return;

	//�����Ă���L�����N�^�[�x�[�X�N���X���v���C���[�N���X�ɃL���X�g����(�_�E���L���X�g)
	auto own = std::dynamic_pointer_cast<Player>(m_pChara.lock());

	//�v���C���[�̑��x��0�ɂ���
	auto prevVel = own->GetRigidbody()->GetVelocity();
	own->GetRigidbody()->SetVelocity(MyLibrary::LibVec3(0.0f, prevVel.y, 0.0f));

	//�A�j���[�V�������I��������
	if (own->GetEndAnim())
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
					return;
				}
				//������Ă��Ȃ�������walk�ɂ���
				else
				{
					ChangeState(StateKind::Walk);
					return;
				}

			}

			//���X�e�B�b�N�����͂���ĂȂ�������State��Idle�ɂ���
			if (Input::GetInstance().GetInputStick(false).first == 0.0f ||
				Input::GetInstance().GetInputStick(false).second == 0.0f)
			{
				ChangeState(StateKind::Idle);
				return;
			}

			//�W�����v�{�^����������Ă�����State��Jump�ɂ���
			if (Input::GetInstance().IsTriggered("Input_Jump"))
			{
				ChangeState(StateKind::Jump);
				return;
			}

			//�U���{�^����������Ă�����State���U���ɂ���
			if (Input::GetInstance().IsTriggered("Input_Attack"))
			{
				ChangeState(StateKind::Attack);
				return;
			}

			//���U���{�^����������Ă�����State�����U���ɂ���
			if (Input::GetInstance().GetIsPushedTriggerButton(true) || Input::GetInstance().GetIsPushedTriggerButton(true))
			{
				ChangeState(StateKind::StrongAttack);
				return;
			}

			//����{�^���������ꂽ��State������ɂ���
			if (Input::GetInstance().IsTriggered("Input_Roll"))
			{
				ChangeState(StateKind::Roll);
				return;
			}

			//�A�C�e���{�^���������ꂽ��A�C�e�����g�p����
			if (Input::GetInstance().IsTriggered("X"))
			{
				ChangeState(StateKind::Item);
				return;
			}

			//�V�[���h�������Ă����ꍇ�K�[�h�{�^������������State���K�[�h�ɂ���
			if (Input::GetInstance().IsTriggered("Input_Shield") && m_equipmentShield)
			{
				ChangeState(StateKind::Guard);
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
				return;

			}

			//���X�e�B�b�N�����͂���ĂȂ�������State��Idle�ɂ���
			if (Input::GetInstance().GetInputStick(false).first == 0.0f ||
				Input::GetInstance().GetInputStick(false).second == 0.0f)
			{
				ChangeState(StateKind::Idle);
				return;
			}

			//�A�C�e���{�^���������ꂽ��A�C�e�����g�p����
			if (Input::GetInstance().IsTriggered("X"))
			{
				ChangeState(StateKind::Item);
				return;
			}

			//�V�[���h�������Ă����ꍇ�K�[�h�{�^������������State���K�[�h�ɂ���
			if (Input::GetInstance().IsTriggered("Input_Shield") && m_equipmentShield)
			{
				ChangeState(StateKind::Guard);
				return;
			}
		}

		
	}
}
