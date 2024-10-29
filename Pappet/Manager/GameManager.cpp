#include "GameManager.h"
#include "Character/Player.h"
#include "Camera/Camera.h"
#include "Map/Map.h"

//カメラの初期化で描画バグが発生する
//カメラのせいでマップとモデルの描画がバグる

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
	pMap->Init();

	pPhysics = std::make_shared<MyLibrary::Physics>(pMap->GetCollisionMap());

	//pCamera->Init();

	pPlayer->Init(pPhysics);

	
}

/// <summary>
/// 更新処理
/// </summary>
void GameManager::Update()
{
	pMap->Update();

	pPlayer->SetCameraAngle(pCamera->GetAngle().y);

	pPlayer->Update();

	pCamera->Update(*pPlayer);

	

	//物理更新
	pPhysics->Update();
}

/// <summary>
/// 描画処理
/// </summary>
void GameManager::Draw()
{
	pMap->Draw();
	pCamera->Draw();
	pPlayer->Draw();
}

/// <summary>
/// 終了処理
/// </summary>
void GameManager::End()
{
	pPlayer->End();
	pCamera->End();
	pMap->End();
}
