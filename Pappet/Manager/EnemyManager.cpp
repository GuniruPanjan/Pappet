#include "EnemyManager.h"
#include "Character/Immortal.h"
#include "Character/Bear.h"
#include "External/CsvLoad.h"
#include "GameManager.h"

namespace
{
	int cI = 0;
}

/// <summary>
/// コンストラクタ
/// </summary>
EnemyManager::EnemyManager() :
	m_stageName(""),
	m_frontEnemyHp(0),
	m_frontEnemyMaxHp(0)
{
}

/// <summary>
/// デストラクタ
/// </summary>
EnemyManager::~EnemyManager()
{
	//配列の全要素削除
	m_pEnemys.clear();
	m_pGenerateInfo.clear();
	m_enemyPos.clear();
	m_enemyTarget.clear();
}

/// <summary>
/// 初期化
/// </summary>
/// <param name="stageName">ステージ名前</param>
void EnemyManager::Init(const char* stageName)
{
	m_stageName = stageName;

	//敵生成情報を取得する
	CsvLoad::GetInstance().EnemyDataLoad(m_stageName, m_pGenerateInfo);

	//csv側でマップを指定しているためマップの補正をする
	for (auto& generate : m_pGenerateInfo)
	{
		//マップが1なら
		if (generate->mapNumber == 1)
		{
			//そのマップの出現する敵の数を更新する
			int num = m_enemyGenerationCountPerOneMap[generate->mapNumber];
			num++;
			m_enemyGenerationCountPerOneMap[generate->mapNumber];
		}
	}
}

/// <summary>
/// ゲームの仕様上での初期化
/// </summary>
/// <param name="physics">物理クラス</param>
/// <param name="gameManager">ゲームマネジャー</param>
/// <param name="init">初期化するかどうか</param>
void EnemyManager::GameInit(std::shared_ptr<MyLibrary::Physics> physics, GameManager* gameManager, bool init)
{
	//敵の当たり判定削除
	for (auto& enemy : m_pEnemys)
	{
		//削除
		enemy->Finalize(physics);
	}

	if (init)
	{
		m_pEnemys.clear();
	}


	auto thisMapName = gameManager->GetThisMapName();

	if (thisMapName == 1 || thisMapName == 2 || thisMapName == 3 ||
		thisMapName == 4 || thisMapName == 5)
	{
		//敵生成情報をまわして
		for (auto& generate : m_pGenerateInfo)
		{
			//今のマップが一致しているとき
			if (generate->mapNumber == thisMapName)
			{
				//生成済みのキャラを初期化する
				if (generate->isCreated)
				{
					EnemyInit(generate->posX, generate->posY, generate->posZ, generate->enemyName, physics);
				}
			}
		}
	}
}

/// <summary>
/// 更新
/// </summary>
/// <param name="physics">物理クラス</param>
/// <param name="gameManager">ゲームマナジャー</param>
/// <param name="playerPos">プレイヤーポジション</param>
/// <param name="playerDir">プレイヤーの方向</param>
/// <param name="isPlayerChase">プレイヤーを発見したかどうか</param>
void EnemyManager::Update(std::shared_ptr<MyLibrary::Physics> physics, GameManager* gameManager, MyLibrary::LibVec3 playerPos, MyLibrary::LibVec3 playerDir, bool isPlayerChase, bool init)
{
	m_enemyPos.clear();
	m_enemyTarget.clear();

	//ここがおかしい
	//今のマップがどのマップか取得する
	auto thisMapName = gameManager->GetThisMapName();

	if (thisMapName == 1 || thisMapName == 2 || thisMapName == 3 ||
		thisMapName == 4 || thisMapName == 5)
	{
		//敵生成情報をまわして
		for (auto& generate : m_pGenerateInfo)
		{

			//今のマップが一致しているとき
			if (generate->mapNumber == thisMapName)
			{
				//生成済みでなければ
				if (!generate->isCreated)
				{
					//生成済みにして敵を生成する
					generate->isCreated = true;
					CreateEnemy(generate->posX, generate->posY, generate->posZ, generate->enemyName, physics);
				}
			}
		}
	}

	//敵の更新する
	for (auto& enemy : m_pEnemys)
	{
		//更新
		enemy->Update(playerPos, isPlayerChase);

		m_enemyPos.emplace_back(enemy->GetPos());
		m_enemyTarget.emplace_back(enemy->GetTarget());
		
	}
}

/// <summary>
/// 描画
/// </summary>
void EnemyManager::Draw()
{
	//敵を描画する
	for (auto& enemy : m_pEnemys)
	{
		enemy->Draw();
	}
}

/// <summary>
/// 終了処理
/// </summary>
void EnemyManager::End()
{
	for (auto& enemy : m_pEnemys)
	{
		enemy->End();
	}
}

/// <summary>
/// 敵の生成
/// </summary>
/// <param name="posX">X座標</param>
/// <param name="posY">Y座標</param>
/// <param name="posZ">Z座標</param>
/// <param name="name">キャラクター名</param>
/// <param name="physics">物理ポインタ</param>
void EnemyManager::CreateEnemy(float posX, float posY, float posZ, std::string name, std::shared_ptr<MyLibrary::Physics> physics)
{
	if (name == "Immortal")
	{
		immortal = std::make_shared<Immortal>();
		immortal->Init(posX, posY, posZ, physics);
		m_pEnemys.emplace_back(immortal);
	}
	if (name == "bear")
	{
		bear = std::make_shared<Bear>();
		bear->Init(posX, posY, posZ, physics);
		m_pEnemys.emplace_back(bear);
	}
}

/// <summary>
/// 敵のゲームの仕様上での初期化
/// </summary>
/// <param name="posX">X座標</param>
/// <param name="posY">Y座標</param>
/// <param name="posZ">Z座標</param>
/// <param name="name">キャラクター名</param>
/// <param name="physics">物理ポインタ</param>
void EnemyManager::EnemyInit(float posX, float posY, float posZ, std::string name, std::shared_ptr<MyLibrary::Physics> physics)
{
	if (name == "Immortal")
	{
		//std::shared_ptr<Immortal> add = std::make_shared<Immortal>();
		immortal = std::make_shared<Immortal>();
		immortal->GameInit(posX, posY, posZ, physics);
		m_pEnemys.emplace_back(immortal);

	}
	if (name == "bear")
	{
		//std::shared_ptr<Bear> add = std::make_shared<Bear>();
		bear = std::make_shared<Bear>();
		bear->GameInit(posX, posY, posZ, physics);
		m_pEnemys.emplace_back(bear);
	}
}
