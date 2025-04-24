#include "PlayerStateStrongAttack.h"
#include "Character/CharacterBase.h"
#include "External/CsvLoad.h"
#include "Input/Input.h"
#include "Character/Player.h"

namespace
{
	/*�A�i���O�X�e�B�b�N�ɂ��ړ��֘A*/
	constexpr float cAnalogRangeMin = 0.0f;		//�A�i���O�X�e�B�b�N�̓��͔���ŏ��͈�
	constexpr float cAnalogRangeMax = 0.8f;		//�A�i���O�X�e�B�b�N�̓��͔���ő�͈�
	constexpr float cAnalogInputMax = 1000.0f;	//�A�i���O�X�e�B�b�N������͂����x�N�g���̍ő�

	//����ł̈ړ�����
	constexpr float cStrongMove1 = 0.5f;
	constexpr float cStrongMove2 = 1.0f;
	constexpr float cStrongMove3 = 4.0f;
	constexpr float cStrongMove4 = 0.0f;

	//����̕������������
	bool cOneAvoidance = false;
}

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="chara"></param>
PlayerStateStrongAttack::PlayerStateStrongAttack(std::shared_ptr<CharacterBase> chara) :
	StateBase(chara),
	m_move(0.0f),
	m_strongMove()
{
	//�W�����v��Ԃ���J�ڂ��Ă�����\�ɂ���@
	m_jumping = chara->GetJumpCan();

	//���݂̃X�e�[�g�����U����Ԃɂ���
	m_nowState = StateKind::StrongAttack;

	m_equipmentSword = chara->GetSword();
	m_equipmentFist = chara->GetFist();
	m_equipmentShield = chara->GetShield();

	auto animName = GetStrongAnim();
	chara->ChangeStateAnim(CsvLoad::GetInstance().GetAnimData(chara->GetCharacterName(), animName), true);
	chara->NotInitAnim(true);
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
PlayerStateStrongAttack::~PlayerStateStrongAttack()
{
}

/// <summary>
/// ������
/// </summary>
/// <param name="md"></param>
void PlayerStateStrongAttack::Init(int md)
{
	m_stageCol = md;
}

/// <summary>
/// �X�V
/// </summary>
void PlayerStateStrongAttack::Update()
{
	//�����傪�v���C���[���ǂ������`�F�b�N����
	if (!CheckState()) return;

	//�����Ă���L�����N�^�[�x�[�X�N���X���v���C���[�N���X�ɃL���X�g����(�_�E���L���X�g)
	auto own = std::dynamic_pointer_cast<Player>(m_pChara.lock());

	//��񂾂��󂯕t����
	if (!cOneAvoidance)
	{
		//�R���g���[���[�̍��X�e�B�b�N�̓��͂��擾
		auto input = Input::GetInstance().GetInputStick(false);

		m_leftX = input.first;
		m_leftZ = input.second;

		cOneAvoidance = true;
	}

	//�f��̋��U��
	if (m_equipmentFist && !m_jumping)
	{
		//���U�������Ԃ�
		own->SetStrongAttack(true);

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

		//�t���[���ł̈ړ�
		if (own->GetFrame() >= 0.0f && own->GetFrame() <= 20.0f)
		{
			m_move = cStrongMove1;
		}
		else if (own->GetFrame() >= 20.0f && own->GetFrame() <= 40.0f)
		{
			m_move = cStrongMove2;
		}
		else if (own->GetFrame() >= 40.0f && own->GetFrame() <= 45.0f)
		{
			m_move = -cStrongMove3;
		}
		else
		{
			m_move = cStrongMove4;
		}

		m_strongMove = VScale(VGet(sinf(own->GetAngle()), 0.0f, cosf(own->GetAngle())), m_move);

		//�ړ����x�����肷��
		MyLibrary::LibVec3 prevVelocity = own->GetRigidbody()->GetVelocity();
		MyLibrary::LibVec3 newVelocity = MyLibrary::LibVec3(-m_strongMove.x, prevVelocity.y, -m_strongMove.z);
		own->GetRigidbody()->SetVelocity(newVelocity);;
	}
	//����̋��U��
	else if (m_equipmentSword && !m_jumping)
	{
		//���U�������Ԃ�
		own->SetStrongAttack(true);

		//�ړ����x�����肷��
		MyLibrary::LibVec3 prevVelocity = own->GetRigidbody()->GetVelocity();
		MyLibrary::LibVec3 newVelocity = MyLibrary::LibVec3(0.0f, prevVelocity.y, 0.0f);
		own->GetRigidbody()->SetVelocity(newVelocity);
	}
	//�W�����v�̋��U��
	else if (m_jumping)
	{
		//�W�����v�U�������Ԃ�
		own->SetJumpAttack(true);

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

		//�t���[���ł̈ړ�
		if (own->GetFrame() >= 0.0f && own->GetFrame() <= 20.0f)
		{
			m_move = cStrongMove2;
		}
		else if (own->GetFrame() >= 20.0f && own->GetFrame() <= 30.0f)
		{
			m_move = cStrongMove1;
		}
		else
		{
			m_move = cStrongMove4;
		}


		m_strongMove = VScale(VGet(sinf(own->GetAngle()), 0.0f, cosf(own->GetAngle())), m_move);

		//�ړ����x�����肷��
		MyLibrary::LibVec3 prevVelocity = own->GetRigidbody()->GetVelocity();
		MyLibrary::LibVec3 newVelocity = MyLibrary::LibVec3(-m_strongMove.x, prevVelocity.y, -m_strongMove.z);
		own->GetRigidbody()->SetVelocity(newVelocity);
	}

	//�A�j���[�V�������I��������
	if (own->GetEndAnim())
	{
		//����������
		own->SetStrongAttack(false);
		own->SetJumpAttack(false);
		own->GetRigidbody()->SetJumpNow(false);

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
					m_jumping = false;

					ChangeState(StateKind::Dash);
					return;
				}
				//������Ă��Ȃ�������walk�ɂ���
				else
				{
					m_jumping = false;

					ChangeState(StateKind::Walk);
					return;
				}

			}

			//���X�e�B�b�N�����͂���ĂȂ�������State��Idle�ɂ���
			if (Input::GetInstance().GetInputStick(false).first == 0.0f ||
				Input::GetInstance().GetInputStick(false).second == 0.0f)
			{
				m_jumping = false;

				ChangeState(StateKind::Idle);
				return;
			}

			//�W�����v�{�^����������Ă�����State��Jump�ɂ���
			if (Input::GetInstance().IsTriggered("Input_Jump"))
			{
				m_jumping = false;

				ChangeState(StateKind::Jump);
				return;
			}

			//�U���{�^����������Ă�����State���U���ɂ���
			if (Input::GetInstance().IsTriggered("Input_Attack"))
			{
				m_jumping = false;

				ChangeState(StateKind::Attack);
				return;
			}

			//���U���{�^����������Ă�����State�����U���ɂ���
			if (Input::GetInstance().GetIsPushedTriggerButton(true) || Input::GetInstance().GetIsPushedTriggerButton(true))
			{
				m_jumping = false;

				ChangeState(StateKind::StrongAttack);
				return;
			}

			//����{�^���������ꂽ��State������ɂ���
			if (Input::GetInstance().IsTriggered("Input_Roll"))
			{
				m_jumping = false;

				ChangeState(StateKind::Roll);
				return;
			}

			//�A�C�e���{�^���������ꂽ��A�C�e�����g�p����
			if (Input::GetInstance().IsTriggered("X"))
			{
				m_jumping = false;

				ChangeState(StateKind::Item);
				return;
			}

			//�V�[���h�𑕔����Ă����ꍇ�K�[�h�{�^������������State���K�[�h�ɂ���
			if (Input::GetInstance().IsTriggered("Input_Shield") && m_equipmentShield)
			{
				m_jumping = false;

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
				m_jumping = false;

				ChangeState(StateKind::Walk);
				return;

			}

			//���X�e�B�b�N�����͂���ĂȂ�������State��Idle�ɂ���
			if (Input::GetInstance().GetInputStick(false).first == 0.0f ||
				Input::GetInstance().GetInputStick(false).second == 0.0f)
			{
				m_jumping = false;

				ChangeState(StateKind::Idle);
				return;
			}

			//�A�C�e���{�^���������ꂽ��A�C�e�����g�p����
			if (Input::GetInstance().IsTriggered("X"))
			{
				m_jumping = false;

				ChangeState(StateKind::Item);
				return;
			}

			//�V�[���h�𑕔����Ă����ꍇ�K�[�h�{�^������������State���K�[�h�ɂ���
			if (Input::GetInstance().IsTriggered("Input_Shield") && m_equipmentShield)
			{
				m_jumping = false;

				ChangeState(StateKind::Guard);
				return;
			}
		}

		
	}
}

/// <summary>
/// ���U���A�j���[�V������ύX����
/// </summary>
/// <returns></returns>
std::string PlayerStateStrongAttack::GetStrongAnim()
{
	//�󒆂ł̋��U��
	if (m_jumping)
	{
		return std::string("JumpStrongAttack");
	}
	//�f��̋��U��
	else if (m_equipmentFist)
	{
		return std::string("StrongAttack1");
	}
	//���̋��U��
	else if (m_equipmentSword)
	{
		return std::string("StrongAttack2");

	}

}
