#include "MapTutorial.h"
#include "MapRest.h"
#include "Manager/EffectManager.h"

namespace
{
	//�}�b�v�̃R���W�����p�X
	constexpr const char* cMapCollisionName = "Data/Map/CollisionTutorialMap.mv1";
	//���̃p�X
	constexpr const char* cMapFloorName = "Data/Map/TutorialMapFloor.mv1";
	//�ǂ̃p�X
	constexpr const char* cMapWallName = "Data/Map/TutorialMapWall.mv1";
	//�V��̃p�X
	constexpr const char* cMapCeilingName = "Data/Map/TutorialMapCeiling.mv1";
	//�x���I�u�W�F�N�g�p�X
	constexpr const char* cMapRestObjectName = "Data/Object/coffin.mv1";
	//�x���n�_�̔��a
	constexpr float  cRestRadius = 50.0f;
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
	constexpr float cBossDepth = 50.0f;
	//�{�X�����̓����������
	bool cOne = false;
	//���������̃T�C�Y
	constexpr float cSmallFloor = 0.4f;
	//�傫�����̃T�C�Y
	constexpr float cBigFloor = 2.0f;
	//���ʂ̕ǂ̃T�C�Y
	constexpr float cNormalWall = 2.5f;
	//�傫���ǂ̃T�C�Y
	constexpr float cBigWall = 5.0f;

	//�G�t�F�N�g�̍Đ�����
	int cEffectTime = 50;
	//�G�t�F�N�g�Đ�
	bool cEffectOne = false;
	//�G�t�F�N�g�̍Đ��ʒu
	VECTOR cEffectPos = VGet(-10.0f, 50.0f, 0.0f);
	VECTOR cEffectPos1 = VGet(-10.0f, 50.0f, 30.0f);
	VECTOR cEffectPos2 = VGet(-10.0f, 50.0f, -30.0f);

	//�V���O���g��
	auto& cEffect = EffectManager::GetInstance();

	// �{�X�����̈ʒu
	const MyLibrary::LibVec3 cBossRoomDeadPos = MyLibrary::LibVec3(-80.0f, 400.0f, 0.0f);
	const MyLibrary::LibVec3 cBossEnterTriggerDeadPos = MyLibrary::LibVec3(10.0f, 400.0f, 0.0f);
	const MyLibrary::LibVec3 cCoreUpdateDeadPos = MyLibrary::LibVec3(-1000.0f, -1000.0f, -1000.0f);
	const MyLibrary::LibVec3 cBossEnterPos = MyLibrary::LibVec3(-10.0f, 400.0f, 0.0f);
	const MyLibrary::LibVec3 cBossEnterResetPos = MyLibrary::LibVec3(-10.0f, 50.0f, 0.0f);

	// �G�t�F�N�g�̐�������
	constexpr int cEffectCreateTime = 20;
	constexpr int cEffectCreateInterval = 30;
	const VECTOR cRestLuminescencePos = VGet(-150.0f, 1.0f, 130.0f);

	// ���̈ʒu
	const VECTOR cFloorPositions[] = {
		VGet(-1652.0f, -7.0f, 0.0f),
		VGet(-1352.0f, -7.0f, 0.0f),
		VGet(-1052.0f, -7.0f, 0.0f),
		VGet(-752.0f, -7.0f, 0.0f),
		VGet(-452.0f, -7.0f, 0.0f),
		VGet(-152.0f, -7.0f, 0.0f)
	};

	// �ǂ̈ʒu
	const VECTOR cWallPositions[] = {
		VGet(-1929.0f, 90.0f, 0.0f),
		VGet(-1862.0f, 90.0f, 67.0f),
		VGet(-1862.0f, 90.0f, -67.0f),
		VGet(-1809.0f, 90.0f, -120.0f),
		VGet(-1809.0f, 90.0f, 120.0f),
		VGet(-1500.0f, 90.0f, -120.0f),
		VGet(-1500.0f, 90.0f, 120.0f),
		VGet(-1200.0f, 90.0f, -120.0f),
		VGet(-1200.0f, 90.0f, 120.0f),
		VGet(-900.0f, 90.0f, -120.0f),
		VGet(-900.0f, 90.0f, 120.0f),
		VGet(-600.0f, 90.0f, -120.0f),
		VGet(-600.0f, 90.0f, 120.0f),
		VGet(-300.0f, 90.0f, -120.0f),
		VGet(-300.0f, 90.0f, 120.0f),
		VGet(-8.0f, 90.0f, -120.0f),
		VGet(-8.0f, 90.0f, 120.0f),
		VGet(-8.0f, 90.0f, -240.0f),
		VGet(-8.0f, 90.0f, 240.0f)
	};

	// ���ʂ̑傫���̕ǂ̈ʒu
	const VECTOR cNormalWallPositions[] = {
		VGet(-1652.0f, 90.0f, -168.0f),
		VGet(-1352.0f, 90.0f, -168.0f),
		VGet(-1052.0f, 90.0f, -168.0f),
		VGet(-752.0f, 90.0f, -168.0f),
		VGet(-452.0f, 90.0f, -168.0f),
		VGet(-152.0f, 90.0f, -168.0f),
		VGet(-1652.0f, 90.0f, 168.0f),
		VGet(-1352.0f, 90.0f, 168.0f),
		VGet(-1052.0f, 90.0f, 168.0f),
		VGet(-752.0f, 90.0f, 168.0f),
		VGet(-452.0f, 90.0f, 168.0f),
		VGet(-152.0f, 90.0f, 168.0f)
	};

	// �傫���ǂ̈ʒu
	const VECTOR cBigWallPositions[] = {
		VGet(290.0f, 90.0f, 335.0f),
		VGet(290.0f, 90.0f, -335.0f),
		VGet(625.0f, 90.0f, 0.0f)
	};

	// �V��̈ʒu
	const VECTOR cCeilingPosition = VGet(-450.0f, 190.0f, 0.0f);

	// �x���n�_�̈ʒu
	const VECTOR cRestObjectPosition = VGet(-150.0f, 0.0f, 120.0f);
}

MapTutorial::MapTutorial()
{
}

MapTutorial::~MapTutorial()
{
	MV1DeleteModel(m_collisionHandle);
	MV1DeleteModel(m_floorHandle);
	MV1DeleteModel(m_wallHandle);
	MV1DeleteModel(m_ceilingHandle);
	MV1DeleteModel(m_restObjectHandle);
	DeleteLightHandle(m_light);

}

void MapTutorial::DataInit()
{
	//�f�[�^���[�h
	LoadData("", cMapCollisionName, "");
	//�p�[�c���[�h
	LoadPartsData(cMapFloorName, cMapWallName, cMapCeilingName, cMapRestObjectName);

	m_stageName = "stageTutorial";
}

void MapTutorial::Init(std::shared_ptr<MyLibrary::Physics> physics)
{
	m_pPhysics = physics;

	m_size = 0.15f;
	m_width = 20.0f;
	m_hight = 100.0f;
	m_depth = 200.0f;

	//���f���̃T�C�Y�ύX
	MV1SetScale(m_collisionHandle, VGet(m_size, m_size, m_size));

	//�|�W�V�����ݒ�
	m_mapCollisionPos = VGet(0.0f, 76.0f, 0.0f);
	m_mapRestPos = MyLibrary::LibVec3(-150.0f, 0.0f, 130.0f);
	m_mapBossRoomPos = MyLibrary::LibVec3(70.0f, 0.0f, 0.0f);
	m_mapBossEnterPos = MyLibrary::LibVec3(0.0f, 50.0f, 0.0f);
	m_mapCoreCollisionePos = MyLibrary::LibVec3(0.0f, 100.0f, 0.0f);
	m_mapBossEnterTriggerPos = MyLibrary::LibVec3(-20.0f, 50.0f, 0.0f);


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
	//�{�X�����̔��菉����
	InitBossEnter(cBossWidth, cBossHight, cBossDepth, m_mapBossEnterTriggerPos);

	cOne = false;
}

std::shared_ptr<MapBase> MapTutorial::Update(bool warp, bool enter, bool Dead)
{
	MyLibrary::LibVec3::Size size = MyLibrary::LibVec3::Size(m_width, m_hight, m_depth);
	MyLibrary::LibVec3::Size triggerSize = MyLibrary::LibVec3::Size(cBossWidth, cBossHight, cBossDepth);

	m_pSearch->Update(m_mapRestPos);
	m_pBossRoom->Update(m_mapBossRoomPos);
	m_pRect->Update(m_mapBossEnterPos, size);
	m_pRectTrigger->Update(m_mapBossEnterTriggerPos, triggerSize);

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
			m_mapBossEnterPos = cBossEnterPos;

			cOne = true;
		}
	}
	else
	{
		//��񂾂����s
		if (cOne)
		{
			//�{�X����������̃|�W�V����
			m_mapBossEnterPos = cBossEnterResetPos;

			cOne = false;
		}
	}

	//�G�t�F�N�g�̐���
	if (cEffectTime >= cEffectCreateTime && !Dead)
	{
		cEffect.EffectCreate("Smoke", cEffectPos1);
		cEffect.EffectCreate("Smoke", cEffectPos2);

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
std::shared_ptr<MapBase> MapTutorial::WarpUpdate(bool warp, bool rest)
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

void MapTutorial::JudgeUpdate()
{
}

void MapTutorial::CoreUpdate()
{
}

void MapTutorial::Draw()
{
	//3D���f���̃|�W�V�����ݒ�
	MV1SetPosition(m_collisionHandle, m_mapCollisionPos);

	//���ʂ̏��`��
	for (const auto& pos : cFloorPositions)
	{
		PartDrawSet(m_floorHandle, VGet(m_size, m_size, m_size), pos, VGet(0.0f, 0.0f, 0.0f));
	}

	//�傫�����`��
	PartDrawSet(m_floorHandle, VGet(m_size * cBigFloor, m_size, m_size * cBigFloor), VGet(290.0f, -7.0f, 0.0f), VGet(0.0f, 0.0f, 0.0f));

	//���������`��
	PartDrawSet(m_floorHandle, VGet(m_size * cSmallFloor, m_size, m_size * cSmallFloor), VGet(-1862.0f, -7.0f, 0.0f), VGet(0.0f, 0.0f, 0.0f));

	//�ǂ̕`��
	for (const auto& pos : cWallPositions)
	{
		if (pos.x == cWallPositions[2].x)
		{
			PartDrawSet(m_wallHandle, VGet(m_size, m_size, m_size), pos, VGet(0.0f, DX_PI_F / 2.0f, 0.0f));
		}
		else
		{
			PartDrawSet(m_wallHandle, VGet(m_size, m_size, m_size), pos, VGet(0.0f, 0.0f, 0.0f));
		}

	}

	//���ʂ̑傫���̕Ǖ`��
	for (const auto& pos : cNormalWallPositions)
	{
		PartDrawSet(m_wallHandle, VGet(m_size * cNormalWall, m_size, m_size * cNormalWall), pos, VGet(0.0f, DX_PI_F / 2.0f, 0.0f));
	}

	//�傫���Ǖ`��
	for (const auto& pos : cBigWallPositions)
	{
		if (pos.x == cBigWallPositions[2].x && pos.y == cBigWallPositions[2].y && pos.z == cBigWallPositions[2].z)
		{
			PartDrawSet(m_wallHandle, VGet(m_size * cBigWall, m_size, m_size * cBigWall), pos, VGet(0.0f, 0.0f, 0.0f));
		}
		else
		{
			PartDrawSet(m_wallHandle, VGet(m_size * cBigWall, m_size, m_size * cBigWall), pos, VGet(0.0f, DX_PI_F / 2.0f, 0.0f));
		}
		
	}

	//�V��`��
	PartDrawSet(m_ceilingHandle, VGet(m_size, m_size, m_size), cCeilingPosition, VGet(0.0f, 0.0f, 0.0f));

	//�x���n�_�`��
	PartDrawSet(m_restObjectHandle, VGet(0.2f, 0.2f, 0.2f), cRestObjectPosition, VGet(0.0f, 0.0f, 0.0f));
}

void MapTutorial::CoreDraw()
{
}

void MapTutorial::End(std::shared_ptr<MyLibrary::Physics> physics, bool title)
{
	MV1DeleteModel(m_collisionHandle);
	MV1DeleteModel(m_floorHandle);
	MV1DeleteModel(m_wallHandle);
	MV1DeleteModel(m_ceilingHandle);
	MV1DeleteModel(m_restObjectHandle);
	DeleteLightHandle(m_light);

	if (!title)
	{
		Finalize(physics);
	}
}
