#include "Player.h"
#include "Manager/EffectManager.h"
#include "Manager/HandleManager.h"
#include "Manager/EnemyManager.h"
#include "Manager/CoreManager.h"
#include "External/CsvLoad.h"
#include "Item/Weapon.h"
#include "Item/Shield.h"
#include "Item/Armor.h"
#include "Item/Tool.h"
#include "Manager/GameManager.h"
#include "Manager/SEManager.h"

#include "State/Player/PlayerStateIdle.h"
#include "State/Player/PlayerStateTaking.h"
#include "Input/Input.h"

#include <cassert>

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
	constexpr float cWalkSpeed = 1.5f;
	//�_�b�V���ɂ��������鑬�x
	constexpr float cDashSpeed = 4.0f;
	//�������[�V�����̃v���C�^�C��
	constexpr float cAnimWalkTime = 0.5f;
	//�����A�j���[�V�����̃g�[�^���^�C��
	constexpr float cAnimWalkReverseTimeInit = 35.0f;
	//�{�^����������Ă��邩�̊m�F�p�ϐ�
	int cAbutton = 0;
	int cRbutton = 0;
	bool cRstickButton = false;
	//�X�e�B�b�N�̓��͂𓾂�
	int cAnX = 0;
	int cAnY = 0;
	//����ł̈ړ�����
	float cAvoidanceMove = 0.0f;
	//����̕������������
	bool cOneAvoidance = false;
	//�U���ł̈ړ�����
	float cAttackMove = 0.0f;
	//�U���ł̒ǉ��U������
	int cAddAttackTime = 0;
	//���̍U���͈�
	constexpr float cFistAttackRadius = 25.0f;
	//�U���̔���͈�
	constexpr float cPartAttackRadius = 16.0f;
	//���U���̍U���͈�
	constexpr float cStrengthAttackRadius = 60.0f;
	//���̕�
	constexpr float cShieldWidth = 40.0f;
	//���̉�
	constexpr float cShieldHight = 120.0f;
	//���̉��s
	constexpr float cShieldDepht = 20.0f;
	//���̍��G�͈�
	constexpr float cShieldSearchRadius = 100.0f;
	//���݂̃A�^�b�N�̃i���o�[������
	int cNowAttackNumber = 0;
	//�U���̏I������
	int cIsEndAttack = 0;
	//�����̍U���͏���
	float cEquipmentAttack = 0.0f;
	//�G����̃_���[�W�擾
	float cDamage = 0.0f;
	//�l�`�̃��f���p�X
	constexpr const char* cPath = "Data/Player/PlayerModelPuppet.mv1";
	//��������񂾂�����������
	bool cEquipmentOne = false;
	//���̒��ɓ��鎞�̃|�W�V�����ݒ�
	bool cEnterPos = false;

	//�V���O���g��
	auto& handle = HandleManager::GetInstance();
	auto& cEffect = EffectManager::GetInstance();

	constexpr float cStaminaUnder = -10.0f;       //�X�^�~�i�Œ�l
	constexpr float cStaminaUnderTo = -9.0f;      //�X�^�~�i�Œ�l����̈ړ�


	int cAnimIdx;

	bool cAngle = false;
	bool cJumpOne = false;
	bool cRollOne = false;

	bool cHit = false;         //�U����̂Ɏ󂯂�Ƃ��̔���
	bool cShieldHit = false;   //�U�������Ɏ󂯂�Ƃ��̔���

	//�������d�͂�^����͈�
	constexpr float cGravity = 20.0f;
}

Player::Player() :
	CharacterBase(Collidable::Priority::Low, ObjectTag::Player),
	m_attackMove(VGet(0.0f,0.0f,0.0f)),
	m_pEnemyAttackCol(),
	m_staminaBreak(false),
	ms_maxStatus(),
	m_xpad(),
	m_attackNumber(0),
	m_mapNow(0),
	m_searchRadius(0.0f),
	m_updateX(0.0f),
	m_updateY(0.0f),
	m_updateZ(0.0f),
	m_attackDamage(0.0f),
	m_equipmentMuscle(0.0f),
	m_equipmentSkill(0.0f),
	m_menuOpen(false),
	m_restTouch(false),
	m_rest(false),
	m_itemPick(false),
	m_bigRest(false),
	m_lockonTarget(false),
	m_warp(false),
	m_bossStart(false),
	m_moveAnimFrameIndex(0),
	m_moveAnimFrameRight(0),
	m_attackLig1(0),
	m_attackLig2(0),
	m_legAttackLig1(0),
	m_legAttackLig2(0),
	m_moveAnimShieldFrameIndex(0),
	m_moveAnimShieldFrameHandIndex(0),
	m_lockAngle(0.0f),
	m_avoidanceNow(false),
	m_shieldOne(false),
	m_deadReset(false),
	m_message(false),
	m_read(false),
	m_moveWeaponFrameMatrix(),
	m_moveShieldFrameMatrix(),
	m_rollMove(VGet(0.0f,0.0f,0.0f)),
	m_moveVector(VGet(0.0f,0.0f,0.0f)),
	m_attackLigPos1(VGet(0.0f,0.0f,0.0f)),
	m_attackLigPos2(VGet(0.0f, 0.0f, 0.0f)),
	m_attackLegLigPos1(VGet(0.0f,0.0f,0.0f)),
	m_attackLegLigPos2(VGet(0.0f,0.0f,0.0f)),
	m_shieldPos(),
	m_shieldSize(),
	m_shieldSearchPos(),
	m_notRoll(0),
	m_start(0.0f),
	m_reset(0.0f),
	m_loop(false),
	m_attackStrong(false),
	m_jumpAttack(false),
	m_attackInit(false),
	m_attackEnd(false),
	m_action(true)
{

	//�J�v�Z���^
	auto collider = Collidable::AddCollider(MyLibrary::CollidableData::Kind::Capsule, false);
	auto capsuleCol = dynamic_cast<MyLibrary::CollidableDataCapsule*>(collider.get());
	capsuleCol->m_len = 50.0f;
	capsuleCol->m_radius = 12.0f;
	capsuleCol->m_vec = MyLibrary::LibVec3(0.0f, capsuleCol->m_vec.y + 2.0f, 0.0f);

	m_capsuleY = capsuleCol->m_vec.y;
	m_capsuleRadius = capsuleCol->m_radius;

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
	m_animChange.sa_bossEnter = false;
	m_animChange.sa_shieldIdle = false;
	m_animChange.sa_enterShield = false;
	m_animChange.sa_imapact = false;
	m_animChange.sa_strengthAttack = false;
	
	//�L�����N�^�[����ݒ�
	m_characterName = "Player";

	//���f���ǂݍ���
	m_modelHandle = handle.GetModelHandle(cPath);

	//���f���̃T�C�Y�ݒ�
	MV1SetScale(m_modelHandle, VGet(cModelSizeScale, cModelSizeScale, cModelSizeScale));

	//Csv�ɂ��f�[�^�̓ǂݍ���
	CsvLoad::GetInstance().AnimDataLoad("Player", m_animIdx);
	CsvLoad::GetInstance().StatusLoad(m_status, "Player");

	for (int i = 0; i < 10; i++)
	{
		m_armorOne[i] = false;
	}

	cEquipmentOne = false;
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
void Player::Init(std::shared_ptr<MyLibrary::Physics> physics, GameManager* manager, Weapon& weapon, Shield& shield, Armor& armor, bool anim, int colData)
{
	m_pPhysics = physics;

	//�����蔻��̒ǉ�
	Collidable::Init(m_pPhysics);

	//�v���C���[�̏����ʒu�ݒ�
	rigidbody->Init(true);
	if (manager->GetThisMapName() == 0)
	{
		m_updateX = -200.0f;
		m_updateY = 12.0f;
		m_updateZ = 350.0f;
	}
	else if (manager->GetThisMapName() == 1)
	{
		m_updateX = 485.0f;
		m_updateY = 12.0f;
		m_updateZ = -800.0f;
	}
	else if (manager->GetThisMapName() == 6)
	{
		m_updateX = -1850.0f;
		m_updateY = 12.0f;
		m_updateZ = 0.0f;
	}
	
	rigidbody->SetPos(MyLibrary::LibVec3(m_updateX, m_updateY, m_updateZ));
	rigidbody->SetNextPos(rigidbody->GetPos());
	rigidbody->SetVec(MyLibrary::LibVec3(0.0f, 40.0f, 0.0f));
	m_collisionPos = rigidbody->GetPos();
	SetModelPos();
	MV1SetPosition(m_modelHandle, m_modelPos.ConversionToVECTOR());

	m_searchRadius = 200.0f;

	//���j���[�����
	m_menuOpen = false;

	//���������ꍇ
	m_attackRadius = cFistAttackRadius;

	m_pStrengthAttack = std::make_shared<AttackObject>(cStrengthAttackRadius);
	m_pLigAttack = std::make_shared<AttackLigObject>(MyLibrary::LibVec3(m_attackLigPos1.x, m_attackLigPos1.y, m_attackLigPos1.z), MyLibrary::LibVec3(m_attackLigPos2.x, m_attackLigPos2.y, m_attackLigPos2.z), cFistAttackRadius);
	m_pLigLegAttack = std::make_shared<AttackLigObject>(MyLibrary::LibVec3(m_attackLegLigPos1.x, m_attackLegLigPos1.y, m_attackLegLigPos1.z), MyLibrary::LibVec3(m_attackLegLigPos2.x, m_attackLegLigPos2.y, m_attackLegLigPos2.z), cFistAttackRadius);

	m_pSearch = std::make_shared<PlayerSearchObject>(m_searchRadius);
	m_pSearch->Init(m_pPhysics, rigidbody->GetPos());

	m_shieldSize = MyLibrary::LibVec3::Size(cShieldWidth, cShieldHight, cShieldDepht);

	m_pShield = std::make_shared<ShieldObject>(cShieldWidth, cShieldHight, cShieldDepht);
	m_pShieldSearch = std::make_shared<PlayerSearchObject>(cShieldSearchRadius);

	//�X�e�[�g�p�^�[���̏�����
	m_pState = std::make_shared<PlayerStateIdle>(std::dynamic_pointer_cast<Player>(shared_from_this()));
	m_pState->ChangeState(StateBase::StateKind::Idle);
	m_pState->Init(colData);

	//�U�����������������
	m_attackNumber = 0;
	m_jumpAttack = false;
	m_attackStrong = false;

	//HP�̍ő�񕜗�
	m_maxHeel = 80;

	m_staminaBreak = false;

	//�U���Ŏ��񂾎��q�b�g���������
	m_anim.s_hit = false;

	//����������
	if (!cEquipmentOne)
	{
		weapon.SetFist(true);
		shield.SetFist(true);
		armor.SetBody(true);

		cEquipmentOne = true;
	}

	m_armorOne[0] = true;
}

/// <summary>
/// �Q�[���̎d�l��ŏ���������
/// </summary>
/// <param name="physics"></param>
void Player::GameInit(std::shared_ptr<MyLibrary::Physics> physics, int colData)
{
	m_pPhysics = physics;

	m_searchRadius = 200.0f;

	//����ł����ꍇ
	if (m_anim.s_isDead)
	{
		Collidable::Init(m_pPhysics);
		m_pSearch->Init(m_pPhysics, rigidbody->GetPos());

		m_anim.s_isDead = false;
	}

	CsvLoad::GetInstance().StatusLoad(m_status, "Player");

	//�v���C���[�̏����ʒu�ݒ�
	rigidbody->Init(true);
	rigidbody->SetPos(MyLibrary::LibVec3(m_updateX, m_updateY, m_updateZ));
	rigidbody->SetNextPos(rigidbody->GetPos());
	rigidbody->SetVec(MyLibrary::LibVec3(0.0f, 40.0f, 0.0f));
	m_collisionPos = rigidbody->GetPos();
	SetModelPos();
	MV1SetPosition(m_modelHandle, m_modelPos.ConversionToVECTOR());

	//�X�e�[�g�p�^�[���̏�����
	m_pState = std::make_shared<PlayerStateIdle>(std::dynamic_pointer_cast<Player>(shared_from_this()));
	m_pState->ChangeState(StateBase::StateKind::Idle);
	m_pState->Init(colData);

	//�U�����������������
	m_attackNumber = 0;
	m_jumpAttack = false;
	m_attackStrong = false;

	m_deadReset = false;
	m_staminaBreak = false;

	//�U���Ŏ��񂾎��q�b�g���������
	m_anim.s_hit = false;

	//HP�̍ő�񕜗�
	m_maxHeel = 80;

}

void Player::Finalize()
{
	Collidable::Finalize(m_pPhysics);
	m_pSearch->Finalize(m_pPhysics);
}

void Player::Update(Weapon& weapon, Shield& shield, Armor& armor, EnemyManager& enemy, CoreManager& core, VECTOR restpos, Tool& tool, SEManager& se, bool boss, bool dead, std::shared_ptr<MyLibrary::Physics> physics)
{
	//�������C��
	//�}�b�v���x���n�_��������
	if (m_mapNow == 0)
	{
		//�W�����v���Ă��Ȃ��ꍇ
		if (rigidbody->GetPos().y < cGravity)
		{
			//�d�͂��Ȃ��������Ƃɂ���
			rigidbody->SetUseGravity(false);
		}
		else
		{
			//�d�͂�^����
			rigidbody->SetUseGravity(true);
		}
		
	}
	else
	{
		//�d�͂�^����
		rigidbody->SetUseGravity(true);
	}


	//�^�[�Q�b�g��������
	m_pState->SetTarget(m_lockonTarget);
	//���b�N�I�����A���O����ς���
	if (m_lockonTarget && m_pState->GetState() != StateBase::StateKind::Roll)
	{
		if (m_pState->GetState() == StateBase::StateKind::StrongAttack)
		{
			//��񂾂��␳����
			if (!cAngle)
			{
				m_angle = m_lockAngle;

				cAngle = true;
			}
		}
		else
		{
			cAngle = false;

			m_angle = m_lockAngle;
		}
		
	}

	//����łȂ��Ƃ��̃X�e�[�g�̍X�V
	if (!m_anim.s_isDead)
	{
		//�A�N�V�����ł��Ȃ�����
		if (m_action)
		{
			//�X�e�[�g�̍X�V
			m_pState->Update();
		}
		//�A�N�V�����ł���悤�ɂ���
		else if (!m_action)
		{
			//�����������A�N�V�����ł���悤�ɂ���
			if (Input::GetInstance().IsReleased("Input_Roll"))
			{
				m_action = true;
			}
		}
		
	}

	if (m_pState->GetState() == StateBase::StateKind::Jump)
	{
		//�W�����v�U�����\�ɂ���
		m_jumpCan = true;
	}
	else
	{
		//�W�����v�U�����o���Ȃ�����
		m_jumpCan = false;
	}

	//�Ƃ肠��������Ƃ�
	m_status.s_core = core.GetCore();
	m_levelStatus.sl_all = m_levelStatus.sl_hp + m_levelStatus.sl_muscle + m_levelStatus.sl_skill + m_levelStatus.sl_stamina;

	//�A�j���[�V�����ňړ����Ă���t���[���̔ԍ�����������
	m_moveAnimFrameIndex = MV1SearchFrame(m_modelHandle, "mixamorig:Hips");
	m_moveAnimFrameRight = MV1SearchFrame(m_modelHandle, "mixamorig:RightHandThumb2");

	//���������Ă��Ȃ����̃��O
	if (weapon.GetFist())
	{
		m_attackLig1 = MV1SearchFrame(m_modelHandle, "mixamorig:RightForeArm");
		m_attackLig2 = MV1SearchFrame(m_modelHandle, "mixamorig:RightHandThumb4");

		m_attackLigPos1 = MV1GetFramePosition(m_modelHandle, m_attackLig1);
		m_attackLigPos2 = MV1GetFramePosition(m_modelHandle, m_attackLig2);
	}
	//���������Ă��鎞�̃��O
	else if (!weapon.GetFist())
	{
		m_attackLigPos1 = weapon.WeaponPattern();
		m_attackLigPos2 = weapon.WeaponTip();
	}

	//���̃��O�����
	m_legAttackLig1 = MV1SearchFrame(m_modelHandle, "mixamorig:RightUpLeg");
	m_legAttackLig2 = MV1SearchFrame(m_modelHandle, "mixamorig:RightToe_End");

	m_attackLegLigPos1 = MV1GetFramePosition(m_modelHandle, m_legAttackLig1);
	m_attackLegLigPos2 = MV1GetFramePosition(m_modelHandle, m_legAttackLig2);

	//�����\���鎞�̃A�j���[�V�����t���[���擾
	m_moveAnimShieldFrameIndex = MV1SearchFrame(m_modelHandle, "mixamorig:LeftShoulder");
	m_moveAnimShieldFrameHandIndex = MV1SearchFrame(m_modelHandle, "mixamorig:LeftHand");

	//����⏂���A�^�b�`����t���[���̃��[�J�������[���h�ϊ��s����擾����
	m_moveWeaponFrameMatrix = MV1GetFrameLocalWorldMatrix(m_modelHandle, m_moveAnimFrameRight);
	m_moveShieldFrameMatrix = MV1GetFrameLocalWorldMatrix(m_modelHandle, m_moveAnimShieldFrameHandIndex);

	//�p�b�h���͎擾
	GetJoypadXInputState(DX_INPUT_KEY_PAD1, &m_xpad);

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
		MV1SetAttachAnimBlendRate(m_modelHandle, m_currentAnimNo, m_animBlendRate);

	}

	//���񂾎��̃A�j���[�V����
	if (m_status.s_hp <= 0)
	{
		if (!m_anim.s_isDead)
		{
			m_anim.s_isDead = true;

			m_pState->ChangeState(StateBase::StateKind::Death);

			m_lockonTarget = false;

			Finalize();
		}
	}

	//���S�A�j���[�V�������I��������
	if (m_anim.s_isDead && m_nowFrame >= 68.0f)
	{
		m_deadReset = true;
	}

	//���̍��G�̃|�W�V�����X�V
	m_shieldSearchPos = MyLibrary::LibVec3(rigidbody->GetPos().x + sinf(m_angle) * -100.0f, rigidbody->GetPos().y + 15.0f, rigidbody->GetPos().z - cosf(m_angle) * 100.0f);


	//�v���C���[�������Ă��鎞����
	if (!m_anim.s_isDead)
	{
		//���̔���
		if (m_pState->GetState() == StateBase::StateKind::Guard || m_animChange.sa_imapact)
		{
			if (!m_shieldOne)
			{
				m_pShield->Init(m_pPhysics, m_shieldPos);
				m_pShieldSearch->Init(m_pPhysics, m_shieldSearchPos);
				m_shieldOne = true;
			}

			//���ŃK�[�h�ł���G
			if (m_pShieldSearch->GetIsStay())
			{
				//���̓����蔻��
				if (m_pShield->GetIsStay())
				{
					m_animChange.sa_imapact = true;
					cHit = false;
					cShieldHit = false;
				}
			}

		}
		else
		{
			m_pShield->CollisionEnd();
			m_pShieldSearch->CollisionEnd();
			m_shieldOne = false;
		}

		//�_���[�W��H�炤����
		if (cHit && !m_animChange.sa_imapact)
		{
			//�q�b�g�A�j���[�V�����ɂ���
			m_anim.s_hit = true;

			//�_���[�W���󂯂�A�j���[�V����
			m_pState->ChangeState(StateBase::StateKind::Damage);

			for (auto damage : enemy.GetEnemyDamage())
			{
				if (damage > 0)
				{
					m_status.s_hp -= damage - (m_status.s_defense / 10);
				}
			}
			//Hit�G�t�F�N�g
			cEffect.EffectCreate("PlayerHit", VGet(rigidbody->GetPos().x, rigidbody->GetPos().y + 20.0f, rigidbody->GetPos().z));
			//HitSe�Đ�
			PlaySoundMem(se.GetPlayerHitSE(), DX_PLAYTYPE_BACK, true);

			cHit = false;
		}
		//���󂯂����Ƃ��̏���
		else if (!cShieldHit && m_animChange.sa_imapact)
		{
			//���󂯃A�j���[�V�����ɂ���
			m_anim.s_hit = false;

			//�_���[�W���󂯂�A�j���[�V����
			m_pState->ChangeState(StateBase::StateKind::Damage);

			for (auto damage : enemy.GetEnemyDamage())
			{
				if (damage > 0 && shield.GetUgly())
				{
					m_status.s_stamina -= damage - (shield.GetStrengthUgly() / 10);
				}
				else if (damage > 0 && shield.GetWood())
				{
					m_status.s_stamina -= damage - (shield.GetStrengthWood() / 10);
				}
			}

			//GuardSe�Đ�
			PlaySoundMem(se.GetGuardSE(), DX_PLAYTYPE_BACK, true);

			cShieldHit = true;
		}

		//�X�^�~�i�����l�ȉ��ɂȂ����猸��Ȃ��悤�ɂ���
		if (m_status.s_stamina <= cStaminaUnder)
		{
			m_status.s_stamina = cStaminaUnderTo;
		}

		//���j���[���J���Ă���Ԃ̓A�N�V�����ł��Ȃ�
		if (!m_menuOpen)
		{
			//�A�N�V�������ł��Ȃ�����
			if (!m_animChange.sa_avoidance && !m_anim.s_hit && !m_animChange.sa_recovery && !m_animChange.sa_bossEnter && !m_animChange.sa_imapact
				&& !m_rest && !m_animChange.sa_strengthAttack)
			{
				Action(restpos, tool, shield, se, boss, dead);
			}
		}

		//���������Ă��Ȃ����̃A�j���[�V����
		if (weapon.GetFist() && shield.GetFist())
		{
			//���������Ă���A�j���[�V�������𔻒肷��
			m_fist = true;
			m_sword = false;
			m_equipment = false;
			m_shield = false;
		}
		//���������Ƃ��̃A�j���[�V����
		else if (!weapon.GetFist() || !shield.GetFist())
		{
			//���������Ă���A�j���[�V�������𔻒肷��
			m_equipment = true;

			//�������킩�𔻒f����
			if (!weapon.GetFist())
			{
				m_sword = true;
				m_fist = false;
			}
			else
			{
				m_sword = false;
				m_fist = true;
			}
			
			//���𔻒f����
			if (!shield.GetFist())
			{
				m_shield = true;
			}
			else
			{
				m_shield = false;
			}
		}

		//�h������Ă��Ȃ����̏���
		if (armor.GetBody())
		{
			ArmorChange(0, cPath);	
		}
		//�h����������̏���
		else if (!armor.GetBody())
		{
			ArmorChange(1, armor.GetPath());	
		}
		
	}
	//���S���ĂȂ����̃A�j���[�V�����X�V
	if (!m_anim.s_isDead)
	{
		//�A�j���[�V�����̍X�V
		m_isAnimationFinish = UpdateStateAnim(m_currentAnimNo, m_start, m_reset, m_loop);
	}
	//���S�����Ƃ��̃A�j���[�V�����X�V
	else if (m_anim.s_isDead && m_nowFrame <= 68.0f)
	{
		m_isAnimationFinish = UpdateStateAnim(m_currentAnimNo);
	}

	//�v���C���[�̃|�W�V����������
	SetModelPos();
	
	//�����̍X�V
	weapon.Update(m_moveWeaponFrameMatrix);
	shield.Update(m_moveShieldFrameMatrix);

	//�����ł̃X�e�[�^�X�㏸���v���X����
	if (weapon.GetFist())
	{
		cEquipmentAttack = 0.0f;
		m_equipmentMuscle = 0.0f;
		m_equipmentSkill = 0.0f;
	}
	//�������X�e�[�^�X
	else if (weapon.GetBlack())
	{
		cEquipmentAttack = weapon.GetBlackAttack();
		m_equipmentMuscle = weapon.GetBlackMuscle();
		m_equipmentSkill = weapon.GetBlackSkill();
	}
	//�؂̃o�b�g�X�e�[�^�X
	else if (weapon.GetBat())
	{
		cEquipmentAttack = weapon.GetBatAttack();
		m_equipmentMuscle = weapon.GetBatMuscle();
		m_equipmentSkill = weapon.GetBatSkill();
	}

	if (armor.GetBody())
	{
		m_status.s_defense = 0.0f;
	}
	else if (armor.GetCommon())
	{
		m_status.s_defense = armor.GetCommonDefence();
	}

	//�X�^�~�i��
	if (m_pState->GetState() != StateBase::StateKind::Attack && m_pState->GetState() != StateBase::StateKind::Damage && m_pState->GetState() != StateBase::StateKind::Dash &&
		m_pState->GetState() != StateBase::StateKind::Guard && m_pState->GetState() != StateBase::StateKind::Jump && m_pState->GetState() != StateBase::StateKind::Roll &&
		m_pState->GetState() != StateBase::StateKind::StrongAttack && m_pState->GetState() != StateBase::StateKind::Damage && m_pState->GetState() != StateBase::StateKind::Item)
	{
		//�X�^�~�i�̍ő�l�������Ɖ񕜂���
		if (m_status.s_stamina < ms_maxStatus.sm_stamina)
		{
			m_status.s_stamina += 0.5f;
		}
	}
	//�����\������ԂƃA�C�e���g�p��Ԃ͉񕜂��x���Ȃ�
	else if (m_pState->GetState() == StateBase::StateKind::Guard || m_pState->GetState() == StateBase::StateKind::Item)
	{
		if (m_status.s_stamina < ms_maxStatus.sm_stamina)
		{
			m_status.s_stamina += 0.2f;
		}
	}

	//�X�^�~�i�؂�
	if (m_status.s_stamina <= 0.0f)
	{
		m_staminaBreak = true;
		m_animChange.sa_dashMove = false;
		m_status.s_speed = cWalkSpeed;
	}
	else if (m_status.s_stamina >= 50.0f)
	{
		m_staminaBreak = false;
	}

	//����̃|�W�V�����X�V
	MyLibrary::LibVec3 centerPos = rigidbody->GetPos();
	MyLibrary::LibVec3 attackPos = MyLibrary::LibVec3(rigidbody->GetPos().x + sinf(m_angle) * -25.0f, rigidbody->GetPos().y + 15.0f, rigidbody->GetPos().z - cosf(m_angle) * 25.0f);
	MyLibrary::LibVec3 ligAttackPos1 = MyLibrary::LibVec3(m_attackLigPos1.x, m_attackLigPos1.y, m_attackLigPos1.z);
	MyLibrary::LibVec3 ligAttackPos2 = MyLibrary::LibVec3(m_attackLigPos2.x, m_attackLigPos2.y, m_attackLigPos2.z);
	MyLibrary::LibVec3 LegLigAttackPos1 = MyLibrary::LibVec3(m_attackLegLigPos1.x, m_attackLegLigPos1.y, m_attackLegLigPos1.z);
	MyLibrary::LibVec3 LegLigAttackPos2 = MyLibrary::LibVec3(m_attackLegLigPos2.x, m_attackLegLigPos2.y, m_attackLegLigPos2.z);
	MyLibrary::LibVec3 StrengthAttackPos = MyLibrary::LibVec3(m_attackLigPos2.x, m_attackLigPos2.y, m_attackLigPos2.z);
	m_shieldPos = MyLibrary::LibVec3(rigidbody->GetPos().x + sinf(m_angle) * -15.0f, rigidbody->GetPos().y + 25.0f, rigidbody->GetPos().z - cosf(m_angle) * 15.0f);

	//sin��X����width�̃T�C�Y���o��
	if (sinf(m_angle) > 0)
	{
		m_shieldSize.width = 30.0f + sinf(m_angle) * -15.0f;
	}
	else if (sinf(m_angle) < 0)
	{
		m_shieldSize.width = 30.0f - sinf(m_angle) * -15.0f;
	}

	//cos��Z����depth�T�C�Y���o��
	if (cosf(m_angle) > 0)
	{
		m_shieldSize.depth = 30.0f + cosf(m_angle) * -15.0f;
	}
	else if (cosf(m_angle) < 0)
	{
		m_shieldSize.depth = 30.0f - cosf(m_angle) * -15.0f;
	}

	//���󂯏I��
	if (m_animChange.sa_imapact && m_isAnimationFinish)
	{
		m_animChange.sa_imapact = false;
	}


	//����̍X�V
	m_pSearch->Update(centerPos);
	m_pLigAttack->Update(ligAttackPos1, ligAttackPos2);
	m_pLigLegAttack->Update(LegLigAttackPos1, LegLigAttackPos2);
	m_pStrengthAttack->Update(StrengthAttackPos);
	m_pShield->Update(m_shieldPos, m_shieldSize);
	m_pShieldSearch->Update(m_shieldSearchPos);

	//���ݒ�
	if (m_anim.s_hit)
	{
		//�U�����胊�Z�b�g
		m_pLigAttack->CollisionEnd();
		m_pLigLegAttack->CollisionEnd();
	}

	//���݂��I��点��
	if (m_anim.s_hit && m_isAnimationFinish)
	{
		m_anim.s_hit = false;
	}

	//���������̃X�^�~�i����
	if (m_pState->GetState() == StateBase::StateKind::Dash)
	{
		m_status.s_stamina -= 0.1f;
	}
	//�W�����v�������̃X�^�~�i����
	if (rigidbody->GetJump())
	{
		//��񂾂����s
		if (!cJumpOne)
		{
			m_status.s_stamina -= 20.0f;

			cJumpOne = true;
		}
	}
	else
	{
		cJumpOne = false;
	}
	//����ɂ��X�^�~�i����
	if (m_pState->GetState() == StateBase::StateKind::Roll)
	{
		//��񂾂����s
		if (!cRollOne)
		{
			m_status.s_stamina -= 20.0f;

			cRollOne = true;
		}
	}
	else
	{
		cRollOne = false;
	}

	//�U���͊i�[
	m_attackDamage = (m_status.s_attack + (m_status.s_muscle * 0.5f) + (m_status.s_skill * 0.5f)) + (cEquipmentAttack + ((m_status.s_muscle - 1) * m_equipmentMuscle) + ((m_status.s_skill - 1) * m_equipmentSkill));

	//�U�������Ă��鎞�܂Ƃ�
	//�U����i�K��
	if (m_attackNumber == 1)
	{
		m_pLigAttack->SetAttack(m_attackDamage);

		if (m_attackInit)
		{
			cEffect.EffectCreate("PlayerAttack", m_attackLigPos2);

			//�U��SE�Đ�
			PlaySoundMem(se.GetAttackSE(), DX_PLAYTYPE_BACK, true);

			m_status.s_stamina -= 25.0f;
			m_pLigAttack->Init(m_pPhysics);

			m_attackInit = false;
		}
		else if (m_attackEnd)
		{
			//��������Z�b�g
			m_pLigAttack->CollisionEnd();

			m_attackEnd = false;
		}

		//�G�t�F�N�g���Đ�����
		if (m_nowFrame >= 25.0f && m_nowFrame <= 35.0f)
		{
			//�؂���ɃG�t�F�N�g������
			cEffect.UpdateEffectPosition("PlayerAttack", m_attackLigPos2);
			cEffect.UpdateEffectRotation("PlayerAttack", VGet(0.0f, m_angle - (DX_PI_F / 2.0f), 0.0f));

		}
	}
	//�U��2�i�K��
	else if(m_attackNumber == 2)
	{
		m_pLigAttack->SetAttack((m_attackDamage) * 1.1);

		//�U�����蔭���t���[��
		if (m_attackInit)
		{
			cEffect.EffectCreate("PlayerAttack", m_attackLigPos2);


			//�U��SE�Đ�
			PlaySoundMem(se.GetAttackSE(), DX_PLAYTYPE_BACK, true);

			m_status.s_stamina -= 25.0f;
			m_pLigAttack->Init(m_pPhysics);

			m_attackInit = false;
		}
		else if (m_attackEnd)
		{
			//�U�����胊�Z�b�g
			m_pLigAttack->CollisionEnd();

			m_attackEnd = false;
		}

		//�G�t�F�N�g���Đ�����
		if (m_nowFrame >= 55.0f && m_nowFrame <= 65.0f)
		{
			//�؂���ɃG�t�F�N�g������
			cEffect.UpdateEffectPosition("PlayerAttack", m_attackLigPos2);
			cEffect.UpdateEffectRotation("PlayerAttack", VGet(0.0f, m_angle - (DX_PI_F / 2.0f), 0.0f));

		}
	}
	//�U��3�i�K��
	else if (m_attackNumber == 3)
	{
		m_pLigAttack->SetAttack((m_attackDamage) * 1.2);

		//�U�����蔭���t���[��
		if (m_attackInit)
		{
			cEffect.EffectCreate("PlayerAttack", m_attackLigPos2);

			//�U��SE�Đ�
			PlaySoundMem(se.GetAttackSE(), DX_PLAYTYPE_BACK, true);

			m_status.s_stamina -= 25.0f;
			m_pLigAttack->Init(m_pPhysics);

			m_attackInit = false;
		}
		else if (m_attackEnd)
		{
			//�U�����胊�Z�b�g
			m_pLigAttack->CollisionEnd();

			m_attackEnd = false;
		}

		//�G�t�F�N�g���Đ�����
		if (m_nowFrame >= 85.0f && m_nowFrame <= 95.0f)
		{
			//�؂���ɃG�t�F�N�g������
			cEffect.UpdateEffectPosition("PlayerAttack", m_attackLigPos2);
			cEffect.UpdateEffectRotation("PlayerAttack", VGet(0.0f, m_angle - (DX_PI_F / 2.0f), 0.0f));

		}
	}


	//�W�����v�U������������
	if (m_jumpAttack)
	{
		m_pLigAttack->SetAttack((m_attackDamage) * 1.8);

		//�U�����蔭���t���[��
		if (m_nowFrame == 15.0f)
		{
			//�G�t�F�N�g�����
			cEffect.EffectCreate("PlayerAttack", m_attackLigPos2);

			m_status.s_stamina -= 50.0f;
			m_pLigAttack->Init(m_pPhysics);
		}
		else if (m_nowFrame >= 20.0f)
		{
			//�U�����胊�Z�b�g
			m_pLigAttack->CollisionEnd();
		}

		//�G�t�F�N�g���Đ�����
		if (m_nowFrame >= 15.0f && m_nowFrame <= 20.0f)
		{
			//�؂���ɃG�t�F�N�g������
			cEffect.UpdateEffectPosition("PlayerAttack", m_attackLigPos2);
			cEffect.UpdateEffectRotation("PlayerAttack", VGet(0.0f, m_angle - (DX_PI_F / 2.0f), 0.0f));
		}
	}
	//�����U������������
	else if (m_attackStrong)
	{
		//����������������
		if (m_fist)
		{
			m_pLigLegAttack->SetAttack((m_attackDamage) * 1.5);

			//�U�����蔭���t���[��
			if (m_nowFrame == 35.0f)
			{
				//�G�t�F�N�g�����
				cEffect.EffectCreate("PlayerShockWave", m_attackLegLigPos2);

				m_status.s_stamina -= 40.0f;
				m_pLigLegAttack->Init(m_pPhysics);
			}
			else if (m_nowFrame >= 45.0f)
			{
				//�U�����胊�Z�b�g
				m_pLigLegAttack->CollisionEnd();
			}

			//�G�t�F�N�g���Đ�����
			if (m_nowFrame >= 35.0f && m_nowFrame <= 45.0f)
			{
				cEffect.UpdateEffectPosition("PlayerShockWave", m_attackLegLigPos2);
				cEffect.UpdateEffectRotation("PlayerShockWave", VGet(0.0f, m_angle, 0.0f));
			}

		}
		//���������Ȃǂ̎�ނ�������
		else if (m_sword)
		{
			m_pLigAttack->SetAttack((m_attackDamage) * 1.8f);
			m_pStrengthAttack->SetAttack((m_attackDamage) * 1.8f);

			//�G�t�F�N�g���Đ�����
			if (m_nowFrame >= 15.0f && m_nowFrame <= 60.0f)
			{
				//�؂���ɃG�t�F�N�g������
				cEffect.UpdateEffectPosition("PlayerStrong", m_attackLigPos2);
				cEffect.UpdateEffectPosition("PlayerStrongBig", m_attackLigPos2);
			}

			if (m_nowFrame == 15.0f)
			{
				//�G�t�F�N�g�����
				cEffect.EffectCreate("PlayerStrong", m_attackLigPos2);

				//��SE�𗬂�
				PlaySoundMem(se.GetStormSE(), DX_PLAYTYPE_BACK, true);
			}

			//�U�����蔭���t���[��
			if (m_nowFrame == 45.0f)
			{
				m_status.s_stamina -= 120.0f;
				m_pLigAttack->Init(m_pPhysics);
			}
			if (m_nowFrame == 55.0f)
			{
				//�G�t�F�N�g�����
				cEffect.EffectCreate("PlayerStrongBig", m_attackLigPos2);
				cEffect.EffectCreate("ShockWave", m_attackLigPos2);

				//�Ռ�SE�𗬂�
				PlaySoundMem(se.GetImpactSE(), DX_PLAYTYPE_BACK, true);

				m_pStrengthAttack->Init(m_pPhysics);
			}
			else if (m_nowFrame >= 60.0f)
			{
				//�U�����胊�Z�b�g
				m_pLigAttack->CollisionEnd();
				m_pStrengthAttack->CollisionEnd();
			}
		}
	}

	//��
	if (m_pState->GetState() == StateBase::StateKind::Item && tool.GetHeel().sa_number > 0)
	{
		//�����s
		if (!m_animChange.sa_recovery)
		{
			tool.SetHeel(1);

			//�񕜃G�t�F�N�g
			cEffect.EffectCreate("Heel", m_collisionPos.ConversionToVECTOR());

			//��SE�Đ�
			PlaySoundMem(se.GetHeelSE(), DX_PLAYTYPE_BACK, true);

			m_animChange.sa_recovery = true;
		}
	}

	//�񕜂���
	if (!m_isAnimationFinish && m_animChange.sa_recovery)
	{
		//HP�̍ő�l�𒴂��Ȃ��悤�ɉ񕜂���
		if (ms_maxStatus.sm_hp > m_status.s_hp && m_heel < m_maxHeel)
		{
			m_heel++;

			m_status.s_hp++;
		}
	}
	//�񕜏I��
	else if (m_isAnimationFinish && m_animChange.sa_recovery)
	{
		m_animChange.sa_recovery = false;

		m_heel = 0;
	}

	
	//�{�X�����ɓ��郂�[�V�����I��
	if (m_isAnimationFinish && m_animChange.sa_bossEnter)
	{
		m_animChange.sa_bossEnter = false;
	}
	//�{�X�����ɓ��郂�[�V������
	else if (m_animChange.sa_bossEnter)
	{
		//�����s
		if (cEnterPos)
		{
			//�ŏ��̃X�e�[�W
			if (m_mapNow == 1)
			{
				rigidbody->SetPos(MyLibrary::LibVec3(15.0f, 12.0f, 0.0f));

				m_angle = 1.5f;
			}
			//�`���[�g���A���X�e�[�W
			else if (m_mapNow == 6)
			{
				rigidbody->SetPos(MyLibrary::LibVec3(-30.0f, 12.0f, 0.0f));

				m_angle = -1.5f;
			}

			

			cEnterPos = false;
		}
	}

	//�A�C�e���擾�I��
	if (m_animChange.sa_taking)
	{
		if (m_nowFrame >= 60.0f)
		{
			m_animChange.sa_taking = false;
		}
		//�擾���ɊԈ���ĉ������������A�C�e���擾���I������
		if (m_animChange.sa_avoidance)
		{
			m_animChange.sa_taking = false;
		}

	}

	physics->CheckUpdate();
}

/// <summary>
/// �v���C���[�̃A�N�V��������
/// </summary>
void Player::Action(VECTOR restpos, Tool& tool, Shield& shield, SEManager& se, bool boss, bool dead)
{
	
	if (boss && !dead)
	{
		if (!m_lockonTarget)
		{
			//��񂾂�����
			if (m_xpad.Buttons[7] == 1 && !cRstickButton)
			{
				m_lockonTarget = true;
				cRstickButton = true;
			}
			else if (m_xpad.Buttons[7] == 0)
			{
				cRstickButton = false;
			}
		}
		else if (m_lockonTarget)
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
	}
	else
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
			else if (m_xpad.Buttons[7] == 0)
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
		else if (m_pSearch->GetIsExit())
		{
			m_lockonTarget = false;
			cRstickButton = false;
		}
	}
	
	//�x��
	//�x���ł�����
	if (m_restTouch && m_mapNow != 0)
	{
		//Y�{�^���������ꂽ��
		if (m_xpad.Buttons[15] == 1)
		{
			//�������ʒu�X�V
			m_updateX = m_modelPos.x;
			m_updateY = m_modelPos.y;
			m_updateZ = m_modelPos.z;

			m_rest = true;

			cEffect.EffectCreate("Rest", restpos);

			//�x��SE�Đ�
			PlaySoundMem(se.GetRestSE(), DX_PLAYTYPE_BACK, true);
		}
	}
	else if(m_mapNow != 0)
	{
		m_rest = false;
	}

	//���x�����グ����x��
	//�}�b�v�ŕ�����
	if (m_restTouch && m_mapNow == 0)
	{
		//Y�{�^���������ꂽ��
		if (m_xpad.Buttons[15] == 1)
		{
			//�������ʒu�X�V
			m_updateX = m_modelPos.x;
			m_updateY = m_modelPos.y;
			m_updateZ = m_modelPos.z;

			m_rest = true;
			m_bigRest = true;

			cEffect.EffectCreate("Rest", restpos);

			//�x��SE�Đ�
			PlaySoundMem(se.GetRestSE(), DX_PLAYTYPE_BACK, true);
		}
	}
	else if(m_mapNow == 0)
	{
		m_rest = false;
		m_bigRest = false;
	}

	//�{�X�̕����ɓ���
	if (m_bossStart)
	{
		//Y�{�^������������
		if (m_xpad.Buttons[15] == 1)
		{
			m_pState->ChangeState(StateBase::StateKind::EnterBoss);

			m_animChange.sa_bossEnter = true;

			cEnterPos = true;
		}
	}
	
	//�A�C�e�����E��
	if (m_itemPick)
	{
		//Y�{�^������������
		if (m_xpad.Buttons[15] == 1)
		{
			//�����s�ƃA�j���[�V�����J��
			if (!m_animChange.sa_taking)
			{
				//�A�C�e���擾SE�Đ�
				PlaySoundMem(se.GetItemSE(), DX_PLAYTYPE_BACK, true);

				//�A�C�e�������
				m_pState->ChangeState(StateBase::StateKind::Taking);
				m_animChange.sa_taking = true;

			}
		}
	}

	//���b�Z�[�W��ǂ߂�
	if (m_message)
	{
		//Y�{�^������������
		if (m_xpad.Buttons[15] == 1)
		{
			m_read = true;
		}
	}
	//���b�Z�[�W��ǂ߂Ȃ�����
	else
	{
		m_read = false;
	}

	//���j���[���J��
	if (m_xpad.Buttons[4] == 1)
	{
		m_menuOpen = true;
	}
}

/// <summary>
/// �}�b�v�����[�v���邽�߂̊֐�
/// </summary>
void Player::WarpMap()
{
	//Y�{�^���������ꂽ��
	if (m_xpad.Buttons[15] == 1)
	{
		//�}�b�v��ς���
		m_warp = true;
	}
	else
	{
		m_warp = false;
	}
}

/// <summary>
/// �`�揈��
/// </summary>
void Player::Draw(Armor& armor, int font)
{
	rigidbody->SetPos(rigidbody->GetNextPos());
	m_collisionPos = rigidbody->GetPos();

#if false
	DrawFormatString(1000, 600, 0xffffff, "move : %d", m_anim.s_moveflag);
	DrawFormatString(1000, 700, 0xffffff, "attack : %d", m_anim.s_attack);
	DrawFormatString(1000, 800, 0xffffff, "hit : %d", m_anim.s_hit);
	DrawFormatString(1000, 100, 0xffffff, "death : %d", m_anim.s_isDead);
	DrawFormatString(1000, 150, 0xffffff, "avoidance : %d", m_animChange.sa_avoidance);
	DrawFormatString(1000, 200, 0xffffff, "bossEnter : %d", m_animChange.sa_bossEnter);
	DrawFormatString(1000, 250, 0xffffff, "dash : %d", m_animChange.sa_dashMove);
	DrawFormatString(1000, 300, 0xffffff, "enterShield : %d", m_animChange.sa_enterShield);
	DrawFormatString(1000, 350, 0xffffff, "imapact : %d", m_animChange.sa_imapact);
	DrawFormatString(1000, 400, 0xffffff, "recover : %d", m_animChange.sa_recovery);
	DrawFormatString(1000, 450, 0xffffff, "shieldIdle : %d", m_animChange.sa_shieldIdle);
	DrawFormatString(1000, 500, 0xffffff, "strengthAttack : %d", m_animChange.sa_strengthAttack);
	DrawFormatString(1000, 550, 0xffffff, "taking : %d", m_animChange.sa_taking);
	DrawFormatString(1000, 650, 0xffffff, "touch : %d", m_animChange.sa_touch);
#endif
#if false
	DrawFormatString(1000, 150, 0xffffff, "posx : %f", rigidbody->GetPos().x);   //15   -700
	DrawFormatString(1000, 200, 0xffffff, "posy : %f", rigidbody->GetPos().y);   //12   
	DrawFormatString(1000, 250, 0xffffff, "posz : %f", rigidbody->GetPos().z);   //0    370
	DrawFormatString(1000, 300, 0xffffff, "m_angle : %f", m_angle);             //1.5
#endif

#if false
	DrawFormatString(0, 400, 0xffffff, "m_nowAnim : %d", m_nowAnimIdx);
	DrawFormatString(0, 500, 0xffffff, "m_nowSpeed : %f", m_nowFrame);
	DrawFormatString(200, 100, 0xffffff, "m_blend : %f", m_animBlendRate);
	DrawFormatString(200, 200, 0xffffff, "addattacktime : %d", cAddAttackTime);
	DrawFormatString(200, 300, 0xffffff, "avoidancemove : %f", cAvoidanceMove);
	DrawFormatString(200, 400, 0xffffff, "attackNumber : %d", m_attackNumber);
	DrawFormatString(200, 500, 0xffffff, "nowAttackNumber : %d", cNowAttackNumber);

#endif
#if false
	//DrawFormatString(200, 100, 0xffffff, "animtime : %f", cAnimWalkTime);
	DrawFormatString(200, 600, 0xffffff, "weapon : %d", m_pWeapon->GetBlack());
	DrawFormatString(200, 650, 0xffffff, "shield : %d", m_pShield->GetUgly());
	DrawFormatString(200, 700, 0xffffff, "fist : %d", m_pWeapon->GetFist());
	DrawFormatString(200, 750, 0xffffff, "fist : %d", m_pShield->GetFist());
#endif

#if false
	DrawFormatString(200, 300, 0xffffff, "�A�j�� : %d", m_currentAnimNo);
#endif

	MV1SetPosition(m_modelHandle, VSub(m_modelPos.ConversionToVECTOR(), VGet(0.0f, 12.0f, 0.0f)));
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
	case ObjectTag::Rect:
#if _DEBUG
		message += "�{�X�����̓����";
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
	case ObjectTag::EnemyAttack:
#if _DEBUG
		message += "�U��";
#endif
		//HP��0�ȏゾ�ƃq�b�g����
		if (m_status.s_hp > 0.0f)
		{
			//��𒆂ƃq�b�g���ƃ{�X�����ɓ����Ă��鎞�͍U����������Ȃ�
			if (!m_avoidanceNow && !m_anim.s_hit && !m_animChange.sa_bossEnter && !m_animChange.sa_imapact)
			{
				cHit = true;

				m_anim.s_hit = true;
			}
		}
		break;
	case ObjectTag::EnemySearch:
#if _DEBUG
		message += "���G";
#endif
		break;
	case ObjectTag::Item:
#if _DEBUG
		message += "�A�C�e��";
#endif

		m_itemPick = true;

		break;
	case ObjectTag::Rest:
#if _DEBUG
		message += "�x��";
#endif
		break;
	case ObjectTag::Rect:
#if _DEBUG
		message += "�{�X�̓���";
#endif
		break;
	case ObjectTag::Message:
#if _DEBUG
		message += "���b�Z�[�W";
#endif

		m_message = true;

		break;
	}
#if _DEBUG

	message += "�Ɠ�������\n";
	printfDx(message.c_str());
#endif
}


/// <summary>
/// �h���ς���
/// </summary>
/// <param name="one">�h��̃i���o�[�w��</param>
/// <param name="path">�p�X</param>
void Player::ArmorChange(int one, std::string path)
{
	//��񂾂����s
	if (!m_armorOne[one])
	{
		//�A�j���[�V���������������ꍇ�͒ʂ�
		if (m_currentAnimNo != 1)
		{
			//�ҋ@�A�j���[�V�����ݒ�
			m_pState->ChangeState(StateBase::StateKind::Idle);
		}

		//���������
		MV1DeleteModel(m_modelHandle);
		//���f���ǂݍ���
		m_modelHandle = handle.GetModelHandle(path);

		//���f���̃T�C�Y�ݒ�
		MV1SetScale(m_modelHandle, VGet(cModelSizeScale, cModelSizeScale, cModelSizeScale));

		//�ҋ@�A�j���[�V�����ݒ�
		m_pState->ChangeState(StateBase::StateKind::Idle);

		m_armorOne[one] = true;

	}
	//���̔z���false�ɂ���
	for (int i = 0; i < 10; i++)
	{
		if (i != one)
		{
			m_armorOne[i] = false;
		}
	}
}

/// <summary>
/// ���x���A�b�v�ɂ��X�e�[�^�X�㏸
/// </summary>
void Player::ChangeStatus()
{
	CsvLoad::GetInstance().StatusLoad(m_status, "Player");

	//�Ƃ肠�����K���ɕ␳���Ă���
	m_status.s_hp = m_status.s_hp + (m_levelStatus.sl_hp * 30);
	m_status.s_stamina = m_status.s_stamina + (m_levelStatus.sl_stamina * 10);

	//����ł���
	m_status.s_muscle = m_levelStatus.sl_muscle;
	m_status.s_skill = m_levelStatus.sl_skill;
	//�X�e�[�^�X�̍ő�l������
	ms_maxStatus.sm_hp = m_status.s_hp;
	ms_maxStatus.sm_stamina = m_status.s_stamina;
}

void Player::SetModelPos()
{
	m_modelPos = m_collisionPos;
}
