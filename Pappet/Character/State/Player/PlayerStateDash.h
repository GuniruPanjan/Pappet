#pragma once
#include "Character/StateBase.h"

/// <summary>
/// �v���C���[�̃_�b�V�����
/// </summary>
class PlayerStateDash : public StateBase
{
public:
	//�R���X�g���N�^
	PlayerStateDash(std::shared_ptr<CharacterBase> chara);
	//�f�X�g���N�^
	~PlayerStateDash() override;
	//������
	void Init(int md) override;
	//�X�V
	virtual void Update() override;

	//������Ԃ���A�j���[�V��������������
	std::string GetRunAnim();

private:
	int m_noInputFrame;
};

