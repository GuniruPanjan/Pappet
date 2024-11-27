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
	//—‡‘Ì
	if (m_body.sa_equipment)
	{
		m_body.sa_defense = 0.0f;
	}
	//•½–}‚È–h‹ï
	else if (m_common.sa_equipment)
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
