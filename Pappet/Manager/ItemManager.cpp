#include "ItemManager.h"
#include "GameManager.h"
#include "External/CsvLoad.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
ItemManager::ItemManager():
	m_stageName("")
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
void ItemManager::Update(std::shared_ptr<MyLibrary::Physics> physics, GameManager* gameManager)
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
				generate->isCreated = true;
				CreateItem(generate->posX, generate->posY, generate->posZ, generate->itemName, physics);
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

	}
	if (name == "Shield")
	{

	}
	if (name == "Armor")
	{

	}
	if (name == "Tool")
	{

	}
}
