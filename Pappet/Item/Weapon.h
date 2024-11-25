#pragma once
#include "ItemBase.h"

/// <summary>
/// ����̃N���X
/// </summary>
class Weapon : public ItemBase
{
public:
	//�R���X�g���N�^
	Weapon();
	//�f�X�g���N�^
	virtual ~Weapon();

	//������
	void Init();
	//�X�V����
	void Update(MATRIX mat);
	//�`�揈��
	void Draw();
	//�I������
	void End();

	//�X�e�[�^�X�̔���
	float GetFistAttack() { return m_fist.sw_attack; }
	float GetBlackAttack() { return m_black.sw_attack; }

	//�����������̔���
	bool GetFist() { return m_fist.sw_equipment; }
	bool SetFist(bool set) { return m_fist.sw_equipment = set; }
	bool GetBlack() { return m_black.sw_equipment; }
	bool SetBlack(bool set) { return m_black.sw_equipment = set; }

private:
	//���̍\����
	sWeapon m_fist;      //�f��
	sWeapon m_black;     //������
};

