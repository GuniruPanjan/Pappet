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

					//何のアイテムかを判断する
					CheckItem(generate->itemName, generate->SmallCore, generate->MediumCore, generate->Rubbish, generate->BlackSword, generate->Distorted, generate->ArmorNormal);
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

			//とった場合
			if (taking && m_itemPick && item->GetItemBox())
			{
				m_item.SmallCore += item->GetItemKinds().SmallCore;
				m_item.MediumCore += item->GetItemKinds().MediumCore;
				m_item.Rubbish += item->GetItemKinds().Rubbish;
				m_item.BlackSword += item->GetItemKinds().BlackSword;
				m_item.Distorted += item->GetItemKinds().Distorted;
				m_item.ArmorNormal += item->GetItemKinds().ArmorNormal;

				item->SetItemBox(false);
			}

		}
	}
}

/// <summary>
/// 描画処理
/// </summary>
void ItemManager::Draw()
{
#if true
	DrawFormatString(200, 600, 0xffffff, "SmallCore : %d", m_item.SmallCore);
	DrawFormatString(200, 700, 0xffffff, "MediumCore : %d", m_item.MediumCore);
	DrawFormatString(200, 800, 0xffffff, "Rubbish : %d", m_item.Rubbish);
	DrawFormatString(200, 100, 0xffffff, "BlackSword : %d", m_item.BlackSword);
	DrawFormatString(200, 150, 0xffffff, "Distorted : %d", m_item.Distorted);
	DrawFormatString(200, 200, 0xffffff, "ArmorNormal : %d", m_item.ArmorNormal);
#endif
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

/// <summary>
/// 何のアイテムか判断する
/// </summary>
/// <param name="item"></param>
void ItemManager::CheckItem(std::string name, int SmallCore, int MediumCore, int Rubbish, int BlackSword, int Distorted, int ArmorNormal)
{
	if (name == "Weapon")
	{
		m_pweapon->ItemGudgment(SmallCore, MediumCore, Rubbish, BlackSword, Distorted, ArmorNormal);
	}
	if (name == "Shield")
	{
		m_pshield->ItemGudgment(SmallCore, MediumCore, Rubbish, BlackSword, Distorted, ArmorNormal);
	}
	if (name == "Armor")
	{
		m_parmor->ItemGudgment(SmallCore, MediumCore, Rubbish, BlackSword, Distorted, ArmorNormal);
	}
	if (name == "Tool")
	{
		//m_ptool = std::make_shared<Tool>();
		//m_ptool->ItenInit(posX, posY, posZ, physics);
		//m_pItems.emplace_back(m_pweapon);
	}
}
