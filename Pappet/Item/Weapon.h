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
	void Draw();
	//終了処理
	void End();

	//ステータスの判定
	float GetFistAttack() { return m_fist.sw_attack; }
	float GetBlackAttack() { return m_black.sw_attack; }

	//装備したかの判定
	bool GetFist() { return m_fist.sw_equipment; }
	bool SetFist(bool set) { return m_fist.sw_equipment = set; }
	bool GetBlack() { return m_black.sw_equipment; }
	bool SetBlack(bool set) { return m_black.sw_equipment = set; }

private:
	//剣の構造体
	sWeapon m_fist;      //素手
	sWeapon m_black;     //黒い剣
};

