#include "MapFirst.h"

namespace
{
	//�}�b�v�̃p�X
	constexpr const char* cMapName = "Data/Map/Map.mv1";
	//�}�b�v�̃R���W�����p�X
	constexpr const char* cMapCollisionName = "Data/Map/Collision.mv1";
	//�R�A�̃p�X
	constexpr const char* cCoreName = "Data/Object/Core.mv1";
	//�x���n�_�̔��a
	constexpr float cRestRadius = 50.0f;
	//�{�X����������̔��a
	constexpr float cBossRommRadius = 60.0f;
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
}

/// <summary>
/// �f�[�^������
/// </summary>
void MapFirst::DataInit()
{
	//�f�[�^���[�h
	LoadData(cMapName, cMapCollisionName, cCoreName);
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
	m_mapPos = VGet(0.0f, 250.0f, 0.0f);
	m_mapCollisionPos = VGet(-241.0f, -277.0f, -173.0f);
	m_mapRestPos = MyLibrary::LibVec3(100.0f, 0.0f, -75.0f);
	m_mapBossRoomPos = MyLibrary::LibVec3(-80.0f, 0.0f, 0.0f);
	//m_mapBossEnterPos = MyLibrary::LibVec3(-10.0f, 50.0f, 0.0f);
	m_mapBossEnterPos = MyLibrary::LibVec3(0.0f, 400.0f, 0.0f);

	//���C�g�֌W
	ChangeLightTypeDir(VGet(-1.0f, 0.0f, 0.0f));
	m_light = CreateDirLightHandle(VGet(1.0f, 0.0f, 0.0f));

	//���G���菉����
	InitSearch(cRestRadius, m_mapRestPos);
	//�{�X���������
	InitBossRoom(cBossRommRadius, m_mapBossEnterPos);
	//�{�X���������
	InitRect(m_width, m_hight, m_depth, m_mapBossEnterPos);
}

/// <summary>
/// �X�V����
/// </summary>
/// <returns>���݂̃}�b�v</returns>
std::shared_ptr<MapBase> MapFirst::Update()
{
	m_pSearch->Update(m_mapRestPos);
	m_pBossRoom->Update(m_mapBossRoomPos);
	m_pRect->Update(m_mapBossEnterPos);

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

}

/// <summary>
/// �`�揈��
/// </summary>
void MapFirst::Draw()
{
	//3D���f���̃|�W�V�����ݒ�
	MV1SetPosition(m_handle, m_mapPos);
	MV1SetPosition(m_collisionHandle, m_mapCollisionPos);

	//3D���f���`��
	MV1DrawModel(m_handle);
}

/// <summary>
/// �R�A�`��
/// </summary>
void MapFirst::CoreDraw()
{
	//3D���f���`��
	MV1DrawModel(m_coreHandle);
}

/// <summary>
/// �I������
/// </summary>
void MapFirst::End(std::shared_ptr<MyLibrary::Physics> physics)
{
	//���������
	MV1DeleteModel(m_handle);
	MV1DeleteModel(m_collisionHandle);
	DeleteLightHandle(m_light);

	Finalize(physics);
}