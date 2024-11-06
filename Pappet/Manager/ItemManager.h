#pragma once
#include <memory>
#include <map>
#include <string>
#include <List>
#include "Library/MyLibrary.h"

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
	void Init();
	//�X�V����
	void Update();

	//�`�揈��
	void Draw();

	//�I������
	void End();

private:

};

