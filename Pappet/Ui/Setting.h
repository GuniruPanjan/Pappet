#pragma once
#include "Manager/SEManager.h"
#include "DxLib.h"
#include "Ui/UIBase.h"
#include <memory>
#include <map>
#include <string>
#include <List>

class SelectManager;
class Shield;
class Weapon;
class Armor;
class Player;
class CoreManager;
class Font;

/// <summary>
/// �ݒ菈���p�̃N���X
/// </summary>
class Setting : public UIBase
{
public:

	struct SelectXY
	{
		int oneX;
		int oneY;
		int secondX;
		int secondY;
	};

	struct SelectEquipment
	{
		bool right = false;
		bool left = false;
		bool armor = false;
	};

	//���x�����グ�����ʊi�[����\����
	struct LevelUp
	{
		int sl_all;
		int sl_hp;
		int sl_stamina;
		int sl_muscle;
		int sl_skill;
	}ms_levelUP;

public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	Setting();

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~Setting();

	/// <summary>
	/// ����������
	/// </summary>
	void Init();

	/// <summary>
	/// �X�V����
	/// </summary>
	void Update(SEManager& se);

	/// <summary>
	/// ���j���[�̍X�V����
	/// </summary>
	void MenuUpdate(Player& player, SEManager& se);

	/// <summary>
	/// �X�e�[�^�X��ʂ�ς���X�V����
	/// </summary>
	//void MenuChange();

	/// <summary>
	/// �����̍X�V����
	/// </summary>
	void EquipmentUpdate();

	/// <summary>
	/// �x���̍X�V����
	/// </summary>
	void RestUpdate(Player& player, CoreManager& core, bool rest, SEManager& se);

	/// <summary>
	/// ���x���A�b�v����
	/// </summary>
	void LevelUpdate(Player& player, CoreManager& core);

	/// <summary>
	/// ���x���A�b�v����2
	/// </summary>
	void LevelUp(CoreManager& core, int origin, int& level, int now);

	/// <summary>
	/// �A�C�e���{�b�N�X�X�V����
	/// </summary>
	void ItemBoxUpdate();

	/// <summary>
	/// �����I����ʍX�V����
	/// </summary>
	void EquipmentDecisionUpdate(Weapon& weapon, Shield& shield, Armor& armor, ItemManager& item);

	/// <summary>
	/// �`�揈��
	/// </summary>
	void Draw();

	/// <summary>
	/// �ݒ�̕`��
	/// </summary>
	/// <param name="volume">����</param>
	void SettingDraw(SEManager& se);

	/// <summary>
	/// ���j���[�`��
	/// </summary>
	void MenuDraw(int rb, int lb, int box);

	//�w�i���Â�����
	void MenuBackDraw();

	/// <summary>
	/// �X�e�[�^�X��ʂ�ς���`�揈��
	/// </summary>
	//void MenuChangeDraw();

	/// <summary>
	/// �����̕`�揈��
	/// </summary>
	void EquipmentDraw(Player& player);

	/// <summary>
	/// �x���̕`�揈��
	/// </summary>
	void RestDraw(bool rest);

	/// <summary>
	/// ���x���A�b�v�`��
	/// </summary>
	void LevelUpDraw(Player& player, CoreManager& core);

	/// <summary>
	/// �A�C�e���{�b�N�X�`�揈��
	/// </summary>
	//void ItemBoxDraw();

	/// <summary>
	/// �����I����ʕ`�揈��
	/// </summary>
	void EquipmentDecisionDraw(ItemManager& item);

	/// <summary>
	/// ���[�v�ł��Ȃ����̕`�揈��
	/// </summary>
	void CaveatDraw();

	/// <summary>
	/// �I������
	/// </summary>
	void End();

	/// <summary>
	/// ���ʂ��Ƃ�
	/// </summary>
	/// <returns>���ʂ�Ԃ�</returns>
	int GetVolume() { return m_volumeSize; }

	//SE�����
	int GetSe() { return m_seSize; }

	//�J�������x�����
	float GetCamera() { return m_cameraSize; }


	/// <summary>
	/// �ݒ�V�[�����o�����߂̔�����Ƃ�
	/// </summary>
	/// <returns>�����Ԃ�</returns>
	bool GetSettingScene() { return m_settingScene; }

	/// <summary>
	/// �ݒ�V�[�����o�����߂̔�������߂�
	/// </summary>
	/// <param name="scene">������Ƃ�</param>
	/// <returns>���ʂ�Ԃ�</returns>
	bool SetSettingScene(bool scene) { return m_settingScene = scene; }

	/// <summary>
	/// �������J��������Ƃ�
	/// </summary>
	/// <returns>�����Ԃ�</returns>
	bool GetEquipment() { return m_equipmentMenu; }

	/// <summary>
	/// �������J����������߂�
	/// </summary>
	/// <param name="re">������Ƃ�</param>
	/// <returns>���ʂ�Ԃ�</returns>
	bool SetEquipment(bool re) { return m_equipmentMenu = re; }

	/// <summary>
	/// �A�C�e����ʂ��J��������Ƃ�
	/// </summary>
	/// <returns>���ʂ�Ԃ�</returns>
	bool GetItem() { return m_itemMenu; }

	/// <summary>
	/// �A�C�e�����J����������߂�
	/// </summary>
	/// <param name="re">��������</param>
	/// <returns>���ʂ�Ԃ�</returns>
	bool Setitem(bool re) { return m_itemMenu = re; }

	/// <summary>
	/// �X�e�[�^�X�̃��x��Up��ʂ��J������
	/// </summary>
	bool GetLevel() { return m_statusLevel; }

	/// <summary>
	/// ������I�������ʑJ�ڔ�������
	/// </summary>
	/// <returns></returns>
	bool GetDecision() { return m_decisionEquipment; }

	/// <summary>
	/// ���̑������J���Ă��邩
	/// </summary>
	/// <returns></returns>
	SelectEquipment GetSelect() { return m_select; }

	/// <summary>
	/// ��ʂɖ߂锻����Ƃ�
	/// </summary>
	/// <returns>�����Ԃ�</returns>
	bool GetReturn() { return m_returnMenu; }

	/// <summary>
	/// ��ʂɖ߂锻������߂�
	/// </summary>
	/// <param name="re">������Ƃ�</param>
	/// <returns>���ʂ�Ԃ�</returns>
	bool SetReturn(bool re) { return m_returnMenu = re; }

	/// <summary>
	/// �^�C�g���ɖ߂锻����Ƃ�
	/// </summary>
	/// <returns>������Ƃ�</returns>
	bool GetTitle() { return m_titleMenu; }

	/// <summary>
	/// �x���X�^�[�g
	/// </summary>
	/// <returns></returns>
	bool GetReset() { return m_reset; }
	bool SetReset(bool set) { return m_reset = set; }

	/// <summary>
	/// �x���n�_�Ƀ��[�v���邩�̔�����Ƃ�
	/// </summary>
	/// <returns></returns>
	bool GetRestWarp() { return m_restWarp; }
	bool SetRestWarp(bool set) { return m_restWarp = set; }

	//�҂����Ԃ�ݒ肷��
	int SetWait(int set) { return m_waitTime = set; }

private:

	void SettingChange(int& setting, int& cSetting, int one, int two, int three, int four, int five, int six, int seven, int eight, int nine, int ten);
	void CameraChange(float& setting, float& cSetting, float one, float two, float three, float four, float five, float six, float seven, float eight, float nine, float ten);
	void SettingBarChange(int Decision, float& bar, float& cBar);
	void SettingBarDraw(float bar, int x, int y);

	void WeaponUpdate(std::list<std::string> list, Weapon& weapon, int right);
	void WeaponDraw(std::list<std::string> list, int right);
	void ShieldUpdate(std::list<std::string> list, Shield& shield, int left);
	void ShieldDraw(std::list<std::string> list, int left);
	void ArmorUpdate(std::list<std::string> list, Armor& armor, int body);
	void ArmorDraw(std::list<std::string> list, int body);

private:

	XINPUT_STATE m_xpad;      //�p�b�h����

	int m_black;              //�����摜�i�[�ϐ�
	int m_white;              //�����摜�i�[�ϐ�
	int m_back;               //�����摜�i�[�ϐ�
	int m_volumeSize;         //����
	int m_seSize;             //SE����
	float m_cameraSize;       //�J�������x
	int m_menuSelect[5];      //���j���[�I�� 
	int m_selectX;            //�I���摜��X���W
	int m_selectY;            //�I���摜��Y���W
	int m_blackPal;           //�����摜�̃A���t�@�l�ۑ�
	int m_whitePal;           //�����摜�̃A���t�@�l�ۑ�
	int m_button;             //���������Ԃ𓾂�
	int m_thumb;              //���������Ԃ𓾂�
	int m_waitTime;           //���͂�҂��߂̎���
	int m_brightnessColor;    //���邳�̐F
	int m_bgmColor;           //���ʂ̐F
	int m_seColor;            //SE�̐F
	int m_cameraColor;        //�J�����̐F
	int m_returnColor;        //�߂�̐F
	int m_menuColor[5];       //���j���[�̐F
	int m_core;               //�\�L�p�ɃR�A���擾����
	int m_right;              //�E�����̑I��ł�Ƃ�����i�[����ϐ�
	int m_left;               //�������̑I��ł�Ƃ�����i�[����ϐ�
	int m_armor;              //�Z�����̑I��ł�Ƃ�����i�[����ϐ�

	float m_brightBar;        //���邳�̃o�[
	float m_volumeBar;        //���ʂ̃o�[
	float m_seBar;            //SE�̃o�[
	float m_cameraBar;        //�J�����̃o�[

	bool m_one;               //�P������
	bool m_settingScene;      //�ݒ肷�邽�߂̉�ʂ��Ăԕϐ�
	bool m_brightness;        //���邳�ݒ�
	bool m_volume;            //BGM�ݒ�
	bool m_se;                //SE�ݒ�
	bool m_camera;            //�J�������x
	bool m_equipmentMenu;     //�������j���[
	bool m_itemMenu;          //�A�C�e���{�b�N�X���j���[
	bool m_decisionEquipment; //�����I�����
	bool m_returnMenu;        //�߂郁�j���[
	bool m_titleMenu;         //�^�C�g�����j���[
	bool m_statusLevel;       //�X�e�[�^�X���J������
	bool m_blend;             //�u�����h����
	bool m_up;                //���x�����グ���邩�̔���
	bool m_menuDecision;      //���j���[�ł̌��蔻��
	bool m_restWarp;          //�x���n�_�����[�v���邽�߂̔���
	bool m_caveat;            //���ӏ����̕\��
	bool m_reset;             //�x���ɂ�郊�Z�b�g


	//�I�񂾑���
	SelectEquipment m_select;

	//�I��ł���F��ς���
	SelectXY m_change;
	SelectXY m_equipmentColorPos;
	SelectXY m_selectObject;

	std::list<std::string> m_weaponList;      //����̓��菇�ɓ���郊�X�g
	std::list<std::string> m_shieldList;      //���̓��菇�ɓ���郊�X�g
	std::list<std::string> m_armorList;       //�Z�̓��菇�ɓ���郊�X�g

	//�X�}�[�g�|�C���^
	std::shared_ptr<SelectManager> m_pSelect = std::make_shared<SelectManager>();
	std::shared_ptr<Font> m_pFont;
	std::shared_ptr<Font> m_pSmallFont;
	std::shared_ptr<Font> m_pBigFont;
};




