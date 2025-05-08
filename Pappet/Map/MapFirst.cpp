#include "MapFirst.h"
#include "MapRest.h"
#include "Manager/EffectManager.h"

namespace
{
	//�}�b�v�̃p�X
	constexpr const char* cMapName = "Data/Map/Map1.mv1";
	//�}�b�v�̃R���W�����p�X
	constexpr const char* cMapCollisionName = "Data/Map/CollisionMap1.mv1";
	//�R�A�̃p�X
	constexpr const char* cCoreName = "Data/Object/Core.mv1";
	//�x���I�u�W�F�N�g�p�X
	constexpr const char* cMapRestObjectName = "Data/Object/coffin.mv1";
	//�x���n�_�̔��a
	constexpr float cRestRadius = 50.0f;
	//�{�X����������̔��a
	constexpr float cBossRommRadius = 60.0f;
	//�R�A�̔��a
	constexpr float cCoreRadius = 70.0f;
	//�R�A�̃T�C�Y
	constexpr float cCoreSize = 0.5f;
	//�{�X�����̕�
	constexpr float cBossWidth = 20.0f;
	//�{�X�����̉�
	constexpr float cBossHight = 100.0f;
	//�{�X�����̉��s
	constexpr float cBossDepth = 30.0f;
	//�{�X�����̓����������
	bool cOne = false;

	//�G�t�F�N�g�̍Đ�����
	int cEffectTime = 50;
	//�G�t�F�N�g�Đ�
	bool cEffectOne = false;
	//�G�t�F�N�g�̍Đ��ʒu
	VECTOR cEffectPos = VGet(-10.0f, 50.0f, 0.0f);

	//�V���O���g��
	auto& cEffect = EffectManager::GetInstance();

	// �}�b�v�̈ʒu
	const VECTOR cMapPos = VGet(0.0f, 250.0f, 0.0f);
	const VECTOR cMapCollisionPos = VGet(-241.0f, -277.0f, -173.0f);
	const VECTOR cMapCorePos = VGet(-830.0f, 50.0f, 0.0f);
	const MyLibrary::LibVec3 cMapRestPos = MyLibrary::LibVec3(80.0f, 0.0f, -50.0f);
	const MyLibrary::LibVec3 cMapBossRoomPos = MyLibrary::LibVec3(-80.0f, 0.0f, 0.0f);
	const MyLibrary::LibVec3 cMapBossEnterPos = MyLibrary::LibVec3(-10.0f, 50.0f, 0.0f);
	const MyLibrary::LibVec3 cMapCoreCollisionePos = MyLibrary::LibVec3(cMapCorePos.x, 0.0f, cMapCorePos.z);
	const MyLibrary::LibVec3 cMapBossEnterTriggerPos = MyLibrary::LibVec3(10.0f, 50.0f, 0.0f);

	// �{�X�����̈ʒu
	const MyLibrary::LibVec3 cBossRoomDeadPos = MyLibrary::LibVec3(-80.0f, 400.0f, 0.0f);
	const MyLibrary::LibVec3 cBossEnterTriggerDeadPos = MyLibrary::LibVec3(10.0f, 400.0f, 0.0f);
	const MyLibrary::LibVec3 cCoreUpdateDeadPos = MyLibrary::LibVec3(-1000.0f, -1000.0f, -1000.0f);
	const MyLibrary::LibVec3 cBossEnterPosDead = MyLibrary::LibVec3(-10.0f, 400.0f, 0.0f);
	const MyLibrary::LibVec3 cBossEnterPosReset = MyLibrary::LibVec3(-10.0f, 50.0f, 0.0f);

	// �G�t�F�N�g�̐�������
	constexpr int cEffectCreateTime = 20;
	constexpr int cEffectCreateInterval = 30;
	const VECTOR cRestLuminescencePos = VGet(cMapRestPos.x, cMapRestPos.y + 1.0f, cMapRestPos.z - 20.0f);
	const VECTOR cRestObjectPos = VGet(cMapRestPos.x, cMapRestPos.y, cMapRestPos.z - 30.0f);
	constexpr float cCoreRotationSpeed = 0.001f;
	const VECTOR cCoreScale = VGet(cCoreSize, cCoreSize, cCoreSize);

	//���G���A�̈ʒu
	const MyLibrary::LibVec3 cSecondAreaPos = MyLibrary::LibVec3(-1000.0f, -1000.0f, -1000.0f);
}

/// <summary>
/// �R���X�g���N�^
/// </summary>
MapFirst::MapFirst()
{
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
MapFirst::~MapFirst()
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
void MapFirst::DataInit()
{
	//�f�[�^���[�h
	LoadData(cMapName, cMapCollisionName, cCoreName);
	//�p�[�c���[�h
	LoadPartsData("", "", "", cMapRestObjectName);

	m_stageName = "stage1";
}

/// <summary>
/// ����������
/// </summary>
void MapFirst::Init(std::shared_ptr<MyLibrary::Physics> physics)
{
	m_pPhysics = physics;

	//�}�b�v�̃T�C�Y
	m_size = 0.12f;
	m_width = 20.0f;
	m_hight = 100.0f;
	m_depth = 100.0f;

	//���f���̃T�C�Y�ύX
	MV1SetScale(m_handle, VGet(m_size, m_size, m_size));
	MV1SetScale(m_collisionHandle, VGet(m_size, m_size, m_size));

	//�|�W�V�����ݒ�
	m_mapPos = cMapPos;
	m_mapCollisionPos = cMapCollisionPos;
	m_mapCorePos = cMapCorePos;
	m_mapRestPos = cMapRestPos;
	m_mapBossRoomPos = cMapBossRoomPos;
	m_mapBossEnterPos = cMapBossEnterPos;
	m_mapCoreCollisionePos = cMapCoreCollisionePos;
	m_mapBossEnterTriggerPos = cMapBossEnterTriggerPos;
	m_mapSecondArea = cSecondAreaPos;

	//���C�g�֌W
	ChangeLightTypeDir(VGet(-1.0f, 0.0f, 0.0f));
	m_light = CreateDirLightHandle(VGet(1.0f, 0.0f, 0.0f));

	//���G���菉����
	InitSearch(cRestRadius, m_mapRestPos);
	//�{�X���������
	InitBossRoom(cBossRommRadius, m_mapBossEnterPos);
	//�{�X���������
	InitRect(m_width, m_hight, m_depth, m_mapBossEnterPos);
	//�R�A�̔��菉����
	InitCore(cCoreRadius, m_mapCoreCollisionePos);
	//�Q�ڂ̃G���A�ɍs�����߂̔���
	InitMapSecond(cCoreRadius, m_mapSecondArea);
	//�{�X�����̔��菉����
	InitBossEnter(cBossWidth, cBossHight, cBossDepth, m_mapBossEnterTriggerPos);

	cOne = false;
}

/// <summary>
/// �X�V����
/// </summary>
/// <returns>���݂̃}�b�v</returns>
std::shared_ptr<MapBase> MapFirst::Update(bool warp, bool enter, bool Dead)
{
	MyLibrary::LibVec3::Size size = MyLibrary::LibVec3::Size(m_width, m_hight, m_depth);
	MyLibrary::LibVec3::Size triggerSize = MyLibrary::LibVec3::Size(cBossWidth, cBossHight, cBossDepth);

	m_pSearch->Update(m_mapRestPos);
	m_pBossRoom->Update(m_mapBossRoomPos);
	m_pRect->Update(m_mapBossEnterPos, size);
	m_pRectTrigger->Update(m_mapBossEnterTriggerPos, triggerSize);
	m_pMapSecond->Update(m_mapSecondArea);


	//�{�X�����񂾂Ƃ�
	if (Dead)
	{
		m_mapBossRoomPos = cBossRoomDeadPos;
		m_mapBossEnterTriggerPos = cBossEnterTriggerDeadPos;

		m_pCore->Update(m_mapCoreCollisionePos);

	}
	else if (!Dead)
	{
		m_pCore->Update(cCoreUpdateDeadPos);
	}

	if (enter || Dead)
	{
		//��񂾂����s
		if (!cOne)
		{
			//�{�X����������̃|�W�V����
			m_mapBossEnterPos = cBossEnterPosDead;

			cOne = true;
		}
	}
	else
	{
		//��񂾂����s
		if (cOne)
		{
			//�{�X����������̃|�W�V����
			m_mapBossEnterPos = cBossEnterPosReset;

			cOne = false;
		}
	}

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

	return shared_from_this();   //���g�̃|�C���^
}

/// <summary>
/// ���[�v�̍X�V����
/// </summary>
/// <param name="warp"></param>
/// <returns></returns>
std::shared_ptr<MapBase> MapFirst::WarpUpdate(bool warp, bool secondWarp, bool rest)
{
	if (warp)
	{
		return std::make_shared<MapRest>();
	}
	else if (rest)
	{
		return std::make_shared<MapRest>();
	}

	return shared_from_this();   //���g�̃|�C���^
}

/// <summary>
/// �����������̔���
/// </summary>
void MapFirst::JudgeUpdate()
{
	if (m_pSearch->GetIsStay())
	{
		int a = 0;
	}
}

/// <summary>
/// �{�X�����񂾂�o��
/// </summary>
void MapFirst::CoreUpdate()
{
	m_angle += 0.001f;

	if (m_pCore->GetIsStay())
	{
		int a = 1;
	}
}

/// <summary>
/// �`�揈��
/// </summary>
void MapFirst::Draw()
{
	float scale = 0.2f;

	//3D���f���̃|�W�V�����ݒ�
	MV1SetPosition(m_handle, m_mapPos);
	MV1SetPosition(m_collisionHandle, m_mapCollisionPos);

	//3D���f���`��
	MV1DrawModel(m_handle);

	//�x���n�_�`��
	PartDrawSet(m_restObjectHandle, VGet(scale, scale, scale), cRestObjectPos, VGet(0.0f, 0.0f, 0.0f));
}

/// <summary>
/// �R�A�`��
/// </summary>
void MapFirst::CoreDraw()
{
	//3D���f���̃|�W�V�����ݒ�
	MV1SetPosition(m_coreHandle, m_mapCorePos);

	//3D���f���̉�]
	MV1SetRotationXYZ(m_coreHandle, VGet(0.0f, m_angle, 0.0f));
	//�傫����ς���
	MV1SetScale(m_coreHandle, VGet(cCoreSize, cCoreSize, cCoreSize));

	//3D���f���`��
	MV1DrawModel(m_coreHandle);
}

/// <summary>
/// �I������
/// </summary>
void MapFirst::End(std::shared_ptr<MyLibrary::Physics> physics, bool title)
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
