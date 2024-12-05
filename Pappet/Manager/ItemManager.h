#pragma once
#include <memory>
#include <map>
#include <string>
#include <List>
#include "Library/MyLibrary.h"

class ItemBase;
class Armor;
class Shield;
class Weapon;

/// <summary>
/// アイテムを管理するクラス
/// </summary>
class ItemManager
{
private:
	//アイテムの構造体
	struct Item
	{
		int SmallCore = 0;       //小型コア
		int MediumCore = 0;      //中型コア
		int Rubbish = 0;         //ゴミ
		int BlackSword = 0;      //黒い剣
		int Distorted = 0;       //歪んだ盾
		int ArmorNormal = 0;     //普通の鎧
	};

	//アイテム生成情報
	struct ItemGenerateInfo
	{
		std::string itemName;      //アイテムの名前
		int mapNumber;             //マップの番号
		int posX;                  //アイテムのX座標
		int posY;                  //アイテムのY座標
		int posZ;                  //アイテムのZ座標

		bool isCreated = false;    //生成済みかどうか
		bool isTaking = false;     //取られたかどうか
	};

public:
	//コンストラクタ
	ItemManager();
	//デストラクタ
	virtual ~ItemManager();
	//初期化処理
	void Init(const char* stageName);
	//更新処理
	void Update(std::shared_ptr<MyLibrary::Physics> physics);

	//描画処理
	void Draw();

	//終了処理
	void End();

private:
	//アイテムの管理
	std::list<std::shared_ptr<ItemBase>> m_pItems;
	//アイテムの生成情報
	std::list<std::shared_ptr<ItemGenerateInfo>> m_pGenerateInfo;

	std::shared_ptr<Armor> m_parmor;
	std::shared_ptr<Shield> m_pshield;
	std::shared_ptr<Weapon> m_pweapon;

	//ステージ名
	const char* m_stageName;

	//ステージ毎のアイテムの生成数
	std::unordered_map<int, int> m_itemGenerationCountPerOneMap;

};

