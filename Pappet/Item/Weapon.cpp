#include "Weapon.h"
#include "Manager/HandleManager.h"
#include "Manager/EffectManager.h"

namespace
{
	//�A�C�e����
	constexpr const char* cItemName = "Weapon";

	//�V���O���g��
	auto& handle = HandleManager::GetInstance();
	auto& effect = EffectManager::GetInstance();
}

/// <summary>
/// �R���X�g���N�^
/// </summary>
Weapon::Weapon() :
	m_weaponPatternFrame(0),
	m_weaponTipFrame(0),
	m_weaponPatternName(),
	m_weaponTipName(),
	m_size(0.0f)
{
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Weapon::~Weapon()
{
	//���������
	MV1DeleteModel(m_itemHandle);
	MV1DeleteModel(m_enemyItemHandle);

	//���������
	handle.Clear();
}

/// <summary>
/// ����������
/// </summary>
void Weapon::Init()
{
	//����������
	if (m_black.sw_equipment)
	{
		m_black.sw_attack = 30.0f;
		m_black.sw_muscle = 2.0f;   //�␳B
		m_black.sw_skill = 1.5f;    //�␳C
		m_black.sw_radius = 25.0f;
		m_itemHandle = handle.GetModelHandle("Data/Weapon/BlackSword.mv1");

		m_size = 0.1f;

		m_pos = VGet(-3.256f, -1.628f, 4.884f);

		m_weaponMatrixX = 0.224f;
		m_weaponMatrixY = 2.827f;
		m_weaponMatrixZ = 1.616f;

		m_weaponPatternName = "Pattern";
		m_weaponTipName = "Tip";
	}
	//�o�b�g����
	else if (m_bat.sw_equipment)
	{
		m_bat.sw_attack = 20.0f;
		m_bat.sw_muscle = 2.5f;    //�␳A
		m_bat.sw_skill = 0.5f;     //�␳E
		m_bat.sw_radius = 25.0f;
		m_itemHandle = handle.GetModelHandle("Data/Weapon/Bat.mv1");

		m_size = 1.0f;

		m_pos = VGet(4.884f, 1.628f, -1.628f);

		m_weaponMatrixX = 0.314f;
		m_weaponMatrixY = 0.180f;
		m_weaponMatrixZ = 4.892f;

		m_weaponPatternName = "Sphere";
		m_weaponTipName = "Tip";
	}
}

/// <summary>
/// �}�b�v�A�C�e���Ƃ��Ă̏���������
/// </summary>
/// <param name="posX"></param>
/// <param name="posY"></param>
/// <param name="posZ"></param>
/// <param name="physics"></param>
void Weapon::ItemInit(float posX, float posY, float posZ, std::shared_ptr<MyLibrary::Physics> physics)
{
	//���
	m_pPhysics = physics;

	InitSearch(MyLibrary::LibVec3(posX, posY, posZ), 80.0f);

	m_centerPos = MyLibrary::LibVec3(posX, posY, posZ);
}

/// <summary>
/// �X�V����
/// </summary>
/// <param name="mat">�s��</param>
void Weapon::Update(MATRIX mat)
{
	MV1SetMatrix(m_itemHandle, MGetIdent());

	MATRIX scale = MGetScale(VGet(m_size, m_size, m_size));
	
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
/// ����̎����Č���
/// </summary>
/// <returns></returns>
VECTOR Weapon::WeaponPattern()
{
	m_weaponPatternFrame = MV1SearchFrame(m_itemHandle, m_weaponPatternName);

	return MV1GetFramePosition(m_itemHandle, m_weaponPatternFrame);
}

/// <summary>
/// ����̐�[������
/// </summary>
/// <returns></returns>
VECTOR Weapon::WeaponTip()
{
	m_weaponTipFrame = MV1SearchFrame(m_itemHandle, m_weaponTipName);

	return MV1GetFramePosition(m_itemHandle, m_weaponTipFrame);
}

void Weapon::ItemUpdate(bool taking)
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
/// <param name="mat">�s��</param>
void Weapon::Draw()
{
	//���f���`��
	MV1DrawModel(m_itemHandle);
}

/// <summary>
/// �}�b�v�A�C�e���Ƃ��Ă̕`�揈��
/// </summary>
void Weapon::ItemDraw()
{
	if (m_effectTime <= 30)
	{
		m_effectTime++;
	}
	else
	{
		effect.EffectCreate("Item", m_centerPos.ConversionToVECTOR());

		m_effectTime = 0;
	}
}

/// <summary>
/// �I������
/// </summary>
void Weapon::End()
{
	//���������
	MV1DeleteModel(m_itemHandle);
	MV1DeleteModel(m_enemyItemHandle);

	//���������
	handle.Clear();
}
