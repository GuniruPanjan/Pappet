#include "Armor.h"

Armor::Armor()
{
}

Armor::~Armor()
{
}

/// <summary>
/// ������
/// </summary>
void Armor::Init()
{
	//���}�Ȗh��
	if (m_common.sa_equipment)
	{
		m_common.sa_defense = 30.0f;

		//���f���p�X������
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
