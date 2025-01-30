#include "UI.h"
#include "Item/Weapon.h"
#include "Item/Shield.h"
#include "Item/Armor.h"
#include "Character/Player.h"
#include "Manager/EnemyManager.h"

namespace
{
	int c_maxHP = 800;
	int c_maxHPWidth = 1000;
}

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
	m_deadBack = 0;
	m_deadA = 0;
	m_waitResetTime = 0;
	m_deadReset = false;

	m_blackSword = MyLoadGraph("Data/UI/������UI.png", 3, 3);
	m_fist = MyLoadGraph("Data/UI/��UI.png", 3, 3);
	m_body = MyLoadGraph("Data/UI/����UI.png", 3, 3);
	m_uglyShield = MyLoadGraph("Data/UI/�X����UI.png", 4, 4);
	m_commonArmor = MyLoadGraph("Data/UI/���}�ȊZUI.png", 3, 3);
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
}

/// <summary>
/// �Q�[����UI�̕`�揈��
/// </summary>
/// <param name="player"></param>
/// <param name="enemy"></param>
/// <param name="eq"></param>
/// <param name="map"></param>
/// <param name="item"></param>
void UI::Draw(Player& player, EnemyManager& enemy, Setting& eq, MapManager& map, ItemManager& item)
{
	//�X�e�[�^�X�o�[�̕`��
	StatusDraw(180, 30, player);

	//�v���C���[�̏�ԃA�C�R���`��
	DrawGraph(-50, 0, m_statusIcon, true);

	//�����̕`��
	DrawGraph(150, 550, m_equipmentFrame, true);
	DrawGraph(0, 665, m_equipmentFrame, true);
	DrawGraph(300, 665, m_equipmentFrame, true);
	DrawGraph(150, 780, m_equipmentFrame, true);

	//�R�A�o�[�̕`��
	DrawGraph(1050, 750, m_coreBackBar, true);

	SetFontSize(40);

	//�R�A���`��
	DrawFormatString(1400, 905, 0xffffff, "%d", player.GetStatus().s_core);

	SetFontSize(40);
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
/// ���S�`�揈��
/// </summary>
void UI::DiedDraw()
{
	//���S���̔w�i�̓���
	if (m_deadBack < 150)
	{
		m_deadBack++;
	}
	//���S�̕����̓���
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

	//�w�i�̐F�𔖂�����
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_deadBack);
	DrawBox(0, 100, 2000, 400, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//���S���̕������o��
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_deadA);
	DrawGraph(-150, -100, m_dead, true);
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
	int HPBarWidth = (int)((float)player.GetStatus().s_hp / player.GetMaxStatus().sm_hp * (185 + (50 * player.GetLevelStatus().sl_hp)));

	//�X�^�~�i�ő啝���v�Z
	int StaminaBar = player.GetMaxStatus().sm_stamina / 10;
	//�X�^�~�i�ő啝���v�Z
	int StaminaBarWidth = (int)((float)player.GetStatus().s_stamina / player.GetMaxStatus().sm_stamina * (150 + (10 * player.GetLevelStatus().sl_stamina)));

	if (HPBarWidth > 0)
	{
		DrawRectGraph(x + 5, y + 8, 0, 0, HPBarWidth, 45, m_hpBar, true);
	}


	for (int i = 0; i < HPbar; i++)
	{
		//�����̃o�[��`��
		if (i > 0 && i != HPbar - 1)
		{
			if (i == 1)
			{
				DrawGraph(x + 23, y, m_backCenterBar, true);
			}
			else
			{
				DrawGraph(x + 23 + (50 * (i - 1)), y, m_backCenterBar, true);
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
			DrawGraph(x + 23 + (50 * (i - 1)), y, m_backRightBar, true);
		}

		
	}

	if (StaminaBarWidth > 0)
	{
		DrawRectGraph(x - 8, y + 50, 0, 0, StaminaBarWidth, 50, m_staminaBar, true);
	}

	//���[�̃o�[��`��
	DrawGraph(x, y + 50, m_backLeftBar, true);

	for (int i = 0; i < StaminaBar; i++)
	{
		//�����̃o�[��`��
		if (i > 0 && i != StaminaBar - 1)
		{
			if (i == 1)
			{
				DrawGraph(x + 23, y +50, m_backCenterBar, true);
			}
			else
			{
				DrawGraph(x + 23 + (10 * (i - 1)), y + 50, m_backCenterBar, true);
			}
		}
		//�E�[�̃o�[��`��
		else if (i == StaminaBar - 1)
		{
			DrawGraph(x + 23 + (10 * (i - 1)) + (2 * i), y + 50, m_backRightBar, true);
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
	//3D���W����2D���W�ɕϊ�
	VECTOR screenPos;
	
	screenPos = ConvWorldPosToScreenPos(pos);

	int screenX = (int)screenPos.x - 100;   //HP�o�[�̏ꏊ�𒲐�
	int screenY = (int)screenPos.y - 300;   //HP�o�[�̍����𒲐�

	//HP�o�[�̍ő啝���v�Z
	int HPBarWidth = (int)((float)hp / maxHP * 200);

	if (HPBarWidth > 0)
	{
		DrawRectGraph(screenX, screenY, 0, 0, HPBarWidth, 30, m_hpBar, true);
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
	//HP�o�[�̍ő啝���v�Z
	int HPBarWidth = (int)((float)hp / maxHP * 800);

	SetFontSize(30);
	//�{�X�̓��Ď�
	DrawString(450, 670, subName, 0xffffff);

	SetFontSize(40);

	//�{�X�̖��O
	DrawString(450, 700, name, 0xffffff);

	if (HPBarWidth > 0)
	{
		DrawRectGraph(450, 750, 0, 0, HPBarWidth, 50, m_hpBar, true);
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
}
