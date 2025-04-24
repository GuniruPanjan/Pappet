#pragma once
#include "Character/StateBase.h"

/// <summary>
/// �v���C���[�̃W�����v���
/// </summary>
class PlayerStateJump : public StateBase
{
public:
	//�R���X�g���N�^
	PlayerStateJump(std::shared_ptr<CharacterBase> chara);
	//�f�X�g���N�^
	~PlayerStateJump();
	//������
	void Init(int md) override;
	//�X�V
	virtual void Update() override;

private:
	//��ԑJ�ڂ̂��߂̃����o�֐��|�C���^
	using UpdateFunc_t = void (PlayerStateJump::*)();
	UpdateFunc_t m_updateFund;

	//�W�����v�㏸���
	void UpUpdate();
	//�W�����v�����
	void LoopUpdate();
	//�W�����v���~���
	void DownUpdate();
	//�W�����v�t���[����
	int m_jumpFrame;
};

