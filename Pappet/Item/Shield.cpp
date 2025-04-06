#include "Shield.h"
#include "Manager/HandleManager.h"
#include "Manager/EffectManager.h"

namespace
{
	//�A�C�e����
	constexpr const char* cItemName = "Shield";

	//�V���O���g��
	auto& handle = HandleManager::GetInstance();
	auto& effect = EffectManager::GetInstance();
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
	if (m_ugly.ss_equipment)
	{
		m_itemHandle = handle.GetModelHandle("Data/Shield/Shield.mv1");
		m_ugly.ss_cut = 90.0f;
		m_ugly.ss_strength = 100.0f;

		m_pos = VGet(0.0f, 0.0f, 7.787f);

		m_shieldMatrixX = 0.0f;
		m_shieldMatrixY = 3.142f;
		m_shieldMatrixZ = 0.0f;
	}
	else if (m_wood.ss_equipment)
	{
		m_itemHandle = handle.GetModelHandle("Data/Shield/WoodenShield.mv1");
		m_wood.ss_cut = 60.0f;
		m_wood.ss_strength = 50.0f;

		m_pos = VGet(0.0f, 0.0f, 6.512f);

		m_shieldMatrixX = 0.0f;
		m_shieldMatrixY = 3.231f;
		m_shieldMatrixZ = 1.750f;

	}
}

/// <summary>
/// �}�b�v�A�C�e���Ƃ��Ă̏���������
/// </summary>
/// <param name="posX"></param>
/// <param name="posY"></param>
/// <param name="posZ"></param>
/// <param name="physics"></param>
void Shield::ItemInit(float posX, float posY, float posZ, std::shared_ptr<MyLibrary::Physics> physics)
{
	//���
	m_pPhysics = physics;

	InitSearch(MyLibrary::LibVec3(posX, posY, posZ), 80.0f);

	m_centerPos = MyLibrary::LibVec3(posX, posY, posZ);
}

/// <summary>
/// �X�V����
/// </summary>
/// <param name="mat">����̍s��</param>
void Shield::Update(MATRIX mat)
{
	MV1SetMatrix(m_itemHandle, MGetIdent());

	//�A�^�b�`���郂�f�����t���[���̍��W�����_�ɂ��邽�߂̕��s�ړ��s����쐬
	m_transMatrix = MGetTranslate(VScale(m_pos, -1.0f));

	m_transMatrix = MMult(m_transMatrix, MGetRotX(m_shieldMatrixX));
	m_transMatrix = MMult(m_transMatrix, MGetRotY(m_shieldMatrixY));
	m_transMatrix = MMult(m_transMatrix, MGetRotZ(m_shieldMatrixZ));

	m_mixMatrix = MMult(m_transMatrix, mat);

	MV1SetMatrix(m_itemHandle, m_mixMatrix);
}

void Shield::ItemUpdate(bool taking)
{
	//�A�C�e�����܂�����ĂȂ����̏���
	if (!m_isTaking && !taking)
	{
		if (m_pSearch->GetIsStay())
		{
			m_itemPick = true;
		}
		else
		{
			m_itemPick = false;
		}
	}
	//�A�C�e�������ꂽ���̏���
	else if (m_itemPick && taking)
	{
		//��񂾂����s

		ItemFinalize(m_pPhysics);

		m_itemPick = false;
		m_isTaking = true;
		m_itemBox = true;
	}

	m_pSearch->Update(m_centerPos);

}

/// <summary>
/// �`�揈��
/// </summary>
void Shield::Draw()
{
	//���f���`��
	MV1DrawModel(m_itemHandle);
}

/// <summary>
/// �}�b�v�A�C�e���Ƃ��Ă̕`�揈��
/// </summary>
void Shield::ItemDraw()
{
	if (m_effectTime <= m_effectTimeMax)
	{
		m_effectTime++;
	}
	else
	{
		effect.EffectCreate("Item", m_centerPos.ConversionToVECTOR());

		m_effectTime = 0;
	}
}

void Shield::End()
{
	//���������
	MV1DeleteModel(m_itemHandle);

	handle.Clear();
}
