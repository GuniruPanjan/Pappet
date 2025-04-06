#pragma once
#include "ItemBase.h"

/// <summary>
/// ���̃N���X
/// </summary>
class Shield : public ItemBase
{
public:
	//�R���X�g���N�^
	Shield();
	//�f�X�g���N�^
	virtual ~Shield();

	//������
	void Init();
	//�A�C�e���̏���������
	void ItemInit(float posX, float posY, float posZ, std::shared_ptr<MyLibrary::Physics> physics);
	//�X�V����
	void Update(MATRIX mat);
	//�}�b�v�A�C�e���Ƃ��Ă̍X�V����
	void ItemUpdate(bool taking);
	//�`�揈��
	void Draw();
	//�}�b�v�A�C�e���Ƃ��Ă̕`�揈��
	void ItemDraw();
	//�`�揈��
	void End();

	//�����������̔���
	bool GetFist() { return m_fist.ss_equipment; }
	bool SetFist(bool set) { return m_fist.ss_equipment = set; }

	bool GetUgly() { return m_ugly.ss_equipment; }
	bool SetUgly(bool set) { return m_ugly.ss_equipment = set; }

	bool GetWood() { return m_wood.ss_equipment; }
	bool SetWood(bool set) { return m_wood.ss_equipment = set; }

	//���̋��x
	float GetStrengthUgly() { return m_ugly.ss_strength; }
	float GetStrengthWood() { return m_wood.ss_strength; }
	
private:
	//���̍\����
	sShield m_fist;    //�f��
	sShield m_ugly;    //�X����
	sShield m_wood;    //�؂̏�

	float m_shieldMatrixX = 0.0f;
	float m_shieldMatrixY = 0.0f;
	float m_shieldMatrixZ = 0.0f;
};

