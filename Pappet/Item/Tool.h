#pragma once
#include "ItemBase.h"

/// <summary>
/// 道具のクラス
/// </summary>
class Tool : public ItemBase
{
	//コンストラクタ
	Tool();
	//デストラクタ
	virtual ~Tool();

	//初期化処理
	void Init();
	//アイテムの初期化処理
	void ItemInit(float posX, float posY, float posZ, std::shared_ptr<MyLibrary::Physics> physics);
};

