#include "ItemBase.h"

/// <summary>
/// 終了処理
/// </summary>
/// <param name="physics"></param>
void ItemBase::ItemFinalize(std::shared_ptr<MyLibrary::Physics> physics)
{
	m_pSearch->Finalize(physics);
}

void ItemBase::ItemGudgment(int SmallCore, int MediumCore, int Rubbish, int BlackSword, int Distorted, int ArmorNormal)
{
	m_item.SmallCore = SmallCore;
	m_item.MediumCore = MediumCore;
	m_item.Rubbish = Rubbish;
	m_item.BlackSword = BlackSword;
	m_item.Distorted = Distorted;
	m_item.ArmorNormal = ArmorNormal;
}

/// <summary>
/// 終了処理
/// </summary>
void ItemBase::ItemEnd()
{
}

/// <summary>
/// アイテム情報を読み込む
/// </summary>
/// <param name="name"></param>
void ItemBase::LoadData(std::string name)
{

}

/// <summary>
/// 索敵判定をする当たり判定を作成
/// </summary>
/// <param name="pos"></param>
/// <param name="radius"></param>
void ItemBase::InitSearch(MyLibrary::LibVec3 pos, float radius)
{
	m_pSearch = std::make_shared<SearchObject>(radius);
	m_pSearch->Init(m_pPhysics, pos, true);
}
