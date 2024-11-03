#pragma once
#include <memory>
#include <map>
#include <string>
#include <List>
#include "Library/MyLibrary.h"

/// <summary>
/// 敵を管理するクラス
/// </summary>
class EnemyManager
{
public:

	struct EnemyGenerateInfo
	{
		std::string enemyName;     //敵の名前
		int mapNumber;             //マップの番号
		int posX;                  //敵のX座標
		int posY;                  //敵のY座標
		int posZ;                  //敵のZ座標

		bool isCreated = false;    //生成済みかどうか
	};
};

