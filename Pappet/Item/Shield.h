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

	//アイテムの初期化処理
	void ItemInit(float posX, float posY, float posZ, std::shared_ptr<MyLibrary::Physics> physics);
	//マップアイテムとしての更新処理
	void ItemUpdate();

	//初期化
	void Init();
	//更新処理
	void Update(MATRIX mat);
	//描画処理
	void Draw();
	//描画処理
	void End();

	//装備したかの判定
	bool GetFist() { return m_fist.ss_equipment; }
	bool SetFist(bool set) { return m_fist.ss_equipment = set; }
	bool GetUgly() { return m_ugly.ss_equipment; }
	bool SetUgly(bool set) { return m_ugly.ss_equipment = set; }
	
private:
	//盾の構造体
	sShield m_fist;    //素手
	sShield m_ugly;    //醜い盾
};

