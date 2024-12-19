#include "ItemManager.h"
#include "GameManager.h"
#include "External/CsvLoad.h"
#include "Item/Weapon.h"
#include "Item/Shield.h"
#include "Item/Armor.h"
#include "Item/Tool.h"

/// <summary>
/// コンストラクタ
/// </summary>
ItemManager::ItemManager():
	m_stageName(""),
	m_itemPick(false)
{
}

/// <summary>
/// デストラクタ
/// </summary>
ItemManager::~ItemManager()
{
	//配列の全要素削除
}

/// <summary>
/// 初期化処理
/// </summary>
/// <param name="stageName">ステージ名</param>
void ItemManager::Init(const char* stageName)
{
	m_stageName = stageName;

	//アイテム生成情報を取得する
	CsvLoad::GetInstance().ItemDataLoad(m_stageName, m_pGenerateInfo);

	//csv側でマップを指定しているためマップの補正をする
	for (auto& generate : m_pGenerateInfo)
	{
		//マップが1なら
		if (generate->mapNumber == 1)
		{
			//そのマップの出現するアイテムの数を更新する
			int num = m_itemGenerationCountPerOneMap[generate->mapNumber];
			num++;
			m_itemGenerationCountPerOneMap[generate->mapNumber];
		}
	}
}

/// <summary>
/// 更新処理
/// </summary>
/// <param name="physics">フィジックス</param>
void ItemManager::Update(std::shared_ptr<MyLibrary::Physics> physics, GameManager* gameManager, bool taking)
{

	//今のマップがどのマップか取得する
	auto thisMapName = gameManager->GetThisMapName();

	if (thisMapName == 1 || thisMapName == 2 || thisMapName == 3 ||
		thisMapName == 4 || thisMapName == 5)
	{
		//アイテム生成情報をまわして
		for (auto& generate : m_pGenerateInfo)
		{
			//今のマップが一致しているとき
			if (generate->mapNumber == thisMapName)
			{
				//生成済みでなければ
				if (!generate->isCreated)
				{
					generate->isCreated = true;
					CreateItem(generate->posX, generate->posY, generate->posZ, generate->itemName, physics);
				}
				
			}
		}
	}


	//マップが0以外だと動かす
	if (thisMapName != 0)
	{
		//マップのアイテムとして更新する
		for (auto& item : m_pItems)
		{
			item->ItemUpdate(taking);

			if (!m_itemPick)
			{
				m_itemPick = item->GetItemPick();
			}

		}
	}
}

/// <summary>
/// 描画処理
/// </summary>
void ItemManager::Draw()
{
}

/// <summary>
/// 終了処理
/// </summary>
void ItemManager::End()
{
}

/// <summary>
/// アイテムの生成
/// </summary>
/// <param name="posX">X座標</param>
/// <param name="posY">Y座標</param>
/// <param name="posZ">Z座標</param>
/// <param name="name">キャラクター名</param>
/// <param name="physics">物理ポインタ</param>
void ItemManager::CreateItem(float posX, float posY, float posZ, std::string name, std::shared_ptr<MyLibrary::Physics> physics)
{
	if (name == "Weapon")
	{
		m_pweapon = std::make_shared<Weapon>();
		m_pweapon->ItemInit(posX, posY, posZ, physics);
		m_pItems.emplace_back(m_pweapon);
	}
	if (name == "Shield")
	{
		m_pshield = std::make_shared<Shield>();
		m_pshield->ItemInit(posX, posY, posZ, physics);
		m_pItems.emplace_back(m_pshield);
	}
	if (name == "Armor")
	{
		m_parmor = std::make_shared<Armor>();
		m_parmor->ItemInit(posX, posY, posZ, physics);
		m_pItems.emplace_back(m_parmor);
	}
	if (name == "Tool")
	{
		//m_ptool = std::make_shared<Tool>();
		//m_ptool->ItenInit(posX, posY, posZ, physics);
		//m_pItems.emplace_back(m_pweapon);
	}
}
