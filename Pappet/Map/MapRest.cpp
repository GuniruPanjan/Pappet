#include "MapRest.h"

namespace
{
	//�}�b�v�̃p�X
	constexpr const char* cMapName = "Data/Map/RestMap.mv1";
	//�}�b�v�̃R���W�����p�X
	constexpr const char* cMapCollisionName = "Data/Map/RestMapCollision.mv1";
	//�R�A�̃p�X
	constexpr const char* cCoreName = "Data/Object/Core.mv1";
	//�x���n�_�̔��a
	constexpr float cRestRadius = 100.0f;
	//�{�X���������̔��a
	constexpr float cBossRommRadius = 60.0f;
	//�R�A�̔��a
	constexpr float cCoreRadius = 70.0f;
	//�R�A�̃T�C�Y
	constexpr float cCoreSize = 0.5f;
}

/// <summary>
/// �R���X�g���N�^
/// </summary>
MapRest::MapRest()
{
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
MapRest::~MapRest()
{
	//���������
	MV1DeleteModel(m_handle);
	MV1DeleteModel(m_collisionHandle);
	MV1DeleteModel(m_coreHandle);
	DeleteLightHandle(m_light);
}

/// <summary>
/// �f�[�^�̏�����
/// </summary>
void MapRest::DataInit()
{
	//�f�[�^���[�h
	LoadData(cMapName, cMapCollisionName, cCoreName);
}

/// <summary>
/// ����������
/// </summary>
/// <param name="physics"></param>
void MapRest::Init(std::shared_ptr<MyLibrary::Physics> physics)
{
	m_pPhysics = physics;

	//�}�b�v�̃T�C�Y
	m_size = 0.01f;

	//���f���̃T�C�Y�ύX
	MV1SetScale(m_handle, VGet(m_size, m_size, m_size));
	MV1SetScale(m_collisionHandle, VGet(m_size, m_size, m_size));

	m_mapPos = VGet(-200.0f, -25.0f, 0.0f);
	m_mapCollisionPos = VGet(-220.0f, -40.0f, 395.0f);
	//m_mapCollisionPos = VGet(0.0f, 0.0f, 0.0f);
	m_mapCorePos = VGet(0.0f, 0.0f, 0.0f);
	m_mapRestPos = MyLibrary::LibVec3(0.0f, 0.0f, 0.0f);
	m_mapBossRoomPos = MyLibrary::LibVec3(0.0f, 0.0f, 0.0f);
	m_mapBossEnterPos = MyLibrary::LibVec3(0.0f, 0.0f, 0.0f);
	m_mapCoreCollisionePos = MyLibrary::LibVec3(m_mapCorePos.x, 0.0f, m_mapCorePos.z);

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
}

/// <summary>
/// �X�V����
/// </summary>
/// <returns></returns>
std::shared_ptr<MapBase> MapRest::Update(bool warp)
{
	if (warp)
	{

	}

	return shared_from_this();  //���g�̃|�C���^
}

/// <summary>
/// ���[�v�̍X�V����
/// </summary>
/// <param name="warp"></param>
/// <returns></returns>
std::shared_ptr<MapBase> MapRest::WarpUpdate(bool warp)
{
	if (warp)
	{

	}

	return shared_from_this();  //���g�̃|�C���^
}

/// <summary>
/// �����������̔���
/// </summary>
void MapRest::JudgeUpdate()
{
}

/// <summary>
/// �{�X�����񂾂�o��
/// </summary>
void MapRest::CoreUpdate()
{
}

/// <summary>
/// �`�揈��
/// </summary>
void MapRest::Draw()
{
	//3D���f���̃|�W�V�����ݒ�
	MV1SetPosition(m_handle, m_mapPos);
	MV1SetPosition(m_collisionHandle, m_mapCollisionPos);

	//3D���f���`��
	MV1DrawModel(m_handle);
	//MV1DrawModel(m_collisionHandle);
}

/// <summary>
/// �R�A�`�揈��
/// </summary>
void MapRest::CoreDraw()
{
}

/// <summary>
/// �I������
/// </summary>
/// <param name="physics"></param>
void MapRest::End(std::shared_ptr<MyLibrary::Physics> physics)
{
	//���������
	MV1DeleteModel(m_handle);
	MV1DeleteModel(m_collisionHandle);
	MV1DeleteModel(m_coreHandle);
	DeleteLightHandle(m_light);

	Finalize(physics);
}