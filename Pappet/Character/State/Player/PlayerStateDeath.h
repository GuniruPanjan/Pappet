#pragma once
#include "Character/StateBase.h"

/// <summary>
/// �v���C���[�̎��S���
/// </summary>
class PlayerStateDeath : public StateBase
{
public:
	//�R���X�g���N�^
	PlayerStateDeath(std::shared_ptr<CharacterBase> chara);
	//�f�X�g���N�^
	~PlayerStateDeath() override;
	//������
	void Init(int md) override;
	//�X�V
	virtual void Update() override;

};

