#include "Bear.h"

namespace
{
	//�L�����N�^�[��
	constexpr const char* cCharacterName = "bear";
	//���f���p�X
	constexpr const char* cModelPath = "Data/Enemy/BearModel.mv1";
	//���f���̃T�C�Y�̊g�嗦
	constexpr float cModelSize = 0.7f;
	//���f���̒���
	constexpr float cCapsuleLen = 40.0f;
	//���f���̔��a
	constexpr float cCapsuleRadius = 25.0f;
	//���f���̍��W�����킹��
	constexpr float cModelPosY = 24.0f;
	//���S�I��
	bool cDead = false;
	//���S�����Ƃ��̃A�j���[�V����
	constexpr float cDeadFrame = 137.0f;
	//���G�͈�
	constexpr float cSearchRadius = 120.0f;
}

/// <summary>
/// �R���X�g���N�^
/// </summary>
Bear::Bear() :
	EnemyBase(Collidable::Priority::High)
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
Bear::~Bear()
{
	//���������
	MV1DeleteModel(m_modelHandle);
}

/// <summary>
/// ������
/// </summary>
/// <param name="posX"></param>
/// <param name="posY"></param>
/// <param name="posZ"></param>
/// <param name="physics"></param>
void Bear::Init(float posX, float posY, float posZ, std::shared_ptr<MyLibrary::Physics> physics)
{
	//���
	m_pPhysics = physics;

	m_isExist = true;

	Collidable::Init(m_pPhysics);

	//�����N���X�̏�����
	InitRigidbody(posX, posY + cCapsuleRadius, posZ);

	//���S���W�̐ݒ�
	CalculationCenterPos(1.0f, cModelSize);

	//���f���̍��W��ݒ�
	SetModelPos();
	MV1SetPosition(m_modelHandle, m_modelPos.ConversionToVECTOR());

	//���G��������铖���蔻����쐬
	InitSearch(cSearchRadius, 35.0f);

	//���f���̃T�C�Y�ݒ�
	MV1SetScale(m_modelHandle, VGet(cModelSize, cModelSize, cModelSize));

	//�A�j���[�V�����ݒ�
	m_nowAnimNo = MV1AttachAnim(m_modelHandle, m_animIdx["Idle"]);
	m_nowAnimIdx = m_animIdx["Idle"];

	m_anim.s_isDead = false;
	cDead = false;

	m_status.s_hp = 1.0f;
}

/// <summary>
/// �Q�[���̎d�l��ł̏���������
/// </summary>
/// <param name="posX">X���W</param>
/// <param name="posY">Y���W</param>
/// <param name="posZ">Z���W</param>
/// <param name="physics">�����N���X</param>
void Bear::GameInit(float posX, float posY, float posZ, std::shared_ptr<MyLibrary::Physics> physics)
{
	m_pPhysics = physics;

	Collidable::Init(m_pPhysics);

	//���G��������铖���蔻����쐬
	InitSearch(cSearchRadius, 35.0f);

	//���S���W�̐ݒ�
	CalculationCenterPos(1.0f, cModelSize);
	
	//�����N���X�̏�����
	InitRigidbody(posX, posY + cCapsuleRadius, posZ);

	//���f���̍��W��ݒ�
	SetModelPos();
	MV1SetPosition(m_modelHandle, m_modelPos.ConversionToVECTOR());

	//���f���̃T�C�Y�ݒ�
	MV1SetScale(m_modelHandle, VGet(cModelSize, cModelSize, cModelSize));

	//�A�j���[�V�����ݒ�
	m_nowAnimNo = MV1AttachAnim(m_modelHandle, m_animIdx["Idle"]);
	m_nowAnimIdx = m_animIdx["Idle"];

	m_anim.s_isDead = false;
	cDead = false;

}

/// <summary>
/// �X�V����
/// </summary>
/// <param name="playerPos"></param>
/// <param name="isChase"></param>
void Bear::Update(MyLibrary::LibVec3 playerPos, bool isChase)
{
	//�A�j���[�V�����̍X�V
	if (!cDead)
	{
		m_isAnimationFinish = UpdateAnim(m_nowAnimNo, ANIMATION_MAX);
	}
	//���S�����Ƃ��̃A�j���[�V�����X�V
	else if (cDead && m_nowFrame <= cDeadFrame)
	{
		m_isAnimationFinish = UpdateAnim(m_nowAnimNo, ANIMATION_MAX);
	}

	UpdateAnimationBlend();

	DistanceUpdate(playerPos);

	//�ړ�����
	MoveUpdate();

	//�^�[�Q�b�g���
	TargetNow();
	//�U�����󂯂���
	if (m_isEnterHit)
	{
		m_status.s_hp -= m_col->GetAttack() - m_status.s_defense;
	}
	//�v���C���[���{�X�����ɓ�������
	if (m_isBossDiscovery && !cDead)
	{
		Action(playerPos, isChase);
	}



	TriggerUpdate();
	HitTriggerUpdate();

	//����̍X�V
	MyLibrary::LibVec3 centerPos = rigidbody.GetPos();
	m_pSearch->Update(centerPos);

	//���񂾎�
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
/// �{�X�̍s��
/// </summary>
/// <param name="playerPos">�v���C���[�̃|�W�V����</param>
/// <param name="isChase">�v���C���[</param>
void Bear::Action(MyLibrary::LibVec3 playerPos, bool isChase)
{
	//�G���v���C���[�̈ʒu�ɂ���ĕ�����␳����
	float Cx = m_modelPos.x - playerPos.x;
	float Cz = m_modelPos.z - playerPos.z;

	//�U�����ĂȂ���
	if (!m_anim.s_attack)
	{
		//���������߂�
		AngleUpdate(playerPos);
	}

	//�v���C���[���͈͊O��������
	if (!m_pSearch->GetIsStay())
	{
		WalkUpdate("Walk", 2);

		//�U�����ĂȂ���
		if (!m_anim.s_attack)
		{
			//�����A�j���[�V����
			m_anim.s_moveflag = true;
			//�X�s�[�h
			m_status.s_speed = 0.01f;

			m_move = VScale(m_difPlayer, m_status.s_speed);

			//�ړ�����
			m_moveVec = MyLibrary::LibVec3(m_move.x, m_move.y, m_move.z);
		}
		
	}
	//�˒������ɓ�����
	else if (m_pSearch->GetIsStay())
	{
		//�����_���s����0���o����
		if (m_randomAction == 0)
		{
			m_anim.s_moveflag = false;
			m_anim.s_attack = true;

			AttackUpdate("Attack1", 3);
		}
		//�����_���s����1���o����
		else if (m_randomAction == 1)
		{
			m_anim.s_moveflag = false;
			m_anim.s_attack = true;

			AttackUpdate("Attack2", 4);
		}
		//�����_���s����2���o����
		else if (m_randomAction == 2)
		{
			m_anim.s_moveflag = false;
			m_anim.s_attack = true;

			AttackUpdate("Attack3", 5);
		}

		m_moveVec = MyLibrary::LibVec3(0.0f, 0.0f, 0.0f);
	}


	//�A�j���[�V�������I���x�Ƀ����_���ȍs�����s��
	if (m_isAnimationFinish)
	{
		m_anim.s_attack = false;
		m_randomAction = GetRand(2);
	}

}

/// <summary>
/// �`�揈��
/// </summary>
void Bear::Draw()
{
	//�����蔻����W���擾���ă��f���̕`����W��ݒ肷��
	SetDrawModelPos(cModelPosY);
	//3D���f���̉�]�n���Z�b�g����
	MV1SetRotationXYZ(m_modelHandle, VGet(0.0f, m_angle, 0.0f));
	//���f���̕`��
	MV1DrawModel(m_modelHandle);
}
