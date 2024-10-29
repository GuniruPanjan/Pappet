#include "Player.h"
#include "Manager/EffectManager.h"
#include "Manager/HandleManager.h"
#include "External/CsvLoad.h"

namespace
{
	//���f���T�C�Y
	constexpr float cModelSizeScale = 0.4f;

	//�A�j���[�V�����̐؂�ւ��ɂ�����t���[����
	constexpr float cAnimChangeFrame = 5.0f;
	constexpr float cAnimChangeSpeed = 1.0f / cAnimChangeFrame;

	//�A�j���[�V�����u�����h���̍ő�
	constexpr float cAnimBlendRateMax = 1.0f;

	//�_�b�V�����m�F���邽�߂̕ϐ�
	int cAbutton = 0;

	//�|�W�V�����ݒ�
	VECTOR m_pos;

	//�V���O���g��
	auto& handle = HandleManager::GetInstance();
	auto& effect = EffectManager::GetInstance();
}

Player::Player() :
	CharacterBase(Collidable::Priority::High, ObjectTag::Player),
	m_xpad(),
	m_cameraAngle(0.0f),
	m_isDead(false),
	m_dashMove(false)
{
	//�J�v�Z���^
	auto collider = Collidable::AddCollider(MyLibrary::CollidableData::Kind::Capsule, false);
	auto capsuleCol = dynamic_cast<MyLibrary::CollidableDataCapsule*>(collider.get());
	capsuleCol->m_len = 50.0f;
	capsuleCol->m_radius = 12.0f;
	capsuleCol->m_vec = MyLibrary::LibVec3(0.0f, capsuleCol->m_vec.y + 2.0f, 0.0f);

	//���x���̏�����
	m_levelStatus.sl_hp = 1;
	m_levelStatus.sl_stamina = 1;
	m_levelStatus.sl_muscle = 1;
	m_levelStatus.sl_skill = 1;
	
	//�G�t�F�N�g�ǂݍ���
	effect.EffectLoad("Rest", "Data/Effect/Benediction.efkefc", 210, 10.0f);
	effect.EffectLoad("Heal", "Data/Effect/AnotherEffect/Sylph13.efkefc", 160, 20.0f);
	effect.EffectLoad("Imapct", "Data/Effect/HitEffect.efkefc", 30, 7.0f);

	//���f���ǂݍ���
	m_modelHandle = handle.GetModelHandle("Data/Player/PlayerModel.mv1");

	//���f���̃T�C�Y�ݒ�
	MV1SetScale(m_modelHandle, VGet(cModelSizeScale, cModelSizeScale, cModelSizeScale));

	//Csv�ɂ��f�[�^�̓ǂݍ���
	CsvLoad::GetInstance().AnimDataLoad("Player", m_animIdx);
	CsvLoad::GetInstance().StatusLoad(m_status, "Player");
}

Player::~Player()
{
	//���������
	MV1DeleteModel(m_modelHandle);
	//�������폜
	handle.Clear();
}

/// <summary>
/// ����������
/// </summary>
/// <param name="physics">�����N���X�̃|�C���^</param>
void Player::Init(std::shared_ptr<MyLibrary::Physics> physics)
{
	m_pos = VGet(0.0f, 0.0f, 0.0f);

	m_pPhysics = physics;

	//�����蔻��̒ǉ�
	Collidable::Init(m_pPhysics);

	//�v���C���[�̏����ʒu�ݒ�
	rigidbody.Init(false);
	rigidbody.SetPos(MyLibrary::LibVec3(485.0f, 0.0f, -800.0f));
	rigidbody.SetNextPos(rigidbody.GetPos());
	rigidbody.SetVec(MyLibrary::LibVec3(0.0f, 40.0f, 0.0f));
	m_collisionPos = rigidbody.GetPos();
	SetModelPos();
	MV1SetPosition(m_modelHandle, m_modelPos.ConversionToVECTOR());

	//�ҋ@�A�j���[�V�����ݒ�
	m_nowAnimNo = MV1AttachAnim(m_modelHandle, m_animIdx["Idle"]);
	m_nowAnimIdx = m_animIdx["Idle"];
}

void Player::Finalize()
{
	Collidable::Finalize(m_pPhysics);
}

void Player::Update()
{
	//�p�b�h���͎擾
	GetJoypadXInputState(DX_INPUT_KEY_PAD1, &m_xpad);

	//�A�j���[�V�����̍X�V
	m_isAnimationFinish = UpdateAnim(m_nowAnimNo);

	//�A�j���[�V�����̐؂�ւ�
	if (m_prevAnimNo != -1)
	{
		//�t���[���ŃA�j���[�V������؂�ւ���
		m_animBlendRate += cAnimChangeSpeed;
		if (m_animBlendRate >= cAnimBlendRateMax)
		{
			m_animBlendRate = cAnimBlendRateMax;
		}

		//�A�j���[�V�����̃u�����h����ݒ肷��
		MV1SetAttachAnimBlendRate(m_modelHandle, m_prevAnimNo, cAnimBlendRateMax - m_animBlendRate);
		MV1SetAttachAnimBlendRate(m_modelHandle, m_nowAnimNo, m_animBlendRate);
	}

	//���񂾎��̃A�j���[�V����
	if (m_status.s_hp <= 0)
	{
		if (!m_isDead)
		{
			m_isDead = true;

			m_nowAnimIdx = m_animIdx["Death"];
			ChangeAnim(m_nowAnimIdx);

			Finalize();
		}
	}

	//���W���擾����
	m_collisionPos = rigidbody.GetPos();

	//�A�i���O�X�e�B�b�N���g���Ĉړ�
	int analogX = 0;
	int analogY = 0;

	float SetAngleX = 0.0f;
	float SetAngleY = 0.0f;

	if (!m_isDead)
	{
		GetJoypadAnalogInput(&analogX, &analogY, DX_INPUT_PAD1);
	}

	//�ړ�������ݒ肷��
	auto moveVec = VGet(static_cast<float>(-analogX), 0.0f, static_cast<float>(analogY));    //�x�N�g���̒���

	//�x�N�g���̒������擾����
	float len = VSize(moveVec);
	//�x�N�g���̒�����0.0�`1.0�̊����ɕϊ�����
	float rate = len / 1000.0f;
	//�A�i���O�X�e�B�b�N�����Ȕ͈͂����O����
	rate = (rate - 0.1f) / (0.8f - 0.1f);
	rate = min(rate, 1.0f);
	rate = max(rate, 0.0f);

	//���x������ł���̂ňړ��x�N�g���ɔ��f����
	moveVec = VNorm(moveVec);
	float speed = m_status.s_speed * rate;

	moveVec = VScale(moveVec, speed);

	//camera�̊p�x����
	//�R���g���[���[�ɂ��ړ����������肷��
	MATRIX mtx = MGetRotY(m_cameraAngle + DX_PI_F);
	moveVec = VTransform(moveVec, mtx);

	//���C�u�����̃x�N�^�[�ɕϊ�����
	m_moveVec = MyLibrary::LibVec3(static_cast<float>(moveVec.x), static_cast<float>(moveVec.y), static_cast<float>(moveVec.z));

	//�ړ���������v���C���[�ւ̌������������肷��
	//�ړ����Ă��Ȃ��ꍇ(�[���x�N�g��)�̏ꍇ�͕ύX���Ȃ�
	if (VSquareSize(moveVec) > 0.0f)
	{
		m_angle = atan2f(-moveVec.z, moveVec.x) - DX_PI_F / 2;

		//�v���C���[����������
		m_moveflag = true;
	}
	//�v���C���[�������ĂȂ�������
	else if (VSquareSize(moveVec) <= 0.0f)
	{
		m_moveflag = false;
	}

	MyLibrary::LibVec3 prevVelocity = rigidbody.GetVelocity();
	MyLibrary::LibVec3 newVelocity = MyLibrary::LibVec3(m_moveVec.x, prevVelocity.y, m_moveVec.z);
	rigidbody.SetVelocity(newVelocity);

	//�v���C���[�������Ă��鎞����
	if (!m_isDead)
	{
		m_modelPos = m_modelPos + m_moveVec;

		Action();

		NotWeaponAnimation();
		AllAnimation();
	}
}

/// <summary>
/// �v���C���[�̃A�N�V��������
/// </summary>
void Player::Action()
{
	//A�{�^���������ꂽ��_�b�V�������
	if (m_xpad.Buttons[12] == 1)
	{
		if (cAbutton > 50)
		{
			//�_�b�V��
			m_dashMove = true;

			m_status.s_speed = 3.0f;
		}

		if (cAbutton < 51)
		{
			cAbutton++;
		}
	}
	else
	{
		m_dashMove = false;

		m_status.s_speed = 2.0f;

		cAbutton = 0;
	}
}

/// <summary>
/// ����������ĂȂ��Ƃ��̃A�j���[�V����
/// </summary>
void Player::NotWeaponAnimation()
{
	//�U�����������ĂȂ���
	if (!m_hit)
	{
		//����
		if (m_dashMove)
		{
			m_nowAnimIdx = m_animIdx["Run"];
			ChangeAnim(m_nowAnimIdx);
		}
		//����
		else if (m_moveflag)
		{
			m_nowAnimIdx = m_animIdx["Walk"];
			ChangeAnim(m_nowAnimIdx);
		}
	}
}

/// <summary>
/// �S�Ă̏�Ԃ̎��ɍs���A�j���[�V�����֐�
/// </summary>
void Player::AllAnimation()
{
	//�v���C���[�������Ă���Ƃ�����
	if (!m_isDead)
	{
		//�U��������������
		if (m_hit)
		{

		}
		//�U�����������ĂȂ��Ƃ�
		else if (!m_hit)
		{
			//�����ĂȂ���
			if (!m_moveflag)
			{
				m_nowAnimIdx = m_animIdx["Idle"];
				ChangeAnim(m_nowAnimIdx);
			}
		}
	}
}

/// <summary>
/// �`�揈��
/// </summary>
void Player::Draw()
{
	rigidbody.SetPos(rigidbody.GetNextPos());
	m_collisionPos = rigidbody.GetPos();
	SetModelPos();
	MV1SetPosition(m_modelHandle, VSub(m_modelPos.ConversionToVECTOR(), VGet(0.0f, 12.0f, 0.0f)));

#if false
	DrawFormatString(0, 100, 0xffffff, "posx : %f", m_modelPos.x);
	DrawFormatString(0, 200, 0xffffff, "posy : %f", m_modelPos.y);
	DrawFormatString(0, 300, 0xffffff, "posz : %f", m_modelPos.z);
	DrawFormatString(0, 400, 0xffffff, "m_nowAnim : %d", m_nowAnimIdx);
	DrawFormatString(0, 500, 0xffffff, "m_nowSpeed : %f", m_animSpeed);
	DrawFormatString(0, 600, 0xffffff, "moveVecx : %f", m_moveVec.x);
	DrawFormatString(0, 700, 0xffffff, "moveVecy : %f", m_moveVec.y);
	DrawFormatString(0, 800, 0xffffff, "moveVecz : %f", m_moveVec.z);

#endif

	//���f���̉�]�n
	MV1SetRotationXYZ(m_modelHandle, VGet(0.0f, m_angle, 0.0f));

	//�`��
	MV1DrawModel(m_modelHandle);
}

void Player::End()
{
	//���������
	MV1DeleteModel(m_modelHandle);
	//�������폜
	handle.Clear();
}

void Player::OnCollideEnter(const std::shared_ptr<Collidable>& collidable)
{
}

void Player::OnTriggerEnter(const std::shared_ptr<Collidable>& collidable)
{
}

void Player::SetModelPos()
{
	m_modelPos = m_collisionPos;
	//m_collisionPos.y += 50.0f * cModelSizeScale;
	//m_modelPos.y -= 90.0f * cModelSizeScale;
}
