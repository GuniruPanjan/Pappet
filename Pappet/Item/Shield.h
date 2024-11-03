#pragma once
#include "ItemBase.h"

/// <summary>
/// 盾のクラス
/// </summary>
class Shield : public ItemBase
{
public:
	//コンストラクタ
	Shield();
	//デストラクタ
	virtual ~Shield();

	//初期化
	void Init();
	//更新処理
	void Update(MATRIX mat);
	//描画処理
	void Draw();
	//描画処理
	void End();
};

