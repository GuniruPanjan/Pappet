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
	//�����ɂ��������鑬�x
	constexpr float cWalkSpeed = 2.0f;
	//�_�b�V���ɂ��������鑬�x
	constexpr float cDashSpeed = 4.0f;
	//�{�^����������Ă��邩�̊m�F�p�ϐ�
	int cAbutton = 0;
	int cRbutton = 0;
	bool cRstickButton = false;
	//���G�͈�
	constexpr float cSearchRadius = 200.0f;
	//����ł̈ړ�����
	float cAvoidanceMove = 0.0f;
	//�U���ł̒ǉ��U������
	int cAddAttackTime = 0;
	//���̍U���͈�
	constexpr float cFistAttackRadius = 18.0f;
	//���݂̃A�^�b�N�̃i���o�[������
	int cNowAttackNumber = 0;
	//�U���̏I������
	int cIsEndAttack = 0;

	//�V���O���g��
	auto& handle = HandleManager::GetInstance();
	auto& effect = EffectManager::GetInstance();
}

Player::Player() :
	CharacterBase(Collidable::Priority::Low, ObjectTag::Player),
	m_xpad(),
	m_attackNumber(0),
	m_menuOpen(false),
	m_lockonTarget(false),
	m_moveAnimFrameIndex(0),
	m_moveAnimFrameRight(0),
	m_moveAnimShieldFrameIndex(0),
	m_moveAnimShieldFrameHandIndex(0),
	m_cameraAngle(0.0f),
	m_avoidanceNow(false),
	m_moveWeaponFrameMatrix(),
	m_moveShieldFrameMatrix(),
	m_rollMove(VGet(0.0f,0.0f,0.0f))
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

	//�A�j���[�V�����̔��菉����
	m_anim.s_attack = false;
	m_anim.s_hit = false;
	m_anim.s_isDead = false;
	m_anim.s_moveflag = false;
	m_animChange.sa_avoidance = false;
	m_animChange.sa_dashMove = false;
	m_animChange.sa_recovery = false;
	m_animChange.sa_taking = false;
	m_animChange.sa_touch = false;
	
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
	m_pPhysics = physics;

	//�����蔻��̒ǉ�
	Collidable::Init(m_pPhysics);

	//�v���C���[�̏����ʒu�ݒ�
	rigidbody.Init(false);
	rigidbody.SetPos(MyLibrary::LibVec3(485.0f, 12.0f, -800.0f));
	rigidbody.SetNextPos(rigidbody.GetPos());
	rigidbody.SetVec(MyLibrary::LibVec3(0.0f, 40.0f, 0.0f));
	m_collisionPos = rigidbody.GetPos();
	SetModelPos();
	MV1SetPosition(m_modelHandle, m_modelPos.ConversionToVECTOR());

	//���j���[�����
	m_menuOpen = false;

	//���������ꍇ
	m_attackRadius = cFistAttackRadius;

	m_pAttack = std::make_shared<AttackObject>(m_attackRadius);

	m_pSearch = std::make_shared<SearchObject>(cSearchRadius);
	m_pSearch->Init(m_pPhysics, rigidbody.GetPos(), false, false, false, true);

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
	//�A�j���[�V�����ňړ����Ă���t���[���̔ԍ�����������
	m_moveAnimFrameIndex = MV1SearchFrame(m_modelHandle, "mixamorig:Hips");
	m_moveAnimFrameRight = MV1SearchFrame(m_modelHandle, "mixamorig:RightHandThumb2");

	//�����\���鎞�̃A�j���[�V�����t���[���擾
	m_moveAnimShieldFrameIndex = MV1SearchFrame(m_modelHandle, "mixamorig:LeftShoulder");
	m_moveAnimShieldFrameHandIndex = MV1SearchFrame(m_modelHandle, "mixamorig:LeftHand");

	//����⏂���A�^�b�`����t���[���̃��[�J�������[���h�ϊ��s����擾����
	m_moveWeaponFrameMatrix = MV1GetFrameLocalWorldMatrix(m_modelHandle, m_moveAnimFrameRight);
	m_moveShieldFrameMatrix = MV1GetFrameLocalWorldMatrix(m_modelHandle, m_moveAnimShieldFrameHandIndex);


	//�p�b�h���͎擾
	GetJoypadXInputState(DX_INPUT_KEY_PAD1, &m_xpad);

	//�A�j���[�V�����̍X�V
	m_isAnimationFinish = UpdateAnim(m_nowAnimNo, ANIMATION_MAX);

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
		if (!m_anim.s_isDead)
		{
			m_anim.s_isDead = true;

			m_nowAnimIdx = m_animIdx["Death"];

			ChangeAnim(m_nowAnimIdx, m_animOne[0], m_animOne);

			Finalize();
		}
	}

	//�A�i���O�X�e�B�b�N���g���Ĉړ�
	int analogX = 0;
	int analogY = 0;

	float SetAngleX = 0.0f;
	float SetAngleY = 0.0f;

	if (!m_anim.s_isDead && !m_animChange.sa_avoidance && !m_anim.s_attack && !m_animChange.sa_recovery)
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
		m_anim.s_moveflag = true;
	}
	//�v���C���[�������ĂȂ�������
	else if (VSquareSize(moveVec) <= 0.0f)
	{
		m_anim.s_moveflag = false;
	}

	//������ĂȂ��Ƃ�
	if (!m_animChange.sa_avoidance)
	{
		MyLibrary::LibVec3 prevVelocity = rigidbody.GetVelocity();
		MyLibrary::LibVec3 newVelocity = MyLibrary::LibVec3(m_moveVec.x, prevVelocity.y, m_moveVec.z);
		rigidbody.SetVelocity(newVelocity);
	}
	//������Ă�Ƃ�
	else if(m_animChange.sa_avoidance)
	{
		//����ňړ����鋗��
		m_rollMove = VScale(VGet(sinf(m_angle), 0.0f, cosf(m_angle)), cAvoidanceMove);

		//�A���O���̕����Ɉ�苗���ړ���������
		MyLibrary::LibVec3 prevVelocity = rigidbody.GetVelocity();
		MyLibrary::LibVec3 newVelocity = MyLibrary::LibVec3(-m_rollMove.x, prevVelocity.y, -m_rollMove.z);
		rigidbody.SetVelocity(newVelocity);
	}

	//�v���C���[�������Ă��鎞����
	if (!m_anim.s_isDead)
	{
		//���j���[���J���Ă���Ԃ̓A�N�V�����ł��Ȃ�
		if (!m_menuOpen)
		{
			//�A�N�V�������ł��Ȃ�����
			if (!m_animChange.sa_avoidance || !m_anim.s_hit || !m_animChange.sa_recovery)
			{
				Action();
			}
		}

		NotWeaponAnimation();
		AllAnimation();
	}

	//�v���C���[�̃|�W�V����������
	SetModelPos();

	//����̍X�V
	MyLibrary::LibVec3 centerPos = rigidbody.GetPos();
	MyLibrary::LibVec3 attackPos = MyLibrary::LibVec3(rigidbody.GetPos().x + sinf(m_angle) * -25.0f, rigidbody.GetPos().y + 15.0f, rigidbody.GetPos().z - cosf(m_angle) * 25.0f);
	m_pSearch->Update(centerPos);
	m_pAttack->Update(attackPos);

	//����s����
	if (!m_isAnimationFinish && m_animChange.sa_avoidance)
	{
		//�t���[�����
		if (m_nowFrame >= 0.0f && m_nowFrame <= 20.0f)
		{
			m_avoidanceNow = true;

			cAvoidanceMove = 4.0f;
		}
		else if (m_nowFrame >= 20.0f && m_nowFrame <= 30.0f)
		{
			cAvoidanceMove = 1.0f;
		}
		else
		{
			m_avoidanceNow = false;

			cAvoidanceMove = 0.2f;
		}
	}
	//����I��
	else if (m_isAnimationFinish && m_animChange.sa_avoidance)
	{

		m_animChange.sa_avoidance = false;
	}

	//�U����
	if (!m_isAnimationFinish && m_anim.s_attack)
	{
		//��i�K�ڂ̍U��
		if (m_nowFrame <= 40.0f)
		{
			//���݂̃A�^�b�N�i���o�[
			cNowAttackNumber = 1;

			//�U�����蔭���t���[��
			if (m_nowFrame == 25.0f)
			{
				m_pAttack->Init(m_pPhysics);
			}
			else if (m_nowFrame >= 35.0f && m_nowFrame < 40.0f)
			{
				//��������Z�b�g
				m_pAttack->CollisionEnd();
			}
			//�U���I��
			else if (m_nowFrame >= 40.0f && m_attackNumber == 1)
			{
				cIsEndAttack = 1;
			}

		}
		//��i�K�ڂ̍U��
		else if (m_nowFrame <= 70.0f && cIsEndAttack == 1)
		{
			//���݂̃A�^�b�N�i���o�[
			cNowAttackNumber = 2;

			//�U�����蔭���t���[��
			if (m_nowFrame == 55.0f)
			{
				m_pAttack->Init(m_pPhysics);
			}
			else if (m_nowFrame >= 65.0f && m_nowFrame < 70.0f)
			{
				//�U�����胊�Z�b�g
				m_pAttack->CollisionEnd();
			}
			//�U���I��
			else if (m_nowFrame >= 70.0f && m_attackNumber == 2)
			{
				cIsEndAttack = 2;
			}
		}
		//�O�i�K�ڂ̍U��
		else if (m_nowFrame <= 110.0f && cIsEndAttack == 2)
		{
			//���݂̃A�^�b�N�i���o�[
			cNowAttackNumber = 3;

			//�U�����蔭���t���[��
			if (m_nowFrame == 85.0f)
			{
				m_pAttack->Init(m_pPhysics);
			}
			else if (m_nowFrame >= 95.0f && m_nowFrame < 110.0f)
			{
				//�U�����胊�Z�b�g
				m_pAttack->CollisionEnd();
			}
			//�U���I��
			else if (m_nowFrame >= 110.0f)
			{
				cIsEndAttack = 0;
			}
		}
		else
		{
			m_anim.s_attack = false;

			//�U���i�K������������
			m_attackNumber = 0;
			//�U���I��
			cIsEndAttack = 0;
			//�U�����胊�Z�b�g
			m_pAttack->CollisionEnd();
		}
	}
	//�U���I��
	else if (m_isAnimationFinish && m_anim.s_attack)
	{
		m_anim.s_attack = false;

		//�U���i�K������������
		m_attackNumber = 0;
		//�U���I��
		cIsEndAttack = 0;
	}

	//�񕜂���
	if (!m_isAnimationFinish && m_animChange.sa_recovery)
	{

	}
	//�񕜏I��
	else if (m_isAnimationFinish && m_animChange.sa_recovery)
	{
		m_animChange.sa_recovery = false;
	}
}

/// <summary>
/// �v���C���[�̃A�N�V��������
/// </summary>
void Player::Action()
{
	//�^�[�Q�b�g�ł��鎞
	if (!m_lockonTarget && m_pSearch->GetIsStay())
	{
		//��񂾂�����
		if (m_xpad.Buttons[7] == 1 && !cRstickButton)
		{
			m_lockonTarget = true;
			cRstickButton = true;
		}
		else if(m_xpad.Buttons[7] == 0)
		{
			cRstickButton = false;
		}
	}
	//�^�[�Q�b�g���O��
	else if (m_lockonTarget == true && m_pSearch->GetIsStay())
	{
		//��񂾂�����
		if (m_xpad.Buttons[7] == 1 && !cRstickButton)
		{
			m_lockonTarget = false;
			cRstickButton = true;
		}
		else if (m_xpad.Buttons[7] == 0)
		{
			cRstickButton = false;
		}
	}
	//�^�[�Q�b�g�𖳗����O��
	else if (!m_pSearch->GetIsStay())
	{
		m_lockonTarget = false;
		cRstickButton = false;
	}

	//A�{�^���������ꂽ��_�b�V�������
	if (m_xpad.Buttons[12] == 1 && !m_anim.s_attack)
	{
		//�_�b�V��
		if (cAbutton > 50)
		{
			m_animChange.sa_avoidance = false;

			//�_�b�V��
			m_animChange.sa_dashMove = true;

			m_status.s_speed = cDashSpeed;
		}

		if (cAbutton < 51)
		{
			cAbutton++;
		}
	}
	else
	{
		m_animChange.sa_dashMove = false;

		m_status.s_speed = cWalkSpeed;

		//���
		//�������u��
		if (cAbutton > 0 && cAbutton < 30 && m_animChange.sa_avoidance == false)
		{
			m_animChange.sa_avoidance = true;
		}

		cAbutton = 0;
	}

	//�U��
	//R�{�^�����������ƂōU��
	if (m_xpad.Buttons[9] == 1)
	{
		cRbutton++;

		//��񂾂���������悤�ɂ���
		if (cRbutton == 1)
		{
			m_anim.s_attack = true;

			//�ǉ��U����t
			if (cAddAttackTime <= 30 && cAddAttackTime > 0)
			{
				//��i�K�ڂ̍U��
				if (cNowAttackNumber == 1)
				{
					m_attackNumber = 1;
				}
				//�O�i�K�ڂ̍U��
				else if (cNowAttackNumber == 2)
				{
					m_attackNumber = 2;
				}
			}

			//�ǉ��U�����Ԃ�������
			cAddAttackTime = 40;
		}
	}
	else
	{
		cRbutton = 0;
	}
	//�ǉ��U����t���Ԃ����炷
	if (cAddAttackTime <= 40 && cAddAttackTime > 0 && --cAddAttackTime > -1);

	//��
	//X�{�^���������ꂽ��
	if (m_xpad.Buttons[14] == 1 && !m_anim.s_attack)
	{
		m_animChange.sa_recovery = true;
	}
	
	//���j���[���J��
	if (m_xpad.Buttons[4] == 1)
	{
		m_menuOpen = true;
	}
}

/// <summary>
/// ����������ĂȂ��Ƃ��̃A�j���[�V����
/// </summary>
void Player::NotWeaponAnimation()
{
	//�U�����������ĂȂ���
	if (!m_anim.s_hit)
	{
		//����
		if (m_animChange.sa_dashMove && m_anim.s_moveflag)
		{
			m_nowAnimIdx = m_animIdx["Run"];
			ChangeAnim(m_nowAnimIdx, m_animOne[1], m_animOne);
		}
		//����
		else if (m_anim.s_moveflag)
		{
			m_nowAnimIdx = m_animIdx["Walk"];
			ChangeAnim(m_nowAnimIdx, m_animOne[2], m_animOne);
		}
	}
}

/// <summary>
/// �S�Ă̏�Ԃ̎��ɍs���A�j���[�V�����֐�
/// </summary>
void Player::AllAnimation()
{
	//�v���C���[�������Ă���Ƃ�����
	if (!m_anim.s_isDead)
	{
		//�U��������������
		if (m_anim.s_hit)
		{
			m_nowAnimIdx = m_animIdx["Hit"];
			ChangeAnim(m_nowAnimIdx, m_animOne[3], m_animOne);
		}
		//�U�����������ĂȂ��Ƃ�
		else if (!m_anim.s_hit)
		{
			//�����ĂȂ���
			if (!m_anim.s_moveflag && !m_animChange.sa_avoidance && !m_anim.s_attack && !m_animChange.sa_recovery)
			{
				m_nowAnimIdx = m_animIdx["Idle"];
				ChangeAnim(m_nowAnimIdx, m_animOne[4], m_animOne);
			}
			//���
			else if (m_animChange.sa_avoidance)
			{
				m_nowAnimIdx = m_animIdx["Roll"];
				ChangeAnim(m_nowAnimIdx, m_animOne[5], m_animOne);
			}
			//�U��
			else if (m_anim.s_attack && !m_animChange.sa_avoidance && !m_animChange.sa_recovery)
			{
				m_nowAnimIdx = m_animIdx["Attack1"];
				ChangeAnim(m_nowAnimIdx, m_animOne[6], m_animOne, 1.0f);
			}
			//��
			else if (m_animChange.sa_recovery)
			{
				m_nowAnimIdx = m_animIdx["Recovery"];
				ChangeAnim(m_nowAnimIdx, m_animOne[7], m_animOne);
			}
			//�A�C�e�����擾����Ƃ�
			else if (m_animChange.sa_taking)
			{
				m_nowAnimIdx = m_animIdx["Taking"];
				ChangeAnim(m_nowAnimIdx, m_animOne[8], m_animOne);
			}
			//�M�~�b�N���쓮������Ƃ�
			else if (m_animChange.sa_touch)
			{
				m_nowAnimIdx = m_animIdx["Touch"];
				ChangeAnim(m_nowAnimIdx, m_animOne[9], m_animOne);
			}
		}
	}
}

/// <summary>
/// ����������Ă��Ԃ̎��ɍs���A�j���[�V����
/// </summary>
void Player::WeaponAnimation()
{
}

/// <summary>
/// �`�揈��
/// </summary>
void Player::Draw()
{
	rigidbody.SetPos(rigidbody.GetNextPos());
	m_collisionPos = rigidbody.GetPos();
	
	MV1SetPosition(m_modelHandle, VSub(m_modelPos.ConversionToVECTOR(), VGet(0.0f, 12.0f, 0.0f)));

#if false
	DrawFormatString(0, 100, 0xffffff, "posx : %f", m_modelPos.x);
	DrawFormatString(0, 200, 0xffffff, "posy : %f", m_modelPos.y);
	DrawFormatString(0, 300, 0xffffff, "posz : %f", m_modelPos.z);
	DrawFormatString(0, 400, 0xffffff, "m_nowAnim : %d", m_nowAnimIdx);
	DrawFormatString(0, 500, 0xffffff, "m_nowSpeed : %f", m_nowFrame);
	DrawFormatString(0, 600, 0xffffff, "colPosx : %f", m_collisionPos.x);
	DrawFormatString(0, 700, 0xffffff, "colPosy : %f", m_collisionPos.y);
	DrawFormatString(0, 800, 0xffffff, "colPosz : %f", m_collisionPos.z);
	DrawFormatString(200, 100, 0xffffff, "m_blend : %f", m_animBlendRate);
	DrawFormatString(200, 200, 0xffffff, "addattacktime : %d", cAddAttackTime);
	DrawFormatString(200, 300, 0xffffff, "avoidancemove : %f", cAvoidanceMove);
	DrawFormatString(200, 400, 0xffffff, "attackNumber : %d", m_attackNumber);
	DrawFormatString(200, 500, 0xffffff, "nowAttackNumber : %d", cNowAttackNumber);

#endif

	DrawFormatString(200, 100, 0xffffff, "m_lockOnTarget : %d", m_lockonTarget);
	DrawFormatString(200, 150, 0xffffff, "m_attack : %d", m_pAttack->GetIsTrigger());

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
#if _DEBUG
	std::string message = "�v���C���[��";
#endif
	auto tag = collidable->GetTag();
	switch (tag)
	{
	case ObjectTag::Enemy:
#if _DEBUG
		message += "�G";
#endif
		break;
	}
#if _DEBUG
	message += "�Ɠ�������\n";
	printfDx(message.c_str());
#endif
}

void Player::OnCollideStay(const std::shared_ptr<Collidable>& collidable)
{
#if _DEBUG
	std::string message = "�v���C���[��";
#endif
	auto tag = collidable->GetTag();
	switch (tag)
	{
	case ObjectTag::Enemy:
#if _DEBUG
		message += "�G";
#endif
		break;
	}
#if _DEBUG
	message += "�Ɠ�������\n";
	printfDx(message.c_str());
#endif
}

void Player::OnTriggerEnter(const std::shared_ptr<Collidable>& collidable)
{
#if _DEBUG
	std::string message = "�v���C���[��";
#endif
	auto tag = collidable->GetTag();
	switch (tag)
	{
	case ObjectTag::Attack:
#if _DEBUG
		message += "�U��";
#endif
		break;
	case ObjectTag::Shield:
#if _DEBUG
		message += "��";
#endif
		break;
	case ObjectTag::Search:
#if _DEBUG
		message += "���G";
#endif
		break;
	case ObjectTag::Item:
#if _DEBUG
		message += "�A�C�e��";
#endif
		break;
	case ObjectTag::Rest:
#if _DEBUG
		message += "�x��";
#endif
		break;
	case ObjectTag::BossEnter:
#if _DEBUG
		message += "�{�X�̓���";
#endif
		break;
	}
#if _DEBUG
	message += "�Ɠ�������\n";
	printfDx(message.c_str());
#endif
}

void Player::SetModelPos()
{
	m_modelPos = m_collisionPos;
}
