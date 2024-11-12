#include "MapManager.h"
#include "Map/MapFirst.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
MapManager::MapManager() :
	m_pMap(nullptr)
{

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
MapManager::~MapManager()
{
}

/// <summary>
/// �f�[�^�̏�����
/// </summary>
void MapManager::DataInit()
{
	//�ŏ��̃V�[���̃��������������
	m_pMap = std::make_shared<MapFirst>();

	m_pMap->DataInit();
}

/// <summary>
/// ����������
/// </summary>
void MapManager::Init(std::shared_ptr<MyLibrary::Physics> physics)
{
	m_pMap->Init(physics);
}

/// <summary>
/// �X�V����
/// </summary>
void MapManager::Update(std::shared_ptr<MyLibrary::Physics> physics)
{
	std::shared_ptr<MapBase> pNext = m_pMap->Update();
	if (pNext != m_pMap)
	{
		//���݂̃}�b�v�̏I������
		m_pMap->End(physics);

		//Update���Ԃ����V�����V�[���̊J�n�������s��
		m_pMap = pNext;
		m_pMap->DataInit();
		m_pMap->Init(physics);
	}
}

/// <summary>
/// �����������̔���X�V����
/// </summary>
void MapManager::JudgeUpdate()
{
	m_pMap->JudgeUpdate();
}

/// <summary>
/// �`�揈��
/// </summary>
void MapManager::Draw()
{
	m_pMap->Draw();
}

/// <summary>
/// �I������
/// </summary>
void MapManager::End(std::shared_ptr<MyLibrary::Physics> physics)
{
	m_pMap->End(physics);
}
