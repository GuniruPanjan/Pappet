#include "PlayerStateDamage.h"
#include "Character/CharacterBase.h"
#include "External/CsvLoad.h"
#include "Input/Input.h"
#include "Character/Player.h"


namespace
{
	bool cFrame = false;
}

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="chara"></param>
PlayerStateDamage::PlayerStateDamage(std::shared_ptr<CharacterBase> chara) :
	StateBase(chara)
{
	m_equipmentShield = chara->GetShield();

	//���O���������ꂽ��Ԃł�����
	cFrame = chara->GetGuard();

	//�ŏ��Ƀq�b�g��Ԃ�����
	m_hit = chara->GetHit();
	//���݂̃X�e�[�g���U���q�b�g��Ԃɂ���
	m_nowState = StateKind::Damage;
	auto animName = GetHitAnim();
	chara->ChangeStateAnim(CsvLoad::GetInstance().GetAnimData(chara->GetCharacterName(), animName), true);
	chara->NotInitAnim(false);
	chara->GetRigidbody()->SetJump(false);
	chara->GetRigidbody()->SetJumpNow(false);
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
PlayerStateDamage::~PlayerStateDamage()
{
}

/// <summary>
/// ������
/// </summary>
/// <param name="md"></param>
void PlayerStateDamage::Init(int md)
{
	m_stageCol = md;
}

/// <summary>
/// �X�V
/// </summary>
void PlayerStateDamage::Update()
{
	//�����傪�v���C���[���ǂ������`�F�b�N����
	if (!CheckState()) return;

	//�����Ă���L�����N�^�[�x�[�X�N���X���v���C���[�N���X�ɃL���X�g����(�_�E���L���X�g)
	auto own = std::dynamic_pointer_cast<Player>(m_pChara.lock());

	//���O�����I��
	own->FrameEndStateAnim(CsvLoad::GetInstance().GetAnimData(own->GetCharacterName(), "ShieldTransition"), own->GetShieldFrame(), cFrame);

	//�t���[������I��
	own->SetAvoidance(false);

	//�U���Ȃǂ̔���������I������
	own->SetJumpAttack(false);
	own->SetStrongAttack(false);

	//�v���C���[�̑��x��0�ɂ���
	auto prevVel = own->GetRigidbody()->GetVelocity();
	own->GetRigidbody()->SetVelocity(MyLibrary::LibVec3(0.0f, prevVel.y, 0.0f));

	//�A�j���[�V�������I��������
	if (own->GetEndAnim())
	{
		//�X�^�~�i�؂ꂶ��Ȃ������ꍇ
		if (!own->GetStaminaBreak())
		{
			//�V�[���h�𑕔����Ă����ꍇ�K�[�h�{�^������������State���K�[�h�ɂ���
			if (m_equipmentShield)
			{
				//�K�[�h�{�^�����������Ƃ������������Ă����ꍇ
				if (Input::GetInstance().IsTriggered("Input_Shield") || Input::GetInstance().IsPushed("Input_Shield"))
				{
					ChangeState(StateKind::Guard);
					return;
				}
			}

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
		}
		//�X�^�~�i�؂ꂾ�����ꍇ
		else
		{
			//�V�[���h�𑕔����Ă����ꍇ�K�[�h�{�^������������State���K�[�h�ɂ���
			if (m_equipmentShield)
			{
				//�K�[�h�{�^�����������Ƃ������������Ă����ꍇ
				if (Input::GetInstance().IsTriggered("Input_Shield") || Input::GetInstance().IsPushed("Input_Shield"))
				{
					ChangeState(StateKind::Guard);
					return;
				}
			}

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
		}

		
	}
}

std::string PlayerStateDamage::GetHitAnim()
{
	if (m_hit)
	{
		return std::string("Hit");
	}
	else
	{
		return std::string("ShieldImpact");
	}
}
