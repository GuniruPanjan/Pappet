#pragma once
#include <memory>
#include <map>
#include <string>
#include <List>
#include "Library/MyLibrary.h"

class ItemBase;
class Armor;
class Shield;
class Weapon;

/// <summary>
/// �A�C�e�����Ǘ�����N���X
/// </summary>
class ItemManager
{
private:
	//�A�C�e���̍\����
	struct Item
	{
		int SmallCore = 0;       //���^�R�A
		int MediumCore = 0;      //���^�R�A
		int Rubbish = 0;         //�S�~
		int BlackSword = 0;      //������
		int Distorted = 0;       //�c�񂾏�
		int ArmorNormal = 0;     //���ʂ̊Z
	};

	//�A�C�e���������
	struct ItemGenerateInfo
	{
		std::string itemName;      //�A�C�e���̖��O
		int mapNumber;             //�}�b�v�̔ԍ�
		int posX;                  //�A�C�e����X���W
		int posY;                  //�A�C�e����Y���W
		int posZ;                  //�A�C�e����Z���W

		bool isCreated = false;    //�����ς݂��ǂ���
		bool isTaking = false;     //���ꂽ���ǂ���
	};

public:
	//�R���X�g���N�^
	ItemManager();
	//�f�X�g���N�^
	virtual ~ItemManager();
	//����������
	void Init(const char* stageName);
	//�X�V����
	void Update(std::shared_ptr<MyLibrary::Physics> physics);

	//�`�揈��
	void Draw();

	//�I������
	void End();

private:
	//�A�C�e���̊Ǘ�
	std::list<std::shared_ptr<ItemBase>> m_pItems;
	//�A�C�e���̐������
	std::list<std::shared_ptr<ItemGenerateInfo>> m_pGenerateInfo;

	std::shared_ptr<Armor> m_parmor;
	std::shared_ptr<Shield> m_pshield;
	std::shared_ptr<Weapon> m_pweapon;

	//�X�e�[�W��
	const char* m_stageName;

	//�X�e�[�W���̃A�C�e���̐�����
	std::unordered_map<int, int> m_itemGenerationCountPerOneMap;

};

