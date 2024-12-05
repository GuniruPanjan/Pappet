#include "Armor.h"

namespace
{
	//�A�C�e����
	constexpr const char* cItemName = "Armor";
}

Armor::Armor()
{
}

Armor::~Armor()
{
}

/// <summary>
/// �}�b�v�Ƃ��ẴA�C�e������������
/// </summary>
/// <param name="posX"></param>
/// <param name="posY"></param>
/// <param name="posZ"></param>
/// <param name="physics"></param>
void Armor::ItemInit(float posX, float posY, float posZ, std::shared_ptr<MyLibrary::Physics> physics)
{
	//���
	m_pPhysics = physics;

	InitSearch(MyLibrary::LibVec3(posX, posY, posZ), 80.0f);
}

/// <summary>
/// �}�b�v�Ƃ��ẴA�C�e���X�V����
/// </summary>
void Armor::ItemUpdate()
{

}

/// <summary>
/// ������
/// </summary>
void Armor::Init()
{
	//���}�Ȗh��
	if (m_common.sa_equipment)
	{
		m_common.sa_defense = 30.0f;

		//���f���p�X������
		m_path = "Data/Armor/CommonArmor.mv1";
	}
}

void Armor::Update()
{
}

void Armor::Draw()
{
}

void Armor::End()
{
}
