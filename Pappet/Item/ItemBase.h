#pragma once
#include "DxLib.h"
#include "Library/MyLibrary.h"
#include "Object/SearchObject.h"
#include <map>
#include <string>

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
		m_itemPick(false),
		m_isTaking(false),
		m_framePos(VGet(0.0f,0.0f,0.0f)),
		m_pos(VGet(0.0f,0.0f,0.0f)),
		m_transMatrix(),
		m_mixMatrix()
	{

	}
	//�f�X�g���N�^
	virtual ~ItemBase() {};

	//�}�b�v�A�C�e���Ƃ��Ă̏���������
	virtual void ItemInit(float posX, float posY, float posZ, std::shared_ptr<MyLibrary::Physics> physics) {};
	//�}�b�v�A�C�e���Ƃ��Ă̍폜����
	void ItemFinalize(std::shared_ptr<MyLibrary::Physics> physics);
	//�}�b�v�A�C�e���Ƃ��Ă̍X�V����
	virtual void ItemUpdate() {};
	//�}�b�v�A�C�e���Ƃ��Ă̏I������
	void ItemEnd();

	//�A�C�e�����E���邩���̔����Ԃ�
	bool GetItemPick() { return m_itemPick; }
	//�A�C�e�����擾������
	bool SetItemPick(bool set) { return m_isTaking = set; }
protected:
	//�A�C�e������ǂݍ���
	void LoadData(std::string name);
	//�����蔻����쐬
	void InitSearch(MyLibrary::LibVec3 pos, float radius);

protected:
	std::shared_ptr<SearchObject> m_pSearch;     //���G����

	//�A�C�e���̌���
	int m_itemUse;
	//�A�C�e���̃n���h��
	int m_itemHandle;
	//�t���[������
	int m_frameIndex;
	//�A�C�e�������邩�̔���
	bool m_itemPick;
	//���ꂽ���ǂ����̔���
	bool m_isTaking;
	//�t���[���|�W�V����
	VECTOR m_framePos;
	//�|�W�V����
	VECTOR m_pos;
	//���f���̍s��
	MATRIX m_transMatrix;
	//���f���̍����s��
	MATRIX m_mixMatrix;

	std::shared_ptr<MyLibrary::Physics> m_pPhysics;

	MyLibrary::LibVec3 m_centerPos;              //���S���W
};
