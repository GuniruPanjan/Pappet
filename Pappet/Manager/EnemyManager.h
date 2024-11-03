#pragma once
#include <memory>
#include <map>
#include <string>
#include <List>
#include "Library/MyLibrary.h"

/// <summary>
/// �G���Ǘ�����N���X
/// </summary>
class EnemyManager
{
public:

	struct EnemyGenerateInfo
	{
		std::string enemyName;     //�G�̖��O
		int mapNumber;             //�}�b�v�̔ԍ�
		int posX;                  //�G��X���W
		int posY;                  //�G��Y���W
		int posZ;                  //�G��Z���W

		bool isCreated = false;    //�����ς݂��ǂ���
	};
};

