#include "UI.h"
#include "Item/Weapon.h"
#include "Item/Shield.h"
#include "Item/Armor.h"


/// <summary>
/// �R���X�g���N�^
/// </summary>
UI::UI() :
	m_equipmentReturn(false)
{
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
UI::~UI()
{
}

/// <summary>
/// ����������
/// </summary>
/// <param name="player"></param>
/// <param name="enemy"></param>
void UI::Init()
{
}

/// <summary>
/// �����̕`�揈��
/// </summary>
/// <param name="player"></param>
void UI::EquipmentDraw(Weapon& weapon, Shield& shield, Armor& armor)
{
	//�E����
	if (weapon.GetFist())
	{
		//��
		DrawGraph(110, 310, m_fist, true);
	}
	else if (weapon.GetBlack())
	{
		//������
		DrawGraph(140, 310, m_blackSword, true);
	}
	//������
	if (shield.GetFist())
	{
		//��
		DrawGraph(310, 310, m_fist, true);
	}
	else if (shield.GetUgly())
	{
		//�X����
		DrawGraph(335, 330, m_uglyShield, true);
	}
	//�h��
	if (armor.GetBody())
	{
		//����
		DrawGraph(565, 320, m_body, true);
	}
	else if (armor.GetCommon())
	{
		//���}�Ȗh��
		DrawGraph(530, 320, m_commonArmor, true);
	}
	
}

/// <summary>
/// �E����`�揈��
/// </summary>
void UI::RightDraw(Weapon& weapon)
{
	//��
	DrawGraph(70, 56, m_fist, true);
	//������
	DrawGraph(100, 210, m_blackSword, true);
}

/// <summary>
/// ������`�揈��
/// </summary>
void UI::LeftDraw(Shield& shield)
{
	//��
	DrawGraph(70, 56, m_fist, true);
	//�X����
	DrawGraph(85, 230, m_uglyShield, true);
}

/// <summary>
/// �h��`�揈��
/// </summary>
void UI::ArmorDraw(Armor& armor)
{
	//��
	DrawGraph(120, 66, m_body, true);
	//���}�ȊZ
	DrawGraph(85, 220, m_commonArmor, true);
}

/// <summary>
/// �I������
/// </summary>
void UI::End()
{
}
