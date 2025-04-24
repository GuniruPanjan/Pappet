#include "PlayerStateEnterBoss.h"
#include "Character/CharacterBase.h"
#include "External/CsvLoad.h"
#include "Input/Input.h"
#include "Character/Player.h"

namespace
{
	//�s���ł̈ړ�����
	constexpr float cMove = 0.5f;
}

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="chara"></param>
PlayerStateEnterBoss::PlayerStateEnterBoss(std::shared_ptr<CharacterBase> chara) :
	StateBase(chara),
	m_moveVector()
{
	m_equipmentShield = chara->GetShield();

	//���݂̃X�e�[�g���{�X�����ɓ����Ԃɂ���
	m_nowState = StateKind::EnterBoss;
	chara->ChangeStateAnim(CsvLoad::GetInstance().GetAnimData(chara->GetCharacterName(), "BossEnter"), true);
	chara->NotInitAnim(true);
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
PlayerStateEnterBoss::~PlayerStateEnterBoss()
{
}

/// <summary>
/// ������
/// </summary>
/// <param name="md"></param>
void PlayerStateEnterBoss::Init(int md)
{
	m_stageCol = md;
}

/// <summary>
/// �X�V
/// </summary>
void PlayerStateEnterBoss::Update()
{
	//�����傪�v���C���[���ǂ������`�F�b�N����
	if (!CheckState()) return;

	//�����Ă���L�����N�^�[�x�[�X�N���X���v���C���[�N���X�ɃL���X�g����(�_�E���L���X�g)
	auto own = std::dynamic_pointer_cast<Player>(m_pChara.lock());

	//�{�X�����ɓ���s���ňړ����鋗��
	m_moveVector = VScale(VGet(sinf(own->GetAngle()), 0.0f, cosf(own->GetAngle())), cMove);

	//�A���O���̕����Ɉ�苗���ړ���������
	MyLibrary::LibVec3 prevVelocity = own->GetRigidbody()->GetVelocity();
	MyLibrary::LibVec3 newVelocity = MyLibrary::LibVec3(-m_moveVector.x, prevVelocity.y, -m_moveVector.z);
	own->GetRigidbody()->SetVelocity(newVelocity);

	//�A�j���[�V�������I��������
	if (own->GetEndAnim())
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

		//�K�[�h�{�^������������State���K�[�h�ɂ���
		if (Input::GetInstance().IsTriggered("Input_Shield") && m_equipmentShield)
		{
			ChangeState(StateKind::Guard);
			return;
		}
	}
}
