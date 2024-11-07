#include "GameManager.h"
#include "Character/Player.h"
#include "Character/CollidableNpc.h"
#include "Camera/Camera.h"
#include "Map/Map.h"
#include "Manager/EnemyManager.h"
#include "Ui/Setting.h"

//�J�����̏������ŕ`��o�O����������
//�J�����̂����Ń}�b�v�ƃ��f���̕`�悪�o�O��

namespace
{
	
}

/// <summary>
/// �R���X�g���N�^
/// </summary>
GameManager::GameManager() :
	m_nowMap(eMapName::FirstMap),
	m_title(false)
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
	m_pNpc->Init(m_pPhysics);
	m_pSetting->Init();
}

/// <summary>
/// �X�V����
/// </summary>
void GameManager::Update()
{
	m_pMap->Update();

	m_pPlayer->SetCameraAngle(m_pCamera->GetAngle().y);

	m_pPlayer->Update();
	//���b�N�I�����ĂȂ���
	if (!m_pPlayer->GetLock())
	{
		m_pCamera->Update(*m_pPlayer);
	}
	//���b�N�I�����Ă鎞
	else if (m_pPlayer->GetLock())
	{
		m_pCamera->LockUpdate(*m_pPlayer, *m_pEnemy);
	}
	

	m_pEnemy->Update(m_pPhysics, this, m_pPlayer->GetPos(), m_pCamera->GetDirection(), !m_pPlayer->IsGetPlayerDead());

	//���j���[���J��
	if (m_pPlayer->GetMenu())
	{
		m_pSetting->MenuUpdate();

		m_title = m_pSetting->GetTitle();

		m_pPlayer->SetMenu(m_pSetting->GetReturn());
	}
	//���j���[���J����悤�ɂ���
	else
	{
		m_pSetting->SetReturn(true);
	}

	//�����X�V
	m_pPhysics->Update();
}

/// <summary>
/// �`�揈��
/// </summary>
void GameManager::Draw()
{
	m_pMap->Draw();
	m_pPlayer->Draw();
	m_pEnemy->Draw();
	m_pNpc->Draw();

	if (m_pPlayer->GetMenu())
	{
		m_pSetting->MenuDraw();
	}

	m_pCamera->Draw();

}

/// <summary>
/// �I������
/// </summary>
void GameManager::End()
{
	m_pPlayer->End();
	m_pCamera->End();
	m_pMap->End();
	m_pSetting->End();
}

const MyLibrary::LibVec3 GameManager::GetPlayerPos() const
{
	return MyLibrary::LibVec3();
}
