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
	void Draw(MATRIX mat);
	//�I������
	void End();
};

