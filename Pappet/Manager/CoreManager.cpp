#include "CoreManager.h"
#include "Manager/EnemyManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
CoreManager::CoreManager()
{
	m_core = 0.0f;
}

/// <summary>
/// デストラクタ
/// </summary>
CoreManager::~CoreManager()
{
}

/// <summary>
/// 初期化処理
/// </summary>
void CoreManager::Init()
{
	m_core = 0.0f;
}

/// <summary>
/// 更新処理
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
