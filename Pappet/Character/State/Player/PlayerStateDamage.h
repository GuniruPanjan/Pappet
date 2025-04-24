#pragma once
#include "Character/StateBase.h"

/// <summary>
/// �v���C���[�̃_���[�W���
/// </summary>
class PlayerStateDamage : public StateBase
{
public:
	//�R���X�g���N�^
	PlayerStateDamage(std::shared_ptr<CharacterBase> chara);
	//�f�X�g���N�^
	~PlayerStateDamage() override;
	//������
	void Init(int md) override;
	//�X�V
	virtual void Update() override;
	//�q�b�g���̃A�j���[�V�����ύX
	std::string GetHitAnim();
};

