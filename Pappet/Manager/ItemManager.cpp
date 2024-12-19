#include "ItemManager.h"
#include "GameManager.h"
#include "External/CsvLoad.h"
#include "Item/Weapon.h"
#include "Item/Shield.h"
#include "Item/Armor.h"
#include "Item/Tool.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
ItemManager::ItemManager():
	m_stageName(""),
	m_itemPick(false)
{
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
ItemManager::~ItemManager()
{
	//�z��̑S�v�f�폜
}

/// <summary>
/// ����������
/// </summary>
/// <param name="stageName">�X�e�[�W��</param>
void ItemManager::Init(const char* stageName)
{
	m_stageName = stageName;

	//�A�C�e�����������擾����
	CsvLoad::GetInstance().ItemDataLoad(m_stageName, m_pGenerateInfo);

	//csv���Ń}�b�v���w�肵�Ă��邽�߃}�b�v�̕␳������
	for (auto& generate : m_pGenerateInfo)
	{
		//�}�b�v��1�Ȃ�
		if (generate->mapNumber == 1)
		{
			//���̃}�b�v�̏o������A�C�e���̐����X�V����
			int num = m_itemGenerationCountPerOneMap[generate->mapNumber];
			num++;
			m_itemGenerationCountPerOneMap[generate->mapNumber];
		}
	}
}

/// <summary>
/// �X�V����
/// </summary>
/// <param name="physics">�t�B�W�b�N�X</param>
void ItemManager::Update(std::shared_ptr<MyLibrary::Physics> physics, GameManager* gameManager, bool taking)
{

	//���̃}�b�v���ǂ̃}�b�v���擾����
	auto thisMapName = gameManager->GetThisMapName();

	if (thisMapName == 1 || thisMapName == 2 || thisMapName == 3 ||
		thisMapName == 4 || thisMapName == 5)
	{
		//�A�C�e�����������܂킵��
		for (auto& generate : m_pGenerateInfo)
		{
			//���̃}�b�v����v���Ă���Ƃ�
			if (generate->mapNumber == thisMapName)
			{
				//�����ς݂łȂ����
				if (!generate->isCreated)
				{
					generate->isCreated = true;
					CreateItem(generate->posX, generate->posY, generate->posZ, generate->itemName, physics);
				}
				
			}
		}
	}


	//�}�b�v��0�ȊO���Ɠ�����
	if (thisMapName != 0)
	{
		//�}�b�v�̃A�C�e���Ƃ��čX�V����
		for (auto& item : m_pItems)
		{
			item->ItemUpdate(taking);

			if (!m_itemPick)
			{
				m_itemPick = item->GetItemPick();
			}

		}
	}
}

/// <summary>
/// �`�揈��
/// </summary>
void ItemManager::Draw()
{
}

/// <summary>
/// �I������
/// </summary>
void ItemManager::End()
{
}

/// <summary>
/// �A�C�e���̐���
/// </summary>
/// <param name="posX">X���W</param>
/// <param name="posY">Y���W</param>
/// <param name="posZ">Z���W</param>
/// <param name="name">�L�����N�^�[��</param>
/// <param name="physics">�����|�C���^</param>
void ItemManager::CreateItem(float posX, float posY, float posZ, std::string name, std::shared_ptr<MyLibrary::Physics> physics)
{
	if (name == "Weapon")
	{
		m_pweapon = std::make_shared<Weapon>();
		m_pweapon->ItemInit(posX, posY, posZ, physics);
		m_pItems.emplace_back(m_pweapon);
	}
	if (name == "Shield")
	{
		m_pshield = std::make_shared<Shield>();
		m_pshield->ItemInit(posX, posY, posZ, physics);
		m_pItems.emplace_back(m_pshield);
	}
	if (name == "Armor")
	{
		m_parmor = std::make_shared<Armor>();
		m_parmor->ItemInit(posX, posY, posZ, physics);
		m_pItems.emplace_back(m_parmor);
	}
	if (name == "Tool")
	{
		//m_ptool = std::make_shared<Tool>();
		//m_ptool->ItenInit(posX, posY, posZ, physics);
		//m_pItems.emplace_back(m_pweapon);
	}
}
