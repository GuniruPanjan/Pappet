#include "Assassin.h"
#include "Ui/UI.h"
#include "Manager/EffectManager.h"
#include "Manager/SEManager.h"
#include "Item/EnemyWeapon.h"

namespace
{
	//�L�����N�^�[��
	constexpr const char* cCharacterName = "Assassin";
	//���f���p�X
	constexpr const char* cModelPath = "Data/Enemy/Assassin.mv1";
	//����̃p�X
	constexpr const char* cWeaponPath = "Data/Weapon/Dagger.mv1";
	//�A�^�b�`���镐��̃t���[����
	constexpr const char* cFrameName = "Pattern";
	//�U������ɂ��镐��̃t���[����
	constexpr const char* cFrameAttackName = "Tip";
	//�`���[�g���A������
	bool cTutorial;
	//���f���̃T�C�Y�̊g�嗦
	constexpr float cModelSize = 0.5f;
	//���f���̒���
	constexpr float cCapsuleLen = 50.0f;
	//���f���̔��a
	constexpr float cCapsuleRadius = 14.0f;
	//���f���̍��W�����킹��
	constexpr float cModelPosY = 12.0f;
	//���S�I��
	bool cDead = false;
	//���S�����Ƃ��̃A�j���[�V����
	constexpr float cDeadFrame = 65.0f;
	//���G�͈�
	constexpr float cSearchRadius = 120.0f;
	//�ߋ����̍s���Ɉڂ鋗��
	constexpr float cNear = 80.0f;
	//�U���͈�(�i�C�t)
	constexpr float cAttackRadiusKnife = 30.0f;
	//�U���͈�(�R��)
	constexpr float cAttackRadiusKick = 25.0f;
	//����̊p�x
	constexpr float cAngle = 30.0f;
	//����̋���
	constexpr float cAngleDistance = 400.0f;
	//�U�����̈ړ�����
	float cAttackMove = 0.0f;
	//������̈ړ�����
	float cAvoidanceMove = 0.0f;
	//�U���q�b�g���̈ړ�����
	float cHitMove = 0.0f;

	//���̃G�t�F�N�g���ړ�������
	VECTOR cLigPos = VGet(0.0f, 0.0f, 0.0f);

	bool cA[4];

	//�v���C���[�𔭌������Ƃ�
	bool cPlayerLook = false;

	//����̃|�W�V����
	const VECTOR cWeaponPosition = VGet(0.0f, 0.0f, 0.0f);
	//�����X��]
	constexpr float cWeaponX = 0.000f;
	//�����Y��]
	constexpr float cWeaponY = 0.000f;
	//�����Z��]
	constexpr float cWeaponZ = 4.578f;

	//�V���O���g��
	EffectManager& cEffect = EffectManager::GetInstance();

	bool cOne = false;

	// �萔�̒�`
	constexpr float cAvoidanceMoveFrameStart = 30.0f;
	constexpr float cHitMoveFrameStart = 5.0f;
	constexpr float cHitMoveFrameEnd = 25.0f;
	constexpr float cDeathFrame = 36.0f;
	constexpr float cHitEffectOffsetY = 30.0f;
	constexpr float cStrongHitEffectOffsetY = 20.0f;
	constexpr float cModelScale = 0.15f;

	// �萔�̒�`
	constexpr float cAttackMoveDistance1 = 0.5f;
	constexpr float cAttackMoveDistance2 = 0.8f;
	constexpr float cAttackMoveDistance3 = 0.5f;
	constexpr float cAttackMoveDistance4 = -0.4f;
	constexpr float cIdleFrameDuration = 40.0f;
	constexpr float cAttackFrame1 = 5.0f;
	constexpr float cAttackFrame2 = 11.0f;
	constexpr float cAttackFrame3 = 24.0f;
	constexpr float cAttackFrame4 = 25.0f;
	constexpr float cAttackFrame5 = 30.0f;
	constexpr float cAttackFrame6 = 35.0f;
	constexpr float cAttackFrame7 = 40.0f;
	constexpr float cAttackFrame8 = 48.0f;
	constexpr float cAttackFrame9 = 50.0f;
	constexpr float cAttackFrame10 = 55.0f;
	constexpr float cAttackFrame11 = 62.0f;
	constexpr float cAttackFrame12 = 65.0f;
	constexpr float cAttackFrame13 = 70.0f;
	constexpr float cAttackFrame14 = 28.0f;
	constexpr float cAttackFrame15 = 18.0f;
	constexpr float cAttackFrame16 = 20.0f;

	// �����_���A�N�V�����͈̔�
	constexpr int cRandomActionMax = 6;
	constexpr int cActionFixed = 5;

}

/// <summary>
/// �R���X�g���N�^
/// </summary>
Assassin::Assassin():
	EnemyBase(Collidable::Priority::High),
	m_attackPos(),
	m_moveFrameRightHand(0),
	m_frameRightHand(VGet(0.0f,0.0f,0.0f)),
	m_attackMove(VGet(0.0f,0.0f,0.0f)),
	m_avoidanceMove(VGet(0.0f,0.0f,0.0f)),
	m_hitMove(VGet(0.0f, 0.0f, 0.0f)),
	m_weaponFrameMatrix()
{
	//�����蔻��̐ݒ�
	InitCollision(MyLibrary::LibVec3(0.0f, 2.0f, 0.0f), cCapsuleLen, cCapsuleRadius);
	//���f���̓ǂݍ���
	LoadModel(cModelPath);
	//�A�j���[�V������X�e�[�^�X���擾
	LoadData(cCharacterName);
	//���G�͈͂̐ݒ�
	m_searchRadius = cSearchRadius;

	for (int i = 0; i < 2; i++)
	{
		m_daggerFrame[i] = 0;
		m_ligLeftLegFrame[i] = 0;
		m_ligRightLegFrame[i] = 0;
		m_daggerPos[i] = VGet(0.0f, 0.0f, 0.0f);
		m_ligLeftLegPos[i] = VGet(0.0f, 0.0f, 0.0f);
		m_ligRightLegPos[i] = VGet(0.0f, 0.0f, 0.0f);
	}

	//�A�j���[�V�����̔��菉����
	m_anim.s_attack = false;
	m_anim.s_moveflag = false;
	m_anim.s_hit = false;
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Assassin::~Assassin()
{
	//���������
	MV1DeleteModel(m_modelHandle);
}

/// <summary>
/// ����������
/// </summary>
/// <param name="posX"></param>
/// <param name="posY"></param>
/// <param name="posZ"></param>
/// <param name="physics"></param>
void Assassin::Init(float posX, float posY, float posZ, std::shared_ptr<MyLibrary::Physics> physics, bool tutorial, EnemyWeapon& weapon)
{
	cTutorial = tutorial;

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

	//���G�͈͂����铖���蔻��쐬
	InitSearch(cSearchRadius, 35.0f);

	//���f���̃T�C�Y�ݒ�
	MV1SetScale(m_modelHandle, VGet(cModelSize, cModelSize, cModelSize));

	weapon.Init(cWeaponPath, cWeaponPosition, cWeaponX, cWeaponY, cWeaponZ, cModelSize * 0.15f);

	//�A�j���[�V�����ݒ�
	m_nowAnimNo = MV1AttachAnim(m_modelHandle, m_animIdx["Idle"]);
	m_nowAnimIdx = m_animIdx["Idle"];

	float totalAnimFrame = MV1GetAttachAnimTotalTime(m_modelHandle, m_nowAnimNo);

	m_anim.s_isDead = false;
	cDead = false;

	//�ő�HP���擾
	m_maxHP = m_status.s_hp;

	if (cTutorial)
	{
		m_bossName = " ���ׂ̉e ";
		m_subName = " P E P U S I [�`���[�g���A��] ";

		m_bossjudg = true;
	}
	else
	{
		m_bossjudg = false;
	}


	cOne = false;
	m_deadOne = false;

	
}

/// <summary>
/// �Q�[���d�l��ł̏���������
/// </summary>
/// <param name="posX"></param>
/// <param name="posY"></param>
/// <param name="posZ"></param>
/// <param name="physics"></param>
void Assassin::GameInit(float posX, float posY, float posZ, std::shared_ptr<MyLibrary::Physics> physics, bool tutorial, EnemyWeapon& weapon)
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

	weapon.Init(cWeaponPath, cWeaponPosition, cWeaponX, cWeaponY, cWeaponZ, cModelSize * 0.15f);

	//�A�j���[�V�����ݒ�
	m_nowAnimNo = MV1AttachAnim(m_modelHandle, m_animIdx["Idle"]);
	m_nowAnimIdx = m_animIdx["Idle"];

	m_anim.s_isDead = false;
	cDead = false;

	//�ő�HP���擾
	m_maxHP = m_status.s_hp;

	if (cTutorial)
	{
		m_bossName = " ���ׂ̉e ";
		m_subName = " P E P U S I [�`���[�g���A��] ";

		m_bossjudg = true;
	}
	else
	{
		m_bossjudg = false;
	}

	cOne = false;
	m_deadOne = false;
}

/// <summary>
/// �X�V����
/// </summary>
/// <param name="playerPos"></param>
/// <param name="shieldPos"></param>
/// <param name="isChase"></param>
/// <param name="se"></param>
/// <param name="physics"></param>
void Assassin::Update(MyLibrary::LibVec3 playerPos, MyLibrary::LibVec3 shieldPos, bool isChase, SEManager& se, std::shared_ptr<MyLibrary::Physics> physics, EnemyWeapon& weapon)
{
	// �A�j���[�V�����ňړ����Ă���t���[���̔ԍ�����������
	m_moveFrameRightHand = MV1SearchFrame(m_modelHandle, "mixamorig:RightHandThumb2");
	m_ligLeftLegFrame[0] = MV1SearchFrame(m_modelHandle, "mixamorig:LeftLeg");
	m_ligLeftLegFrame[1] = MV1SearchFrame(m_modelHandle, "mixamorig:LeftToe_End_end");
	m_ligRightLegFrame[0] = MV1SearchFrame(m_modelHandle, "mixamorig:RightLeg");
	m_ligRightLegFrame[1] = MV1SearchFrame(m_modelHandle, "mixamorig:RightToe_End_end");

	m_frameRightHand = MV1GetFramePosition(m_modelHandle, m_moveFrameRightHand);
	m_ligLeftLegPos[0] = MV1GetFramePosition(m_modelHandle, m_ligLeftLegFrame[0]);
	m_ligLeftLegPos[1] = MV1GetFramePosition(m_modelHandle, m_ligLeftLegFrame[1]);
	m_ligRightLegPos[0] = MV1GetFramePosition(m_modelHandle, m_ligRightLegFrame[0]);
	m_ligRightLegPos[1] = MV1GetFramePosition(m_modelHandle, m_ligRightLegFrame[1]);

	// ����̍X�V
	MyLibrary::LibVec3 attackKnifePos = MyLibrary::LibVec3(m_frameRightHand.x, m_frameRightHand.y, m_frameRightHand.z);
	MyLibrary::LibVec3 attackLeftKickPos1 = MyLibrary::LibVec3(m_ligLeftLegPos[0].x, m_ligLeftLegPos[0].y, m_ligLeftLegPos[0].z);
	MyLibrary::LibVec3 attackLeftKickPos2 = MyLibrary::LibVec3(m_ligLeftLegPos[1].x, m_ligLeftLegPos[1].y, m_ligLeftLegPos[1].z);
	MyLibrary::LibVec3 attackRightKickPos1 = MyLibrary::LibVec3(m_ligRightLegPos[0].x, m_ligRightLegPos[0].y, m_ligRightLegPos[0].z);
	MyLibrary::LibVec3 attackRightKickPos2 = MyLibrary::LibVec3(m_ligRightLegPos[1].x, m_ligRightLegPos[1].y, m_ligRightLegPos[1].z);

	// ���[�J�������[���h�ϊ��s����擾����
	m_weaponFrameMatrix = MV1GetFrameLocalWorldMatrix(m_modelHandle, m_moveFrameRightHand);

	weapon.WeaponFrame(cFrameName, cFrameAttackName);
	weapon.Update(m_weaponFrameMatrix, cModelScale);

	// ����̊p�x�����߂�
	m_viewAngle = cAngle;
	// ����̋��������߂�
	m_viewDistance = cAngleDistance;

	// �A�j���[�V�����̍X�V
	if (!cDead)
	{
		m_isAnimationFinish = UpdateAnim(m_nowAnimNo, ANIMATION_MAX);
	}
	// ���S�����Ƃ��̃A�j���[�V�����X�V
	else if (cDead && m_nowFrame <= cDeadFrame)
	{
		m_isAnimationFinish = UpdateAnim(m_nowAnimNo, ANIMATION_MAX);
	}

	UpdateAnimationBlend();

	DistanceUpdate(playerPos, shieldPos);
	AttackDistance();

	// ���݃��[�V����
	HitUpdate(10);

	// ���ݏ�Ԃ���������
	if (m_anim.s_hit && m_isAnimationFinish)
	{
		m_anim.s_hit = false;
	}
	// ����ł��鎞
	else if (m_anim.s_hit)
	{
		// �����Ȃ��悤�ɂ���
		m_move = VGet(0.0f, 0.0f, 0.0f);
		m_moveVec = MyLibrary::LibVec3(0.0f, 0.0f, 0.0f);

		m_enemyAnim.s_rool = false;
		m_anim.s_moveflag = false;

		if (m_anim.s_attack)
		{
			// ��������Z�b�g
			m_pAttack->CollisionEnd();
			m_pLigAttack->CollisionEnd();

			m_anim.s_attack = false;
		}

		// ���݈ړ�����
		if (m_nowFrame > cHitMoveFrameStart && m_nowFrame <= cHitMoveFrameEnd)
		{
			cHitMove = -1.0f;
		}
		else
		{
			cHitMove = 0.0f;
		}
	}

	// �U���I��
	if (m_anim.s_attack && m_isAnimationFinish)
	{
		m_anim.s_attack = false;
	}
	// ����I��
	if (m_enemyAnim.s_rool && m_isAnimationFinish)
	{
		m_enemyAnim.s_rool = false;
	}

	// ������̍s��
	if (m_enemyAnim.s_rool)
	{
		if (m_nowFrame <= cAvoidanceMoveFrameStart)
		{
			cAvoidanceMove = -1.0f;

			m_avoidnaceNow = true;
		}
		else
		{
			cAvoidanceMove = 0.0f;

			m_avoidnaceNow = false;
		}
	}

	if (!m_anim.s_attack && !m_enemyAnim.s_rool && !m_anim.s_hit)
	{
		cAttackMove = 0.0f;
		cAvoidanceMove = 0.0f;
		cHitMove = 0.0f;

		// �ړ�����
		MoveUpdate();

		cA[0] = true;

		for (int i = 0; i < 4; i++)
		{
			if (i != 0)
			{
				cA[i] = false;
			}
		}
	}
	// �U���s���ړ�
	else if (m_anim.s_attack && !m_enemyAnim.s_rool && !m_anim.s_hit)
	{
		cAvoidanceMove = 0.0f;
		cHitMove = 0.0f;

		m_attackMove = VScale(VGet(sinf(m_angle), 0.0f, cosf(m_angle)), cAttackMove);

		// �U���ړ�����
		MoveAnimUpdate(m_attackMove);

		cA[1] = true;

		for (int i = 0; i < 4; i++)
		{
			if (i != 1)
			{
				cA[i] = false;
			}
		}
	}
	// ����s���ړ�
	else if (m_enemyAnim.s_rool && !m_anim.s_attack && !m_anim.s_hit)
	{
		cAttackMove = 0.0f;
		cHitMove = 0.0f;

		m_avoidanceMove = VScale(VGet(sinf(m_angle), 0.0f, cosf(m_angle)), cAvoidanceMove);

		// ����ړ�����
		MoveAnimUpdate(m_avoidanceMove);

		cA[2] = true;

		for (int i = 0; i < 4; i++)
		{
			if (i != 2)
			{
				cA[i] = false;
			}
		}
	}
	// ���ݍs���ړ�
	else if (m_anim.s_hit && !m_enemyAnim.s_rool && !m_anim.s_attack)
	{
		cAttackMove = 0.0f;
		cAvoidanceMove = 0.0f;

		m_hitMove = VScale(VGet(sinf(m_angle), 0.0f, cosf(m_angle)), cHitMove);

		// ���݈ړ�����
		MoveAnimUpdate(m_hitMove);

		cA[3] = true;

		for (int i = 0; i < 4; i++)
		{
			if (i != 3)
			{
				cA[i] = false;
			}
		}
	}

	// �^�[�Q�b�g���
	TargetNow();

	// �U�����Ă��Ȃ��Ƃ�
	if (!m_anim.s_attack)
	{
		if (!cOne)
		{
			InitAttack(0.0f);
			InitLigAttack(attackLeftKickPos1, attackLeftKickPos2, cAttackRadiusKick);
			InitAttackDamage(0.0f);

			cOne = true;
		}
	}

	// �U�����󂯂���
	if (m_isEnterHit)
	{
		m_status.s_hp -= m_col->GetAttack() - m_status.s_defense;
		// Hit�G�t�F�N�g
		cEffect.EffectCreate("Hit", VGet(rigidbody->GetPos().x, rigidbody->GetPos().y + cHitEffectOffsetY, rigidbody->GetPos().z));
		// HitSE�Đ�
		PlaySoundMem(se.GetHitSE(), DX_PLAYTYPE_BACK, true);

		// HP��0�ɂȂ�ƃq�b�g���Ȃ�
		if (m_status.s_hp > 0.0f)
		{
			m_anim.s_hit = true;
		}
	}
	else if (m_isEnterStrengtHit)
	{
		m_status.s_hp -= m_strengthCol->GetAttack() - m_status.s_defense;

		// Hit�G�t�F�N�g
		cEffect.EffectCreate("Hit", VGet(rigidbody->GetPos().x, rigidbody->GetPos().y + cStrongHitEffectOffsetY, rigidbody->GetPos().z));

		// HitSE�Đ�
		PlaySoundMem(se.GetHitSE(), DX_PLAYTYPE_BACK, true);

		// HP��0�ɂȂ�ƃq�b�g���Ȃ�
		if (m_status.s_hp > 0.0f)
		{
			m_anim.s_hit = true;
		}
	}

	if (cTutorial)
	{
		// �v���C���[���{�X�����ɓ�������
		if (m_isBossDiscovery && !cDead && !m_anim.s_hit)
		{
			BossAction(playerPos, isChase, se, weapon);
		}
	}
	else
	{
		// ����łȂ���
		if (!m_anim.s_hit)
		{
			// �܂��ł��Ă��Ȃ����ߌ�Ń{�X�A�N�V�����̒��g���ʂ�
			Action(playerPos, isChase, se, weapon);
		}
	}

	TriggerUpdate();
	HitTriggerUpdate();

	// ����̍X�V
	MyLibrary::LibVec3 centerPos = rigidbody->GetPos();
	m_pSearch->Update(centerPos);

	// �`���[�g���A���������ꍇ
	if (cTutorial)
	{
		// ���񂾎�
		if (m_status.s_hp <= 0.0f)
		{
			// �A�j���[�V������������
			m_anim.s_attack = false;
			m_anim.s_hit = false;
			m_anim.s_moveflag = false;

			Death();

			if (m_nowFrame == cDeathFrame)
			{
				// ���SSE�Đ�
				PlaySoundMem(se.GetDiedSE(), DX_PLAYTYPE_BACK, true);
			}

			cDead = true;
			m_isBossDead = true;
		}
		else
		{
			m_isBossDead = false;
		}
	}
	else
	{
		// ���񂾂Ƃ�
		if (m_status.s_hp <= 0.0f)
		{
			// �A�j���[�V������������
			m_anim.s_attack = false;
			m_anim.s_hit = false;
			m_anim.s_moveflag = false;

			Death();

			if (m_nowFrame == cDeathFrame)
			{
				// ���SSE�Đ�
				PlaySoundMem(se.GetDiedSE(), DX_PLAYTYPE_BACK, true);
			}

			cDead = true;
		}
	}
	
}

void Assassin::Action(MyLibrary::LibVec3 playerPos, bool isChase, SEManager& se, EnemyWeapon& weapon)
{
	//����̍X�V
	MyLibrary::LibVec3 attackKnifePos = MyLibrary::LibVec3(m_frameRightHand.x, m_frameRightHand.y, m_frameRightHand.z);
	MyLibrary::LibVec3 attackKnifePos1 = MyLibrary::LibVec3(weapon.GetFramePos1().x, weapon.GetFramePos1().y, weapon.GetFramePos1().z);
	MyLibrary::LibVec3 attackKnifePos2 = MyLibrary::LibVec3(weapon.GetFramePos2().x, weapon.GetFramePos2().y, weapon.GetFramePos2().z);
	MyLibrary::LibVec3 attackLeftKickPos1 = MyLibrary::LibVec3(m_ligLeftLegPos[0].x, m_ligLeftLegPos[0].y, m_ligLeftLegPos[0].z);
	MyLibrary::LibVec3 attackLeftKickPos2 = MyLibrary::LibVec3(m_ligLeftLegPos[1].x, m_ligLeftLegPos[1].y, m_ligLeftLegPos[1].z);
	MyLibrary::LibVec3 attackRightKickPos1 = MyLibrary::LibVec3(m_ligRightLegPos[0].x, m_ligRightLegPos[0].y, m_ligRightLegPos[0].z);
	MyLibrary::LibVec3 attackRightKickPos2 = MyLibrary::LibVec3(m_ligRightLegPos[1].x, m_ligRightLegPos[1].y, m_ligRightLegPos[1].z);

	//����
	if (!IsPlayerInView(playerPos))
	{
		cPlayerLook = true;
	}
	else
	{
		cPlayerLook = false;
	}

	//�v���C���[����������
	if (m_pSearch->GetIsStay() || cPlayerLook)
	{
		// �U�����ĂȂ���
		if (!m_anim.s_attack)
		{
			// ���������߂�
			AngleUpdate(playerPos);
		}

		// �߂�����Ȃ����̍s��
		if (m_difPSize > cNear && !m_enemyAnim.s_rool && m_randomAction != 6)
		{
			WalkUpdate("Walk", 2);

			// �U�����ĂȂ��Ƃ��̏���
			if (!m_anim.s_attack)
			{
				// �����A�j���[�V����
				m_anim.s_moveflag = true;

				m_status.s_speed = 0.02f;

				m_move = VScale(m_difPlayer, m_status.s_speed);

				m_enemyAnim.s_rool = false;
			}
		}
		// �߂��ɍs�������̍s��
		else if (m_difPSize <= cNear)
		{
			// �����_���s����0���o���ꍇ
			// �U��1
			if (m_randomAction == 0)
			{
				// �U�����[�V����������
				m_anim.s_attack = true;

				m_move = VGet(0.0f, 0.0f, 0.0f);

				AttackUpdate("Attack1", 3);

				m_enemyAnim.s_rool = false;
				m_anim.s_moveflag = false;
			}
			// �����_���s����1���o���ꍇ
			// �U��2
			else if (m_randomAction == 1)
			{
				// �U�����[�V����������
				m_anim.s_attack = true;

				m_move = VGet(0.0f, 0.0f, 0.0f);

				AttackUpdate("Attack2", 4);

				// �����ԏI��
				m_avoidnaceNow = false;

				m_enemyAnim.s_rool = false;
				m_anim.s_moveflag = false;
			}
			// �����_���s����2���o���ꍇ
			// �U��3
			else if (m_randomAction == 2)
			{
				// �U�����[�V����������
				m_anim.s_attack = true;

				m_move = VGet(0.0f, 0.0f, 0.0f);

				AttackUpdate("Attack3", 5);

				// �����ԏI��
				m_avoidnaceNow = false;

				m_enemyAnim.s_rool = false;
				m_anim.s_moveflag = false;
			}
			// �����_���s����3���o���ꍇ
			// �U��4
			else if (m_randomAction == 3)
			{
				// �U�����[�V����������
				m_anim.s_attack = true;

				m_move = VGet(0.0f, 0.0f, 0.0f);

				AttackUpdate("Attack4", 6);

				// �����ԏI��
				m_avoidnaceNow = false;

				m_enemyAnim.s_rool = false;
				m_anim.s_moveflag = false;
			}
			// �����_���s����4���o���ꍇ
			// �U��5
			else if (m_randomAction == 4)
			{
				// �U�����[�V����������
				m_anim.s_attack = true;

				m_move = VGet(0.0f, 0.0f, 0.0f);

				AttackUpdate("Attack5", 7);

				// �����ԏI��
				m_avoidnaceNow = false;

				m_anim.s_moveflag = false;
				m_enemyAnim.s_rool = false;
			}
			// �����_���s����5���o���ꍇ
			// ���
			else if (m_randomAction == 5)
			{
				m_enemyAnim.s_rool = true;

				m_nowAnimIdx = m_animIdx["Roll"];
				ChangeAnim(m_nowAnimIdx, m_animOne[8], m_animOne);

				m_anim.s_moveflag = false;
				m_anim.s_attack = false;
			}
			// �����_���s����6�͏o���ꍇ
			else if (m_randomAction == 6)
			{
				// �����ԏI��
				m_avoidnaceNow = false;

				m_enemyAnim.s_rool = false;
				m_anim.s_moveflag = false;
				m_anim.s_attack = false;
			}
		}

		// �U�����̍s��
		if (m_anim.s_attack)
		{
			if (m_randomAction == 0)
			{
				// �U������̃|�W�V����
				InitAttackLigUpdate(attackKnifePos1, attackKnifePos2);

				// �U�����̈ړ����鋗��
				if (m_nowFrame <= cAttackFrame4)
				{
					cAttackMove = cAttackMoveDistance1;
				}
				else
				{
					cAttackMove = 0.0f;
				}

				if (m_nowFrame == cAttackFrame1)
				{
					InitLigAttack(attackKnifePos1, attackKnifePos2, cAttackRadiusKnife);
					InitAttackDamage(m_status.s_attack);
				}
				// �A�j���[�V�����t���[�����ɍU��������o��
				if (m_nowFrame == cAttackFrame3)
				{
					InitAttackUpdate(m_status.s_attack);
				}
				if (m_nowFrame == cAttackFrame5)
				{
					InitAttackDamage(m_status.s_attack);
					// ��������Z�b�g
					m_pAttack->CollisionEnd();
					m_pLigAttack->CollisionEnd();
				}
				// �A�j���[�V�����t���[�����ɍU��������o��
				if (m_nowFrame == cAttackFrame7)
				{
					// �U��SE�Đ�
					PlaySoundMem(se.GetKnifeSE(), DX_PLAYTYPE_BACK, true);

					InitAttackUpdate(m_status.s_attack);
				}
				if (m_nowFrame >= cAttackFrame8)
				{
					InitAttackDamage(0.0f);
					// ��������Z�b�g
					m_pAttack->CollisionEnd();
					m_pLigAttack->CollisionEnd();
				}
			}
			else if (m_randomAction == 1)
			{
				// �U������̃|�W�V����
				InitAttackLigUpdate(attackKnifePos1, attackKnifePos2);

				// �U�����̈ړ����鋗��
				cAttackMove = 0.0f;

				if (m_nowFrame == cAttackFrame1)
				{
					InitLigAttack(attackKnifePos1, attackKnifePos2, cAttackRadiusKnife);
					InitAttackDamage(m_status.s_attack);
				}
				// �A�j���[�V�����t���[�����ɍU��������o��
				if (m_nowFrame == cAttackFrame3)
				{
					// �U��SE�Đ�
					PlaySoundMem(se.GetKnifeSE(), DX_PLAYTYPE_BACK, true);

					InitAttackUpdate(m_status.s_attack);
				}
				if (m_nowFrame >= cAttackFrame14)
				{
					InitAttackDamage(0.0f);
					// ��������Z�b�g
					m_pAttack->CollisionEnd();
					m_pLigAttack->CollisionEnd();
				}
			}
			else if (m_randomAction == 2)
			{
				// �U������̃|�W�V����
				InitAttackLigUpdate(attackKnifePos1, attackKnifePos2);

				// �U�����̈ړ����鋗��
				cAttackMove = 0.0f;

				if (m_nowFrame == cAttackFrame1)
				{
					InitLigAttack(attackKnifePos1, attackKnifePos2, cAttackRadiusKnife);
					InitAttackDamage(m_status.s_attack);
				}
				// �A�j���[�V�����t���[�����ɍU��������o��
				if (m_nowFrame == cAttackFrame2)
				{
					// �U��SE�Đ�
					PlaySoundMem(se.GetKnifeSE(), DX_PLAYTYPE_BACK, true);

					InitAttackUpdate(m_status.s_attack);
				}
				if (m_nowFrame >= cAttackFrame15)
				{
					InitAttackDamage(0.0f);
					// ��������Z�b�g
					m_pAttack->CollisionEnd();
					m_pLigAttack->CollisionEnd();
				}
			}
			else if (m_randomAction == 3)
			{
				// �U������̃|�W�V����
				InitAttackLigUpdate(attackLeftKickPos1, attackLeftKickPos2);

				// �U�����̈ړ����鋗��
				if (m_nowFrame <= cAttackFrame16)
				{
					cAttackMove = 0.0f;
				}
				else if (m_nowFrame > cAttackFrame16 && m_nowFrame <= cAttackFrame7)
				{
					cAttackMove = cAttackMoveDistance2;
				}
				else if (m_nowFrame > cAttackFrame7 && m_nowFrame <= cAttackFrame12)
				{
					cAttackMove = cAttackMoveDistance3;
				}
				else if (m_nowFrame > cAttackFrame12 && m_nowFrame <= cAttackFrame13)
				{
					cAttackMove = cAttackMoveDistance2;
				}
				else
				{
					cAttackMove = 0.0f;
				}

				if (m_nowFrame == cAttackFrame1)
				{
					InitLigAttack(attackLeftKickPos1, attackLeftKickPos2, cAttackRadiusKick);
					InitAttackDamage(m_status.s_attack);
				}
				// �A�j���[�V�����t���[�����ɍU��������o��
				if (m_nowFrame == cAttackFrame11)
				{
					// �U��SE�Đ�
					PlaySoundMem(se.GetKickSE(), DX_PLAYTYPE_BACK, true);

					InitAttackUpdate(m_status.s_attack);
				}
				if (m_nowFrame >= cAttackFrame13)
				{
					InitAttackDamage(0.0f);
					// ��������Z�b�g
					m_pAttack->CollisionEnd();
					m_pLigAttack->CollisionEnd();
				}
			}
			else if (m_randomAction == 4)
			{
				// �U������̃|�W�V����
				InitAttackLigUpdate(attackRightKickPos1, attackRightKickPos2);

				// �U�����̈ړ����鋗��
				if (m_nowFrame <= cAttackFrame3)
				{
					cAttackMove = 0.0f;
				}
				else if (m_nowFrame > cAttackFrame3 && m_nowFrame <= cAttackFrame4)
				{
					cAttackMove = cAttackMoveDistance2;
				}
				else if (m_nowFrame > cAttackFrame6 && m_nowFrame <= cAttackFrame10)
				{
					cAttackMove = cAttackMoveDistance4;
				}
				else
				{
					cAttackMove = 0.0f;
				}

				if (m_nowFrame == cAttackFrame1)
				{
					InitLigAttack(attackRightKickPos1, attackRightKickPos2, cAttackRadiusKick);
					InitAttackDamage(m_status.s_attack);
				}
				// �A�j���[�V�����t���[�����ɍU��������o��
				if (m_nowFrame == cAttackFrame4)
				{
					// �U��SE�Đ�
					PlaySoundMem(se.GetKickSE(), DX_PLAYTYPE_BACK, true);

					InitAttackUpdate(m_status.s_attack);
				}
				if (m_nowFrame >= cAttackFrame6)
				{
					InitAttackDamage(0.0f);
					// ��������Z�b�g
					m_pAttack->CollisionEnd();
					m_pLigAttack->CollisionEnd();
				}
			}
		}

		if (m_randomAction == 6)
		{
			m_nowAnimIdx = m_animIdx["Idle"];
			ChangeAnim(m_nowAnimIdx, m_animOne[9], m_animOne);

			cAttackMove = 0.0f;
			m_move = VGet(0.0f, 0.0f, 0.0f);
			cAvoidanceMove = 0.0f;

			if (m_nowFrame >= cIdleFrameDuration)
			{
				m_isAnimationFinish = true;
			}
		}

		// �A�j���[�V�������I���x�Ƀ����_���ȍs�����s��
		if (m_isAnimationFinish)
		{
			// �v���C���[�������Ă��鎞
			if (cPlayerLook)
			{
				m_randomAction = GetRand(cRandomActionMax);
			}
			// �v���C���[�������Ă��Ȃ���
			else
			{
				// �������悤�ɂ���
				m_randomAction = cActionFixed;
			}
		}

		// �ړ�����
		m_moveVec = MyLibrary::LibVec3(m_move.x, m_move.y, m_move.z);
	}
	//�v���C���[������������
	else if (m_pSearch->GetIsExit() && !cPlayerLook)
	{
		//�A�C�h����Ԃɂ���
		IdleUpdate();
		//�����Ȃ��悤�ɂ���
		m_anim.s_moveflag = false;
		//�U�����Ȃ��悤�ɂ���
		m_anim.s_attack = false;
		//��������Z�b�g
		m_pAttack->CollisionEnd();

		m_enemyAnim.s_rool = false;


		m_moveVec = MyLibrary::LibVec3(0.0f, 0.0f, 0.0f);
	}
}

/// <summary>
/// �{�X�̎��̍s��
/// </summary>
/// <param name="playerPos"></param>
/// <param name="isChase"></param>
/// <param name="se"></param>
void Assassin::BossAction(MyLibrary::LibVec3 playerPos, bool isChase, SEManager& se, EnemyWeapon& weapon)
{
	// ����̍X�V
	MyLibrary::LibVec3 attackKnifePos = MyLibrary::LibVec3(m_frameRightHand.x, m_frameRightHand.y, m_frameRightHand.z);
	MyLibrary::LibVec3 attackKnifePos1 = MyLibrary::LibVec3(weapon.GetFramePos1().x, weapon.GetFramePos1().y, weapon.GetFramePos1().z);
	MyLibrary::LibVec3 attackKnifePos2 = MyLibrary::LibVec3(weapon.GetFramePos2().x, weapon.GetFramePos2().y, weapon.GetFramePos2().z);
	MyLibrary::LibVec3 attackLeftKickPos1 = MyLibrary::LibVec3(m_ligLeftLegPos[0].x, m_ligLeftLegPos[0].y, m_ligLeftLegPos[0].z);
	MyLibrary::LibVec3 attackLeftKickPos2 = MyLibrary::LibVec3(m_ligLeftLegPos[1].x, m_ligLeftLegPos[1].y, m_ligLeftLegPos[1].z);
	MyLibrary::LibVec3 attackRightKickPos1 = MyLibrary::LibVec3(m_ligRightLegPos[0].x, m_ligRightLegPos[0].y, m_ligRightLegPos[0].z);
	MyLibrary::LibVec3 attackRightKickPos2 = MyLibrary::LibVec3(m_ligRightLegPos[1].x, m_ligRightLegPos[1].y, m_ligRightLegPos[1].z);

	// ����
	if (!IsPlayerInView(playerPos))
	{
		cPlayerLook = true;
	}
	else
	{
		cPlayerLook = false;
	}

	// �U�����ĂȂ���
	if (!m_anim.s_attack)
	{
		// ���������߂�
		AngleUpdate(playerPos);
	}

	// �߂�����Ȃ����̍s��
	if (m_difPSize > cNear && !m_enemyAnim.s_rool && m_randomAction != 6)
	{
		WalkUpdate("Walk", 2);

		// �U�����ĂȂ��Ƃ��̏���
		if (!m_anim.s_attack)
		{
			// �����A�j���[�V����
			m_anim.s_moveflag = true;

			m_status.s_speed = 0.02f;

			m_move = VScale(m_difPlayer, m_status.s_speed);

			m_enemyAnim.s_rool = false;
		}
	}
	// �߂��ɍs�������̍s��
	else if (m_difPSize <= cNear)
	{
		// �����_���s����0���o���ꍇ
		// �U��1
		if (m_randomAction == 0)
		{
			// �U�����[�V����������
			m_anim.s_attack = true;

			m_move = VGet(0.0f, 0.0f, 0.0f);

			AttackUpdate("Attack1", 3);

			m_enemyAnim.s_rool = false;
			m_anim.s_moveflag = false;
		}
		// �����_���s����1���o���ꍇ
		// �U��2
		else if (m_randomAction == 1)
		{
			// �U�����[�V����������
			m_anim.s_attack = true;

			m_move = VGet(0.0f, 0.0f, 0.0f);

			AttackUpdate("Attack2", 4);

			// �����ԏI��
			m_avoidnaceNow = false;

			m_enemyAnim.s_rool = false;
			m_anim.s_moveflag = false;
		}
		// �����_���s����2���o���ꍇ
		// �U��3
		else if (m_randomAction == 2)
		{
			// �U�����[�V����������
			m_anim.s_attack = true;

			m_move = VGet(0.0f, 0.0f, 0.0f);

			AttackUpdate("Attack3", 5);

			// �����ԏI��
			m_avoidnaceNow = false;

			m_enemyAnim.s_rool = false;
			m_anim.s_moveflag = false;
		}
		// �����_���s����3���o���ꍇ
		// �U��4
		else if (m_randomAction == 3)
		{
			// �U�����[�V����������
			m_anim.s_attack = true;

			m_move = VGet(0.0f, 0.0f, 0.0f);

			AttackUpdate("Attack4", 6);

			// �����ԏI��
			m_avoidnaceNow = false;

			m_enemyAnim.s_rool = false;
			m_anim.s_moveflag = false;
		}
		// �����_���s����4���o���ꍇ
		// �U��5
		else if (m_randomAction == 4)
		{
			// �U�����[�V����������
			m_anim.s_attack = true;

			m_move = VGet(0.0f, 0.0f, 0.0f);

			AttackUpdate("Attack5", 7);

			// �����ԏI��
			m_avoidnaceNow = false;

			m_anim.s_moveflag = false;
			m_enemyAnim.s_rool = false;
		}
		// �����_���s����5���o���ꍇ
		// ���
		else if (m_randomAction == 5)
		{
			m_enemyAnim.s_rool = true;

			m_nowAnimIdx = m_animIdx["Roll"];
			ChangeAnim(m_nowAnimIdx, m_animOne[8], m_animOne);

			m_anim.s_moveflag = false;
			m_anim.s_attack = false;
		}
		// �����_���s����6�͏o���ꍇ
		else if (m_randomAction == 6)
		{
			// �����ԏI��
			m_avoidnaceNow = false;

			m_enemyAnim.s_rool = false;
			m_anim.s_moveflag = false;
			m_anim.s_attack = false;
		}
	}

	// �U�����̍s��
	if (m_anim.s_attack)
	{
		if (m_randomAction == 0)
		{
			// �U������̃|�W�V����
			InitAttackLigUpdate(attackKnifePos1, attackKnifePos2);

			// �U�����̈ړ����鋗��
			if (m_nowFrame <= cAttackFrame4)
			{
				cAttackMove = cAttackMoveDistance1;
			}
			else
			{
				cAttackMove = 0.0f;
			}

			//�G�t�F�N�g���Đ�����
			if (m_nowFrame >= cAttackFrame7 && m_nowFrame <= cAttackFrame8)
			{
				//�U���̐؂���ɃG�t�F�N�g������
				cEffect.UpdateEffectPosition("BossAttack", VGet(attackKnifePos2.x, attackKnifePos2.y, attackKnifePos2.z));
			}


			if (m_nowFrame == cAttackFrame1)
			{
				InitLigAttack(attackKnifePos1, attackKnifePos2, cAttackRadiusKnife);
				InitAttackDamage(m_status.s_attack);
			}
			// �A�j���[�V�����t���[�����ɍU��������o��
			if (m_nowFrame == cAttackFrame3)
			{
				//�G�t�F�N�g�����
				cEffect.EffectCreate("BossShockWave", VGet(attackKnifePos2.x, attackKnifePos2.y, attackKnifePos2.z));

				// �U��SE�Đ�
				PlaySoundMem(se.GetKnifeSE(), DX_PLAYTYPE_BACK, true);

				InitAttackUpdate(m_status.s_attack);
			}
			if (m_nowFrame == cAttackFrame5)
			{
				InitAttackDamage(m_status.s_attack);
				// ��������Z�b�g
				m_pAttack->CollisionEnd();
				m_pLigAttack->CollisionEnd();
			}
			// �A�j���[�V�����t���[�����ɍU��������o��
			if (m_nowFrame == cAttackFrame7)
			{
				//�G�t�F�N�g�����
				cEffect.EffectCreate("BossAttack", VGet(attackKnifePos2.x, attackKnifePos2.y, attackKnifePos2.z));

				// �U��SE�Đ�
				PlaySoundMem(se.GetKnifeSE(), DX_PLAYTYPE_BACK, true);

				InitAttackUpdate(m_status.s_attack);
			}
			if (m_nowFrame >= cAttackFrame8)
			{
				InitAttackDamage(0.0f);
				// ��������Z�b�g
				m_pAttack->CollisionEnd();
				m_pLigAttack->CollisionEnd();
			}

			//�G�t�F�N�g�����
			if (m_nowFrame >= cAttackFrame3 && m_nowFrame <= cAttackFrame5)
			{
				//�U������̎���ɃG�t�F�N�g������
				cEffect.UpdateEffectPosition("BossShockWave", VGet(attackKnifePos2.x, attackKnifePos2.y, attackKnifePos2.z));
				cEffect.UpdateEffectRotation("BossShockWave", VGet(0.0f, m_angle, 0.0f));
			}

		}
		else if (m_randomAction == 1)
		{
			// �U������̃|�W�V����
			InitAttackLigUpdate(attackKnifePos1, attackKnifePos2);

			// �U�����̈ړ����鋗��
			cAttackMove = 0.0f;

			//�G�t�F�N�g���Đ�����
			if (m_nowFrame >= cAttackFrame3 && m_nowFrame <= cAttackFrame14)
			{
				//�U���̐؂���ɃG�t�F�N�g������
				cEffect.UpdateEffectPosition("BossAttack", VGet(attackKnifePos2.x, attackKnifePos2.y, attackKnifePos2.z));
			}

			if (m_nowFrame == cAttackFrame1)
			{
				InitLigAttack(attackKnifePos1, attackKnifePos2, cAttackRadiusKnife);
				InitAttackDamage(m_status.s_attack);
			}
			// �A�j���[�V�����t���[�����ɍU��������o��
			if (m_nowFrame == cAttackFrame3)
			{
				//�G�t�F�N�g�����
				cEffect.EffectCreate("BossAttack", VGet(attackKnifePos2.x, attackKnifePos2.y, attackKnifePos2.z));

				// �U��SE�Đ�
				PlaySoundMem(se.GetKnifeSE(), DX_PLAYTYPE_BACK, true);

				InitAttackUpdate(m_status.s_attack);
			}
			if (m_nowFrame >= cAttackFrame14)
			{
				InitAttackDamage(0.0f);
				// ��������Z�b�g
				m_pAttack->CollisionEnd();
				m_pLigAttack->CollisionEnd();
			}
		}
		else if (m_randomAction == 2)
		{
			// �U������̃|�W�V����
			InitAttackLigUpdate(attackKnifePos1, attackKnifePos2);

			// �U�����̈ړ����鋗��
			cAttackMove = 0.0f;

			if (m_nowFrame == cAttackFrame1)
			{
				InitLigAttack(attackKnifePos1, attackKnifePos2, cAttackRadiusKnife);
				InitAttackDamage(m_status.s_attack);
			}
			// �A�j���[�V�����t���[�����ɍU��������o��
			if (m_nowFrame == cAttackFrame2)
			{
				//�G�t�F�N�g�����
				cEffect.EffectCreate("BossShockWave", VGet(attackKnifePos1.x, attackKnifePos1.y, attackKnifePos1.z));

				// �U��SE�Đ�
				PlaySoundMem(se.GetKnifeSE(), DX_PLAYTYPE_BACK, true);

				InitAttackUpdate(m_status.s_attack);
			}
			if (m_nowFrame >= cAttackFrame15)
			{
				InitAttackDamage(0.0f);
				// ��������Z�b�g
				m_pAttack->CollisionEnd();
				m_pLigAttack->CollisionEnd();
			}

			//�G�t�F�N�g�����
			if (m_nowFrame >= cAttackFrame2 && m_nowFrame <= cAttackFrame15)
			{
				//�U������̎���ɃG�t�F�N�g������
				cEffect.UpdateEffectPosition("BossShockWave", VGet(attackKnifePos1.x, attackKnifePos1.y, attackKnifePos1.z));
				cEffect.UpdateEffectRotation("BossShockWave", VGet(0.0f, m_angle, 0.0f));
			}

		}
		else if (m_randomAction == 3)
		{
			// �U������̃|�W�V����
			InitAttackLigUpdate(attackLeftKickPos1, attackLeftKickPos2);

			// �U�����̈ړ����鋗��
			if (m_nowFrame <= cAttackFrame16)
			{
				cAttackMove = 0.0f;
			}
			else if (m_nowFrame > cAttackFrame16 && m_nowFrame <= cAttackFrame7)
			{
				cAttackMove = cAttackMoveDistance2;
			}
			else if (m_nowFrame > cAttackFrame7 && m_nowFrame <= cAttackFrame12)
			{
				cAttackMove = cAttackMoveDistance3;
			}
			else if (m_nowFrame > cAttackFrame12 && m_nowFrame <= cAttackFrame13)
			{
				cAttackMove = cAttackMoveDistance2;
			}
			else
			{
				cAttackMove = 0.0f;
			}

			if (m_nowFrame <= cAttackFrame6)
			{
				cLigPos = VGet(attackLeftKickPos1.x, attackLeftKickPos1.y, attackLeftKickPos1.z);
			}
			else if (m_nowFrame >= cAttackFrame6 && m_nowFrame <= cAttackFrame13)
			{
				cLigPos = VGet(attackLeftKickPos2.x, attackLeftKickPos2.y, attackLeftKickPos2.z);
			}

			if (m_nowFrame == cAttackFrame1)
			{
				//�G�t�F�N�g�����
				cEffect.EffectCreate("BossWind", cLigPos);

				InitLigAttack(attackLeftKickPos1, attackLeftKickPos2, cAttackRadiusKick);
				InitAttackDamage(m_status.s_attack);

				//��SE�𗬂�
				PlaySoundMem(se.GetStormSE(), DX_PLAYTYPE_BACK, true);
			}
			else if (m_nowFrame == cAttackFrame2)
			{
				//�G�t�F�N�g�����
				cEffect.EffectCreate("BossWind", cLigPos);
			}
			else if (m_nowFrame == cAttackFrame4)
			{
				//�G�t�F�N�g�����
				cEffect.EffectCreate("BossWind", cLigPos);
			}
			else if (m_nowFrame == cAttackFrame6)
			{
				//�G�t�F�N�g�����
				cEffect.EffectCreate("BossWind", cLigPos);
			}
			else if (m_nowFrame == cAttackFrame8)
			{
				//�G�t�F�N�g�����
				cEffect.EffectCreate("BossWind", cLigPos);
			}

			// �A�j���[�V�����t���[�����ɍU��������o��
			if (m_nowFrame == cAttackFrame11)
			{
				//�G�t�F�N�g�����
				cEffect.EffectCreate("BossWind", cLigPos);

				// �U��SE�Đ�
				PlaySoundMem(se.GetKickSE(), DX_PLAYTYPE_BACK, true);

				InitAttackUpdate(m_status.s_attack);
			}
			if (m_nowFrame >= cAttackFrame13)
			{
				InitAttackDamage(0.0f);
				// ��������Z�b�g
				m_pAttack->CollisionEnd();
				m_pLigAttack->CollisionEnd();
			}

			//�G�t�F�N�g���Đ�����
			if (m_nowFrame >= cAttackFrame1 && m_nowFrame <= cAttackFrame13)
			{
				cEffect.UpdateEffectPosition("BossWind", cLigPos);
			}
		}
		else if (m_randomAction == 4)
		{
			// �U������̃|�W�V����
			InitAttackLigUpdate(attackRightKickPos1, attackRightKickPos2);

			// �U�����̈ړ����鋗��
			if (m_nowFrame <= cAttackFrame3)
			{
				cAttackMove = 0.0f;
			}
			else if (m_nowFrame > cAttackFrame3 && m_nowFrame <= cAttackFrame4)
			{
				cAttackMove = cAttackMoveDistance2;
			}
			else if (m_nowFrame > cAttackFrame6 && m_nowFrame <= cAttackFrame10)
			{
				cAttackMove = cAttackMoveDistance4;
			}
			else
			{
				cAttackMove = 0.0f;
			}

			if (m_nowFrame == cAttackFrame1)
			{
				//�G�t�F�N�g�����
				cEffect.EffectCreate("BossWind", VGet(attackRightKickPos2.x, attackRightKickPos2.y, attackRightKickPos2.z));

				InitLigAttack(attackRightKickPos1, attackRightKickPos2, cAttackRadiusKick);
				InitAttackDamage(m_status.s_attack);
			}
			else if (m_nowFrame == cAttackFrame2)
			{
				//�G�t�F�N�g�����
				cEffect.EffectCreate("BossWind", VGet(attackRightKickPos2.x, attackRightKickPos2.y, attackRightKickPos2.z));

				//��SE�𗬂�
				PlaySoundMem(se.GetStormSE(), DX_PLAYTYPE_BACK, true);
			}

			// �A�j���[�V�����t���[�����ɍU��������o��
			if (m_nowFrame == cAttackFrame4)
			{
				//�G�t�F�N�g�����
				cEffect.EffectCreate("BossWind", VGet(attackRightKickPos2.x, attackRightKickPos2.y, attackRightKickPos2.z));

				// �U��SE�Đ�
				PlaySoundMem(se.GetKickSE(), DX_PLAYTYPE_BACK, true);

				InitAttackUpdate(m_status.s_attack);
			}
			if (m_nowFrame >= cAttackFrame6)
			{
				InitAttackDamage(0.0f);
				// ��������Z�b�g
				m_pAttack->CollisionEnd();
				m_pLigAttack->CollisionEnd();
			}

			//�G�t�F�N�g���Đ�����
			if (m_nowFrame >= cAttackFrame1 && m_nowFrame <= cAttackFrame6)
			{
				cEffect.UpdateEffectPosition("BossWind", VGet(attackRightKickPos2.x, attackRightKickPos2.y, attackRightKickPos2.z));
			}
		}
	}

	if (m_randomAction == 6)
	{
		m_nowAnimIdx = m_animIdx["Idle"];
		ChangeAnim(m_nowAnimIdx, m_animOne[9], m_animOne);

		cAttackMove = 0.0f;
		m_move = VGet(0.0f, 0.0f, 0.0f);
		cAvoidanceMove = 0.0f;

		if (m_nowFrame >= cIdleFrameDuration)
		{
			m_isAnimationFinish = true;
		}
	}

	// �A�j���[�V�������I���x�Ƀ����_���ȍs�����s��
	if (m_isAnimationFinish)
	{
		// �v���C���[�������Ă��鎞
		if (cPlayerLook)
		{
			m_randomAction = GetRand(cRandomActionMax);
		}
		// �v���C���[�������Ă��Ȃ���
		else
		{
			// �������悤�ɂ���
			m_randomAction = cActionFixed;
		}
	}

	// �ړ�����
	m_moveVec = MyLibrary::LibVec3(m_move.x, m_move.y, m_move.z);
}

void Assassin::Draw(EnemyWeapon& weapon)
{
	//�����蔻����W���擾���ă��f���̕`����W��ݒ肷��
	SetDrawModelPos(cModelPosY);
	//3D���f���̉�]�n���Z�b�g����
	MV1SetRotationXYZ(m_modelHandle, VGet(0.0f, m_angle, 0.0f));
	weapon.Draw();
	//���f���̕`��
	MV1DrawModel(m_modelHandle);

#if false
	DrawFormatString(1000, 500, 0xffffff, "move : %d", cA[0]);
	DrawFormatString(1000, 550, 0xffffff, "attack : %d", cA[1]);
	DrawFormatString(1000, 600, 0xffffff, "avoidance : %d", cA[2]);
	DrawFormatString(1000, 650, 0xffffff, "hit : %d", cA[3]);
	DrawFormatString(1000, 700, 0xffffff, "random : %d", m_randomAction);
#endif
}

/// <summary>
/// UI�`��
/// </summary>
/// <param name="ui"></param>
void Assassin::DrawUI(UI& ui)
{
	if (m_isBossDiscovery && !cDead)
	{
		ui.BossHPDraw(m_status.s_hp, m_maxHP, m_bossName, m_subName);
	}
}
