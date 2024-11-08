#include "EnemyBase.h"
#include "Manager/HandleManager.h"
#include "External/CsvLoad.h"

namespace
{
	//�A�j���[�V�����̐؂�ւ��ɂ�����t���[����
	constexpr float cAnimChangeFrame = 5.0f;
	//�A�j���[�V�����̐؂�ւ����x
	constexpr float cAnimChangeRateSpeed = 1.0f / cAnimChangeFrame;

	//�A�j���[�V�����u�����h���̍ő�
	constexpr float cAnimBlendRateMax = 1.0f;

	//�V���O���g��
	auto& handle = HandleManager::GetInstance();
}

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="priority">�D��x</param>
EnemyBase::EnemyBase(Priority priority) :
	CharacterBase(priority, ObjectTag::Enemy),
	m_dropCore(0),
	m_hpRadius(0.0f),
	m_isExist(false),
	m_isDroped(false),
	m_isDiscovery(false),
	m_isTarget(false),
	m_isStayTarget(false),
	m_isExitTarget(false),
	m_isEnterHit(false),
	m_isExitHit(false),
	m_centerPos(),
	m_I(0)
{
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
EnemyBase::~EnemyBase()
{
	//���������
	MV1DeleteModel(m_modelHandle);
}

void EnemyBase::Finalize(std::shared_ptr<MyLibrary::Physics> physics)
{
	Collidable::Finalize(physics);

	//m_pAttack->Finalize(physics);
	m_pSearch->Finalize(physics);
}

void EnemyBase::OnCollideEnter(const std::shared_ptr<Collidable>& collidable)
{
#if _DEBUG
	std::string message = "�G��";
#endif
	auto tag = collidable->GetTag();
	switch (tag)
	{
	case ObjectTag::Player:
#if _DEBUG
		message += "�v���C���[";
#endif
		break;
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

void EnemyBase::OnCollideStay(const std::shared_ptr<Collidable>& collidable)
{
#if _DEBUG
	std::string message = "�G��";
#endif
	auto tag = collidable->GetTag();
	switch (tag)
	{
	case ObjectTag::Player:
#if _DEBUG
		message += "�v���C���[";
#endif
		break;
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

void EnemyBase::OnTriggerEnter(const std::shared_ptr<Collidable>& collidable)
{
#if _DEBUG
	std::string message = "�G��";
#endif
	auto tag = collidable->GetTag();
	switch (tag)
	{
	case ObjectTag::Attack:
#if _DEBUG
		message += "�U��";
#endif
		m_col = dynamic_cast<AttackObject*>(collidable.get());
		m_isEnterHit = true;
		break;
	case ObjectTag::Search:
#if _DEBUG
		message += "���G�͈�";
#endif
		m_isStayTarget = true;
		break;
	case ObjectTag::BossEnter:
#if _DEBUG
		message += "�{�X��������";
#endif
		break;
	default:
		break;
	}

#if _DEBUG
	message += "�Ɠ�������\n";
	printfDx(message.c_str());
#endif
}

void EnemyBase::OnTriggerStay(const std::shared_ptr<Collidable>& collidable)
{
#if _DEBUG
	std::string message = "�G��";
#endif
	auto tag = collidable->GetTag();
	switch (tag)
	{
	case ObjectTag::Attack:
#if _DEBUG
		message += "�U��";
#endif
		break;
	case ObjectTag::Search:
#if _DEBUG
		message += "���G�͈�";
#endif
		m_isStayTarget = true;
		break;
	case ObjectTag::BossEnter:
#if _DEBUG
		message += "�{�X��������";
#endif
		break;
	default:
		break;
	}

#if _DEBUG
	message += "�Ɠ�������\n";
	printfDx(message.c_str());
#endif
}

void EnemyBase::OnTriggerExit(const std::shared_ptr<Collidable>& collidable)
{
#if _DEBUG
	std::string message = "�G��";
#endif
	auto tag = collidable->GetTag();
	switch (tag)
	{
	case ObjectTag::Attack:
#if _DEBUG
		message += "�U��";
#endif
		m_isExitHit = true;
		break;
	case ObjectTag::Search:
#if _DEBUG
		message += "���G�͈�";
#endif
		m_isExitTarget = true;
		break;
	case ObjectTag::BossEnter:
#if _DEBUG
		message += "�{�X��������";
#endif
		break;
	default:
		break;
	}

#if _DEBUG
	message += "���O�ꂽ\n";
	printfDx(message.c_str());
#endif
}

bool EnemyBase::GetStay() const
{
	return m_isStayTarget;
}

bool EnemyBase::GetExit()
{
	return m_isExitTarget;
}

bool EnemyBase::GetIsHit()
{
	bool log = m_anim.s_hit;
	m_anim.s_hit = false;
	return log;
}

int EnemyBase::GetDropCore()
{
	m_isDroped = true;
	return m_dropCore;
}

const float EnemyBase::GetRadius() const
{
	//�������a0(���G�͈͂Ȃ�)�Ȃ�Ƃ肠�����̒l��Ԃ�
	if (m_hpRadius == 0.0f)
	{
		return 5.0f;
	}

	//0�łȂ���΍��G�͈͂̔��a�����̂܂ܕԂ�
	return m_hpRadius;
}

/// <summary>
/// ���̃I�u�W�F�N�g�Ɖ����o����������铖���蔻����쐬
/// </summary>
/// <param name="vec">�x�N�^�[</param>
/// <param name="len">����</param>
/// <param name="radius">���a</param>
void EnemyBase::InitCollision(MyLibrary::LibVec3 vec, float len, float radius)
{
	auto collider = Collidable::AddCollider(MyLibrary::CollidableData::Kind::Capsule, false);
	auto capsuleCol = dynamic_cast<MyLibrary::CollidableDataCapsule*>(collider.get());
	capsuleCol->m_len = len;
	capsuleCol->m_radius = radius;
	capsuleCol->m_vec = MyLibrary::LibVec3(vec.x, capsuleCol->m_vec.y + vec.y, vec.z);
}

/// <summary>
/// ���f����ǂݍ���
/// </summary>
/// <param name="path">���f���p�X</param>
void EnemyBase::LoadModel(std::string path)
{
	m_modelHandle = handle.GetModelHandle(path);
}

/// <summary>
/// �A�j���[�V��������A�X�e�[�^�X����ǂݍ���
/// </summary>
/// <param name="name">�L�����N�^�[��</param>
void EnemyBase::LoadData(std::string name)
{
	CsvLoad::GetInstance().AnimDataLoad(name, m_animIdx);
	CsvLoad::GetInstance().StatusLoad(m_status, name.c_str());
	m_dropCore = m_status.s_core;
}

/// <summary>
/// �����N���X�̏�����
/// </summary>
/// <param name="posX">X���W</param>
/// <param name="posY">Y���W</param>
/// <param name="posZ">Z���W</param>
/// <param name="isUseGravity">�d�͂�^���邩�^���Ȃ���</param>
void EnemyBase::InitRigidbody(float posX, float posY, float posZ, bool isUseGravity)
{
	rigidbody.Init(isUseGravity);
	rigidbody.SetPos(MyLibrary::LibVec3(posX, posY, posZ));
	rigidbody.SetNextPos(rigidbody.GetPos());
	m_collisionPos = rigidbody.GetPos();
}

/// <summary>
/// ���f���̒��S���W���v�Z
/// </summary>
/// <param name="modeldefaultSize">���f���̂��Ƃ��Ƃ̃T�C�Y</param>
/// <param name="modelSize">���f���̊g�嗦</param>
void EnemyBase::CalculationCenterPos(float modeldefaultSize, float modelSize)
{
	m_centerPos = rigidbody.GetPos();
	m_centerPos.y -= (modeldefaultSize * modelSize) / 2;
}

/// <summary>
/// ���f�����W��ݒ�
/// </summary>
void EnemyBase::SetModelPos()
{
	m_modelPos = m_collisionPos;
}

/// <summary>
/// ���G��������铖���蔻����쐬
/// </summary>
/// <param name="radius">���a</param>
void EnemyBase::InitSearch(float radius, float y)
{
	m_pSearch = std::make_shared<SearchObject>(radius);
	m_pSearch->Init(m_pPhysics, MyLibrary::LibVec3(m_modelPos.x, m_modelPos.y + y, m_modelPos.z), true);

	m_hpRadius = radius;
}

void EnemyBase::TriggerUpdate()
{
	m_isStayTarget = false;
}

void EnemyBase::TargetNow()
{
	//�^�[�Q�b�g�ł��鎞
	if (GetStay())
	{
		m_isTarget = true;
	}
	//�^�[�Q�b�g�ł��Ȃ���
	else if (GetExit())
	{
		m_isTarget = false;
	}
}

void EnemyBase::HitTriggerUpdate()
{
	m_isEnterHit = false;
}

/// <summary>
/// ���S������
/// </summary>
void EnemyBase::Death()
{
	if (!m_anim.s_isDead)
	{
		m_anim.s_isDead = true;
		EnemyBase::Finalize(m_pPhysics);

		m_nowAnimIdx = m_animIdx["Death"];
		ChangeAnim(m_nowAnimIdx, m_animOne[1], m_animOne);
	}
}

/// <summary>
/// �A�j���[�V�����u�����h�̍X�V
/// </summary>
void EnemyBase::UpdateAnimationBlend()
{
	//�A�j���[�V�����̐؂�ւ�
	if (m_prevAnimNo != -1)
	{
		//�t���[���ŃA�j���[�V������؂�ւ���
		m_animBlendRate += cAnimChangeRateSpeed;
		if (m_animBlendRate >= cAnimBlendRateMax)
		{
			m_animBlendRate = cAnimBlendRateMax;
		}

		//�A�j���[�V�����̃u�����h����ݒ肷��
		MV1SetAttachAnimBlendRate(m_modelHandle, m_prevAnimNo, cAnimBlendRateMax - m_animBlendRate);
		MV1SetAttachAnimBlendRate(m_modelHandle, m_nowAnimNo, m_animBlendRate);
	}
}

/// <summary>
/// ���f���̍��W�ݒ�
/// </summary>
/// <param name="offset"></param>
void EnemyBase::SetDrawModelPos(float posY)
{
	rigidbody.SetPos(rigidbody.GetNextPos());
	m_collisionPos = rigidbody.GetPos();
	SetModelPos();
	MV1SetPosition(m_modelHandle, VSub(m_modelPos.ConversionToVECTOR(), VGet(0.0f, posY, 0.0f)));
}
