#include "GameManager.h"
#include "Character/Player.h"
#include "Character/CollidableNpc.h"
#include "Camera/Camera.h"
#include "Map/Map.h"
#include "Manager/EnemyManager.h"
#include "Ui/Setting.h"
#include "Ui/UI.h"
#include "EffectManager.h"

//�J�����̏������ŕ`��o�O����������
//�J�����̂����Ń}�b�v�ƃ��f���̕`�悪�o�O��

namespace
{
	//�}�b�v�����[�v���鏀��
	bool cWarp = false;
	//���[�v���Ɉ�񃊃Z�b�g����
	bool cOne = false;
	//�Q�[��BGM���Đ�����
	bool cGameBGMOne = false;
	//�{�XBGM���Đ�����
	bool cBossBGMOne = false;

	//�V���O���g��
	auto& cEffect = EffectManager::GetInstance();
}

/// <summary>
/// �R���X�g���N�^
/// </summary>
GameManager::GameManager() :
	m_nowMap(eMapName::FirstMap),
	m_deadInit(false),
	m_init(false),
	m_title(false)
{
	m_pUi = std::make_shared<UI>();
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
	cGameBGMOne = false;
	cBossBGMOne = false;

	m_pMap->DataInit();

	m_pPhysics = std::make_shared<MyLibrary::Physics>(m_pMap->GetCollisionMap());

	m_pMap->Init(m_pPhysics);
	cEffect.Init();
	//pCamera->Init();

	m_pPlayer->Init(m_pPhysics, this, *m_pWeapon, *m_pShield, *m_pArmor, true);
	m_pPlayer->SetMapNow(FirstMap);
	m_pEnemy = std::make_shared<EnemyManager>();
	m_pEnemy->Init(m_pMap->GetStageName());
	m_pItem = std::make_shared<ItemManager>();
	m_pItem->Init(m_pMap->GetStageName());
	m_pNpc->Init(m_pPhysics);
	m_pSetting->Init();
	m_pUi->Init();
	m_pCore->Init();
	m_pPlayer->ChangeStatus();

	m_pBgm->GameOneInit();
}

/// <summary>
/// �Q�[�����ł̏���������
/// </summary>
void GameManager::GameInit()
{
	cGameBGMOne = false;
	cBossBGMOne = false;

	//�}�b�v��ς��鎞�̏���
	ChangeStage(m_pMap->GetStageName());

	m_pPhysics = std::make_shared<MyLibrary::Physics>(m_pMap->GetCollisionMap());

	m_pMap->Init(m_pPhysics);
	cEffect.Init();

	m_pPlayer->Init(m_pPhysics, this, *m_pWeapon, *m_pShield, *m_pArmor, false);
	m_pEnemy->Init(m_pMap->GetStageName());
	//m_pEnemy->GameInit(m_pPhysics, this, true);
	//m_pItem->Init(m_pMap->GetStageName());
	m_pItem->GameInit(m_pPhysics, this);
	m_pNpc->Init(m_pPhysics);
	m_pSetting->Init();
	m_pUi->Init();
	m_pPlayer->ChangeStatus();

	//�X�e�[�W�P�������ꍇ
	if (m_pMap->GetStageName() == "stage1")
	{
		m_pBgm->GameOneInit();
	}
	//�x���}�b�v�������ꍇ
	else if (m_pMap->GetStageName() == "stageRest")
	{
		m_pBgm->GameRestInit();
	}
}

/// <summary>
/// �X�V����
/// </summary>
void GameManager::Update()
{
	//���[�v���ĂȂ���
	if (!m_pPlayer->GetWarp())
	{
		m_pBgm->Update(m_pSetting->GetVolume());

		//���Đ�
		if (!cGameBGMOne && !m_pMap->GetBossRoom())
		{
			m_pBgm->GameBGM();

			cBossBGMOne = false;
			cGameBGMOne = true;
		}

		m_pPlayer->SetCameraAngle(m_pCamera->GetAngle().y);

		m_pPlayer->Update(*m_pWeapon, *m_pShield, *m_pArmor, *m_pEnemy, *m_pCore, m_pMap->GetRestPos());
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

		m_pItem->Update(m_pPhysics, this, m_pPlayer->GetTaking());
		m_pEnemy->Update(m_pPhysics, this, *m_pCore, m_pPlayer->GetPos(), m_pCamera->GetDirection(), m_pPlayer->GetShieldPos(), !m_pPlayer->IsGetPlayerDead(), m_init);

		m_pMap->JudgeUpdate();

		//�v���C���[�̃{�X�����ɓ��������
		m_pPlayer->SetBossStart(m_pMap->GetBossEnter());

		//�x�����ł��邩
		m_pPlayer->SetRest(m_pMap->GetRest());
		//�A�C�e�����E���邩�ǂ���
		m_pPlayer->SetItemPick(m_pItem->GetItemPick());
		//�{�X�����ɓ�������
		m_pEnemy->SetBossRoom(m_pMap->GetBossRoom());
		//�{�X�����ɓ�������
		if (m_pMap->GetBossRoom())
		{
			if (!cBossBGMOne)
			{
				//�{�X��BGM�Đ�
				m_pBgm->BossBGM();

				cGameBGMOne = false;
				cBossBGMOne = true;
			}
			
		}

		//�{�X�����񂾔���
		if (m_pEnemy->GetBossDead())
		{
			cWarp = true;
			m_pMap->CoreUpdate();

			m_pBgm->BossStopBGM();

			//�X�e�[�W�P�������ꍇ
			if (m_pMap->GetStageName() == "stage1")
			{
				//�N�}
				m_bossEnd.sBear = true;
			}

			if (m_pMap->GetCore())
			{
				m_pPlayer->WarpMap();
			}
		}

		m_pMap->Update(m_pPhysics, m_pPlayer->GetWarp(), m_pPlayer->GetBossStart(), m_pEnemy->GetBossDead());

		//���j���[���J��
		if (m_pPlayer->GetMenu() && !m_pSetting->GetEquipment() && !m_pSetting->GetItem())
		{
			m_pSetting->MenuUpdate();

			m_title = m_pSetting->GetTitle();

			m_pPlayer->SetMenu(m_pSetting->GetReturn());
		}
		//������ʂ��J��
		else if (m_pSetting->GetEquipment() && !m_pSetting->GetDecision())
		{
			m_pSetting->EquipmentUpdate();
		}
		//�A�C�e����ʂ��J��
		else if (m_pSetting->GetItem() && !m_pSetting->GetDecision())
		{
			m_pSetting->ItemBoxUpdate();
		}
		//�����I����ʍX�V
		else if (m_pSetting->GetDecision())
		{
			m_pSetting->EquipmentDecisionUpdate(*m_pWeapon, *m_pShield, *m_pArmor);
			EquipmentUpdate();
		}
		//���j���[���J����悤�ɂ���
		else if(!m_pPlayer->GetMenu() && !m_pSetting->GetEquipment() && !m_pSetting->GetItem())
		{
			m_pSetting->SetReturn(true);
		}

		//������ʂƃA�C�e����ʂ̕ύX�X�V
		if (m_pSetting->GetEquipment() || m_pSetting->GetItem())
		{
			if (!m_pSetting->GetDecision())
			{
				m_pSetting->MenuChange();
			}
			
		}

		//���S�����ꍇ
		if (m_pUi->GetReset())
		{
			//��񂾂����s
			if (m_deadInit == true)
			{
				cEffect.End();
				m_pPlayer->GameInit(m_pPhysics);
				m_pEnemy->GameInit(m_pPhysics, this, m_deadInit);
				m_pMap->TriggerReset();
				m_pUi->Init();
				m_pPlayer->ChangeStatus();

				m_pBgm->BossStopBGM();

				m_deadInit = false;
			}
		}
		else
		{
			m_deadInit = true;
		}

		//�x�������ꍇ
		if (m_pPlayer->GetRest())
		{
			//���x���A�b�v����
			if (m_pSetting->GetLevel())
			{
				m_pSetting->LevelUpdate(*m_pPlayer, *m_pCore);
				m_pPlayer->ChangeStatus();

			}
			//�x������
			else
			{
				//��񂾂����s
				if (m_init == true)
				{
					m_pPlayer->GameInit(m_pPhysics);
					m_pPlayer->ChangeStatus();

					//�x���n�_�ȊO���Ə�����
					if (m_nowMap != 0)
					{
						m_pEnemy->GameInit(m_pPhysics, this, m_init);
					}

					m_pMap->TriggerReset();

					m_init = false;
				}

				m_pSetting->RestUpdate(*m_pPlayer, *m_pCore);
			}

		}
		else
		{
			m_init = true;
		}

		cOne = false;

		//�����X�V
		m_pPhysics->Update();
	}
	//���[�v�����Ƃ�
	else if (m_pPlayer->GetWarp())
	{
		m_pMap->WarpUpdate(m_pPhysics, m_pPlayer->GetWarp());

		//��񂾂����s
		if (!cOne)
		{
			cEffect.End();
			m_pEnemy->End();
			m_pItem->End();
			m_pBgm->GameEnd();
			m_pSetting->End();
			m_pUi->End();
			GameInit();

			m_pPlayer->SetWarp(false);

			cOne = true;
		}
	}

	cEffect.Update();
	
}

/// <summary>
/// �`�揈��
/// </summary>
void GameManager::Draw()
{
	m_pMap->Draw();
	m_pPlayer->Draw(*m_pArmor);
	m_pWeapon->Draw();
	m_pShield->Draw();
	m_pEnemy->Draw();
	m_pNpc->Draw();

	//�{�X�����񂾔���
	if (cWarp)
	{
		m_pMap->CoreDraw();
	}

	m_pCamera->Draw();

	cEffect.Draw();

	m_pUi->Draw(*m_pPlayer, *m_pEnemy, *m_pSetting, *m_pMap, *m_pItem);

	//���j���[�̔w�i�`��
	if (m_pPlayer->GetMenu())
	{
		m_pSetting->MenuBackDraw();
	}

	//���j���[���
	if (m_pPlayer->GetMenu() && !m_pSetting->GetEquipment() && !m_pSetting->GetItem())
	{
		m_pSetting->MenuDraw();
	}
	//�������
	else if (m_pSetting->GetEquipment() && !m_pSetting->GetDecision())
	{
		m_pSetting->EquipmentDraw();
		m_pUi->EquipmentDraw(*m_pWeapon, *m_pShield, *m_pArmor);
	}
	//�A�C�e�����
	else if (m_pSetting->GetItem() && !m_pSetting->GetDecision())
	{
		m_pSetting->ItemBoxDraw();
	}
	//�����I����ʕ`��
	else if (m_pSetting->GetDecision())
	{
		m_pSetting->EquipmentDecisionDraw();
		EquipmentDraw();
	}

	//�A�C�e����ʂƑ�����ʂ̕ύX�`��
	if (m_pSetting->GetEquipment() || m_pSetting->GetItem())
	{
		if (!m_pSetting->GetDecision())
		{
			m_pSetting->MenuChangeDraw();
		}
	}

	//�x����ʕ`��
	if (m_pPlayer->GetRest())
	{
		if (m_pSetting->GetLevel())
		{
			m_pSetting->LevelUpDraw(*m_pPlayer, *m_pCore);
		}
		else
		{
			m_pSetting->RestDraw(m_pPlayer->GetBigRest());
		}
	}


	if (m_pPlayer->GetDead())
	{
		m_pUi->DiedDraw();
	}

	m_pItem->Draw();
}

/// <summary>
/// �X�e�[�W��ς��鎞�̏���
/// </summary>
/// <param name="stageName">�X�e�[�W��</param>
void GameManager::ChangeStage(const char* stageName)
{
	//�x���n�_�������ꍇ
	if (stageName == "stageRest")
	{
		m_nowMap = eMapName::RestMap;
		m_pPlayer->SetMapNow(RestMap);
	}
	//�}�b�v1�������ꍇ
	if (stageName == "stage1")
	{
		m_nowMap = eMapName::FirstMap;
		m_pPlayer->SetMapNow(FirstMap);
	}
}

/// <summary>
/// �I������
/// </summary>
void GameManager::End()
{
	m_pPlayer->End();
	m_pCamera->End();
	m_pMap->End(m_pPhysics);
	m_pSetting->End();
	m_pEnemy->End();
}

/// <summary>
/// �����̍X�V����
/// </summary>
void GameManager::EquipmentUpdate()
{
	//�E����
	if (m_pSetting->GetSelect().right)
	{

	}
	//������
	else if (m_pSetting->GetSelect().left)
	{

	}
	//�h��
	else if (m_pSetting->GetSelect().armor)
	{

	}
}

/// <summary>
/// �����̕`�揈��
/// </summary>
void GameManager::EquipmentDraw()
{
	//�E����
	if (m_pSetting->GetSelect().right)
	{
		m_pUi->RightDraw(*m_pWeapon);
	}
	//������
	else if (m_pSetting->GetSelect().left)
	{
		m_pUi->LeftDraw(*m_pShield);
	}
	//�h��
	else if (m_pSetting->GetSelect().armor)
	{
		m_pUi->ArmorDraw(*m_pArmor);
	}
}

const MyLibrary::LibVec3 GameManager::GetPlayerPos() const
{
	return MyLibrary::LibVec3();
}
