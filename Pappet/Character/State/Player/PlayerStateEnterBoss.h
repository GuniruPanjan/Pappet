#pragma once
#include "Character/StateBase.h"

/// <summary>
/// �v���C���[���{�X�����ɓ�����
/// </summary>
class PlayerStateEnterBoss : public StateBase
{
public:
	//�R���X�g���N�^
	PlayerStateEnterBoss(std::shared_ptr<CharacterBase> chara);
	//�f�X�g���N�^
	~PlayerStateEnterBoss() override;
	//������
	void Init(int md) override;
	//�X�V
	virtual void Update() override;
private:
	VECTOR m_moveVector;
};

