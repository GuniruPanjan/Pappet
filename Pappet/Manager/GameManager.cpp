#include "GameManager.h"
#include "Character/Player.h"
#include "Camera/Camera.h"
#include "Manager/EnemyManager.h"
#include "Ui/Setting.h"
#include "Ui/UI.h"
#include "EffectManager.h"
#include "Manager/SEManager.h"
#include "Item/Tool.h"
#include "External/Font.h"
#include "Manager/FadeManager.h"

namespace
{
	//���[�v���Ɉ�񃊃Z�b�g����
	bool cOne = false;
	//�Q�[��BGM���Đ�����
	bool cGameBGMOne = false;
	//�{�XBGM���Đ�����
	bool cBossBGMOne = false;
	//�x���I����̃t�F�[�h�C������
	bool cFadeIn = false;
	//�`���[�g���A��
	bool cTutorial = false;
	//�`���[�g���A�����N���A�������̔���
	bool cClearTutorial = false;

	//�`���[�g���A�����N���A���Ē��Ԃ܂ł̎���
	float cTutorialTime = 0.0f;
	//��񂾂��s��
	bool cOneLoad = false;

	//���[�h��ʃ����_��
	int cLoad = 0;

	//�V���O���g��
	auto& cEffect = EffectManager::GetInstance();

	//�������o���Z�b�g
	bool cWinRest[] = {
		false,
	};
}

/// <summary>
/// �R���X�g���N�^
/// </summary>
GameManager::GameManager() :
	m_nowMap(eMapName::TutorialMap),
	m_load(0),
	m_loadNow1(-1),
	m_loadNow2(-1),
	m_deadInit(false),
	m_title(false),
	m_isLoading(false),
	m_restInit(false),
	m_fadeTitle(false),
	m_restMap(false)
{
	m_pUi = std::make_shared<UI>();
	m_pWeapon = std::make_shared<Weapon>();
	m_pShield = std::make_shared<Shield>();
	m_pArmor = std::make_shared<Armor>();
	m_pEnemyWeapon = std::make_shared<EnemyWeapon>();
	m_pCore = std::make_shared<CoreManager>();
	m_pBgm = std::make_shared<BgmManager>();
	m_pMap = std::make_shared<MapManager>();
	m_pSe = std::make_shared<SEManager>();
	m_pFont = std::make_shared<Font>();

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
GameManager::~GameManager()
{
	DeleteGraph(m_loadNow1);
	DeleteGraph(m_loadNow2);
}

/// <summary>
/// ������
/// </summary>
void GameManager::Init()
{
	cGameBGMOne = false;
	cBossBGMOne = false;

	m_loadNow1 = m_pUi->MyLoadGraph("Data/SceneBack/NowLoading1.png", 1, 1);
	m_loadNow2 = m_pUi->MyLoadGraph("Data/SceneBack/NowLoading2.png", 1, 1);
	
	//0���x���}�b�v�f�[�^
	//1��HARIBO�̃}�b�v�f�[�^
	//2���Z�J���h�}�b�v�f�[�^�����܂��ł��Ă��Ȃ�
	//6���`���[�g���A���}�b�v�f�[�^
	m_pMap->DataInit(6);

	m_pPhysics = std::make_shared<MyLibrary::Physics>(m_pMap->GetCollisionMap());

	m_pMap->Init(m_pPhysics);
	cEffect.Init();

	m_pPlayer = std::make_shared<Player>();
	m_pPlayer->Init(m_pPhysics, this, *m_pWeapon, *m_pShield, *m_pArmor, true, m_pMap->GetCollisionMap());
	m_pPlayer->SetMapNow(GetThisMapName());
	m_pEnemy = std::make_shared<EnemyManager>();
	m_pEnemy->Init(m_nowMap);
	m_pItem = std::make_shared<ItemManager>();
	m_pItem->Init();
	m_pSetting = std::make_shared<Setting>();
	m_pSetting->Init();
	m_pSe->CharaInit();
	m_pSe->BossInit();
	m_pSe->SceneInit();
	m_pUi->Init();
	m_pCore->Init();
	m_pPlayer->ChangeStatus();
	m_pMessage = std::make_shared<MessageManager>();
	m_pMessage->Init();

	//�t�H���g�̏�����
	m_pFont->FontInit(32);

	m_pFade = std::make_shared<FadeManager>();
	//�t�F�[�h�A�E�g�C��������
	m_pFade->Init();
	
	m_pTool = std::make_shared<Tool>();
	m_pTool->Init();

	m_pBgm->GameOneInit();

	if (m_pMap->GetStageName() == "stageTutorial")
	{
		cTutorial = true;
	}
	else
	{
		cTutorial = false;
	}

	//���[�h����
	m_load = 30;

	m_isLoading = true;      //�K���e�N���X�̌�ɂ���
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

	m_pPlayer->Init(m_pPhysics, this, *m_pWeapon, *m_pShield, *m_pArmor, false, m_pMap->GetCollisionMap());
	m_pEnemy->Init(m_nowMap);
	m_pItem->GameInit(m_pPhysics, this);
	m_pMessage->Init();
	m_pSetting->Init();
	m_pUi->Init();
	m_pPlayer->ChangeStatus();

	m_pTool->Init();

	m_load = 30;

	//�X�e�[�W�P�������ꍇ
	if (m_pMap->GetStageName() == "stage1")
	{
		m_pBgm->GameOneInit();
		cTutorial = false;

	}
	//�x���}�b�v�������ꍇ
	else if (m_pMap->GetStageName() == "stageRest")
	{
		m_pBgm->GameRestInit();
		cTutorial = false;

	}
	//�`���[�g���A���X�e�[�W�������ꍇ
	else if (m_pMap->GetStageName() == "stageTutorial")
	{
		cTutorial = true;
	}
}

/// <summary>
/// �X�V����
/// </summary>
void GameManager::Update()
{
	if (m_isLoading)
	{
		m_pEnemy->EnemyGenerate(m_pPhysics, this, *m_pEnemyWeapon, cTutorial);

		m_pCamera->Update(*m_pPlayer, m_pSetting->GetCamera());

		//�t�F�[�h�C������鏀��
		m_pFade->SetFade(255);
		m_pFade->SetIn(false);

		if (m_load == 0)
		{
			m_isLoading = false;
		}
	}
	else
	{
		//�t�F�[�h�C�����������Ă��Ȃ�
		if (!m_pFade->GetIn())
		{
			m_pFade->FadeIn(5);
		}
		
		//�t�F�[�h�A�E�g������
		if (m_pUi->GetReset())
		{
			//�t�F�[�h�A�E�g�\�ɂ���
			m_pFade->SetOut(false);
			m_pFade->FadeOut(5);
		}
		//�^�C�g���ɖ߂�ۂ̃t�F�[�h�A�E�g
		else if (m_fadeTitle)
		{
			//�t�F�[�h�A�E�g�\�ɂ���
			m_pFade->SetOut(false);
			m_pFade->FadeOut(5);
		}
		//���[�v���̃t�F�[�h�A�E�g
		else if (m_pPlayer->GetWarp() || m_pSetting->GetRestWarp() || m_pPlayer->GetSecondWarp())
		{
			//�t�F�[�h�A�E�g�\�ɂ���
			m_pFade->SetOut(false);
			m_pFade->FadeOut(10);
		}

		//�t�F�[�h�A�E�g�����Ń^�C�g���ɂ��ǂ�
		if (m_pFade->GetOut())
		{
			m_title = m_pSetting->GetTitle();
		}


		//�x��������
		if (m_restInit)
		{
			m_pEnemy->EnemyGenerate(m_pPhysics, this, *m_pEnemyWeapon, cTutorial);

			m_restInit = false;
		}

		//���[�v���ĂȂ���
		if (!m_pPlayer->GetWarp() && !cClearTutorial && !m_pSetting->GetRestWarp() && !m_pPlayer->GetSecondWarp())
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

			//���b�N�I�����ĂȂ���
			m_pCamera->Update(*m_pPlayer, m_pSetting->GetCamera());
			//�{�X�����ɓ�������{�X�����b�N�I������悤�ɂ���
			if (m_pMap->GetBossRoom() && m_pPlayer->GetLock() && !m_pEnemy->GetBossDead(GetThisMapName()))
			{
				m_pCamera->LockBossUpdate(*m_pPlayer, *m_pEnemy);
			}
			//���b�N�I�����Ă鎞
			else if (m_pPlayer->GetLock())
			{
				m_pCamera->LockUpdate(*m_pPlayer, *m_pEnemy);
			}

			m_pItem->Update(m_pPhysics, this, m_pPlayer->GetTaking());
			m_pMessage->Update(m_pPhysics, this, *m_pPlayer);

			//�`���[�g���A�����͓������~�߂�
			if (!m_pMessage->GetStop())
			{
				m_pEnemy->Update(m_pPhysics, this, *m_pCore, m_pPlayer->GetPos(), m_pCamera->GetDirection(), m_pPlayer->GetShieldPos(), !m_pPlayer->IsGetPlayerDead(), *m_pSe, *m_pEnemyWeapon, cTutorial);

				m_pPlayer->Update(*m_pWeapon, *m_pShield, *m_pArmor, *m_pEnemy, *m_pCore, m_pMap->GetRestPos(), *m_pTool, *m_pSe, m_pMap->GetBossRoom(), m_pEnemy->GetBossDead(GetThisMapName()), m_pPhysics);

			}
			
			m_pMap->JudgeUpdate();

			//�v���C���[�̃{�X�����ɓ��������
			m_pPlayer->SetBossStart(m_pMap->GetBossEnter());

			//�x�����ł��邩
			m_pPlayer->SetRest(m_pMap->GetRest());
			//�A�C�e�����E���邩�ǂ���
			m_pPlayer->SetItemPick(m_pItem->GetItemPick());
			//���b�Z�[�W��ǂ߂邩�ǂ���
			m_pPlayer->SetMessegePick(false);
			//�{�X�����ɓ�������
			m_pEnemy->SetBossRoom(m_pMap->GetBossRoom(), GetThisMapName());
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
			if (m_pEnemy->GetBossDead(GetThisMapName()))
			{
				m_pMap->CoreUpdate();

				m_pBgm->BossStopBGM();

				//�X�e�[�W�P�������ꍇ
				if (m_pMap->GetStageName() == "stage1")
				{
					//�N�}
					m_bossEnd.sBear = true;
					//�������o����񂾂��s��
					if (!cWinRest[0])
					{
						m_bossEnd.sWin = true;

						cWinRest[0] = true;
					}

				}
				//�X�e�[�W�`���[�g���A���������ꍇ
				else if (m_pMap->GetStageName() == "stageTutorial")
				{
					//�A�T�V��
					m_bossEnd.sAssassin = true;

					if (cTutorialTime >= 90.0f)
					{
						//�t�F�[�h�A�E�g�\�ɂ���
						m_pFade->SetOut(false);
						//�t�F�[�h�A�E�g������
						m_pFade->FadeOut(10);
						//�t�F�[�h�A�E�g������
						if (m_pFade->GetOut())
						{
							cClearTutorial = true;
						}
					}
					else
					{
						cTutorialTime++;
					}
				}

				//���[�v����
				if (m_pMap->GetCore() || m_pMap->GetMapSecond())
				{
					m_pPlayer->WarpMap(m_pMap->GetCore(), m_pMap->GetMapSecond());
				}
			}

			m_pMap->Update(m_pPhysics, m_pPlayer->GetWarp(), m_pPlayer->GetBossStart(), m_pEnemy->GetBossDead(GetThisMapName()));

			//���j���[���J��
			if (m_pPlayer->GetMenu() && !m_pSetting->GetEquipment() && !m_pSetting->GetItem() && !m_pSetting->GetSettingScene())
			{
				m_pSetting->MenuUpdate(*m_pPlayer, *m_pSe);

				//�^�C�g���ɖ߂�ۂ̃t�F�[�h�A�E�g��������
				if (m_pSetting->GetTitle())
				{
					m_fadeTitle = true;

				}

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
				m_pSetting->EquipmentDecisionUpdate(*m_pWeapon, *m_pShield, *m_pArmor, *m_pItem);
			}
			//���j���[���J����悤�ɂ���
			else if (!m_pPlayer->GetMenu() && !m_pSetting->GetEquipment() && !m_pSetting->GetItem())
			{
				m_pSetting->SetReturn(true);
			}

			//���S�����ꍇ
			if (m_pUi->GetReset() && m_pFade->GetOut())
			{
				//��񂾂����s
				if (m_deadInit == true)
				{
					m_pUi->End();
					m_pPlayer->GameInit(m_pPhysics, m_pMap->GetCollisionMap());
					m_pEnemy->GameInit(m_pPhysics, this, *m_pEnemyWeapon, m_deadInit, cTutorial);
					m_pMap->TriggerReset();
					m_pUi->Init();
					m_pPlayer->ChangeStatus();
					m_pTool->Init();

					m_pBgm->BossStopBGM();

					m_deadInit = false;

					m_load = 20;

					m_isLoading = true;
				}
			}
			else
			{
				m_deadInit = true;
			}

			//�x�������ꍇ
			if (m_pPlayer->GetRest())
			{
				//���x���A�b�v���������Ă��Ȃ��ꍇ
				if (!m_pSetting->GetLevel())
				{
					m_pSetting->RestUpdate(*m_pPlayer, *m_pCore, m_restMap, *m_pSe);
				}
				//���x���A�b�v����
				if (m_pSetting->GetLevel())
				{
					m_pSetting->LevelUpdate(*m_pPlayer, *m_pCore);
					m_pPlayer->ChangeStatus();

				}
				//�x������
				else if (m_pSetting->GetReset())
				{
					//�t�F�[�h�A�E�g�\�ɂ���
					m_pFade->SetOut(false);
					//�t�F�[�h�A�E�g���s
					m_pFade->FadeOut(20);
					//�t�F�[�h�A�E�g����
					if (m_pFade->GetOut())
					{
						//��񂾂����s
						m_pPlayer->GameInit(m_pPhysics, m_pMap->GetCollisionMap());
						m_pPlayer->ChangeStatus();
						m_pTool->Init();

						//�x���n�_�ȊO���Ə�����
						if (m_nowMap != 0)
						{
							m_pEnemy->GameInit(m_pPhysics, this, *m_pEnemyWeapon, m_pSetting->GetReset(), cTutorial);
						}

						m_pMap->TriggerReset();

						m_pSetting->SetReset(false);

						m_restInit = true;

						cFadeIn = true;
					}
				}

				//�����s
				if (cFadeIn)
				{
					//�t�F�[�h�C���\�ɂ���
					m_pFade->SetIn(false);
					//�t�F�[�h�C���J�n
					m_pFade->FadeIn(20);
					//�t�F�[�h�C������
					if (m_pFade->GetIn())
					{
						cFadeIn = false;
					}
				}

			}

			//�ݒ��ʂ��X�V
			if (m_pSetting->GetSettingScene())
			{
				m_pSetting->Update(*m_pSe);
			}

			cOne = false;

			//�`���[�g���A�����͓������~�߂�
			if (!m_pMessage->GetStop())
			{
				//�����X�V
				m_pPhysics->Update();
			}
			
		}
		//���[�v�����Ƃ�
		else if (m_pPlayer->GetWarp() || cClearTutorial || m_pSetting->GetRestWarp() || m_pPlayer->GetSecondWarp())
		{
			//�`���[�g���A�����N���A�����狭�����[�v
			if (cClearTutorial)
			{
				m_pPlayer->SetWarp(true);
			}

			if (m_pFade->GetOut() || cClearTutorial)
			{
				m_pMap->WarpUpdate(m_pPhysics, m_pPlayer->GetWarp(), m_pPlayer->GetSecondWarp(), false, m_pSetting->GetRestWarp());

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

					m_pPlayer->SetNotRest(false);
					m_pPlayer->SetWarp(false);
					m_pPlayer->SetSecondWarp(false);
					m_pSetting->SetRestWarp(false);

					cOne = true;
				}

				cClearTutorial = false;

				m_isLoading = true;
			}
		}

		cEffect.Update();
	}
	
}

/// <summary>
/// �`�揈��
/// </summary>
void GameManager::Draw()
{
	if (m_isLoading)
	{
		//��񂾂��s��
		if (!cOneLoad)
		{
			cLoad = GetRand(1);

			cOneLoad = true;
		}
		//�����_���Ń��[�h��ʂ�ς���
		if (cLoad == 0)
		{
			DrawGraph(0, 0, m_loadNow1, true);
		}
		else if (cLoad == 1)
		{
			DrawGraph(-100, 0, m_loadNow2, true);
		}

		DrawStringToHandle(0, 0, "NowLoading...", 0xffffff, m_pFont->GetHandle());

		// ���[�h�̐i�s�󋵂��v�Z
		int totalLoadTasks = m_load; // �����[�h�^�X�N�����擾����֐��i����j

		m_load--;

		int remainingLoadTasks = m_load; // �c��̃��[�h�^�X�N�����擾
		float progress = 1.0f - (static_cast<float>(remainingLoadTasks) / static_cast<float>(totalLoadTasks));

		// �o�[�̕`��
		int barWidth = 600; // �o�[�̕�
		int barHeight = 20; // �o�[�̍���
		int barX = 100; // �o�[��X���W
		int barY = 50; // �o�[��Y���W
		DrawBox(barX, barY, barX + static_cast<int>(barWidth * progress), barY + barHeight, 0x00ff00, TRUE); // �v���O���X�o�[
		DrawBox(barX, barY, barX + barWidth, barY + barHeight, 0xffffff, false);
	}
	else
	{
		cOneLoad = false;

		m_pMap->Draw();
		m_pEnemy->Draw(*m_pEnemyWeapon);

		//���b�N�I�����Ă鎞�ɕ`�悷��
		if (m_pPlayer->GetLock())
		{
			m_pCamera->Draw(*m_pUi);
		}

		m_pPlayer->Draw(*m_pArmor, m_pFont->GetHandle());
		m_pWeapon->Draw();
		m_pShield->Draw();

		//�{�X�����񂾔���
		if (m_pEnemy->GetBossDead(GetThisMapName()))
		{
			m_pMap->CoreDraw();
		}

		cEffect.Draw();

		//�G�l�~�[��UI�`��
		m_pEnemy->DrawUI(*m_pUi);

		m_pUi->Draw(*m_pPlayer, *m_pEnemy, *m_pSetting, *m_pMap, *m_pItem, *m_pWeapon, *m_pShield, *m_pArmor, *m_pTool, *m_pMessage);

		//���j���[��ʂ̔w�i���Â�����
		if (m_pSetting->GetEquipment() || m_pSetting->GetItem())
		{
			m_pSetting->MenuBackDraw();
		}


		//���j���[���
		if (m_pPlayer->GetMenu() && !m_pSetting->GetEquipment() && !m_pSetting->GetItem() && !m_pSetting->GetSettingScene())
		{
			m_pSetting->MenuDraw(m_pMessage->GetRB(), m_pMessage->GetLB(), m_pUi->GetBox());
		}
		//�������
		else if (m_pSetting->GetEquipment() && !m_pSetting->GetDecision())
		{
			m_pSetting->EquipmentDraw(*m_pPlayer);
			m_pUi->EquipmentDraw(*m_pWeapon, *m_pShield, *m_pArmor);
		}
		//�����I����ʕ`��
		else if (m_pSetting->GetDecision())
		{
			m_pSetting->EquipmentDecisionDraw(*m_pItem);
			EquipmentDraw();
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

		m_pItem->Draw();

		m_pMessage->DrawString();
		m_pMessage->Draw();

		//�`���[�g���A���}�b�v�������ꍇ
		if (m_nowMap == eMapName::TutorialMap)
		{
			m_pMessage->Tutorial(*m_pPlayer);
		}

		//�t�F�[�h�A�E�g�C���`��
		m_pFade->Draw();
		//���S�������`�悷��
		if (m_pPlayer->GetDead() && !m_pUi->GetReset())
		{
			m_pUi->DiedDraw(*m_pSe);
		}
		//�������̉��o��`�悷��
		if (m_bossEnd.sWin)
		{
			m_pUi->GetCoreDraw(*m_pSe);
			//�������o���I�������I������
			m_bossEnd.sWin = m_pUi->GetWinReset();
		}

		//�ݒ��ʂ�`��
		if (m_pSetting->GetSettingScene())
		{
			m_pSetting->Draw();

		}

		//���[�v�ł��Ȃ����̕`��
		m_pSetting->CaveatDraw();
	}

	m_pSetting->SettingDraw(*m_pSe);

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

		m_restMap = true;
	}
	//�}�b�v1�������ꍇ
	if (stageName == "stage1")
	{
		m_nowMap = eMapName::FirstMap;
		m_pPlayer->SetMapNow(FirstMap);
	}
	//�}�b�v2�������ꍇ
	if (stageName == "stage2")
	{
		m_nowMap = eMapName::SecondMap;
		m_pPlayer->SetMapNow(SecondMap);
	}
	//�`���[�g���A���������ꍇ
	if (stageName == "stageTutorial")
	{
		m_nowMap = eMapName::TutorialMap;
		m_pPlayer->SetMapNow(TutorialMap);
	}
}

/// <summary>
/// �I������
/// </summary>
void GameManager::End()
{
	m_pPlayer->End();
	m_pCamera->End();
	m_pMap->End(m_pPhysics, false);
	m_pSetting->End();
	m_pEnemy->End();
	m_pMessage->End();

	DeleteGraph(m_loadNow1);
	DeleteGraph(m_loadNow2);
}

/// <summary>
/// �����̕`�揈��
/// </summary>
void GameManager::EquipmentDraw()
{
	//�E����
	if (m_pSetting->GetSelect().right)
	{
		m_pUi->RightDraw(*m_pWeapon, *m_pItem);
	}
	//������
	else if (m_pSetting->GetSelect().left)
	{
		m_pUi->LeftDraw(*m_pShield, *m_pItem);
	}
	//�h��
	else if (m_pSetting->GetSelect().armor)
	{
		m_pUi->ArmorDraw(*m_pArmor, *m_pItem);
	}
}

const MyLibrary::LibVec3 GameManager::GetPlayerPos() const
{
	return MyLibrary::LibVec3();
}
