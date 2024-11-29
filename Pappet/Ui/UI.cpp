#include "UI.h"
#include "Item/Weapon.h"
#include "Item/Shield.h"
#include "Item/Armor.h"

/// <summary>
/// ƒRƒ“ƒXƒgƒ‰ƒNƒ^
/// </summary>
UI::UI() :
	m_equipmentReturn(false)
{
}

/// <summary>
/// ƒfƒXƒgƒ‰ƒNƒ^
/// </summary>
UI::~UI()
{
}

/// <summary>
/// ‰Šú‰»ˆ—
/// </summary>
/// <param name="player"></param>
/// <param name="enemy"></param>
void UI::Init()
{
	m_deadBack = 0;
	m_deadA = 0;
	m_waitResetTime = 0;
	m_deadReset = false;
}

/// <summary>
/// ‘•”õ‚Ì•`‰æˆ—
/// </summary>
/// <param name="player"></param>
void UI::EquipmentDraw(Weapon& weapon, Shield& shield, Armor& armor)
{
	//‰E•Ší
	if (weapon.GetFist())
	{
		//Œ
		DrawGraph(110, 310, m_fist, true);
	}
	else if (weapon.GetBlack())
	{
		//•‚¢Œ•
		DrawGraph(140, 310, m_blackSword, true);
	}
	//¶•Ší
	if (shield.GetFist())
	{
		//Œ
		DrawGraph(310, 310, m_fist, true);
	}
	else if (shield.GetUgly())
	{
		//X‚¢‚
		DrawGraph(335, 330, m_uglyShield, true);
	}
	//–h‹ï
	if (armor.GetBody())
	{
		//—‡‘Ì
		DrawGraph(565, 320, m_body, true);
	}
	else if (armor.GetCommon())
	{
		//•½–}‚È–h‹ï
		DrawGraph(530, 320, m_commonArmor, true);
	}
	
}

/// <summary>
/// ‰E•Ší•`‰æˆ—
/// </summary>
void UI::RightDraw(Weapon& weapon)
{
	//Œ
	DrawGraph(70, 56, m_fist, true);
	//•‚¢Œ•
	DrawGraph(100, 210, m_blackSword, true);
}

/// <summary>
/// ¶•Ší•`‰æˆ—
/// </summary>
void UI::LeftDraw(Shield& shield)
{
	//Œ
	DrawGraph(70, 56, m_fist, true);
	//X‚¢‚
	DrawGraph(85, 230, m_uglyShield, true);
}

/// <summary>
/// –h‹ï•`‰æˆ—
/// </summary>
void UI::ArmorDraw(Armor& armor)
{
	//—‡
	DrawGraph(120, 66, m_body, true);
	//•½–}‚ÈŠZ
	DrawGraph(85, 220, m_commonArmor, true);
}

/// <summary>
/// €–S•`‰æˆ—
/// </summary>
void UI::DiedDraw()
{
	//€–S‚Ì”wŒi‚Ì“§‰ß
	if (m_deadBack < 150)
	{
		m_deadBack++;
	}
	//€–S‚Ì•¶š‚Ì“§‰ß
	if (m_deadA < 255)
	{
		m_deadA++;

		m_waitResetTime = 0;
	}
	else if (m_deadA >= 255)
	{
		if (m_waitResetTime <= 30)
		{
			m_waitResetTime++;
		}
		else
		{
			m_deadReset = true;
		}
	}

	//”wŒi‚ÌF‚ğ”–‚­‚·‚é
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_deadBack);
	DrawBox(0, 100, 2000, 400, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//€–S‚Ì•¶š‚ğo‚·
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_deadA);
	DrawGraph(-150, -100, m_dead, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

}

/// <summary>
/// I—¹ˆ—
/// </summary>
void UI::End()
{
}
