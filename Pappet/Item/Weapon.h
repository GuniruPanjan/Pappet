#pragma once
#include "ItemBase.h"
#include <memory>
#include <map>
#include <string>

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
	//�A�C�e���̏���������
	void ItemInit(float posX, float posY, float posZ, std::shared_ptr<MyLibrary::Physics> physics);
	//�X�V����
	void Update(MATRIX mat);
	//����̓����蔻��p���O
	VECTOR WeaponPattern();
	VECTOR WeaponTip();
	//�}�b�v�A�C�e���Ƃ��Ă̍X�V����
	void ItemUpdate(bool taking);
	//�`�揈��
	void Draw();
	//�}�b�v�A�C�e���Ƃ��Ă̕`�揈��
	void ItemDraw();
	//�I������
	void End();

	//�X�e�[�^�X�̔���
	float GetFistAttack() { return m_fist.sw_attack; }

	float GetBlackAttack() { return m_black.sw_attack; }
	float GetBlackMuscle() { return m_black.sw_muscle; }
	float GetBlackSkill() { return m_black.sw_skill; }

	float GetBatAttack() { return m_bat.sw_attack; }
	float GetBatMuscle() { return m_bat.sw_muscle; }
	float GetBatSkill() { return m_bat.sw_skill; }

	//�����������̔���
	bool GetFist() { return m_fist.sw_equipment; }
	bool SetFist(bool set) { return m_fist.sw_equipment = set; }

	bool GetBlack() { return m_black.sw_equipment; }
	bool SetBlack(bool set) { return m_black.sw_equipment = set; }

	bool GetBat() { return m_bat.sw_equipment; }
	bool SetBat(bool set) { return m_bat.sw_equipment = set; }

private:
	//���̍\����
	sWeapon m_fist;      //�f��
	sWeapon m_black;     //������
	sWeapon m_bat;       //�؂̃o�b�g

	int m_weaponPatternFrame;
	int m_weaponTipFrame;

	const char* m_weaponPatternName;
	const char* m_weaponTipName;

	float m_size;

	float m_weaponMatrixX = 0.0f;
	float m_weaponMatrixY = 0.0f;
	float m_weaponMatrixZ = 0.0f;
};

