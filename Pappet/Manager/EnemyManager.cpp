#include "EnemyManager.h"
#include "Character/Immortal.h"
#include "External/CsvLoad.h"
#include "GameManager.h"

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
/// 更新
/// </summary>
/// <param name="physics">物理クラス</param>
/// <param name="gameManager">ゲームマナジャー</param>
/// <param name="playerPos">プレイヤーポジション</param>
/// <param name="playerDir">プレイヤーの方向</param>
/// <param name="isPlayerChase">プレイヤーを発見したかどうか</param>
void EnemyManager::Update(std::shared_ptr<MyLibrary::Physics> physics, GameManager* gameManager, MyLibrary::LibVec3 playerPos, MyLibrary::LibVec3 playerDir, bool isPlayerChase)
{
	m_enemyPos.clear();

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
		enemy->Update(gameManager->GetPlayerPos(), isPlayerChase);

		m_enemyPos.emplace_back(enemy->GetPos());
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
		std::shared_ptr<Immortal> add = std::make_shared<Immortal>();
		add->Init(posX, posY, posZ, physics);
		m_pEnemys.emplace_back(add);
	}
}
