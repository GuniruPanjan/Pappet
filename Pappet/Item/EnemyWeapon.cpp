#include "EnemyWeapon.h"
#include "Manager/HandleManager.h"

namespace
{
	//�V���O���g��
	auto& handle = HandleManager::GetInstance();
}

/// <summary>
/// �R���X�g���N�^
/// </summary>
EnemyWeapon::EnemyWeapon()
{
	for (int i = 0; i < 2; i++)
	{
		m_weaponFrame[i] = 0;
		m_weaponFramePos[i] = VGet(0.0f, 0.0f, 0.0f);
	}
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
EnemyWeapon::~EnemyWeapon()
{
	//���������
	MV1DeleteModel(m_itemHandle);

	//���������
	handle.Clear();
}

/// <summary>
/// ����������
/// </summary>
/// <param name="path"></param>
/// <param name="pos"></param>
/// <param name="x"></param>
/// <param name="y"></param>
/// <param name="z"></param>
/// <param name="size"></param>
void EnemyWeapon::Init(std::string path, VECTOR pos, float x, float y, float z, float size)
{
	m_itemHandle = handle.GetModelHandle(path);

	m_pos = pos;

	m_weaponMatrixX = x;
	m_weaponMatrixY = y;
	m_weaponMatrixZ = z;

	MV1SetScale(m_itemHandle, VGet(size, size, size));
}

/// <summary>
/// �X�V����
/// </summary>
/// <param name="mat"></param>
/// <param name="frame"></param>
void EnemyWeapon::Update(MATRIX mat, float size)
{
	MV1SetMatrix(m_itemHandle, MGetIdent());

	MATRIX scale = MGetScale(VGet(size, size, size));

	//�A�^�b�`���郂�f�����t���[���̍��W�����_�ɂ��邽�߂̕��s�ړ��s����쐬
	m_transMatrix = MGetTranslate(VScale(m_pos, -1.0f));

	m_transMatrix = MMult(m_transMatrix, MGetRotX(m_weaponMatrixX));
	m_transMatrix = MMult(m_transMatrix, MGetRotY(m_weaponMatrixY));
	m_transMatrix = MMult(m_transMatrix, MGetRotZ(m_weaponMatrixZ));

	m_transMatrix = MMult(m_transMatrix, scale);
	m_mixMatrix = MMult(m_transMatrix, mat);
	MV1SetMatrix(m_itemHandle, m_mixMatrix);
}

/// <summary>
/// �t���[������
/// </summary>
/// <param name="frame1"></param>
/// <param name="frame2"></param>
void EnemyWeapon::WeaponFrame(const char* frame1, const char* frame2)
{
	m_weaponFrame[0] = MV1SearchFrame(m_itemHandle, frame1);
	m_weaponFrame[1] = MV1SearchFrame(m_itemHandle, frame2);

	m_weaponFramePos[0] = MV1GetFramePosition(m_itemHandle, m_weaponFrame[0]);
	m_weaponFramePos[1] = MV1GetFramePosition(m_itemHandle, m_weaponFrame[1]);
}

/// <summary>
/// �`�揈��
/// </summary>
void EnemyWeapon::Draw()
{
	//���f���`��
	MV1DrawModel(m_itemHandle);
}

/// <summary>
/// �I������
/// </summary>
void EnemyWeapon::End()
{
	//���������
	MV1DeleteModel(m_itemHandle);

	//���������
	handle.Clear();
}
