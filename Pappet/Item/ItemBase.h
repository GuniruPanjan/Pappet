#pragma once
#include "DxLib.h"

/// <summary>
/// アイテムの親クラス
/// </summary>
class ItemBase
{
public:
	//武器の構造体
	struct sWeapon
	{
		float sw_attack;               //攻撃力
		float sw_radius;               //武器の当たり判定
		bool sw_equipment = false;     //装備したかの判定
	};

	//盾の構造体
	struct sShield
	{
		int ss_cut;                    //カット率
		int ss_strength;               //盾強度
		bool ss_equipment = false;     //装備したかの判定
	};

	//防具の構造体
	struct sArmor
	{
		int sa_defense;                //防御力
		bool sa_equipment = false;     //装備したかの判定
	};

	//コンストラクタ
	ItemBase() :
		m_itemUse(0),
		m_itemHandle(-1),
		m_frameIndex(0),
		m_framePos(VGet(0.0f,0.0f,0.0f)),
		m_pos(VGet(0.0f,0.0f,0.0f)),
		m_transMatrix(),
		m_mixMatrix()
	{

	}
	//デストラクタ
	virtual ~ItemBase() {};

protected:
	//アイテムの効果
	int m_itemUse;
	//アイテムのハンドル
	int m_itemHandle;
	//フレーム検索
	int m_frameIndex;
	//フレームポジション
	VECTOR m_framePos;
	//ポジション
	VECTOR m_pos;
	//モデルの行列
	MATRIX m_transMatrix;
	//モデルの合成行列
	MATRIX m_mixMatrix;
};
