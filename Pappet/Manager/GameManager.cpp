#include "GameManager.h"
#include "Character/Player.h"
#include "Camera/Camera.h"
#include "Map/Map.h"
#include "Manager/EnemyManager.h"

//�J�����̏������ŕ`��o�O����������
//�J�����̂����Ń}�b�v�ƃ��f���̕`�悪�o�O��

namespace
{
	
}

/// <summary>
/// �R���X�g���N�^
/// </summary>
GameManager::GameManager() :
	m_nowMap(eMapName::FirstMap)
{
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
GameManager::~GameManager()
{
}

/// <summary>
/// ������
/// </summary>
void GameManager::Init()
{
	m_pMap->Init();

	m_pPhysics = std::make_shared<MyLibrary::Physics>(m_pMap->GetCollisionMap());

	//pCamera->Init();

	m_pPlayer->Init(m_pPhysics);
	m_pEnemy = std::make_shared<EnemyManager>();
	m_pEnemy->Init("stage1");
}

/// <summary>
/// �X�V����
/// </summary>
void GameManager::Update()
{
	m_pMap->Update();

	m_pPlayer->SetCameraAngle(m_pCamera->GetAngle().y);

	m_pPlayer->Update();

	m_pCamera->Update(*m_pPlayer);

	m_pEnemy->Update(m_pPhysics, this, m_pPlayer->GetPos(), m_pCamera->GetDirection(), !m_pPlayer->IsGetPlayerDead());

	//�����X�V
	m_pPhysics->Update();
}

/// <summary>
/// �`�揈��
/// </summary>
void GameManager::Draw()
{
	m_pMap->Draw();
	m_pCamera->Draw();
	m_pPlayer->Draw();
	m_pEnemy->Draw();
}

/// <summary>
/// �I������
/// </summary>
void GameManager::End()
{
	m_pPlayer->End();
	m_pCamera->End();
	m_pMap->End();
}

const MyLibrary::LibVec3 GameManager::GetPlayerPos() const
{
	return MyLibrary::LibVec3();
}
