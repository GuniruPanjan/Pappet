#include "GameManager.h"
#include "Character/Player.h"
#include "Character/CollidableNpc.h"
#include "Camera/Camera.h"
#include "Map/Map.h"
#include "Manager/EnemyManager.h"
#include "Ui/Setting.h"

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
	m_init(false),
	m_title(false)
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
	m_pMap->DataInit();

	m_pPhysics = std::make_shared<MyLibrary::Physics>(m_pMap->GetCollisionMap());

	m_pMap->Init(m_pPhysics);
	//pCamera->Init();

	m_pPlayer->Init(m_pPhysics);
	m_pEnemy = std::make_shared<EnemyManager>();
	m_pEnemy->Init("stage1");
	m_pNpc->Init(m_pPhysics);
	m_pSetting->Init();
}

/// <summary>
/// ゲーム中での初期化処理
/// </summary>
void GameManager::GameInit()
{
	m_pPhysics = std::make_shared<MyLibrary::Physics>(m_pMap->GetCollisionMap());

	m_pMap->Init(m_pPhysics);

	m_pPlayer->Init(m_pPhysics);
	m_pEnemy->Init("stage1");
	m_pNpc->Init(m_pPhysics);
	m_pSetting->Init();
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

		m_pPlayer->Update();
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

		m_pEnemy->Update(m_pPhysics, this, m_pPlayer->GetPos(), m_pCamera->GetDirection(), !m_pPlayer->IsGetPlayerDead(), m_init);

		m_pMap->JudgeUpdate();

		//プレイヤーのボス部屋に入り口判定
		m_pPlayer->SetBossStart(m_pMap->GetBossEnter());

		//休息ができるか
		m_pPlayer->SetRest(m_pMap->GetRest());
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

		m_pMap->Update(m_pPhysics, m_pPlayer->GetWarp(), m_pPlayer->GetBossStart());

		//メニューを開く
		if (m_pPlayer->GetMenu())
		{
			m_pSetting->MenuUpdate();

			m_title = m_pSetting->GetTitle();

			m_pPlayer->SetMenu(m_pSetting->GetReturn());
		}
		//メニューを開けるようにする
		else
		{
			m_pSetting->SetReturn(true);
		}

		//休息した場合
		if (m_pPlayer->GetRest())
		{
			//一回だけ実行
			if (m_init == true)
			{
				m_pPlayer->GameInit(m_pPhysics);
				m_pEnemy->GameInit(m_pPhysics, this, m_init);
				m_pMap->TriggerReset();

				m_init = false;
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
	m_pPlayer->Draw();
	m_pEnemy->Draw();
	m_pNpc->Draw();

	if (m_pPlayer->GetMenu())
	{
		m_pSetting->MenuDraw();
	}
	//ボスが死んだ判定
	if (cWarp)
	{
		m_pMap->CoreDraw();
	}

	m_pCamera->Draw();

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

const MyLibrary::LibVec3 GameManager::GetPlayerPos() const
{
	return MyLibrary::LibVec3();
}
