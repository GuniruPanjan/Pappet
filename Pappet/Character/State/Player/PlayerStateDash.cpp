#include "PlayerStateDash.h"
#include "Input/Input.h"
#include "Character/CharacterBase.h"
#include "Character/Player.h"
#include "External/CsvLoad.h"

namespace
{
	/*�A�i���O�X�e�B�b�N�ɂ��ړ��֘A*/
	constexpr float cAnalogRangeMin = 0.0f;		//�A�i���O�X�e�B�b�N�̓��͔���ŏ��͈�
	constexpr float cAnalogRangeMax = 0.8f;		//�A�i���O�X�e�B�b�N�̓��͔���ő�͈�
	constexpr float cAnalogInputMax = 1000.0f;	//�A�i���O�X�e�B�b�N������͂����x�N�g���̍ő�

	//�_�b�V���ɂ��������鑬�x
	constexpr float cDashSpeed = 4.0f;
}

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="chara"></param>
PlayerStateDash::PlayerStateDash(std::shared_ptr<CharacterBase> chara) :
	StateBase(chara),
	m_noInputFrame(0)
{
	m_equipmentShield = chara->GetShield();
	m_equipmentState = chara->GetEquipment();

	//���݂̃X�e�[�g���_�b�V����Ԃɂ���
	m_nowState = StateKind::Dash;
	auto animName = GetRunAnim();
	chara->ChangeStateAnim(CsvLoad::GetInstance().GetAnimData(chara->GetCharacterName(), animName), false);
	chara->NotInitAnim(false);
	//���x��ݒ肷��
	chara->SetSpeed(cDashSpeed);
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
PlayerStateDash::~PlayerStateDash()
{
}

/// <summary>
/// ������
/// </summary>
/// <param name="md"></param>
void PlayerStateDash::Init(int md)
{
	m_stageCol = md;
}

/// <summary>
/// �X�V
/// </summary>
void PlayerStateDash::Update()
{
	//�����傪�v���C���[���ǂ������`�F�b�N����
	if (!CheckState()) return;

	//�����Ă���L�����N�^�[�x�[�X�N���X���v���C���[�N���X�ɃL���X�g����
	auto own = std::dynamic_pointer_cast<Player>(m_pChara.lock());

	//�x�����ƃX�^�~�i�؂ꂶ��Ȃ������ꍇ�͓����Ȃ�����
	if (!own->GetRest() && !own->GetStaminaBreak())
	{
		//���X�e�B�b�N�����͂���ĂȂ�������State��Idle�ɂ���
		if (Input::GetInstance().GetInputStick(false).first == 0.0f &&
			Input::GetInstance().GetInputStick(false).second == 0.0f)
		{
			if (m_noInputFrame == 2)
			{
				ChangeState(StateKind::Idle);
				return;
			}

			m_noInputFrame++;
		}
		else
		{
			m_noInputFrame = 0;
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

		//�_�b�V���{�^����������ĂȂ�������State��Walk�ɂ���
		if (!Input::GetInstance().IsPushed("Input_Dash"))
		{
			ChangeState(StateKind::Walk);
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
	//�x�����������ꍇ�����I��Idle��Ԃɂ���
	else if(own->GetRest())
	{
		ChangeState(StateKind::Idle);
		return;
	}
	//�X�^�~�i�؂ꂾ�����ꍇ
	else if (own->GetStaminaBreak())
	{
		//���X�e�B�b�N�����͂���ĂȂ�������State��Idle�ɂ���
		if (Input::GetInstance().GetInputStick(false).first != 0.0f ||
			Input::GetInstance().GetInputStick(false).second != 0.0f)
		{
			ChangeState(StateKind::Walk);
			return;
		}
		else
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

	if (m_noInputFrame == 0)
	{
		//�R���g���[���[�̍��X�e�B�b�N�̓��͂��擾
		auto input = Input::GetInstance().GetInputStick(false);

		//�ړ����������肷��
		auto moveDir = VGet(-input.first, 0.0f, input.second);
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
		own->SetModelAngle(atan2f(-moveVec.z, moveVec.x) - DX_PI_F / 2);

		//�ړ����x�����肷��
		MyLibrary::LibVec3 prevVelocity = own->GetRigidbody()->GetVelocity();
		MyLibrary::LibVec3 newVelocity = MyLibrary::LibVec3(move.x, prevVelocity.y, move.z);
		own->GetRigidbody()->SetVelocity(newVelocity);
	}
	else
	{
		//�v���C���[�̑��x��0�ɂ���
		auto prevVel = own->GetRigidbody()->GetVelocity();
		own->GetRigidbody()->SetVelocity(MyLibrary::LibVec3(0.0f, prevVel.y, 0.0f));
	}
	
}

std::string PlayerStateDash::GetRunAnim()
{
	//�������������Ă����ꍇ�̃A�j���[�V����
	if (m_equipmentState)
	{
		return std::string("ShieldRun");
	}
	//�����������Ă��Ȃ��ꍇ�̃A�j���[�V����
	else
	{
		return std::string("Run");
	}
}
