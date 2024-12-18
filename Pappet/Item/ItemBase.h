#pragma once
#include "DxLib.h"
#include "Library/MyLibrary.h"
#include "Object/SearchObject.h"
#include <map>
#include <string>

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
		m_itemPick(false),
		m_isTaking(false),
		m_framePos(VGet(0.0f,0.0f,0.0f)),
		m_pos(VGet(0.0f,0.0f,0.0f)),
		m_transMatrix(),
		m_mixMatrix()
	{

	}
	//デストラクタ
	virtual ~ItemBase() {};

	//マップアイテムとしての初期化処理
	virtual void ItemInit(float posX, float posY, float posZ, std::shared_ptr<MyLibrary::Physics> physics) {};
	//マップアイテムとしての削除処理
	void ItemFinalize(std::shared_ptr<MyLibrary::Physics> physics);
	//マップアイテムとしての更新処理
	virtual void ItemUpdate() {};
	//マップアイテムとしての終了処理
	void ItemEnd();

	//アイテムを拾えるかをの判定を返す
	bool GetItemPick() { return m_itemPick; }
	//アイテムを取得した時
	bool SetItemPick(bool set) { return m_isTaking = set; }
protected:
	//アイテム情報を読み込む
	void LoadData(std::string name);
	//当たり判定を作成
	void InitSearch(MyLibrary::LibVec3 pos, float radius);

protected:
	std::shared_ptr<SearchObject> m_pSearch;     //索敵判定

	//アイテムの効果
	int m_itemUse;
	//アイテムのハンドル
	int m_itemHandle;
	//フレーム検索
	int m_frameIndex;
	//アイテムを取れるかの判定
	bool m_itemPick;
	//取られたかどうかの判定
	bool m_isTaking;
	//フレームポジション
	VECTOR m_framePos;
	//ポジション
	VECTOR m_pos;
	//モデルの行列
	MATRIX m_transMatrix;
	//モデルの合成行列
	MATRIX m_mixMatrix;

	std::shared_ptr<MyLibrary::Physics> m_pPhysics;

	MyLibrary::LibVec3 m_centerPos;              //中心座標
};
