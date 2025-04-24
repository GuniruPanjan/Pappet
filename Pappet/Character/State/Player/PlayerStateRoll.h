#pragma once
#include "Character/StateBase.h"

/// <summary>
/// �v���C���[�̉�����
/// </summary>
class PlayerStateRoll : public StateBase
{
public:
	//�R���X�g���N�^
	PlayerStateRoll(std::shared_ptr<CharacterBase> chara);
	//�f�X�g���N�^
	~PlayerStateRoll() override;
	//������
	void Init(int md) override;
	//�X�V
	virtual void Update() override;

private:
	//��𑬓x
	float m_avoidanceMove;
	//����ňړ�����x�N�^�[
	VECTOR m_rollMove;
	//����̕���
	VECTOR m_rollDirection;
};

