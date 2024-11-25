#pragma once
#include "DxLib.h"
#include "Manager/SEManager.h"
#include<memory>

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
	int m_equipment;      //������ʊi�[�ϐ�
	int m_itemBox;        //�A�C�e����ʊi�[�ϐ�
	int m_selectEquipment;//�����I����ʊi�[�ϐ�

	//�A�C�e���̉摜
	int m_blackSword;     //�������̉摜�i�[�ϐ�
	int m_fist;           //���̉摜�i�[�ϐ�
	int m_body;           //���̂̉摜�i�[�ϐ�
	int m_uglyShield;     //�X�����̉摜�i�[�ϐ�
	int m_commonArmor;    //���}�ȊZ�摜�i�[�ϐ�

	//�X�}�[�g�|�C���^
	std::shared_ptr<SEManager> pse = std::make_shared<SEManager>();
};




