#include "Immortal.h"

//�A�^�b�N��Collidable���o�^����Ă��Ȃ�

namespace
{
	//�L�����N�^�[��
	constexpr const char* cCharacterName = "Immortal";
	//���f���p�X
	constexpr const char* cModelPath = "Data/Enemy/immortalModel.mv1";
	//���f���̃T�C�Y�̊g�嗦
	constexpr float cModelSize = 0.4f;
	//���f���̒���
	constexpr float cCapsuleLen = 40.0f;
	//���f���̔��a
	constexpr float cCapsuleRadius = 14.0f;
	//���f���̍��W�����킹��
	constexpr float cModelPosY = 12.0f;
	//���S�I��
	bool cDead = false;
	//���S�����Ƃ��̃A�j���[�V����
	constexpr float cDeadFrame = 116.0f;
	//���G�͈�
	constexpr float cSearchRadius = 120.0f;
	//�ߋ����̍s���Ɉڂ鋗��
	constexpr float cNear = 50.0f;
	//�U������̔��a
	constexpr float cAttackRadius = 18.0f;
}

/// <summary>
/// �R���X�g���N�^
/// </summary>
Immortal::Immortal() :
	EnemyBase(Collidable::Priority::Middle)
{
	//�����蔻��̐ݒ�
	InitCollision(MyLibrary::LibVec3(0.0f, 2.0f, 0.0f), cCapsuleLen, cCapsuleRadius);

	//���f���̓ǂݍ���
	LoadModel(cModelPath);
	//�A�j���[�V������X�e�[�^�X���擾
	LoadData(cCharacterName);
	//���G�͈͂̐ݒ�
	m_searchRadius = cSearchRadius;

	//�A�j���[�V�����̔��菉����
	m_anim.s_attack = false;
	m_anim.s_moveflag = false;
	m_anim.s_hit = false;
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Immortal::~Immortal()
{
	//���������
	MV1DeleteModel(m_modelHandle);
}

/// <summary>
/// ������
/// </summary>
/// <param name="physics">�����N���X�̃|�C���^</param>
void Immortal::Init(float posX, float posY, float posZ, std::shared_ptr<MyLibrary::Physics> physics)
{
	//���
	m_pPhysics = physics;

	m_isExist = true;

	Collidable::Init(m_pPhysics);

	//�����N���X�̏�����
	InitRigidbody(posX, posY + 14.0f, posZ);

	//���S���W�̐ݒ�
	CalculationCenterPos(1.0f, cModelSize);

	//���f���̍��W��ݒ�
	SetModelPos();
	MV1SetPosition(m_modelHandle, m_modelPos.ConversionToVECTOR());

	//���G��������铖���蔻����쐬
	InitSearch(cSearchRadius, 0.0f);
	InitAttack(cAttackRadius);

	//���f���̃T�C�Y�ݒ�
	MV1SetScale(m_modelHandle, VGet(cModelSize, cModelSize, cModelSize));

	//�A�j���[�V�����ݒ�
	m_nowAnimNo = MV1AttachAnim(m_modelHandle, m_animIdx["Idle"]);
	m_nowAnimIdx = m_animIdx["Idle"];

	//����false�ɂ���
	m_anim.s_isDead = false;
	cDead = false;
}

/// <summary>
/// �X�V
/// </summary>
/// <param name="playerPos">�v���C���[���W</param>
/// <param name="isChase">�v���C���[�Ɛ킦�邩�ǂ���</param>
void Immortal::Update(MyLibrary::LibVec3 playerPos, bool isChase)
{
	//�A�j���[�V�����̍X�V
	if (!cDead)
	{
		m_isAnimationFinish = UpdateAnim(m_nowAnimNo, ANIMATION_MAX);

	}
	//���S�������̃A�j���[�V�����X�V
	else if (cDead && m_nowFrame <= cDeadFrame)
	{
		m_isAnimationFinish = UpdateAnim(m_nowAnimNo, ANIMATION_MAX);
	}

	UpdateAnimationBlend();

	DistanceUpdate(playerPos);

	//���݃��[�V����
	HitUpdate(6);

	//���ݏ�Ԃ���������
	if (m_anim.s_hit && m_isAnimationFinish)
	{
		m_anim.s_hit = false;
	}
	//����ł��鎞
	else if (m_anim.s_hit)
	{
		//�����Ȃ��悤�ɂ���
		m_move = VGet(0.0f, 0.0f, 0.0f);
		m_moveVec = MyLibrary::LibVec3(0.0f, 0.0f, 0.0f);
	}

	//�U���I��
	if (m_anim.s_attack && m_isAnimationFinish)
	{
		m_anim.s_attack = false;
	}
	

	//����łȂ��Ƃ�
	if (!m_anim.s_hit && !m_anim.s_isDead)
	{
		//�A�N�V����
		Action(playerPos, isChase);
	}

	//�ړ�����
	MoveUpdate();

	//�^�[�Q�b�g���
	TargetNow();
	//�U�����󂯂���
	//�U�����������Ă���Ƃ�
	if (m_isEnterHit)
	{
		m_status.s_hp -= m_col->GetAttack() - m_status.s_defense;

		//HP��0�ɂȂ�ƃq�b�g���Ȃ�
		if (m_status.s_hp > 0.0f)
		{
			m_anim.s_hit = true;
		}
	}

	TriggerUpdate();
	HitTriggerUpdate();


	//����̍X�V
	MyLibrary::LibVec3 centerPos = rigidbody.GetPos();
	MyLibrary::LibVec3 attackPos = MyLibrary::LibVec3(rigidbody.GetPos().x + sinf(m_angle) * -25.0f, rigidbody.GetPos().y, rigidbody.GetPos().z - cosf(m_angle) * 25.0f);
	m_pSearch->Update(centerPos);
	m_pAttack->Update(attackPos);

	//���񂾂Ƃ�
	if (m_status.s_hp <= 0.0f)
	{
		//�A�j���[�V������������
		m_anim.s_attack = false;
		m_anim.s_hit = false;
		m_anim.s_moveflag = false;

		Death();
		cDead = true;
	}
}

/// <summary>
/// ����łȂ��Ƃ��ȊO�̃A�N�V����
/// </summary>
/// <param name="playerPos">�v���C���[�̃|�W�V����</param>
/// <param name="isChase">�v���C���[�Ɛ킦�邩�ǂ���</param>
void Immortal::Action(MyLibrary::LibVec3 playerPos, bool isChase)
{
	//�v���C���[����������
	if (m_pSearch->GetIsStay())
	{
		//�U�����ĂȂ���
		if (!m_anim.s_attack)
		{
			//���������߂�
			AngleUpdate(playerPos);
		}

		//�p�x���o���v���C���[�̎�������^��������
		MATRIX mtx = MGetRotY(D2R(m_moveTurning) + DX_PI_F / 2);
		MATRIX mtxR = MGetRotY(D2R(m_moveReverseTurning) - DX_PI_F / 2);

		//�߂�����Ȃ��Ƃ��̍s��
		if (m_difPSize > cNear)
		{
			WalkUpdate("Walk", 2);

			//�U�����ĂȂ��Ƃ��̏���
			if (!m_anim.s_attack)
			{
				//�����A�j���[�V����
				m_anim.s_moveflag = true;

				m_status.s_speed = 0.01f;

				m_move = VScale(m_difPlayer, m_status.s_speed);
			}
		}
		//�߂��ɍs�������̍s��
		else if (m_difPSize <= cNear)
		{
			m_status.s_speed = 0.3f;

			m_move = VNorm(m_difPlayer);

			m_move = VScale(m_move, m_status.s_speed);

			//�����_���s����0���o���ꍇ
			//������
			if (m_randomAction == 0)
			{
				//�������[�V����������
				m_anim.s_moveflag = true;

				m_move = VTransform(m_move, mtxR);

				WalkUpdate("LeftWalk", 3);

			}
			//�����_���s����1���o���ꍇ
			//�E����
			else if (m_randomAction == 1)
			{
				//�������[�V����������
				m_anim.s_moveflag = true;

				m_move = VTransform(m_move, mtx);

				WalkUpdate("RightWalk", 4);
			}
			//�����_���s����2���o���ꍇ
			else if (m_randomAction == 2)
			{
				//�U�����[�V����������
				m_anim.s_attack = true;

				m_move = VGet(0.0f, 0.0f, 0.0f);

				AttackUpdate("Attack1", 5);

				//�A�j���[�V�����t���[�����ɍU��������o��
				if (m_nowFrame == 22)
				{
					InitAttackUpdate(m_status.s_attack);
				}
				else if (m_nowFrame >= 35.0f)
				{
					//��������Z�b�g
					m_pAttack->CollisionEnd();
				}

				m_anim.s_moveflag = false;
			}
		}

		//�A�j���[�V�������I���x�Ƀ����_���ȍs�����s��
		if (m_isAnimationFinish)
		{
			m_randomAction = GetRand(2);
		}

		//�ړ�����
		m_moveVec = MyLibrary::LibVec3(m_move.x, m_move.y, m_move.z);
	}
	//�v���C���[������������
	else if (m_pSearch->GetIsExit())
	{
		//�A�C�h����Ԃɂ���
		IdleUpdate();
		//�����Ȃ��悤�ɂ���
		m_anim.s_moveflag = false;
		//�U�����Ȃ��悤�ɂ���
		m_anim.s_attack = false;
		//��������Z�b�g
		m_pAttack->CollisionEnd();

		m_moveVec = MyLibrary::LibVec3(0.0f, 0.0f, 0.0f);
	}
}

/// <summary>
/// �`��
/// </summary>
void Immortal::Draw()
{
	//�����蔻����W���擾���ă��f���̕`����W��ݒ肷��
	SetDrawModelPos(cModelPosY);
	//3D���f���̉�]�n���Z�b�g����
	MV1SetRotationXYZ(m_modelHandle, VGet(0.0f, m_angle, 0.0f));
	//���f���̕`��
	MV1DrawModel(m_modelHandle);
}
