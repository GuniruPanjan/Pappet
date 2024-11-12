#pragma once
#include <memory>
#include <map>
#include <string>
#include <List>
#include "Library/MyLibrary.h"

class EnemyBase;
class GameManager;

/// <summary>
/// �G���Ǘ�����N���X
/// </summary>
class EnemyManager
{
public:
	/// <summary>
	/// �G�������
	/// </summary>
	struct EnemyGenerateInfo
	{
		std::string enemyName;     //�G�̖��O
		int mapNumber;             //�}�b�v�̔ԍ�
		int posX;                  //�G��X���W
		int posY;                  //�G��Y���W
		int posZ;                  //�G��Z���W

		bool isCreated = false;    //�����ς݂��ǂ���
	};
public:
	//�R���X�g���N�^
	EnemyManager();
	//�f�X�g���N�^
	virtual ~EnemyManager();

	//������
	void Init(const char* stageName);
	//�X�V
	void Update(std::shared_ptr<MyLibrary::Physics> physics, GameManager* gameManager, MyLibrary::LibVec3 playerPos, MyLibrary::LibVec3 playerDir, bool isPlayerChase);
	//�`��
	void Draw();
	//�I������
	void End();

	//���ʂ̓G�̌��݂�HP��Ԃ�
	const int GetFrontEnemyHp() const { return m_frontEnemyHp; }
	//���ʂ̓G�̍ő�HP��Ԃ�
	const int GetFrontEnemyMaxHp() const { return m_frontEnemyMaxHp; }
	//�G�̍��W��Ԃ�
	const std::list<MyLibrary::LibVec3> GetEnemyPos() const { return m_enemyPos; }
	//�G�̃^�[�Q�b�g��Ԃ�
	const std::list<bool> GetEnemyTarget() const { return m_enemyTarget; }
private:
	//�G�̐���
	void CreateEnemy(float posX, float posY, float posZ, std::string name, std::shared_ptr<MyLibrary::Physics> physics);
private:
	//�G�̊Ǘ�
	std::list<std::shared_ptr<EnemyBase>> m_pEnemys;
	//�G�̐������
	std::list<std::shared_ptr<EnemyGenerateInfo>> m_pGenerateInfo;

	//�X�e�[�W��
	const char* m_stageName;

	//�X�e�[�W���̓G�̐�����
	std::unordered_map<int, int> m_enemyGenerationCountPerOneMap;

	//���ʂ̓G��HP
	int m_frontEnemyHp;
	//���ʂ̓G�̍ő�HP
	int m_frontEnemyMaxHp;

	//�G�̍��W
	std::list<MyLibrary::LibVec3> m_enemyPos;
	//�G�̃^�[�Q�b�g����
	std::list<bool> m_enemyTarget;
};

