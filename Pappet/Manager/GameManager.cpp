#include "GameManager.h"
#include "Character/Player.h"
#include "Character/CollidableNpc.h"
#include "Camera/Camera.h"
#include "Map/Map.h"
#include "Manager/EnemyManager.h"
#include "Ui/Setting.h"
#include "Ui/UI.h"

//カメラの初期化で描画バグが発生する
//カメラのせいでマップとモデルの描画がバグる

namespace
{
	//マップをワープする準備
	bool cWarp = false;
	//ワープ時に一回リセットする
	bool cOne = false;
}

/// <summary>
/// コンストラクタ
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
	m_pMap->DataInit();

	m_pPhysics = std::make_shared<MyLibrary::Physics>(m_pMap->GetCollisionMap());

	m_pMap->Init(m_pPhysics);
	//pCamera->Init();

	m_pPlayer->Init(m_pPhysics, *m_pWeapon, *m_pShield, *m_pArmor, true);
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
}

/// <summary>
/// ゲーム中での初期化処理
/// </summary>
void GameManager::GameInit()
{
	//マップを変える時の処理
	ChangeStage(m_pMap->GetStageName());

	m_pPhysics = std::make_shared<MyLibrary::Physics>(m_pMap->GetCollisionMap());

	m_pMap->Init(m_pPhysics);

	m_pPlayer->Init(m_pPhysics, *m_pWeapon, *m_pShield, *m_pArmor, false);
	m_pEnemy->Init(m_pMap->GetStageName());
	m_pItem->Init(m_pMap->GetStageName());
	m_pNpc->Init(m_pPhysics);
	m_pSetting->Init();
	m_pUi->Init();
	m_pPlayer->ChangeStatus();
}

/// <summary>
/// 更新処理
/// </summary>
void GameManager::Update()
{
	//ワープしてない時
	if (!m_pPlayer->GetWarp())
	{
		m_pPlayer->SetCameraAngle(m_pCamera->GetAngle().y);

		m_pPlayer->Update(*m_pWeapon, *m_pShield, *m_pArmor, *m_pEnemy, *m_pCore);
		//ロックオンしてない時
		if (!m_pPlayer->GetLock())
		{
			m_pCamera->Update(*m_pPlayer);
		}
		//ロックオンしてる時
		else if (m_pPlayer->GetLock())
		{
			m_pCamera->LockUpdate(*m_pPlayer, *m_pEnemy);
		}

		m_pItem->Update(m_pPhysics, this, m_pPlayer->GetTaking());
		m_pEnemy->Update(m_pPhysics, this, *m_pCore, m_pPlayer->GetPos(), m_pCamera->GetDirection(), m_pPlayer->GetShieldPos(), !m_pPlayer->IsGetPlayerDead(), m_init);

		m_pMap->JudgeUpdate();

		//プレイヤーのボス部屋に入り口判定
		m_pPlayer->SetBossStart(m_pMap->GetBossEnter());

		//休息ができるか
		m_pPlayer->SetRest(m_pMap->GetRest());
		//アイテムを拾えるかどうか
		m_pPlayer->SetItemPick(m_pItem->GetItemPick());
		//ボス部屋に入ったか
		m_pEnemy->SetBossRoom(m_pMap->GetBossRoom());

		//ボスが死んだ判定
		if (m_pEnemy->GetBossDead())
		{
			cWarp = true;
			m_pMap->CoreUpdate();

			if (m_pMap->GetCore())
			{
				m_pPlayer->WarpMap();
			}
		}

		m_pMap->Update(m_pPhysics, m_pPlayer->GetWarp(), m_pPlayer->GetBossStart(), m_pEnemy->GetBossDead());

		//メニューを開く
		if (m_pPlayer->GetMenu() && !m_pSetting->GetEquipment() && !m_pSetting->GetItem())
		{
			m_pSetting->MenuUpdate();

			m_title = m_pSetting->GetTitle();

			m_pPlayer->SetMenu(m_pSetting->GetReturn());
		}
		//装備画面を開く
		else if (m_pSetting->GetEquipment() && !m_pSetting->GetDecision())
		{
			m_pSetting->EquipmentUpdate();
		}
		//アイテム画面を開く
		else if (m_pSetting->GetItem() && !m_pSetting->GetDecision())
		{
			m_pSetting->ItemBoxUpdate();
		}
		//装備選択画面更新
		else if (m_pSetting->GetDecision())
		{
			m_pSetting->EquipmentDecisionUpdate(*m_pWeapon, *m_pShield, *m_pArmor);
			EquipmentUpdate();
		}
		//メニューを開けるようにする
		else if(!m_pPlayer->GetMenu() && !m_pSetting->GetEquipment() && !m_pSetting->GetItem())
		{
			m_pSetting->SetReturn(true);
		}

		//装備画面とアイテム画面の変更更新
		if (m_pSetting->GetEquipment() || m_pSetting->GetItem())
		{
			if (!m_pSetting->GetDecision())
			{
				m_pSetting->MenuChange();
			}
			
		}

		//死亡した場合
		if (m_pUi->GetReset())
		{
			//一回だけ実行
			if (m_deadInit == true)
			{
				m_pPlayer->GameInit(m_pPhysics);
				m_pEnemy->GameInit(m_pPhysics, this, m_deadInit);
				m_pMap->TriggerReset();
				m_pUi->Init();

				m_deadInit = false;
			}
		}
		else
		{
			m_deadInit = true;
		}

		//休息した場合
		if (m_pPlayer->GetRest())
		{
			//レベルアップ処理
			if (m_pSetting->GetLevel())
			{
				m_pSetting->LevelUpdate(*m_pPlayer, *m_pCore);
				m_pPlayer->ChangeStatus();

			}
			//休息処理
			else
			{
				//一回だけ実行
				if (m_init == true)
				{
					m_pPlayer->GameInit(m_pPhysics);
					m_pPlayer->ChangeStatus();

					//休息地点以外だと初期化
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

		//物理更新
		m_pPhysics->Update();
	}
	//ワープしたとき
	else if (m_pPlayer->GetWarp())
	{
		m_pMap->WarpUpdate(m_pPhysics, m_pPlayer->GetWarp());

		//一回だけ実行
		if (!cOne)
		{
			m_pEnemy->End();
			GameInit();

			m_pPlayer->SetWarp(false);

			cOne = true;
		}
	}

	
}

/// <summary>
/// 描画処理
/// </summary>
void GameManager::Draw()
{
	m_pMap->Draw();
	m_pPlayer->Draw(*m_pArmor);
	m_pWeapon->Draw();
	m_pShield->Draw();
	m_pEnemy->Draw();
	m_pNpc->Draw();

	//ボスが死んだ判定
	if (cWarp)
	{
		m_pMap->CoreDraw();
	}

	m_pCamera->Draw();

	//メニューの背景描画
	if (m_pPlayer->GetMenu())
	{
		m_pSetting->MenuBackDraw();
	}

	//メニュー画面
	if (m_pPlayer->GetMenu() && !m_pSetting->GetEquipment() && !m_pSetting->GetItem())
	{
		m_pSetting->MenuDraw();
	}
	//装備画面
	else if (m_pSetting->GetEquipment() && !m_pSetting->GetDecision())
	{
		m_pSetting->EquipmentDraw();
		m_pUi->EquipmentDraw(*m_pWeapon, *m_pShield, *m_pArmor);
	}
	//アイテム画面
	else if (m_pSetting->GetItem() && !m_pSetting->GetDecision())
	{
		m_pSetting->ItemBoxDraw();
	}
	//装備選択画面描画
	else if (m_pSetting->GetDecision())
	{
		m_pSetting->EquipmentDecisionDraw();
		EquipmentDraw();
	}

	//アイテム画面と装備画面の変更描画
	if (m_pSetting->GetEquipment() || m_pSetting->GetItem())
	{
		if (!m_pSetting->GetDecision())
		{
			m_pSetting->MenuChangeDraw();
		}
	}

	//休息画面描画
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
/// ステージを変える時の処理
/// </summary>
/// <param name="stageName">ステージ名</param>
void GameManager::ChangeStage(const char* stageName)
{
	//休息地点だった場合
	if (stageName == "stageRest")
	{
		m_nowMap = eMapName::RestMap;
		m_pPlayer->SetMapNow(RestMap);
	}
	//マップ1だった場合
	else if (stageName == "stage1")
	{
		m_nowMap == eMapName::FirstMap;
		m_pPlayer->SetMapNow(FirstMap);
	}
}

/// <summary>
/// 終了処理
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
/// 装備の更新処理
/// </summary>
void GameManager::EquipmentUpdate()
{
	//右装備
	if (m_pSetting->GetSelect().right)
	{

	}
	//左装備
	else if (m_pSetting->GetSelect().left)
	{

	}
	//防具
	else if (m_pSetting->GetSelect().armor)
	{

	}
}

/// <summary>
/// 装備の描画処理
/// </summary>
void GameManager::EquipmentDraw()
{
	//右装備
	if (m_pSetting->GetSelect().right)
	{
		m_pUi->RightDraw(*m_pWeapon);
	}
	//左装備
	else if (m_pSetting->GetSelect().left)
	{
		m_pUi->LeftDraw(*m_pShield);
	}
	//防具
	else if (m_pSetting->GetSelect().armor)
	{
		m_pUi->ArmorDraw(*m_pArmor);
	}
}

const MyLibrary::LibVec3 GameManager::GetPlayerPos() const
{
	return MyLibrary::LibVec3();
}
