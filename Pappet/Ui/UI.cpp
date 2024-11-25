#include "UI.h"

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
void UI::EquipmentDraw()
{
	//��
	DrawGraph(110, 310, m_fist, true);

	//��
	DrawGraph(310, 310, m_fist, true);

	//����
	DrawGraph(565, 320, m_body, true);
}

/// <summary>
/// �E����`�揈��
/// </summary>
void UI::RightDraw()
{
	//��
	DrawGraph(70, 56, m_fist, true);
	//������
	DrawGraph(100, 210, m_blackSword, true);
}

/// <summary>
/// ������`�揈��
/// </summary>
void UI::LeftDraw()
{
	//��
	DrawGraph(70, 56, m_fist, true);
	//�X����
	DrawGraph(85, 230, m_uglyShield, true);
}

/// <summary>
/// �h��`�揈��
/// </summary>
void UI::ArmorDraw()
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
