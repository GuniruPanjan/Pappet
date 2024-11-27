#pragma once
#include "ItemBase.h"
#include <string>

/// <summary>
/// �h��̃N���X
/// </summary>
class Armor :  public ItemBase
{
public:
	//�R���X�g���N�^
	Armor();
	//�f�X�g���N�^
	virtual ~Armor();

	//������
	void Init();
	//�X�V����
	void Update();
	//�`�揈��
	void Draw();
	//�I������
	void End();

	//�����������̔���
	bool GetBody() { return m_body.sa_equipment; }
	bool SetBody(bool set) { return m_body.sa_equipment = set; }
	bool GetCommon() { return m_common.sa_equipment; }
	bool SetCommon(bool set) { return m_common.sa_equipment = set; }

	//�h���
	float GetBodyDefence() { return m_body.sa_defense; }
	float GetCommonDefence() { return m_common.sa_defense; }

	//�h��̃p�X
	std::string GetPath() { return m_path; }

private:
	//�h��̍\����
	sArmor m_body;      //����
	sArmor m_common;    //���}�ȊZ

	//�h��̃p�X
	std::string m_path;
};

