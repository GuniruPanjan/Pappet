#pragma once
#include "ItemBase.h"

/// <summary>
/// 武器のクラス
/// </summary>
class Weapon : public ItemBase
{
public:
	//コンストラクタ
	Weapon();
	//デストラクタ
	virtual ~Weapon();

	//初期化
	void Init();
	//更新処理
	void Update(MATRIX mat);
	//描画処理
	void Draw(MATRIX mat);
	//終了処理
	void End();
};

