#include "PlayerStateDeath.h"
#include "Character/Player.h"
#include "External/CsvLoad.h"
#include "Character/CharacterBase.h"


/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="chara"></param>
PlayerStateDeath::PlayerStateDeath(std::shared_ptr<CharacterBase> chara) :
	StateBase(chara)
{
	//���݂̃X�e�[�g�����S��Ԃɂ���
	m_nowState = StateKind::Death;
	chara->ChangeStateAnim(CsvLoad::GetInstance().GetAnimData(chara->GetCharacterName(), "Death"), true);
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
PlayerStateDeath::~PlayerStateDeath()
{
}

/// <summary>
/// ������
/// </summary>
/// <param name="md"></param>
void PlayerStateDeath::Init(int md)
{
	m_stageCol = md;
}

/// <summary>
/// �X�V
/// </summary>
void PlayerStateDeath::Update()
{
	//�����傪�v���C���[���ǂ������`�F�b�N����
	if (!CheckState()) return;

	//�����Ă���L�����N�^�[�x�[�X�N���X���v���C���[�N���X�ɃL���X�g����(�_�E���L���X�g)
	auto own = std::dynamic_pointer_cast<Player>(m_pChara.lock());

	//�v���C���[�̑��x��0�ɂ���
	auto prevVel = own->GetRigidbody()->GetVelocity();
	own->GetRigidbody()->SetVelocity(MyLibrary::LibVec3(0.0f, prevVel.y, 0.0f));
}
