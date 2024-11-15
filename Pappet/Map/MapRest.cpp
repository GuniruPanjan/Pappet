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
	m_size = 0.12f;

	//���f���̃T�C�Y�ύX
	MV1SetScale(m_handle, VGet(m_size, m_size, m_size));
	MV1SetScale(m_collisionHandle, VGet(m_size, m_size, m_size));

	m_mapPos = VGet(0.0f, 0.0f, 0.0f);
	m_mapCollisionPos = VGet(0.0f, 0.0f, 0.0f);
	m_mapCorePos = VGet(-830.0f, 50.0f, 0.0f);

	//���C�g�֌W
	ChangeLightTypeDir(VGet(-1.0f, 0.0f, 0.0f));
	m_light = CreateDirLightHandle(VGet(1.0f, 0.0f, 0.0f));
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

	return std::shared_ptr<MapBase>();
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
