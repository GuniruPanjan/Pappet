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
	//�X�V����
	void Update(MATRIX mat);
	//�`�揈��
	void Draw();
	//�`�揈��
	void End();
};

