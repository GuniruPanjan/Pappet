#pragma once
#include "Character/StateBase.h"

/// <summary>
/// �v���C���[�̃A�C�e���g�p���
/// </summary>
class PlayerStateItem : public StateBase
{
public:
	//�R���X�g���N�^
	PlayerStateItem(std::shared_ptr<CharacterBase> chara);
	//�f�X�g���N�^
	~PlayerStateItem() override;
	//������
	void Init(int md) override;
	//�X�V
	virtual void Update() override;

private:
	bool m_endAnim;      //�A�j���[�V�����I���t���O
};

