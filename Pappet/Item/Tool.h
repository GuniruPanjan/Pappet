#pragma once
#include "ItemBase.h"

/// <summary>
/// ����̃N���X
/// </summary>
class Tool : public ItemBase
{
	//�R���X�g���N�^
	Tool();
	//�f�X�g���N�^
	virtual ~Tool();

	//����������
	void Init();
	//�A�C�e���̏���������
	void ItemInit(float posX, float posY, float posZ, std::shared_ptr<MyLibrary::Physics> physics);
};

