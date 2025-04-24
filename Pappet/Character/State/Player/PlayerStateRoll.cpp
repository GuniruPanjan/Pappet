#include "PlayerStateRoll.h"
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

	constexpr float cAvoidanceSpeed = 0.8f;

	//����ł̈ړ�����
	constexpr float cAvoidanceMove1 = 4.0f;
	constexpr float cAvoidanceMove2 = 1.0f;
	constexpr float cAvoidanceMove3 = 0.2f;
	//����̕������������
	bool cOneAvoidance = false;
	//����̕�������x���߂�
	bool cOneDirection = false;
}

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="chara"></param>
PlayerStateRoll::PlayerStateRoll(std::shared_ptr<CharacterBase> chara) :
	StateBase(chara),
	m_avoidanceMove(0.0f),
	m_rollMove(),
	m_rollDirection(VGet(0.0f,0.0f,0.0f))
{
	m_equipmentShield = chara->GetShield();

	//���݂̃X�e�[�g�������Ԃɂ���
	m_nowState = StateKind::Roll;
	chara->ChangeStateAnim(CsvLoad::GetInstance().GetAnimData(chara->GetCharacterName(), "Roll"), true, cAvoidanceSpeed);
	chara->NotInitAnim(true);

	//����������
	cOneAvoidance = false;
	cOneDirection = false;
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
PlayerStateRoll::~PlayerStateRoll()
{
}

/// <summary>
/// ������
/// </summary>
/// <param name="md"></param>
void PlayerStateRoll::Init(int md)
{
	m_stageCol = md;
}

/// <summary>
/// �X�V
/// </summary>
void PlayerStateRoll::Update()
{
	//�����傪�v���C���[���ǂ������`�F�b�N����
	if (!CheckState()) return;

	//�����Ă���L�����N�^�[�x�[�X�N���X���v���C���[�N���X�ɃL���X�g����
	auto own = std::dynamic_pointer_cast<Player>(m_pChara.lock());

	//�A�j���[�V�������I��������
	if (own->GetEndAnim())
	{
		cOneAvoidance = false;

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
			if (Input::GetInstance().GetInputStick(false).first == 0.0f &&
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

			//�_�b�V���{�^���������ꂽ��State���_�b�V���ɂ���
			if (Input::GetInstance().IsPushed("Input_Dash"))
			{
				ChangeState(StateKind::Dash);
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

	//�^�[�Q�b�g��Ԃ��Ɖ\
	if (m_targetState && own->GetFrame() <= 3.0f)
	{
		//���X�e�B�b�N�������Ă�����
		if (Input::GetInstance().GetInputStick(false).first != 0.0f ||
			Input::GetInstance().GetInputStick(false).second != 0.0f)
		{
			//���X�e�B�b�N���͂��󂯂����
			if (!cOneAvoidance)
			{
				//�R���g���[���[�̍��X�e�B�b�N�̓��͂��擾
				auto input = Input::GetInstance().GetInputStick(false);
				m_leftX = input.first;
				m_leftZ = input.second;

				cOneAvoidance = true;
			}

			//�ړ����������肷��
			auto moveDir = VGet(-m_leftX, 0.0f, m_leftZ);
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
		}
	}



	//�t���[���ňړ����ăt���[���ŉ������
	if (own->GetFrame() >= 0.0f && own->GetFrame() <= 20.0f)
	{
		m_avoidanceMove = cAvoidanceMove1;
		own->SetAvoidance(true);
	}
	else if (own->GetFrame() >= 20.0f && own->GetFrame() <= 30.0f)
	{
		m_avoidanceMove = cAvoidanceMove2;
		own->SetAvoidance(false);

	}
	else
	{
		m_avoidanceMove = cAvoidanceMove3;
		own->SetAvoidance(false);
	}

	//��񂾂�����������
	if (!cOneDirection)
	{
		m_rollDirection = VGet(sinf(own->GetAngle()), 0.0f, cosf(own->GetAngle()));

		cOneDirection = true;
	}

	m_rollMove = VScale(m_rollDirection, m_avoidanceMove);

	//�ړ����x�����肷��
	MyLibrary::LibVec3 prevVelocity = own->GetRigidbody()->GetVelocity();
	MyLibrary::LibVec3 newVelocity = MyLibrary::LibVec3(-m_rollMove.x, prevVelocity.y, -m_rollMove.z);
	own->GetRigidbody()->SetVelocity(newVelocity);
}
