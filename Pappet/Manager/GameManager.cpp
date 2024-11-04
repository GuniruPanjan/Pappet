#include "GameManager.h"
#include "Character/Player.h"
#include "Camera/Camera.h"
#include "Map/Map.h"
#include "Manager/EnemyManager.h"

//カメラの初期化で描画バグが発生する
//カメラのせいでマップとモデルの描画がバグる

namespace
{
	
}

/// <summary>
/// コンストラクタ
/// </summary>
GameManager::GameManager() :
	m_nowMap(eMapName::FirstMap)
{
}

/// <summary>
/// デストラクタ
/// </summary>
GameManager::~GameManager()
{
}

/// <summary>
/// 初期化
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
/// 更新処理
/// </summary>
void GameManager::Update()
{
	m_pMap->Update();

	m_pPlayer->SetCameraAngle(m_pCamera->GetAngle().y);

	m_pPlayer->Update();

	m_pCamera->Update(*m_pPlayer);

	m_pEnemy->Update(m_pPhysics, this, m_pPlayer->GetPos(), m_pCamera->GetDirection(), !m_pPlayer->IsGetPlayerDead());

	//物理更新
	m_pPhysics->Update();
}

/// <summary>
/// 描画処理
/// </summary>
void GameManager::Draw()
{
	m_pMap->Draw();
	m_pCamera->Draw();
	m_pPlayer->Draw();
	m_pEnemy->Draw();
}

/// <summary>
/// 終了処理
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
