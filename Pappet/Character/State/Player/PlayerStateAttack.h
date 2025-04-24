#pragma once
#include "Character/StateBase.h"

/// <summary>
/// �v���C���[�̍U�����
/// </summary>
class PlayerStateAttack : public StateBase
{
public:
	//�R���X�g���N�^
	PlayerStateAttack(std::shared_ptr<CharacterBase> chara);
	//�f�X�g���N�^
	~PlayerStateAttack() override;
	//������
	void Init(int md) override;
	//�X�V
	virtual void Update() override;
	//�A�j���[�V�����ύX
	std::string GetAttackAnim();
private:
	float m_leftX, m_leftZ;
	//�A�j���[�V�����̒i�K
	int m_attackNumber;
	//���͎�t�ł��邩�ǂ���
	bool m_input;
	//�A�j���[�V�������i�K���ɏI�������
	bool m_endAnim;
	//�A�^�b�N�������x�N�^�[
	VECTOR m_attackMove;
	//�����X�s�[�h
	float m_move;
};

