#include "UI.h"
#include "Item/Weapon.h"
#include "Item/Shield.h"
#include "Item/Armor.h"
#include "Character/Player.h"
#include "Manager/EnemyManager.h"
#include "Manager/ItemManager.h"
#include "Manager/MapManager.h"
#include "Item/Tool.h"
#include "External/Font.h"
#include "Manager/MessageManager.h"

namespace
{
	const int cFontSizeLarge = 40;
	const int cFontSizeMedium = 30;
	const int cHpBarWidth = 200;
	const int cBossHpBarWidth = 800;
	const int cStatusDrawX = 180;
	const int cStatusDrawY = 30;
	const int cEquipmentFrameX1 = 150;
	const int cEquipmentFrameY1 = 550;
	const int cEquipmentFrameX2 = 0;
	const int cEquipmentFrameY2 = 665;
	const int cEquipmentFrameX3 = 300;
	const int cEquipmentFrameY3 = 665;
	const int cEquipmentFrameX4 = 150;
	const int cEquipmentFrameY4 = 780;
	const int cCoreBarX = 1050;
	const int cCoreBarY = 750;
	const int cActionUiX = 480;
	const int cActionUiY = 800;
	const int cButtonY = 805;
	const int cItemTakingX = 480;
	const int cItemTakingY = 600;
	const int cOkTextX = 800;
	const int cOkTextY = 820;
	const int cPlayerCoreX = 1400;
	const int cPlayerCoreY = 905;
	const int cDeadTextX = -150;
	const int cDeadTextY = 100;
	const int cEnemyHpBarXOffset = -100;
	const int cEnemyHpBarYOffset = -300;
	const int cBossNameX = 450;
	const int cBossNameY = 700;
	const int cBossSubnameY = 670;
	const int cBossHpBarX = 450;
	const int cBossHpBarY = 750;

	const int cHpBarHeight = 45;
	const int cStaminaBarHeight = 50;
	const int cHpBarOffsetX = 5;
	const int cHpBarOffsetY = 8;
	const int cStaminaBarOffsetX = -8;
	const int cStaminaBarOffsetY = 50;
	const int cHpBarSegmentWidth = 50;
	const int cStaminaBarSegmentWidth = 10;
	const int cHpBarInitialWidth = 185;
	const int cStaminaBarInitialWidth = 150;
	const int cHpBarSegmentXOffset = 23;
	const int cStaminaBarSegmentXOffset = 23;
	const int cStaminaBarSegmentXAdjustment = 2;

	const int cFistX = 70;
	const int cFistY = 56;
	const int cBodyX = 120;
	const int cBodyY = 66;

	const int cRightWeaponX = 305;
	const int cRightWeaponY = 700;
	const int cLeftWeaponX = 5;
	const int cLeftWeaponY = 700;
	const int cLeftWeaponOffsetX = 15;
	const int cLeftWeaponOffsetY = 20;
	const int cArmorX = 195;
	const int cArmorY = 600;
	const int cCommonArmorX = 160;
	const int cCommonArmorY = 600;
	const int cHeelStoneX = 115;
	const int cHeelStoneY = 815;
	const int cHeelStoneTextX = 260;
	const int cHeelStoneTextY = 930;

	const int cWeaponX = 110;
	const int cWeaponY = 310;
	const int cShieldX = 310;
	const int cShieldY = 310;
	const int cShieldOffsetX = 25;
	const int cShieldOffsetY = 20;
	const int cArmorBodyX = 565;
	const int cArmorBodyY = 320;
	const int cArmorCommonX = 530;
	const int cArmorCommonY = 320;

	const int cStatusIconX = -50;
	const int cStatusIconY = 0;
	const int cActionUiXOffset = 20;
	const int cActionTextX = 750;
	const int cActionTextY = 820;
	const int cItemTextX = 700;
	const int cBossTextX = 700;
	const int cWarpTextX = 750;
	const int cMessageTextX = 700;
	const int cTextColor = 0xffffff;
	const int cItemTakingUiX = 500;
	const int cItemTakingUiYBlackSword = 625;
	const int cItemTakingUiYDistorted = 645;
	const int cItemTakingUiYArmorNormal = 635;
	const int cItemTakingUiYBat = 635;
	const int cItemTakingUiYWoodShield = 635;
	const int cItemTakingUiCharX = 800;
	const int cItemTakingUiCharY = 675;

	bool cItemTakingUi = false;
}

/// <summary>
/// �R���X�g���N�^
/// </summary>
UI::UI() :
	m_deadReset(false),
	m_waitResetTime(0),
	m_youDead(0),
	m_equipmentReturn(false),
	m_xpad()
{
	m_pFont = std::make_shared<Font>();
	m_pBigFont = std::make_shared<Font>();
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
	m_youDead = 0;
	m_waitResetTime = 0;
	m_deadReset = false;

	m_heelStone = MyLoadGraph("Data/UI/HeelStoneMini.png", 4, 4);
	m_blackSword = MyLoadGraph("Data/UI/������UI.png", 3, 3);
	m_fist = MyLoadGraph("Data/UI/��UI.png", 3, 3);
	m_body = MyLoadGraph("Data/UI/����UI.png", 3, 3);
	m_uglyShield = MyLoadGraph("Data/UI/�X����UI.png", 4, 4);
	m_commonArmor = MyLoadGraph("Data/UI/���}�ȊZUI.png", 3, 3);
	m_bat = MyLoadGraph("Data/UI/But.png", 3, 3);
	m_woodShield = MyLoadGraph("Data/UI/WoodShield.png", 4, 4);

	m_dead = MyLoadGraph("Data/UI/YOUDIEDGraph.png", 1, 1);
	m_backRightBar = MyLoadGraph("Data/UI/StatusBar�E�[.png", 2, 2);
	m_backLeftBar = MyLoadGraph("Data/UI/StatusBar���[.png", 2, 2);
	m_backCenterBar = MyLoadGraph("Data/UI/StatusBar����.png", 2, 2);
	m_backOutBar = MyLoadGraph("Data/UI/StatusBar�O�g.png", 2, 2);
	m_hpBar = MyLoadGraph("Data/UI/HPBarRed.png", 2, 2);
	m_staminaBar = MyLoadGraph("Data/UI/StaminaBar.png", 2, 2);
	m_coreBackBar = MyLoadGraph("Data/UI/CoreBar.png", 1, 1);
	m_statusIcon = MyLoadGraph("Data/UI/StatusIcon.png", 1, 1);
	m_equipmentFrame = MyLoadGraph("Data/UI/Frame.png", 1, 1);
	m_actionUI = MyLoadGraph("Data/UI/Action.png", 1, 1);
	m_itemTaking = MyLoadGraph("Data/UI/�A�C�e���擾�͂�.png", 1, 1);
	m_yButton = MyLoadGraph("Data/UI/YButton.png", 3, 3);
	m_bButton = MyLoadGraph("Data/UI/BButton.png", 3, 3);
	m_tagetLock = MyLoadGraph("Data/UI/TagetLock.png", 1, 1);

	m_pFont->FontInit(40);
	m_pBigFont->FontInit(80);
}

/// <summary>
/// �Q�[����UI�̕`�揈��
/// </summary>
/// <param name="player"></param>
/// <param name="enemy"></param>
/// <param name="eq"></param>
/// <param name="map"></param>
/// <param name="item"></param>
void UI::Draw(Player& player, EnemyManager& enemy, Setting& eq, MapManager& map, ItemManager& item, Weapon& weapon, Shield& shield, Armor& armor, Tool& tool, MessageManager& message)
{
	//�p�b�h���͎擾
	GetJoypadXInputState(DX_INPUT_KEY_PAD1, &m_xpad);

	//�X�e�[�^�X�o�[�̕`��
	StatusDraw(cStatusDrawX, cStatusDrawY, player);

	//�v���C���[�̏�ԃA�C�R���`��
	DrawGraph(cStatusIconX, cStatusIconY, m_statusIcon, true);

	//�����̕`��
	DrawGraph(cEquipmentFrameX1, cEquipmentFrameY1, m_equipmentFrame, true);
	DrawGraph(cEquipmentFrameX2, cEquipmentFrameY2, m_equipmentFrame, true);
	DrawGraph(cEquipmentFrameX3, cEquipmentFrameY3, m_equipmentFrame, true);
	DrawGraph(cEquipmentFrameX4, cEquipmentFrameY4, m_equipmentFrame, true);

	//�������Ă��镨�`��
	EquipmentUIDraw(weapon, shield, armor, tool);

	//�R�A�o�[�̕`��
	DrawGraph(cCoreBarX, cCoreBarY, m_coreBackBar, true);

	SetFontSize(cFontSizeLarge);

	//�x������Ƃ��́@�x������
	//�A�C�e���̎��́@�A�C�e�������
	//�{�X�����ɓ���Ƃ��́@�������̒��ɓ���
	//���[�v����Ƃ��́@�]�ڂ���
	if (player.GetItemPick() || player.GetRestTouch() || player.GetBossEnter() || map.GetCore() || player.GetMessagePick())
	{
		DrawGraph(cActionUiX, cActionUiY, m_actionUI, true);
		DrawGraph(cActionUiX + cActionUiXOffset, cButtonY, m_yButton, true);
		//�x��
		if (player.GetRestTouch())
		{
			DrawFormatString(cActionTextX, cActionTextY, cTextColor, "�x������");
		}
		//�A�C�e��
		else if (player.GetItemPick())
		{
			DrawFormatString(cItemTextX, cActionTextY, cTextColor, "�A�C�e�������");
		}
		//�{�X���������
		else if (player.GetBossEnter())
		{
			DrawFormatString(cBossTextX, cActionTextY, cTextColor, "�������̒��ɓ���");
		}
		//���[�v
		else if (map.GetCore())
		{
			DrawFormatString(cWarpTextX, cActionTextY, cTextColor, "�]�ڂ���");
		}
		//���b�Z�[�W
		else if (player.GetMessagePick())
		{
			DrawFormatString(cMessageTextX, cActionTextY, cTextColor, "���b�Z�[�W��ǂ�");
		}
	}

	//�A�C�e�����������
	if (player.GetTaking())
	{
		if (player.GetNowFrame() == 2.0f)
		{
			cItemTakingUi = true;
		}
	}

	if (cItemTakingUi)
	{
		DrawGraph(cItemTakingX, cItemTakingY, m_itemTaking, true);
		DrawGraph(cActionUiX, cActionUiY, m_actionUI, true);
		DrawGraph(cActionUiX + cActionUiXOffset, cButtonY, m_bButton, true);

		DrawFormatString(cOkTextX, cOkTextY, cTextColor, "O K");

		ItemTakingUI(item.m_uiItem.u_BlackSword, m_blackSword, cItemTakingUiX, cItemTakingUiYBlackSword, cItemTakingUiCharX, cItemTakingUiCharY, "������");
		ItemTakingUI(item.m_uiItem.u_Distorted, m_uglyShield, cItemTakingUiX, cItemTakingUiYDistorted, cItemTakingUiCharX, cItemTakingUiCharY, "�c�񂾏�");
		ItemTakingUI(item.m_uiItem.u_ArmorNormal, m_commonArmor, cItemTakingUiX, cItemTakingUiYArmorNormal, cItemTakingUiCharX, cItemTakingUiCharY, "���ʂ̊Z");
		ItemTakingUI(item.m_uiItem.u_Bat, m_bat, cItemTakingUiX, cItemTakingUiYBat, cItemTakingUiCharX, cItemTakingUiCharY, "�؂̃o�b�g");
		ItemTakingUI(item.m_uiItem.u_WoodShield, m_woodShield, cItemTakingUiX, cItemTakingUiYWoodShield, cItemTakingUiCharX, cItemTakingUiCharY, "�؂̏�");

		//Bbutton�������ƕ���
		if (m_xpad.Buttons[13] == 1)
		{
			item.m_uiItem.u_SmallCore = 0;
			item.m_uiItem.u_MediumCore = 0;
			item.m_uiItem.u_Rubbish = 0;
			item.m_uiItem.u_BlackSword = 0;
			item.m_uiItem.u_Distorted = 0;
			item.m_uiItem.u_ArmorNormal = 0;
			item.m_uiItem.u_Bat = 0;
			item.m_uiItem.u_WoodShield = 0;

			cItemTakingUi = false;
		}
	}

	if (message.GetDraw())
	{
		DrawGraph(cActionUiX, cActionUiY, m_actionUI, true);
		DrawGraph(cActionUiX + cActionUiXOffset, cButtonY, m_bButton, true);

		DrawFormatString(cOkTextX, cOkTextY, cTextColor, "O K");
	}

	//�R�A���`��
	DrawFormatString(cPlayerCoreX, cPlayerCoreY, cTextColor, "%d", player.GetStatus().s_core);

	SetFontSize(cFontSizeLarge);
}

/// <summary>
/// �����̕`�揈��
/// </summary>
/// <param name="player"></param>
void UI::EquipmentDraw(Weapon& weapon, Shield& shield, Armor& armor)
{
	const int OffsetX = 30;
	const int ShieldOffsetX = 25;
	const int OffsetY = 20;

	//�E����
	if (weapon.GetFist())
	{
		//��
		DrawGraph(cWeaponX, cWeaponY, m_fist, true);
	}
	else if (weapon.GetBlack())
	{
		//������
		DrawGraph(cWeaponX + OffsetX, cWeaponY, m_blackSword, true);
	}
	else if (weapon.GetBat())
	{
		//�؂̖_
		DrawGraph(cWeaponX + OffsetX, cWeaponY, m_bat, true);
	}

	//������
	if (shield.GetFist())
	{
		//��
		DrawGraph(cShieldX, cShieldY, m_fist, true);
	}
	else if (shield.GetUgly())
	{
		//�X����
		DrawGraph(cShieldX + ShieldOffsetX, cShieldY + OffsetY, m_uglyShield, true);
	}
	else if (shield.GetWood())
	{
		//�؂̏�
		DrawGraph(cShieldX + ShieldOffsetX, cShieldY + OffsetY, m_woodShield, true);
	}

	//�h��
	if (armor.GetBody())
	{
		//����
		DrawGraph(cArmorBodyX, cArmorBodyY, m_body, true);
	}
	else if (armor.GetCommon())
	{
		//���}�Ȗh��
		DrawGraph(cArmorCommonX, cArmorCommonY, m_commonArmor, true);
	}
}

/// <summary>
/// �E����`�揈��
/// </summary>
void UI::RightDraw(Weapon& weapon, ItemManager& item)
{
	int x = 100;
	int y = 210;
	int offset = 150;
	//��
	DrawGraph(cFistX, cFistY, m_fist, true);

	//�A�C�e�����擾�������ɕ`�悷��
	for (const auto& itemName : item.GetItemOrder())
	{
		//�������`��
		if (itemName == "BlackSword")
		{
			DrawGraph(x, y, m_blackSword, true);

			y += offset;

		}
		//�؂̃o�b�g�`��
		else if (itemName == "Bat")
		{
			DrawGraph(x, y, m_bat, true);

			y += offset;

		}

	}
}

/// <summary>
/// ������`�揈��
/// </summary>
void UI::LeftDraw(Shield& shield, ItemManager& item)
{
	int x = 85;
	int y = 230;
	int offset = 150;


	//��
	DrawGraph(cFistX, cFistY, m_fist, true);


	//�A�C�e�����擾�������ɕ`��
	for (const auto& itemName : item.GetItemOrder())
	{
		//�X�����`��
		if (itemName == "Distorted")
		{	
			DrawGraph(x, y, m_uglyShield, true);

			y += offset;

		}
		//�؂̏��`��
		else if (itemName == "WoodShield")
		{
			DrawGraph(x, y, m_woodShield, true);

			y += offset;

		}
		
	}
}

/// <summary>
/// �h��`�揈��
/// </summary>
void UI::ArmorDraw(Armor& armor, ItemManager& item)
{
	int x = 85;
	int y = 220;
	int offset = 180;

	//��
	DrawGraph(cBodyX, cBodyY, m_body, true);

	//�A�C�e�����擾�������ɕ`��
	for (const auto& itemName : item.GetItemOrder())
	{
		if (itemName == "ArmorNormal")
		{
			//���}�ȊZ
			DrawGraph(x, y, m_commonArmor, true);

			y += offset;

		}
	}
}

/// <summary>
/// �Q�[����ʂ̑���UI�`�揈��
/// </summary>
/// <param name="weapon"></param>
/// <param name="shield"></param>
/// <param name="armor"></param>
void UI::EquipmentUIDraw(Weapon& weapon, Shield& shield, Armor& armor, Tool& tool)
{
	int rightX = 12;

	

	//�E����
	if (weapon.GetFist())
	{
		//��
		DrawGraph(cRightWeaponX, cRightWeaponY, m_fist, true);
	}
	else if (weapon.GetBlack())
	{
		//������
		DrawGraph(cRightWeaponX + rightX, cRightWeaponY, m_blackSword, true);
	}
	else if (weapon.GetBat())
	{
		//�؂̖_
		DrawGraph(cRightWeaponX + rightX, cRightWeaponY, m_bat, true);
	}

	//������
	if (shield.GetFist())
	{
		//��
		DrawGraph(cLeftWeaponX, cLeftWeaponY, m_fist, true);
	}
	else if (shield.GetUgly())
	{
		//�X����
		DrawGraph(cLeftWeaponX + cLeftWeaponOffsetX, cLeftWeaponY + cLeftWeaponOffsetY, m_uglyShield, true);
	}
	else if (shield.GetWood())
	{
		//�؂̏�
		DrawGraph(cLeftWeaponX + cLeftWeaponOffsetX, cLeftWeaponY + cLeftWeaponOffsetY, m_woodShield, true);
	}

	//�h��
	if (armor.GetBody())
	{
		//����
		DrawGraph(cArmorX, cArmorY, m_body, true);
	}
	else if (armor.GetCommon())
	{
		//���}�Ȗh��
		DrawGraph(cCommonArmorX, cCommonArmorY, m_commonArmor, true);
	}

	if (tool.GetHeel().sa_number > 0)
	{
		//�A�C�e��
		DrawGraph(cHeelStoneX, cHeelStoneY, m_heelStone, true);

		DrawFormatString(cHeelStoneTextX, cHeelStoneTextY, cTextColor, "%d", tool.GetHeel().sa_number);
	}
}

/// <summary>
/// �A�C�e������������̕`��
/// </summary>
/// <param name="item"></param>
/// <param name="handle"></param>
void UI::ItemTakingUI(int item, int handle, int x, int y, int charX, int charY, const char* letter)
{
	int X = 1100;
	int Y = 675;

	if (item > 0)
	{
		DrawGraph(x, y, handle, true);

		DrawFormatString(charX, charY, 0xffffff, letter);

		DrawFormatString(X, Y, 0xffffff, "%d", item);
	}

}

/// <summary>
/// ���S�`�揈��
/// </summary>
void UI::DiedDraw()
{
	int Max = 255;
	int WaitTime = 30;

	//���S�̕����̓���
	if (m_youDead < Max)
	{
		m_youDead++;

		m_waitResetTime = 0;
	}
	else if (m_youDead >= Max)
	{
		if (m_waitResetTime <= WaitTime)
		{
			m_waitResetTime++;
		}
		else
		{
			m_deadReset = true;
		}
	}

	//���S���̕������o��
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_youDead);
	DrawGraph(cDeadTextX, cDeadTextY, m_dead, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

/// <summary>
/// �X�e�[�^�X�o�[�̕`�揈��
/// </summary>
/// <param name="x"></param>
/// <param name="y"></param>
/// <param name="player"></param>
void UI::StatusDraw(int x, int y, Player& player)
{
	//HP�ő啝���v�Z
	int HPbar = player.GetMaxStatus().sm_hp / 30;
	//HP�ő啝���v�Z
	int HPBarWidth = (int)((float)player.GetStatus().s_hp / player.GetMaxStatus().sm_hp * (cHpBarInitialWidth + (cHpBarSegmentWidth * player.GetLevelStatus().sl_hp)));

	//�X�^�~�i�ő啝���v�Z
	int StaminaBar = player.GetMaxStatus().sm_stamina / 10;
	//�X�^�~�i�ő啝���v�Z
	int StaminaBarWidth = (int)((float)player.GetStatus().s_stamina / player.GetMaxStatus().sm_stamina * (cStaminaBarInitialWidth + (cStaminaBarSegmentWidth * player.GetLevelStatus().sl_stamina)));

	if (HPBarWidth > 0)
	{
		DrawRectGraph(x + cHpBarOffsetX, y + cHpBarOffsetY, 0, 0, HPBarWidth, cHpBarHeight, m_hpBar, true);
	}

	for (int i = 0; i < HPbar; i++)
	{
		//�����̃o�[��`��
		if (i > 0 && i != HPbar - 1)
		{
			if (i == 1)
			{
				DrawGraph(x + cHpBarSegmentXOffset, y, m_backCenterBar, true);
			}
			else
			{
				DrawGraph(x + cHpBarSegmentXOffset + (cHpBarSegmentWidth * (i - 1)), y, m_backCenterBar, true);
			}
		}
		//���[�̃o�[��`��
		else if (i == 0)
		{
			DrawGraph(x, y, m_backLeftBar, true);
		}
		//�E�[�̃o�[��`��
		else if (i == HPbar - 1)
		{
			DrawGraph(x + cHpBarSegmentXOffset + (cHpBarSegmentWidth * (i - 1)), y, m_backRightBar, true);
		}
	}

	if (StaminaBarWidth > 0)
	{
		DrawRectGraph(x + cStaminaBarOffsetX, y + cStaminaBarOffsetY, 0, 0, StaminaBarWidth, cStaminaBarHeight, m_staminaBar, true);
	}

	//���[�̃o�[��`��
	DrawGraph(x, y + cStaminaBarOffsetY, m_backLeftBar, true);

	for (int i = 0; i < StaminaBar; i++)
	{
		//�����̃o�[��`��
		if (i > 0 && i != StaminaBar - 1)
		{
			if (i == 1)
			{
				DrawGraph(x + cStaminaBarSegmentXOffset, y + cStaminaBarOffsetY, m_backCenterBar, true);
			}
			else
			{
				DrawGraph(x + cStaminaBarSegmentXOffset + (cStaminaBarSegmentWidth * (i - 1)), y + cStaminaBarOffsetY, m_backCenterBar, true);
			}
		}
		//�E�[�̃o�[��`��
		else if (i == StaminaBar - 1)
		{
			DrawGraph(x + cStaminaBarSegmentXOffset + (cStaminaBarSegmentWidth * (i - 1)) + (cStaminaBarSegmentXAdjustment * i), y + cStaminaBarOffsetY, m_backRightBar, true);
		}
	}
}

/// <summary>
/// �G��HP�`�揈��
/// </summary>
/// <param name="pos"></param>
/// <param name="hp"></param>
/// <param name="maxHP"></param>
void UI::EnemyHPDraw(VECTOR pos, int hp, int maxHP)
{
	int Height = 30;

	//3D���W����2D���W�ɕϊ�
	VECTOR screenPos;

	screenPos = ConvWorldPosToScreenPos(pos);

	int screenX = (int)screenPos.x + cEnemyHpBarXOffset;   //HP�o�[�̏ꏊ�𒲐�
	int screenY = (int)screenPos.y + cEnemyHpBarYOffset;   //HP�o�[�̍����𒲐�

	//HP�o�[�̍ő啝���v�Z
	int HPBarWidth = (int)((float)hp / maxHP * cHpBarWidth);

	if (HPBarWidth > 0)
	{
		DrawRectGraph(screenX, screenY, 0, 0, HPBarWidth, Height, m_hpBar, true);
	}
}

/// <summary>
/// �{�X��HP�`�揈��
/// </summary>
/// <param name="hp"></param>
/// <param name="maxHP"></param>
/// <param name="name"></param>
void UI::BossHPDraw(int hp, int maxHP, const char* name, const char* subName)
{
	const int Height = 50;

	//HP�o�[�̍ő啝���v�Z
	int HPBarWidth = (int)((float)hp / maxHP * cBossHpBarWidth);

	SetFontSize(cFontSizeMedium);
	//�{�X�̓��Ď�
	DrawString(cBossNameX, cBossSubnameY, subName, cTextColor);

	SetFontSize(cFontSizeMedium);

	//�{�X�̖��O
	DrawString(cBossNameX, cBossNameY, name, cTextColor);

	if (HPBarWidth > 0)
	{
		DrawRectGraph(cBossHpBarX, cBossHpBarY, 0, 0, HPBarWidth, Height, m_hpBar, true);
	}
}

/// <summary>
/// �I������
/// </summary>
void UI::End()
{
	DeleteGraph(m_blackSword);
	DeleteGraph(m_fist);
	DeleteGraph(m_body);
	DeleteGraph(m_uglyShield);
	DeleteGraph(m_commonArmor);
	DeleteGraph(m_bat);
	DeleteGraph(m_woodShield);
	DeleteGraph(m_dead);
	DeleteGraph(m_backRightBar);
	DeleteGraph(m_backLeftBar);
	DeleteGraph(m_backCenterBar);
	DeleteGraph(m_backOutBar);
	DeleteGraph(m_hpBar);
	DeleteGraph(m_staminaBar);
	DeleteGraph(m_coreBackBar);
	DeleteGraph(m_statusIcon);
	DeleteGraph(m_equipmentFrame);
	DeleteGraph(m_actionUI);
	DeleteGraph(m_itemTaking);
	DeleteGraph(m_yButton);
	DeleteGraph(m_bButton);
}
