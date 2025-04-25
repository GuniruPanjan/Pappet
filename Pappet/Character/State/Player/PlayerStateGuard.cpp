#include "PlayerStateGuard.h"
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

	constexpr float cWalkAnimSpeed = 0.5f;

	//�����ɂ��������鑬�x
	constexpr float cWalkSpeed = 1.5f;
	//����ɂ��������鑬�x
	constexpr float cDashSpeed = 4.0f;

	//��񂾂��s��
	bool cOne = false;
	bool cIdle = false;
	bool cFrame = true;
}

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="chara"></param>
PlayerStateGuard::PlayerStateGuard(std::shared_ptr<CharacterBase> chara) :
	StateBase(chara),
	m_dir(),
	m_run(false)
{
	//���݂̃X�e�[�g��ҋ@��Ԃɂ���
	m_nowState = StateKind::Guard;
	chara->ChangeStateAnim(CsvLoad::GetInstance().GetAnimData(chara->GetCharacterName(), "ShieldStart"), true);
	chara->NotInitAnim(true);

	cOne = false;
	cIdle = false;
	cFrame = true;
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
PlayerStateGuard::~PlayerStateGuard()
{
}

/// <summary>
/// ������
/// </summary>
/// <param name="md"></param>
void PlayerStateGuard::Init(int md)
{
	m_stageCol = md;
}

/// <summary>
/// �X�V
/// </summary>
void PlayerStateGuard::Update()
{
	//�����傪�v���C���[���ǂ������`�F�b�N����
	if (!CheckState()) return;

	//�����Ă���L�����N�^�[�x�[�X�N���X���v���C���[�N���X�ɃL���X�g����(�_�E���L���X�g)
	auto own = std::dynamic_pointer_cast<Player>(m_pChara.lock());

	//�x�����͓����Ȃ�����
	if (!own->GetRest())
	{
		//�����Ă��Ȃ�������
		if (Input::GetInstance().GetInputStick(false).first == 0.0f &&
			Input::GetInstance().GetInputStick(false).second == 0.0f)
		{
			//�V�[���h�̍\���n�߂��I�������
			if (own->GetEndAnim() && !cIdle)
			{
				cOne = true;

				cIdle = true;
			}
		}
		else if (Input::GetInstance().GetInputStick(false).first != 0.0f ||
			Input::GetInstance().GetInputStick(false).second != 0.0f)
		{
			cOne = true;
		}


		//�����\������Ԃō��X�e�B�b�N�����͂���Ă�����V�[���h���\������Ԃɂ���
		if (Input::GetInstance().GetInputStick(false).first != 0.0f ||
			Input::GetInstance().GetInputStick(false).second != 0.0f)
		{
			//�^�[�Q�b�g���Ă��Ԃ̎�
			if (m_targetState)
			{
				if (m_dir == eDir::Forward || m_dir == eDir::Back)
				{
					own->FrameStateChangeAnim(CsvLoad::GetInstance().GetAnimData(own->GetCharacterName(), "ShieldTransition"), own->GetShieldFrame(), cFrame);
					own->SetGuard(cFrame);
				}
			}
			//�^�[�Q�b�g���Ă��Ȃ���
			else
			{
				own->FrameStateChangeAnim(CsvLoad::GetInstance().GetAnimData(own->GetCharacterName(), "ShieldTransition"), own->GetShieldFrame(), cFrame);
				own->SetGuard(cFrame);
			}
		}
		//�����\������ԂŎ~�܂����ꍇ���O�ɂ��A�j���[�V������������߂�
		else if (Input::GetInstance().GetInputStick(false).first == 0.0f &&
			Input::GetInstance().GetInputStick(false).second == 0.0f)
		{
			own->FrameEndStateAnim(CsvLoad::GetInstance().GetAnimData(own->GetCharacterName(), "ShieldTransition"), own->GetShieldFrame(), cFrame);
			own->SetGuard(cFrame);
		}

		//�����\���Ă��Ԃ��O�����ꍇ�ƃX�^�~�i�؂ꂶ��Ȃ������ꍇ
		if (!Input::GetInstance().IsPushed("Input_Shield") && !own->GetStaminaBreak())
		{
			//���O�A�j���[�V������������߂�
			own->FrameEndStateAnim(CsvLoad::GetInstance().GetAnimData(own->GetCharacterName(), "ShieldTransition"), own->GetShieldFrame(), cFrame);
			own->SetGuard(cFrame);

			//���X�e�B�b�N�����͂���Ă�����State��Walk��Dash�ɂ���
			if (Input::GetInstance().GetInputStick(false).first != 0.0f ||
				Input::GetInstance().GetInputStick(false).second != 0.0f)
			{
				//�_�b�V���{�^��������������Ă���_�b�V��
				if (Input::GetInstance().IsPushed("Input_Dash"))
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
			//�A�C�h����Ԃɂ���
			else
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
		else if (own->GetStaminaBreak())
		{
			//���O�A�j���[�V������������߂�
			own->FrameEndStateAnim(CsvLoad::GetInstance().GetAnimData(own->GetCharacterName(), "ShieldTransition"), own->GetShieldFrame(), cFrame);
			own->SetGuard(cFrame);

			//���X�e�B�b�N�����͂���Ă�����State��Walk��Dash�ɂ���
			if (Input::GetInstance().GetInputStick(false).first != 0.0f ||
				Input::GetInstance().GetInputStick(false).second != 0.0f)
			{
				ChangeState(StateKind::Walk);
				return;
			}
			//�A�C�h����Ԃɂ���
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
		}

		//�����\���Ȃ���W�����v�ł��Ȃ��悤�ɂ���
		//�W�����v�{�^����������Ă�����State��Jump�ɂ���
		if (Input::GetInstance().IsTriggered("Input_Jump"))
		{
			//���O�A�j���[�V������������߂�
			own->FrameEndStateAnim(CsvLoad::GetInstance().GetAnimData(own->GetCharacterName(), "ShieldTransition"), own->GetShieldFrame(), cFrame);
			own->SetGuard(cFrame);

			ChangeState(StateKind::Jump);
			return;
		}
		//�����\���Ȃ���U���ł��Ȃ��悤�ɂ���
		//�U���{�^����������Ă�����State���U���ɂ���
		else if (Input::GetInstance().IsTriggered("Input_Attack"))
		{
			//���O�A�j���[�V������������߂�
			own->FrameEndStateAnim(CsvLoad::GetInstance().GetAnimData(own->GetCharacterName(), "ShieldTransition"), own->GetShieldFrame(), cFrame);
			own->SetGuard(cFrame);

			ChangeState(StateKind::Attack);
			return;
		}
		//�����\���Ȃ���U���ł��Ȃ��悤�ɂ���
		//���U���{�^����������Ă�����State�����U���ɂ���
		else if (Input::GetInstance().GetIsPushedTriggerButton(true) || Input::GetInstance().GetIsPushedTriggerButton(true))
		{
			//���O�A�j���[�V������������߂�
			own->FrameEndStateAnim(CsvLoad::GetInstance().GetAnimData(own->GetCharacterName(), "ShieldTransition"), own->GetShieldFrame(), cFrame);
			own->SetGuard(cFrame);

			ChangeState(StateKind::StrongAttack);
			return;
		}
		//�����\���Ȃ������ł��Ȃ��悤�ɂ���
		//����{�^���������ꂽ��State������ɂ���
		else if (Input::GetInstance().IsTriggered("Input_Roll"))
		{
			//���O�A�j���[�V������������߂�
			own->FrameEndStateAnim(CsvLoad::GetInstance().GetAnimData(own->GetCharacterName(), "ShieldTransition"), own->GetShieldFrame(), cFrame);
			own->SetGuard(cFrame);

			ChangeState(StateKind::Roll);
			return;
		}
		//�����\���Ȃ���A�C�e�������Ȃ��悤�ɂ���
		//�A�C�e���{�^���������ꂽ��A�C�e�����g�p����
		else if (Input::GetInstance().IsTriggered("X"))
		{
			//���O�A�j���[�V������������߂�
			own->FrameEndStateAnim(CsvLoad::GetInstance().GetAnimData(own->GetCharacterName(), "ShieldTransition"), own->GetShieldFrame(), cFrame);
			own->SetGuard(cFrame);

			ChangeState(StateKind::Item);
			return;
		}



		//�R���g���[���[�̍��X�e�B�b�N�̓��͂��擾
		auto input = Input::GetInstance().GetInputStick(false);
		auto dirLog = m_dir;
		m_dir = GetDirection(input.first, input.second);

		//���O�̓��͕����ƈقȂ�Ƃ�
		if (Input::GetInstance().GetInputStick(false).first != 0.0f ||
			Input::GetInstance().GetInputStick(false).second != 0.0f)
		{
			//�_�b�V���{�^��������������Ă���_�b�V��
			if (Input::GetInstance().IsPushed("Input_Dash"))
			{
				m_run = true;

				//�X�s�[�h��ݒ肷��
				own->SetSpeed(cDashSpeed);
			}
			else
			{
				m_run = false;

				//�X�s�[�h��ݒ肷��
				own->SetSpeed(cWalkSpeed);
			}

			//�A�j���[�V������ύX����
			if (m_dir == eDir::Right || m_dir == eDir::BackRight || m_dir == eDir::ForwardRight)
			{
				//�^�[�Q�b�g���Ă��鎞��
				if (m_targetState)
				{
					//���O�A�j���[�V������������߂�
					own->FrameEndStateAnim(CsvLoad::GetInstance().GetAnimData(own->GetCharacterName(), "ShieldTransition"), own->GetShieldFrame(), cFrame);
					own->SetGuard(cFrame);
				}
			}
			if (m_dir == eDir::Left || m_dir == eDir::BackLeft || m_dir == eDir::ForwardLeft)
			{
				//�^�[�Q�b�g���Ă��鎞��
				if (m_targetState)
				{
					//���O�A�j���[�V������������߂�
					own->FrameEndStateAnim(CsvLoad::GetInstance().GetAnimData(own->GetCharacterName(), "ShieldTransition"), own->GetShieldFrame(), cFrame);
					own->SetGuard(cFrame);
				}
			}

			auto animName = GetGuardAnim(m_dir);
			own->ChangeStateAnim(CsvLoad::GetInstance().GetAnimData("Player", animName), false, cWalkAnimSpeed, m_revese);


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

			//�����Ă�Ƃ��ƃ^�[�Q�b�g���Ă��Ȃ���
			if (!m_targetState || m_run)
			{
				//�L�����N�^�[�̃A���O�������߂�
				own->SetModelAngle(atan2f(-moveVec.z, moveVec.x) - DX_PI_F / 2);
			}


			//�ړ����x�����肷��
			MyLibrary::LibVec3 prevVelocity = own->GetRigidbody()->GetVelocity();
			MyLibrary::LibVec3 newVelocity = MyLibrary::LibVec3(move.x, prevVelocity.y, move.z);
			own->GetRigidbody()->SetVelocity(newVelocity);

		}
		else if (Input::GetInstance().GetInputStick(false).first == 0.0f &&
			Input::GetInstance().GetInputStick(false).second == 0.0f)
		{

			m_run = false;

			if (cOne)
			{
				own->ChangeStateAnim(CsvLoad::GetInstance().GetAnimData("Player", "ShieldIdle"), true, cWalkAnimSpeed, false);
				own->NotInitAnim(false);

				cOne = false;
			}

			//�ړ����x��0�ɂ���
			MyLibrary::LibVec3 prevVelocity = own->GetRigidbody()->GetVelocity();
			MyLibrary::LibVec3 newVelocity = MyLibrary::LibVec3(0.0f, prevVelocity.y, 0.0f);
			own->GetRigidbody()->SetVelocity(newVelocity);
		}
	}
	//�x�����������ꍇ�����I��Idle��Ԃɂ���
	else
	{
		//���O�A�j���[�V������������߂�
		own->FrameEndStateAnim(CsvLoad::GetInstance().GetAnimData(own->GetCharacterName(), "ShieldTransition"), own->GetShieldFrame(), cFrame);
		own->SetGuard(cFrame);

		ChangeState(StateKind::Idle);
		return;
	}

}

std::string PlayerStateGuard::GetGuardAnim(eDir dir)
{
	//�^�[�Q�b�g���Ă��ꍇ
	if (m_targetState)
	{
		//�����ĂȂ��Ƃ�
		if (!m_run)
		{
			if (dir == eDir::Forward)
			{
				return std::string("ShieldWalk");
				//�t�Đ����邩�ǂ���
				m_revese = false;
			}
			if (dir == eDir::Right || dir == eDir::BackRight || dir == eDir::ForwardRight)
			{
				return std::string("ShieldSideWalk");
				//�t�Đ����邩�ǂ���
				m_revese = true;
			}
			if (dir == eDir::Left || dir == eDir::BackLeft || dir == eDir::ForwardLeft)
			{
				return std::string("ShieldSideWalk");
				//�t�Đ����邩�ǂ���
				m_revese = false;
			}
			else if (dir == eDir::Back)
			{
				return std::string("ShieldWalk");
				//�t�Đ����邩�ǂ���
				m_revese = true;
			}
		}
		//�����Ă�Ƃ�
		else
		{
			return std::string("ShieldRun");
		}
		
	}
	//�^�[�Q�b�g���Ă��Ȃ��ꍇ
	else
	{
		//�����Ă�ꍇ
		if (m_run)
		{
			return std::string("ShieldRun");
		}
		//�����ĂȂ��ꍇ
		else
		{
			return std::string("ShieldWalk");
		}
	}
}
