#pragma once
#include "Character/StateBase.h"

/// <summary>
/// �v���C���[�̃K�[�h���
/// </summary>
class PlayerStateGuard : public StateBase
{
public:
	//�R���X�g���N�^
	PlayerStateGuard(std::shared_ptr<CharacterBase> chara);
	//�f�X�g���N�^
	~PlayerStateGuard() override;
	//������
	void Init (int md) override;
	//�X�V
	virtual void Update() override;

	//���͕�����������A�j���[�V�������擾����
	std::string GetGuardAnim(eDir dir);

private:
	eDir m_dir;
	bool m_run;

};

