#include "CoreManager.h"
#include "Manager/EnemyManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
CoreManager::CoreManager()
{
	m_core = 0.0f;
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
CoreManager::~CoreManager()
{
}

/// <summary>
/// ����������
/// </summary>
void CoreManager::Init()
{
	m_core = 0.0f;
}

/// <summary>
/// �X�V����
/// </summary>
void CoreManager::Update(int core)
{
	m_core = core;
}

void CoreManager::DeadUpdate()
{
}

void CoreManager::End()
{
}
