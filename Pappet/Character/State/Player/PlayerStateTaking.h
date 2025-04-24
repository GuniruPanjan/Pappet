#pragma once
#include "Character/StateBase.h"

/// <summary>
/// �v���C���[�̃A�C�e���擾���
/// </summary>
class PlayerStateTaking : public StateBase
{
public:
	//�R���X�g���N�^
	PlayerStateTaking(std::shared_ptr<CharacterBase> chara);
	//�f�X�g���N�^
	~PlayerStateTaking() override;
	//������
	void Init (int md) override;
	//�X�V
	virtual void Update() override;
private:
	bool m_animEnd;
};

