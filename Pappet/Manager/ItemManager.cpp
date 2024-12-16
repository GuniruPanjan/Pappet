#include "ItemManager.h"
#include "GameManager.h"
#include "External/CsvLoad.h"

/// <summary>
/// コンストラクタ
/// </summary>
ItemManager::ItemManager():
	m_stageName("")
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
void ItemManager::Update(std::shared_ptr<MyLibrary::Physics> physics, GameManager* gameManager)
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
				generate->isCreated = true;
				CreateItem(generate->posX, generate->posY, generate->posZ, generate->itemName, physics);
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

	}
	if (name == "Shield")
	{

	}
	if (name == "Armor")
	{

	}
	if (name == "Tool")
	{

	}
}
