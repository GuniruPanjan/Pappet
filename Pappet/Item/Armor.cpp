#include "Armor.h"

Armor::Armor()
{
}

Armor::~Armor()
{
}

/// <summary>
/// ‰Šú‰»
/// </summary>
void Armor::Init()
{
	//•½–}‚È–h‹ï
	if (m_common.sa_equipment)
	{
		m_common.sa_defense = 30.0f;

		//ƒ‚ƒfƒ‹ƒpƒX‚ğ‘‚­
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
