#pragma once
#include "Character/StateBase.h"

/// <summary>
/// �v���C���[�̕������
/// </summary>
class PlayerStateWalk : public StateBase
{
public:
	//�R���X�g���N�^
	PlayerStateWalk(std::shared_ptr<CharacterBase> chara);
	//�f�X�g���N�^
	~PlayerStateWalk() override;
	//������
	void Init(int md) override;
	//�X�V
	virtual void Update() override;

	//���͕�����������A�j���[�V�������擾����
	std::string GetWalkAnim(eDir dir);

private:
	eDir m_dir;
	int m_walkCount;
	int m_noInputFrame;
	int m_input;
};

