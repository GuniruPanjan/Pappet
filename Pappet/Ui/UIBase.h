#pragma once
#include "DxLib.h"
#include "Manager/SEManager.h"
#include<memory>

class ItemManager;

/// <summary>
/// UI�֘A�̉摜�Ȃǂ�ǂݍ��ނ��߂̃N���X
/// </summary>
class UIBase
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	UIBase();

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~UIBase();

	/// <summary>
	/// �摜�̃T�C�Y��ς��ă��[�h����֐�
	/// </summary>
	/// <param name="FileName">�摜��FileName</param>
	/// <param name="XSize">�摜�̉��������鐔</param>
	/// <param name="YSize">�摜�̏c�������鐔</param>
	/// <returns>�T�C�Y��ς����摜��Ԃ�</returns>
	int MyLoadGraph(const char* FileName, int XSize, int YSize);

protected:
	int m_heelStone;      //�񕜐΂̉摜�i�[�ϐ�
	int m_eqFist;         //�����̌��摜�i�[�ϐ�
	int m_sword;          //���̉摜�i�[�ϐ�
	int m_eqSword;        //�����̌��摜�i�[�ϐ�
	int m_eqShield;       //�����̏��摜�i�[�ϐ�

	//UI���
	int m_equipment;      //������ʊi�[�ϐ�
	//int m_itemBox;        //�A�C�e����ʊi�[�ϐ�
	int m_selectEquipment;//�����I����ʊi�[�ϐ�
	int m_rest;           //�x���I����ʊi�[�ϐ�
	int m_levelUp;        //���x���A�b�v��ʊi�[�ϐ�
	int m_backRightBar;   //�o�[�̉E�[�̉摜�i�[�ϐ�
	int m_backLeftBar;    //�o�[�̍��[�̉摜�i�[�ϐ�
	int m_backCenterBar;  //�o�[�̒����̉摜�i�[�ϐ�
	int m_backOutBar;     //�o�[�̊O�g�摜�i�[�ϐ�   
	int m_hpBar;          //HP�o�[�[�̉摜�i�[�ϐ�
	int m_staminaBar;     //�X�^�~�i�o�[�̉摜�i�[�ϐ�
	int m_coreBackBar;    //�R�A�o�[�̔w�i�摜�i�[�ϐ�
	int m_statusIcon;     //�X�e�[�^�X�A�C�R���摜�i�[�ϐ�
	int m_equipmentFrame; //�����̘g�摜�i�[�ϐ�
	int m_tagetLock;      //�^�[�Q�b�g���b�N�摜�i�[�ϐ�
	int m_menu;           //���j���[�摜�i�[�ϐ�

	//Setting���
	int m_settingHandle;        //�ݒ�̕����摜�i�[�ϐ�
	int m_brightnessHandle;     //���邳�̕����摜�i�[�ϐ�
	int m_bgmHandle;            //BGM�̕����摜�i�[�ϐ�
	int m_seHandle;             //SE�̕����摜�i�[�ϐ�
	int m_cameraHandle;         //�J�����̕����摜�i�[�ϐ�
	int m_returnHandle;         //�߂�̕����摜�i�[�ϐ�
	int m_edgeHandle;           //�[�̉摜�i�[�ϐ�
	int m_barHandle;            //�o�[�̉摜�i�[�ϐ�
	int m_lineHandle;           //���C���̉摜�i�[�ϐ�
	int m_arrowHandle;          //�I���̉摜�i�[�ϐ�

	//�A�C�e���̉摜
	int m_blackSword;     //�������̉摜�i�[�ϐ�
	int m_fist;           //���̉摜�i�[�ϐ�
	int m_body;           //���̂̉摜�i�[�ϐ�
	int m_uglyShield;     //�X�����̉摜�i�[�ϐ�
	int m_commonArmor;    //���}�ȊZ�摜�i�[�ϐ�
	int m_bat;            //�؂̃o�b�g�摜�i�[�ϐ�
	int m_woodShield;     //�؂̏��摜�i�[�ϐ�

	//UI�̉摜
	int m_dead;           //���S���̉摜�i�[�ϐ�
	int m_victory;        //�������̉摜�i�[�ϐ�
	int m_selectUi;       //�I�����̉摜�i�[�ϐ�
	int m_actionUI;       //�A�N�V�������i�[����ϐ�
	int m_itemTaking;     //�A�C�e�����擾�����ϐ�
	int m_yButton;        //Y�{�^���摜�i�[�ϐ�
	int m_bButton;        //B�{�^���摜�i�[�ϐ�
	int m_aButton;        //A�{�^���摜�i�[�ϐ�
	int m_xButton;        //X�{�^���摜�i�[�ϐ�
	int m_rbButton;       //RB�{�^���摜�i�[�ϐ�
	int m_rtButton;       //RT�{�^���摜�i�[�ϐ�
	int m_lbButton;       //LB�{�^���摜�i�[�ϐ�
	int m_startButton;    //�X�^�[�g�{�^���摜�i�[�ϐ�
	int m_lStick;         //���X�e�B�b�N�摜�i�[�ϐ�
	int m_rStick;         //�E�X�e�B�b�N�摜�i�[�ϐ�
	int m_messageUI;      //���b�Z�[�W�摜�i�[�ϐ�

	ItemManager* m_pItem;
};




