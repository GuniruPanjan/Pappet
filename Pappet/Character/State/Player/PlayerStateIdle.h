#pragma once
#include "Character/StateBase.h"

/// <summary>
/// �v���C���[�̑ҋ@���
/// </summary>
class PlayerStateIdle : public StateBase
{
public:
	//�R���X�g���N�^
	PlayerStateIdle(std::shared_ptr<CharacterBase> chara);
	//�f�X�g���N�^
	~PlayerStateIdle() override;
	//����������
	void Init(int md) override;
	//�X�V����
	virtual void Update() override;
};

