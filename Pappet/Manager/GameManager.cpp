#include "GameManager.h"
#include "Character/Player.h"
#include "Camera/Camera.h"
#include "Map/Map.h"

//�J�����̏������ŕ`��o�O����������
//�J�����̂����Ń}�b�v�ƃ��f���̕`�悪�o�O��

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
	pMap->Init();

	pPhysics = std::make_shared<MyLibrary::Physics>(pMap->GetCollisionMap());

	//pCamera->Init();

	pPlayer->Init(pPhysics);

	
}

/// <summary>
/// �X�V����
/// </summary>
void GameManager::Update()
{
	pMap->Update();

	pPlayer->SetCameraAngle(pCamera->GetAngle().y);

	pPlayer->Update();

	pCamera->Update(*pPlayer);

	

	//�����X�V
	pPhysics->Update();
}

/// <summary>
/// �`�揈��
/// </summary>
void GameManager::Draw()
{
	pMap->Draw();
	pCamera->Draw();
	pPlayer->Draw();
}

/// <summary>
/// �I������
/// </summary>
void GameManager::End()
{
	pPlayer->End();
	pCamera->End();
	pMap->End();
}
