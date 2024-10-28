#include "GameManager.h"
#include "Character/Player.h"
#include "Camera/Camera.h"
#include "Map/Map.h"


namespace
{
	
}

/// <summary>
/// コンストラクタ
/// </summary>
GameManager::GameManager()
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
	
	pCamera->Init();
	pMap->Init();

	pPhysics = std::make_shared<MyLibrary::Physics>(pMap->GetCollisionMap());

	pPlayer->Init(pPhysics);

	
}

/// <summary>
/// 更新処理
/// </summary>
void GameManager::Update()
{
	pCamera->Update(*pPlayer);
	pMap->Update();
	pPlayer->Update();
	pPhysics->Update();
}

/// <summary>
/// 描画処理
/// </summary>
void GameManager::Draw()
{
	pCamera->Draw();
	pMap->Draw();
	pPlayer->Draw();
}

/// <summary>
/// 終了処理
/// </summary>
void GameManager::End()
{
	pMap->End();
	pCamera->End();
	pPlayer->Finalize();
}
