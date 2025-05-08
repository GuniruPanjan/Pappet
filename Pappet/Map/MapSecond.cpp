#include "MapSecond.h"
#include "MapRest.h"
#include "Manager/EffectManager.h"

namespace
{
	//�}�b�v�̃R���W�����p�X
	constexpr const char* cMapCollisionName = "Data/Map/Map2Collision.mv1";

	//�G�t�F�N�g�̍Đ�����
	int cEffectTime = 50;
	//�G�t�F�N�g�Đ�
	bool cEffectOne = false;
	//�G�t�F�N�g�̍Đ��ʒu
	VECTOR cEffectPos = VGet(-10.0f, 50.0f, 0.0f);

	// �G�t�F�N�g�̐�������
	constexpr int cEffectCreateTime = 20;
	constexpr int cEffectCreateInterval = 30;
	const VECTOR cRestLuminescencePos = VGet(0.0f, 0.0f + 1.0f, 0.0f - 20.0f);
	const VECTOR cRestObjectPos = VGet(0.0f, 0.0f, 0.0f - 30.0f);
	constexpr float cCoreRotationSpeed = 0.001f;
	const VECTOR cCoreScale = VGet(0.0f, 0.0f, 0.0f);

	//�V���O���g��
	auto& cEffect = EffectManager::GetInstance();
}

/// <summary>
/// �R���X�g���N�^
/// </summary>
MapSecond::MapSecond()
{
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
MapSecond::~MapSecond()
{
	//���������
	MV1DeleteModel(m_handle);
	MV1DeleteModel(m_collisionHandle);
	MV1DeleteModel(m_coreHandle);
	DeleteLightHandle(m_light);
}

/// <summary>
/// �f�[�^������
/// </summary>
void MapSecond::DataInit()
{
	//�f�[�^���[�h
	LoadData("", cMapCollisionName, "");
	//�p�[�c���[�h
	LoadPartsData("", "", "", "");

	m_stageName = "stage2";
}

/// <summary>
/// ����������
/// </summary>
/// <param name="physics"></param>
void MapSecond::Init(std::shared_ptr<MyLibrary::Physics> physics)
{
	m_pPhysics = physics;

	//�}�b�v�̃T�C�Y
	m_size = 0.12f;
	m_width = -400.0f;
	m_hight = -3000.0f;
	m_depth = 0.0f;

	//���f���̃T�C�Y�ύX
	MV1SetScale(m_collisionHandle, VGet(m_size, m_size, m_size));

	//���C�g�֌W
	ChangeLightTypeDir(VGet(-1.0f, 0.0f, 0.0f));
	m_light = CreateDirLightHandle(VGet(1.0f, 0.0f, 0.0f));

	//���G���菉����
	InitSearch(0.0f, m_mapRestPos);
	//�{�X���������
	InitBossRoom(0.0f, m_mapBossEnterPos);
	//�{�X���������
	InitRect(m_width, m_hight, m_depth, m_mapBossEnterPos);
	//�R�A�̔��菉����
	InitCore(0.0f, m_mapCoreCollisionePos);
	//�Q�ڂ̃G���A�ɍs�����߂̔���
	InitMapSecond(0.0f, m_mapSecondArea);
	//�{�X�����̔��菉����
	InitBossEnter(0.0f, 0.0f, 0.0f, m_mapBossEnterTriggerPos);
}

std::shared_ptr<MapBase> MapSecond::Update(bool warp, bool enter, bool Dead)
{
	MyLibrary::LibVec3::Size size = MyLibrary::LibVec3::Size(m_width, m_hight, m_depth);
	MyLibrary::LibVec3::Size triggerSize = MyLibrary::LibVec3::Size(0.0f, 0.0f, 0.0f);

	m_pSearch->Update(m_mapRestPos);
	m_pBossRoom->Update(m_mapBossRoomPos);
	m_pRect->Update(m_mapBossEnterPos, size);
	m_pRectTrigger->Update(m_mapBossEnterTriggerPos, triggerSize);
	m_pMapSecond->Update(m_mapSecondArea);

	//�G�t�F�N�g�̐���
	if (cEffectTime >= cEffectCreateTime && !Dead)
	{
		cEffect.EffectCreate("Smoke", cEffectPos);

		cEffectTime = 0;
	}
	else
	{
		cEffectTime++;
	}

	//�G�t�F�N�g�̐���
	if (m_effect >= cEffectCreateInterval)
	{
		cEffect.EffectCreate("RestLuminescence", cRestLuminescencePos);

		m_effect = 0;
	}
	else
	{
		m_effect++;
	}

	//�G�t�F�N�g����
	if (!cEffectOne && enter && !Dead)
	{
		cEffect.EffectCreate("Distortion", cEffectPos);

		cEffectOne = true;
	}
	else if (!enter)
	{
		cEffectOne = false;
	}

	return shared_from_this();  //���M�̃|�C���^��Ԃ�
}

std::shared_ptr<MapBase> MapSecond::WarpUpdate(bool warp, bool secondWarp, bool rest)
{
	return shared_from_this();  //���M�̃|�C���^��Ԃ�
}

void MapSecond::JudgeUpdate()
{
}

void MapSecond::CoreUpdate()
{
}

void MapSecond::Draw()
{
	//3D���f���̃|�W�V�����ݒ�
	MV1SetPosition(m_collisionHandle, VGet(0.0f, -200.0f, 0.0f));

	//3D���f���`��
	MV1DrawModel(m_collisionHandle);
}

void MapSecond::CoreDraw()
{
}

void MapSecond::End(std::shared_ptr<MyLibrary::Physics> physics, bool title)
{
	//���������
	MV1DeleteModel(m_handle);
	MV1DeleteModel(m_collisionHandle);
	MV1DeleteModel(m_coreHandle);
	DeleteLightHandle(m_light);

	if (!title)
	{
		Finalize(physics);
	}
}
