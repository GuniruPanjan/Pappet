#include "Armor.h"

namespace
{
	//アイテム名
	constexpr const char* cItemName = "Armor";
}

Armor::Armor()
{
}

Armor::~Armor()
{
}

/// <summary>
/// マップとしてのアイテム初期化処理
/// </summary>
/// <param name="posX"></param>
/// <param name="posY"></param>
/// <param name="posZ"></param>
/// <param name="physics"></param>
void Armor::ItemInit(float posX, float posY, float posZ, std::shared_ptr<MyLibrary::Physics> physics)
{
	//代入
	m_pPhysics = physics;

	InitSearch(MyLibrary::LibVec3(posX, posY, posZ), 80.0f);
}

/// <summary>
/// マップとしてのアイテム更新処理
/// </summary>
void Armor::ItemUpdate()
{

}

/// <summary>
/// 初期化
/// </summary>
void Armor::Init()
{
	//平凡な防具
	if (m_common.sa_equipment)
	{
		m_common.sa_defense = 30.0f;

		//モデルパスを書く
		m_path = "Data/Armor/CommonArmor.mv1";
	}
}

void Armor::Update()
{
}

void Armor::Draw()
{
}

void Armor::End()
{
}
