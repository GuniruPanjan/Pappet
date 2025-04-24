#include "PlayerStateAttack.h"
#include "Character/CharacterBase.h"
#include "External/CsvLoad.h"
#include "Input/Input.h"
#include "Character/Player.h"

namespace
{
	/*�A�i���O�X�e�B�b�N�ɂ��ړ��֘A*/
	constexpr float cAnalogRangeMin = 0.1f;		//�A�i���O�X�e�B�b�N�̓��͔���ŏ��͈�
	constexpr float cAnalogRangeMax = 0.8f;		//�A�i���O�X�e�B�b�N�̓��͔���ő�͈�
	constexpr float cAnalogInputMax = 1000.0f;	//�A�i���O�X�e�B�b�N������͂����x�N�g���̍ő�

	//�U���ɂ��ړ����x
	constexpr float cAttackMove1 = 0.5f;
	constexpr float cAttackMove2 = 0.3f;
	constexpr float cAttackMove3 = 0.2f;
	//�U�����x
	constexpr float cAttackSpeed = 1.0f;

	//�ǉ��U���\�ɂ���
	bool cAttack = false;

	//���菉����
	bool cOne = false;
}

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="chara"></param>
PlayerStateAttack::PlayerStateAttack(std::shared_ptr<CharacterBase> chara) :
	StateBase(chara),
	m_leftX(0),
	m_leftZ(0),
	m_attackNumber(0),
	m_input(false),
	m_endAnim(false),
	m_attackMove(),
	m_move(0.0f)
{
	//���������Ԃ�
	m_equipmentSword = chara->GetSword();
	m_equipmentFist = chara->GetFist();
	m_equipmentState = chara->GetEquipment();
	m_equipmentShield = chara->GetShield();
	//���݂̃X�e�[�g���U����Ԃɂ���
	m_nowState = StateKind::Attack;
	auto animName = GetAttackAnim();
	chara->ChangeStateAnim(CsvLoad::GetInstance().GetAnimData(chara->GetCharacterName(), animName), true, cAttackSpeed);
	chara->NotInitAnim(false);
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
PlayerStateAttack::~PlayerStateAttack()
{
}

/// <summary>
/// ������
/// </summary>
/// <param name="md"></param>
void PlayerStateAttack::Init(int md)
{
	m_stageCol = md;
}

/// <summary>
/// �X�V
/// </summary>
void PlayerStateAttack::Update()
{
	//�����傪�v���C���[���ǂ������`�F�b�N����
	if (!CheckState()) return;

	//�����Ă���L�����N�^�[�x�[�X�N���X���v���C���[�N���X�ɃL���X�g����
	auto own = std::dynamic_pointer_cast<Player>(m_pChara.lock());

	//���������Ȃǂ̏ꍇ
	if (m_equipmentSword || m_equipmentFist)
	{
		//�U����i�K��
		if (own->GetFrame() <= 40.0f)
		{
			own->SetAttackNumber(1);

			//���͎�t����
			if (own->GetFrame() == 0.0f) m_input = true;
			if (own->GetFrame() == 5.0f) m_input = false;

			//�U�����͎���
			if (own->GetFrame() >= 20.0f)
			{
				//�X�^�~�i�������Ԃ�R����͂���Ǝ��̍U���ɂȂ���
				if (Input::GetInstance().IsTriggered("Input_Attack") && !own->GetStaminaBreak())
				{
					m_attackNumber = 1;
				}
			}
			//�U������
			if (own->GetFrame() == 25.0f)
			{
				if (!cOne)
				{
					own->SetAttackInit(true);

					cOne = true;
				}
			}
			if (own->GetFrame() == 35.0f)
			{
				if (cOne)
				{
					own->SetAttackEnd(true);

					cOne = false;
				}
			}


			//�U���ɂ��ړ����x�ݒ�
			if (own->GetFrame() <= 25.0f) m_move = 0.0f;
			if (own->GetFrame() >= 25.0f && own->GetFrame() < 35.0f) m_move = cAttackMove1;
			if (own->GetFrame() >= 35.0f && own->GetFrame() < 40.0f)
			{
				//�ǉ��U�����͂��󂯕t���ĂȂ�������
				if (m_attackNumber != 1)
				{
					//�A�j���[�V�����X�s�[�h��ς���
					own->SetAnimSpeed(0.3f);
				}

				m_move = cAttackMove2;
			}
			if (own->GetFrame() == 40.0f) m_move = 0.0f;

			//�U����i�K�ڏI��
			if (own->GetFrame() >= 38.0f && m_attackNumber != 1)
			{
				m_endAnim = true;
			}
		}
		//�U����i�K��
		else if (own->GetFrame() <= 70.0f && own->GetFrame() > 40.0f && m_attackNumber == 1)
		{
			own->SetAttackNumber(2);

			//���͎�t����
			if (own->GetFrame() == 40.0f) m_input = true;
			if (own->GetFrame() == 45.0f) m_input = false;

			if (own->GetFrame() >= 40.0f && own->GetFrame() <= 50.0f)
			{
				//�A�j���[�V�����X�s�[�h��ς���
				own->SetAnimSpeed(cAttackSpeed);
			}

			//�U�����͎���
			if (own->GetFrame() >= 50.0f)
			{
				//�X�^�~�i�������Ԃ�R����͂���Ǝ��̍U���ɂȂ���
				if (Input::GetInstance().IsTriggered("Input_Attack") && !own->GetStaminaBreak())
				{
					cAttack = true;
				}
			}
			//�U������
			if (own->GetFrame() >= 55.0f && own->GetFrame() <= 60.0f)
			{
				if (!cOne)
				{
					own->SetAttackInit(true);

					cOne = true;
				}

			}

			if (own->GetFrame() >= 65.0f && own->GetFrame() <= 70.0f)
			{
				if (cOne)
				{
					own->SetAttackEnd(true);

					cOne = false;
				}

			}

			//�U���ɂ��ړ����x�ݒ�
			if (own->GetFrame() <= 55.0F) m_move = 0.0f;
			if (own->GetFrame() >= 55.0f && own->GetFrame() <= 65.0f) m_move = cAttackMove1;
			if (own->GetFrame() >= 65.0f && own->GetFrame() <= 70.0f)
			{
				//�ǉ��U�����͂��󂯕t���ĂȂ�������
				if (!cAttack)
				{
					//�A�j���[�V�����X�s�[�h��ς���
					own->SetAnimSpeed(0.3f);
				}

				m_move = cAttackMove3;
			}
			if (own->GetFrame() == 70.0f) m_move = 0.0f;

			//�U����i��ڏI��
			if (own->GetFrame() >= 68.0f && cAttack)
			{
				//��񂾂����s
				if (cAttack)
				{
					m_attackNumber = 2;

					cAttack = false;
				}
			}
			//�U���I��
			else if (own->GetFrame() >= 68.0f && m_attackNumber != 2)
			{
				

				m_endAnim = true;
			}
		}
		//�U���O�i�K��
		else if (own->GetFrame() <= 112.0f && own->GetFrame() > 70.0f && m_attackNumber == 2)
		{
			own->SetAttackNumber(3);

			if (own->GetFrame() >= 70.0f && own->GetFrame() <= 80.0f)
			{
				//�A�j���[�V�����X�s�[�h��ς���
				own->SetAnimSpeed(cAttackSpeed);
			}
			//�U������
			if (own->GetFrame() >= 85.0f && own->GetFrame() <= 90.0f)
			{
				int a = 1;

				if (!cOne)
				{
					own->SetAttackInit(true);

					cOne = true;
				}
			}
			if (own->GetFrame() >= 95.0f && own->GetFrame() <= 100.0f)
			{
				if (cOne)
				{
					own->SetAttackEnd(true);

					cOne = false;
				}
			}

			//���͎�t����
			if (own->GetFrame() == 71.0f) m_input = true;
			if (own->GetFrame() == 75.0f) m_input = false;

			//�U���ɂ��ړ����x�ݒ�
			if (own->GetFrame() <= 85.0f) m_move = 0.0f;
			if (own->GetFrame() >= 85.0f && own->GetFrame() <= 95.0f) m_move = cAttackMove1;
			if (own->GetFrame() >= 95.0f && own->GetFrame() <= 110.0f) m_move = cAttackMove3;
			if (own->GetFrame() == 110.0f) m_move = 0.0f;

			//�U���O�i��ڏI��
			if (own->GetFrame() >= 110.0f)
			{
				m_endAnim = true;
			}
		}
	}

	//���͎�t
	if (m_input)
	{
		//�R���g���[���[�̍��X�e�B�b�N�̓��͂��擾
		auto input = Input::GetInstance().GetInputStick(false);

		m_leftX = input.first;
		m_leftZ = input.second;
	}

	//�ړ����������肷��
	auto moveDir = VGet(m_leftX, 0.0f, -m_leftZ);
	//�ړ��x�N�g���̒������擾����
	float len = VSize(moveDir);

	//�x�N�g���̒�����0.0�`1.0�̊����ɕϊ�����
	float rate = len / cAnalogInputMax;

	//�A�i���O�X�e�B�b�N�����Ȕ͈͂����O����(�f�b�h�]�[��)
	rate = (rate - cAnalogRangeMin) / (cAnalogRangeMax - cAnalogRangeMin);
	rate = min(rate, 1.0f);
	rate = max(rate, 0.0f);

	//���x������ł���̂ňړ��x�N�g���ɔ��f����
	moveDir = VNorm(moveDir);
	float speed = own->GetStatus().s_speed * rate;

	//�����x�N�g���ƈړ��͂������Ĉړ��x�N�g���𐶐�����
	auto moveVec = VScale(moveDir, speed);

	//camera�̊p�x����
	//�R���g���[���[�ɂ��ړ����������肷��
	MATRIX mtx = MGetRotY(own->GetCameraAngle() + DX_PI_F);
	moveVec = VTransform(moveVec, mtx);

	//���C�u�����̃x�N�^�[�ɕϊ�����
	MyLibrary::LibVec3 move = MyLibrary::LibVec3(static_cast<float>(moveVec.x), static_cast<float>(moveVec.y), static_cast<float>(moveVec.z));
	//�L�����N�^�[�̃A���O�������߂�
	own->SetAngle(atan2f(-moveVec.z, moveVec.x) - DX_PI_F / 2);

	m_attackMove = VScale(VGet(sinf(own->GetAngle()), 0.0f, cosf(own->GetAngle())), m_move);

	//�ړ����x�����肷��
	MyLibrary::LibVec3 prevVelocity = own->GetRigidbody()->GetVelocity();
	MyLibrary::LibVec3 newVelocity = MyLibrary::LibVec3(-m_attackMove.x, prevVelocity.y, -m_attackMove.z);
	own->GetRigidbody()->SetVelocity(newVelocity);

	//�A�j���[�V�����I����
	if (m_endAnim)
	{
		//�U���I��
		own->SetAttackNumber(0);

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

			//�V�[���h�𑕔����Ă����ꍇ�K�[�h�{�^������������State���K�[�h�ɂ���
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
			if (Input::GetInstance().GetInputStick(false).first == 0.0f &&
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

			//�V�[���h�𑕔����Ă����ꍇ�K�[�h�{�^������������State���K�[�h�ɂ���
			if (Input::GetInstance().IsTriggered("Input_Shield") && m_equipmentShield)
			{
				ChangeState(StateKind::Guard);
				return;
			}
		}
		
	}

}

/// <summary>
/// �U���A�j���X�V
/// </summary>
/// <returns></returns>
std::string PlayerStateAttack::GetAttackAnim()
{
	//���������Ȃǂ̏ꍇ
	if (m_equipmentSword || m_equipmentFist)
	{
		//�U��
		return std::string("Attack1");
	}
}
