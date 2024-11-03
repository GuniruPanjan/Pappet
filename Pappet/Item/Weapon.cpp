#include "Weapon.h"
#include "Manager/HandleManager.h"

namespace
{
	//�s��
	constexpr float cWeaponMatrixY = 5.655f;
	constexpr float cWeaponMatrixZ = 1.795f;

	//�V���O���g��
	auto& handle = HandleManager::GetInstance();
}

/// <summary>
/// �R���X�g���N�^
/// </summary>
Weapon::Weapon()
{
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Weapon::~Weapon()
{
	//���������
	MV1DeleteModel(m_itemHandle);

	//���������
	handle.Clear();
}

/// <summary>
/// ����������
/// </summary>
void Weapon::Init()
{
	//������������
	m_weapon.sw_attack = 30.0f;
	m_weapon.sw_radius = 25.0f;
	m_itemHandle = handle.GetModelHandle("Data/Weapon/Sword.mv1");
}

/// <summary>
/// �X�V����
/// </summary>
/// <param name="mat">�s��</param>
void Weapon::Update(MATRIX mat)
{
	MV1SetMatrix(m_itemHandle, MGetIdent());
	//�t���[������
	m_frameIndex = MV1SearchFrame(m_itemHandle, "2:Sphere");
	//�t���[���̃|�W�V����
	m_framePos = MV1GetFramePosition(m_itemHandle, m_frameIndex);
	//�A�^�b�`���郂�f�����t���[���̍��W�����_�ɂ��邽�߂̕��s�ړ��s����쐬
	m_transMatrix = MGetTranslate(VScale(m_pos, -1.0f));

	m_transMatrix = MMult(m_transMatrix, MGetRotY(cWeaponMatrixY));
	m_transMatrix = MMult(m_transMatrix, MGetRotZ(cWeaponMatrixZ));

	m_mixMatrix = MMult(m_transMatrix, mat);

	MV1SetMatrix(m_itemHandle, m_mixMatrix);
}

/// <summary>
/// �`�揈��
/// </summary>
/// <param name="mat">�s��</param>
void Weapon::Draw(MATRIX mat)
{
	//���f���̃|�W�V����
	m_pos = VAdd(m_framePos, m_pos);
}

/// <summary>
/// �I������
/// </summary>
void Weapon::End()
{
	//���������
	MV1DeleteModel(m_itemHandle);

	//���������
	handle.Clear();
}
