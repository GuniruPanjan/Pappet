#include "Shield.h"
#include "Manager/HandleManager.h"

namespace
{
	//����̍s��
	constexpr float cShieldMatrixY = 3.142f;
	constexpr float cShieldMatrixZ = 0.0f;

	//�V���O���g��
	auto& handle = HandleManager::GetInstance();
}

/// <summary>
/// �R���X�g���N�^
/// </summary>
Shield::Shield()
{
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Shield::~Shield()
{
	//���������
	MV1DeleteModel(m_itemHandle);

	handle.Clear();
}

/// <summary>
/// ����������
/// </summary>
void Shield::Init()
{
	//���̏�����
	m_itemHandle = handle.GetModelHandle("Data/Weapon/Shield.mv1");
	m_shield.ss_cut = 90.0f;
	m_shield.ss_strength = 20.0f;
}

/// <summary>
/// �X�V����
/// </summary>
/// <param name="mat">����̍s��</param>
void Shield::Update(MATRIX mat)
{
	MV1SetMatrix(m_itemHandle, MGetIdent());

	//�t���[��������
	m_frameIndex = MV1SearchFrame(m_itemHandle, "0:fbx");

	m_framePos = MV1GetFramePosition(m_itemHandle, m_frameIndex);

	m_pos = VAdd(m_framePos, m_pos);

	//�A�^�b�`���郂�f�����t���[���̍��W�����_�ɂ��邽�߂̕��s�ړ��s����쐬
	m_transMatrix = MGetTranslate(VScale(m_pos, -1.0f));

	m_transMatrix = MMult(m_transMatrix, MGetRotY(cShieldMatrixY));
	m_transMatrix = MMult(m_transMatrix, MGetRotZ(cShieldMatrixZ));

	m_mixMatrix = MMult(m_transMatrix, mat);

	MV1SetMatrix(m_itemHandle, m_mixMatrix);
}

/// <summary>
/// �`�揈��
/// </summary>
void Shield::Draw()
{
	//���f���`��
	MV1DrawModel(m_itemHandle);
}

void Shield::End()
{
	//���������
	MV1DeleteModel(m_itemHandle);

	handle.Clear();
}
