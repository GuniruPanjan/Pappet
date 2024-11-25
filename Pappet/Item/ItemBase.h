#pragma once
#include "DxLib.h"

/// <summary>
/// �A�C�e���̐e�N���X
/// </summary>
class ItemBase
{
public:
	//����̍\����
	struct sWeapon
	{
		float sw_attack;               //�U����
		float sw_radius;               //����̓����蔻��
		bool sw_equipment = false;     //�����������̔���
	};

	//���̍\����
	struct sShield
	{
		int ss_cut;                    //�J�b�g��
		int ss_strength;               //�����x
		bool ss_equipment = false;     //�����������̔���
	};

	//�h��̍\����
	struct sArmor
	{
		int sa_defense;                //�h���
		bool sa_equipment = false;     //�����������̔���
	};

	//�R���X�g���N�^
	ItemBase() :
		m_itemUse(0),
		m_itemHandle(-1),
		m_frameIndex(0),
		m_framePos(VGet(0.0f,0.0f,0.0f)),
		m_pos(VGet(0.0f,0.0f,0.0f)),
		m_transMatrix(),
		m_mixMatrix()
	{

	}
	//�f�X�g���N�^
	virtual ~ItemBase() {};

protected:
	//�A�C�e���̌���
	int m_itemUse;
	//�A�C�e���̃n���h��
	int m_itemHandle;
	//�t���[������
	int m_frameIndex;
	//�t���[���|�W�V����
	VECTOR m_framePos;
	//�|�W�V����
	VECTOR m_pos;
	//���f���̍s��
	MATRIX m_transMatrix;
	//���f���̍����s��
	MATRIX m_mixMatrix;
};
