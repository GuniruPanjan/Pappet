#include "PlayerStateWalk.h"
#include "Input/Input.h"
#include "Character/Player.h"

#include "External/CsvLoad.h"


namespace
{
	/*�A�i���O�X�e�B�b�N�ɂ��ړ��֘A*/
	constexpr float cAnalogRangeMin = 0.0f;		//�A�i���O�X�e�B�b�N�̓��͔���ŏ��͈�
	constexpr float cAnalogRangeMax = 0.8f;		//�A�i���O�X�e�B�b�N�̓��͔���ő�͈�
	constexpr float cAnalogInputMax = 1000.0f;	//�A�i���O�X�e�B�b�N������͂����x�N�g���̍ő�

	constexpr float cWalkAnimSpeed = 0.5f;

	//�����ɂ��������鑬�x
	constexpr float cWalkSpeed = 1.5f;

	//�C���v�b�g��xy���i�[����
	float cInputX = 0.0f;
	float cInputY = 0.0f;
}

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="chara"></param>
PlayerStateWalk::PlayerStateWalk(std::shared_ptr<CharacterBase> chara) :
	StateBase(chara),
	m_dir(),
	m_walkCount(0),
	m_noInputFrame(0),
	m_input(0)
{
	//���݂̃X�e�[�g�������Ԃɂ���
	m_nowState = StateKind::Walk;

	//���̃V�[���ɑJ�ڂ����u�Ԃ̍��X�e�B�b�N�̓��͊p�x���擾���Ă���
	auto input = Input::GetInstance().GetInputStick(false);
	m_dir = GetDirection(input.first, -input.second);

	m_equipmentState = chara->GetEquipment();
	m_equipmentShield = chara->GetShield();

	auto animName = GetWalkAnim(m_dir);
	chara->ChangeStateAnim(CsvLoad::GetInstance().GetAnimData(chara->GetCharacterName(), animName), false);
	chara->NotInitAnim(false);
	//���x�����߂�
	chara->SetSpeed(cWalkSpeed);
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
PlayerStateWalk::~PlayerStateWalk()
{
}

/// <summary>
/// ������
/// </summary>
/// <param name="md"></param>
void PlayerStateWalk::Init(int md)
{
	m_stageCol = md;
}

/// <summary>
/// �X�V
/// </summary>
void PlayerStateWalk::Update()
{
	//�����傪�v���C���[���ǂ����`�F�b�N����
	if (!CheckState()) return;

	//�_�E���L���X�g
	auto own = std::dynamic_pointer_cast<Player>(m_pChara.lock());

	//�x�������X�^�~�i�؂ꂾ�����ꍇ�͓����Ȃ�����
	if (!own->GetRest() && !own->GetStaminaBreak())
	{
		//���X�e�B�b�N�����͂���Ă��Ȃ�������State��Idle�ɂ���
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

		//���j���[���J����Ă��Ȃ��ăW�����v�{�^����������Ă�����State��Jump�ɂ���
		if (Input::GetInstance().IsTriggered("Input_Jump") && !own->GetMenu() && own->GetAction())
		{
			ChangeState(StateKind::Jump);
			return;
		}

		//���j���[���J����Ă��Ȃ��čU���{�^����������Ă�����State���U���ɂ���
		if (Input::GetInstance().IsTriggered("Input_Attack") && !own->GetMenu())
		{
			ChangeState(StateKind::Attack);
			return;
		}

		//���U���{�^����������Ă�����State�����U���ɂ���
		if (Input::GetInstance().GetIsPushedTriggerButton(true) || Input::GetInstance().GetIsPushedTriggerButton(true))
		{
			//���j���[���J����Ă��Ȃ�������
			if (!own->GetMenu())
			{
				ChangeState(StateKind::StrongAttack);
				return;
			}

		}


		if (m_input < 30)
		{
			//���j���[���J����Ă��Ȃ��ĉ���{�^���������ꂽ��State������ɂ���
			if (Input::GetInstance().IsReleased("Input_Roll") && !own->GetMenu() && own->GetAction())
			{
				ChangeState(StateKind::Roll);
				return;
			}
		}

		//���j���[���J����Ă��Ȃ��ă_�b�V���{�^���������ꂽ��State���_�b�V���ɂ���
		if (Input::GetInstance().IsPushed("Input_Dash") && !own->GetMenu() && own->GetAction())
		{
			m_input++;
			if (m_input >= 30)
			{
				ChangeState(StateKind::Dash);
				return;
			}
		}
		else
		{
			//����������
			m_input = 0;
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
	//�x�����������ꍇ�����I��Idle��Ԃɂ���
	else if(own->GetRest())
	{
		ChangeState(StateKind::Idle);
		return;
	}
	//�X�^�~�i�؂ꂾ�����ꍇ
	else if (own->GetStaminaBreak())
	{
		//���X�e�B�b�N�����͂���Ă��Ȃ�������State��Idle�ɂ���
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
	}

	

	//���X�e�B�b�N�����͂���Ă�����
	if (m_noInputFrame == 0)
	{
		//�R���g���[���[�̍��X�e�B�b�N�̓��͂��擾
		auto input = Input::GetInstance().GetInputStick(false);
		auto dirLog = m_dir;
		m_dir = GetDirection(input.first, input.second);

		//���O�̓��͕����ƈقȂ�Ƃ�
		if (dirLog != m_dir)
		{
			//�A�j���[�V������ύX����
			auto animName = GetWalkAnim(m_dir);
			own->ChangeStateAnim(CsvLoad::GetInstance().GetAnimData("Player", animName), false, cWalkAnimSpeed, m_revese);
			own->NotInitAnim(false);
		}

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

		//�^�[�Q�b�g���Ă��Ȃ���
		if (!m_targetState)
		{
			//�L�����N�^�[�̃A���O�������߂�
			own->SetModelAngle(atan2f(-moveVec.z, moveVec.x) - DX_PI_F / 2);
		}

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

/// <summary>
/// ���͕�����������A�j���[�V�������擾����
/// </summary>
/// <param name="dir"></param>
/// <returns></returns>
std::string PlayerStateWalk::GetWalkAnim(eDir dir)
{
	//�^�[�Q�b�g��Ԃ���
	if (m_targetState)
	{
		//���������Ă��Ȃ���
		if (!m_equipmentState)
		{
			if (dir == eDir::Forward)
			{
				//�t�Đ����邩�ǂ���
				m_revese = false;
				return std::string("Walk");
			}
			else if (dir == eDir::Right || dir == eDir::ForwardRight || dir == eDir::BackRight)
			{
				//�t�Đ����邩�ǂ���
				m_revese = false;
				return std::string("RightWalk");
			}
			else if (dir == eDir::Left || dir == eDir::ForwardLeft || dir == eDir::BackLeft)
			{
				//�t�Đ����邩�ǂ���
				m_revese = false;
				return std::string("LeftWalk");
			}
			else if (dir == eDir::Back)
			{
				//�t�Đ����邩�ǂ���
				m_revese = true;
				return std::string("Walk");
			}
		}
		//���������Ă��鎞
		else
		{
			if (dir == eDir::Forward)
			{
				//�t�Đ����邩�ǂ���
				m_revese = false;
				return std::string("ShieldWalk");
			}
			else if (dir == eDir::Right || dir == eDir::ForwardRight || dir == eDir::BackRight)
			{
				//�t�Đ����邩�ǂ���
				m_revese = false;
				return std::string("WeaponSideWalk");
			}
			else if (dir == eDir::Left || dir == eDir::ForwardLeft || dir == eDir::BackLeft)
			{
				//�t�Đ����邩�ǂ���
				m_revese = true;
				return std::string("WeaponSideWalk");
			}
			else if (dir == eDir::Back)
			{
				//�t�Đ����邩�ǂ���
				m_revese = true;
				return std::string("ShieldWalk");
			}
		}
		
	}
	//�^�[�Q�b�g���Ă��Ȃ����
	else
	{
		if (!m_equipmentState)
		{
			return std::string("Walk");
			//�t�Đ����邩�ǂ���
			m_revese = false;
		}
		else
		{
			return std::string("ShieldWalk");
			//�t�Đ����邩�ǂ���
			m_revese = false;
		}
	}
	

}
