#include "MapBase.h"
#include "Manager/HandleManager.h"

namespace
{
	//�V���O���g��
	auto& handle = HandleManager::GetInstance();
}

/// <summary>
/// �R���X�g���N�^
/// </summary>
MapBase::MapBase() :
	m_light(0),
	m_handle(-1),
	m_collisionHandle(-1),
	m_coreHandle(-1),
	m_floorHandle(-1),
	m_wallHandle(-1),
	m_ceilingHandle(-1),
	m_restObjectHandle(-1),
	m_effect(100),
	m_size(0.0f),
	m_width(0.0f),
	m_hight(0.0f),
	m_depth(0.0f),
	m_angle(0.0f),
	m_stageName(""),
	m_mapPos(VGet(0.0f,0.0f,0.0f)),
	m_mapCollisionPos(VGet(0.0f,0.0f,0.0f)),
	m_mapCorePos(VGet(0.0f,0.0f,0.0f)),
	m_mapRestPos(),
	m_mapBossEnterPos(),
	m_mapSecondArea(),
	m_mapCoreCollisionePos()
{
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
MapBase::~MapBase()
{
	//���������
	MV1DeleteModel(m_handle);
	MV1DeleteModel(m_collisionHandle);
	MV1DeleteModel(m_coreHandle);
	MV1DeleteModel(m_floorHandle);
	MV1DeleteModel(m_wallHandle);
	MV1DeleteModel(m_ceilingHandle);
	MV1DeleteModel(m_restObjectHandle);
}

void MapBase::Finalize(std::shared_ptr<MyLibrary::Physics> physics)
{
	m_pSearch->Finalize(physics);
	m_pBossRoom->Finalize(physics);
	m_pCore->Finalize(physics);
	m_pRect->Finalize(physics);
	m_pRectTrigger->Finalize(physics);
	m_pMapSecond->Finalize(physics);
}

/// <summary>
/// ���f����ǂݍ���
/// </summary>
/// <param name="mapPath">���f���̃p�X</param>
/// <param name="collisionPath">�R���W�����̃��f���p�X</param>
void MapBase::LoadData(std::string mapPath, std::string collisionPath, std::string corePath)
{
	m_handle = handle.GetModelHandle(mapPath);
	m_collisionHandle = handle.GetModelHandle(collisionPath);
	m_coreHandle = handle.GetModelHandle(corePath);
}

/// <summary>
/// ���f���̃p�[�c��ǂݍ���
/// </summary>
/// <param name="floorPath">��</param>
/// <param name="wallPath">��</param>
/// <param name="ceilingPath">�V��</param>
/// <param name="restPath">�x��</param>
void MapBase::LoadPartsData(std::string floorPath, std::string wallPath, std::string ceilingPath, std::string restPath)
{
	m_floorHandle = handle.GetModelHandle(floorPath);
	m_wallHandle = handle.GetModelHandle(wallPath);
	m_ceilingHandle = handle.GetModelHandle(ceilingPath);
	m_restObjectHandle = handle.GetModelHandle(restPath);
}

/// <summary>
/// ���G�����铖���蔻����쐬
/// </summary>
/// <param name="radius">���a</param>
/// <param name="posX">X���W</param>
/// <param name="posY">Y���W</param>
/// <param name="posZ">Z���W</param>
void MapBase::InitSearch(float radius, MyLibrary::LibVec3 pos)
{
	m_pSearch = std::make_shared<SearchObject>(radius);
	m_pSearch->Init(m_pPhysics, pos, false, true);
}

/// <summary>
/// �{�X�����̓����������쐬
/// </summary>
/// <param name="radius">���a</param>
/// <param name="pos">�|�W�V����</param>
void MapBase::InitBossRoom(float radius, MyLibrary::LibVec3 pos)
{
	m_pBossRoom = std::make_shared<SearchObject>(radius);
	m_pBossRoom->Init(m_pPhysics, pos, false, false, false, true);
}

/// <summary>
/// ��`�̓����蔻����쐬
/// </summary>
/// <param name="width">��</param>
/// <param name="hight">����</param>
/// <param name="depth">���s</param>
void MapBase::InitRect(float width, float hight, float depth, MyLibrary::LibVec3 pos)
{
	m_pRect = std::make_shared<RectObject>(width, hight, depth);
	m_pRect->Init(m_pPhysics, pos, true);
}

/// <summary>
/// �R�A�̓����蔻����쐬
/// </summary>
/// <param name="radius">���a</param>
/// <param name="pos">�|�W�V����</param>
void MapBase::InitCore(float radius, MyLibrary::LibVec3 pos)
{
	m_pCore = std::make_shared<SearchObject>(radius);
	m_pCore->Init(m_pPhysics, pos, false, false, false, false, true);
}

/// <summary>
/// ��ڂ̃}�b�v�ɍs�����߂̔�����쐬
/// </summary>
/// <param name="radius"></param>
/// <param name="pos"></param>
void MapBase::InitMapSecond(float radius, MyLibrary::LibVec3 pos)
{
	m_pMapSecond = std::make_shared<SearchObject>(radius);
	m_pMapSecond->Init(m_pPhysics, pos, false, false, false, false, true);
	
}

/// <summary>
/// �{�X����������̓����蔻����쐬
/// </summary>
/// <param name="width"></param>
/// <param name="hight"></param>
/// <param name="depth"></param>
/// <param name="pos"></param>
void MapBase::InitBossEnter(float width, float hight, float depth, MyLibrary::LibVec3 pos)
{
	m_pRectTrigger = std::make_shared<RectObjectTrigger>(width, hight, depth);
	m_pRectTrigger->Init(m_pPhysics, pos, true);
}

/// <summary>
/// ���f���̃p�[�c���ɕ`�悷��Ƃ��Ɏg��
/// </summary>
/// <param name="handle"></param>
/// <param name="scale"></param>
/// <param name="position"></param>
/// <param name="rotation"></param>
void MapBase::PartDrawSet(int handle, VECTOR scale, VECTOR position, VECTOR rotation)
{
	MV1SetScale(handle, scale);
	MV1SetPosition(handle, position);
	MV1SetRotationXYZ(handle, rotation);
	MV1DrawModel(handle);
}

/// <summary>
/// �I������
/// </summary>
/// <param name="physics"></param>
void MapBase::MapEnd(std::shared_ptr<MyLibrary::Physics> physics)
{
	handle.Clear();
}
