#pragma once
#include "UIBase.h"


class Player;
class EnemyManager;
class Setting;
class MapManager;
class ItemManager;
class Weapon;
class Shield;
class Armor;
class Tool;
class MessageManager;
class Font;
class SEManager;

/// <summary>
/// UI�֌W�̃N���X
/// </summary>
class UI : public UIBase
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	UI();

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~UI();

	/// <summary>
	/// ����������
	/// </summary>
	/// <param name="player">�v���C���[�̌Ăяo��</param>
	/// <param name="enemy">�G�l�~�[�̌Ăяo��</param>
	void Init();

	/// <summary>
	/// �`�揈��
	/// </summary>
	/// <param name="player">�v���C���[�̌Ăяo��</param>
	/// <param name="enemy">�G�l�~�[�̌Ăяo��</param>
	/// <param name="eq">�����̌Ăяo��</param>
	/// <param name="map">�}�b�v�̌Ăяo��</param>
	/// <param name="item">�A�C�e���̌Ăяo��</param>
	void Draw(Player& player, EnemyManager& enemy, Setting& eq, MapManager& map, ItemManager& item, Weapon& weapon, Shield& shield, Armor& armor, Tool& tool, MessageManager& message);

	/// <summary>
	/// �����̕`�揈��
	/// </summary>
	void EquipmentDraw(Weapon& weapon, Shield& shield, Armor& armor);

	/// <summary>
	/// �E����`�揈��
	/// </summary>
	void RightDraw(Weapon& weapon, ItemManager& item);

	/// <summary>
	/// ������`�揈��
	/// </summary>
	void LeftDraw(Shield& shield, ItemManager& item);

	/// <summary>
	/// �h��`�揈��
	/// </summary>
	void ArmorDraw(Armor& armor, ItemManager& item);



	//�����̕`��
	void EquipmentUIDraw(Weapon& weapon, Shield& shield, Armor& armor, Tool& tool);

	/// <summary>
	/// �A�C�e����������̕`��
	/// </summary>
	/// <param name="item"></param>
	/// <param name="handle"></param>
	void ItemTakingUI(int item, int handle, int x, int y, int charX, int charY, const char* letter);

	/// <summary>
	/// ���񂾂Ƃ��̕`��
	/// </summary>
	void DiedDraw(SEManager& se);

	//�{�X��|�����Ƃ��������o�`��
	void GetCoreDraw(SEManager& se);

	/// <summary>
	/// �X�e�[�^�X�o�[�̕`�揈��
	/// </summary>
	/// <param name="x"></param>
	/// <param name="y"></param>
	/// <param name="player"></param>
	void StatusDraw(int x, int y, Player& player);

	/// <summary>
	/// �G��HP�`�揈��
	/// </summary>
	/// <param name="enemy"></param>
	void EnemyHPDraw(VECTOR pos, int hp, int maxHP);

	/// <summary>
	/// �{�X��HP�`�揈��
	/// </summary>
	void BossHPDraw(int hp, int maxHP, const char* name, const char* subName);

	/// <summary>
	/// �I������
	/// </summary>
	void End();

	/// <summary>
	/// ���b�N��ʕϐ�
	/// </summary>
	int GetLockUI() { return m_tagetLock; }

	//���S���ă��Z�b�g����
	bool GetReset() { return m_deadReset; }

	//�������o�����Z�b�g����
	bool GetWinReset() { return m_winReset; }

private:
	unsigned int m_hpColor = 0xff0000;    //HP�J���[
	unsigned int m_staminaColor = 0x00ff00;       //�X�^�~�i�J���[

	//���S�̉摜���߃A���t�@�l
	int m_youDead;
	//�������̉摜���߃A���t�@�l
	int m_youWin;
	//�A���t�@�l���ω����邩�̔���
	bool m_alphaValue;

	//���o���̃��Z�b�g�܂ł̎���
	int m_waitResetTime;
	//���S���̃��Z�b�g
	bool m_deadReset;
	//�������̃��Z�b�g
	bool m_winReset;

	//������ʂ���߂�
	bool m_equipmentReturn;

	std::shared_ptr<Font> m_pFont;          //�t�H���g
	std::shared_ptr<Font> m_pSmallFont;     //���t�H���g

	XINPUT_STATE m_xpad;                //�p�b�h����
};


