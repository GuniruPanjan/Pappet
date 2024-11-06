#pragma once
#include <memory>
#include <map>
#include <string>
#include <List>
#include "Library/MyLibrary.h"

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
	void Init();
	//更新処理
	void Update();

	//描画処理
	void Draw();

	//終了処理
	void End();

private:

};

