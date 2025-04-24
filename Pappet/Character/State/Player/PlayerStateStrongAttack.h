#pragma once
#include "Character/StateBase.h"

/// <summary>
/// �v���C���[�̋��U�����
/// </summary>
class PlayerStateStrongAttack : public StateBase
{
public:
	//�R���X�g���N�^
	PlayerStateStrongAttack(std::shared_ptr<CharacterBase> chara);
	//�f�X�g���N�^
	~PlayerStateStrongAttack() override;
	//������
	void Init (int md) override;
	//�X�V
	virtual void Update() override;
	//�v���C���[�̏󋵂ɂ���ăA�j���[�V��������������
	std::string GetStrongAnim();

private:
	float m_move;
	VECTOR m_strongMove;

};

