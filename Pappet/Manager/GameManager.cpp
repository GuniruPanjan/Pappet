#include "GameManager.h"
#include "Character/Player.h"
#include "Camera/Camera.h"
#include "Map/Map.h"


namespace
{
	
}

/// <summary>
/// �R���X�g���N�^
/// </summary>
GameManager::GameManager()
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
	
	pCamera->Init();
	pMap->Init();

	pPhysics = std::make_shared<MyLibrary::Physics>(pMap->GetCollisionMap());

	pPlayer->Init(pPhysics);

	
}

/// <summary>
/// �X�V����
/// </summary>
void GameManager::Update()
{
	pCamera->Update(*pPlayer);
	pMap->Update();
	pPlayer->Update();
	pPhysics->Update();
}

/// <summary>
/// �`�揈��
/// </summary>
void GameManager::Draw()
{
	pCamera->Draw();
	pMap->Draw();
	pPlayer->Draw();
}

/// <summary>
/// �I������
/// </summary>
void GameManager::End()
{
	pMap->End();
	pCamera->End();
	pPlayer->Finalize();
}
