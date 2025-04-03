#pragma once
#include <memory>
#include <map>
#include <string>
#include <List>
#include "Library/MyLibrary.h"

class EnemyBase;
class GameManager;
class Immortal;
class Bear;
class Assassin;
class CoreManager;
class UI;
class SEManager;
class EnemyWeapon;

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
	void Init(int mapNumber);
	//�Q�[���̎d�l��ł̏�����
	void GameInit(std::shared_ptr<MyLibrary::Physics> physics, GameManager* gameManager, EnemyWeapon& weapon, bool init = false, bool tutorial = false);
	//�G�����
	void EnemyGenerate(std::shared_ptr<MyLibrary::Physics> physics, GameManager* gameManager, EnemyWeapon& weapon, bool tutorial = false);
	//�X�V
	void Update(std::shared_ptr<MyLibrary::Physics> physics, GameManager* gameManager, CoreManager& core, MyLibrary::LibVec3 playerPos, MyLibrary::LibVec3 playerDir, MyLibrary::LibVec3 shieldPos, bool isPlayerChase, SEManager& se, EnemyWeapon& weapon,
		        bool tutorial = false);
	//�`��
	void Draw(EnemyWeapon& weapon);
	//UI�`��
	void DrawUI(UI& ui);
	//�I������
	void End();

	//�G��HP��Ԃ�
	const int GetHP();
	//�G�̍ő�HP��Ԃ�
	const int GetMaxHP();

	//���ʂ̓G�̌��݂�HP��Ԃ�
	const int GetFrontEnemyHp() const { return m_frontEnemyHp; }
	//���ʂ̓G�̍ő�HP��Ԃ�
	const int GetFrontEnemyMaxHp() const { return m_frontEnemyMaxHp; }
	//�G�̏��擾
	std::vector<bool> m_enemyIsDead;
	//�G�̍��W��Ԃ�
	const std::list<MyLibrary::LibVec3>& GetEnemyPos() const { return m_enemyPos; }
	//�G�̃^�[�Q�b�g��Ԃ�
	const std::list<bool> GetEnemyTarget() const { return m_enemyTarget; }
	//�G���v���C���[�ɍU���ł��邩��Ԃ�
	const std::list<bool> GetEnemyAttackHit() const { return m_enemyAttackHit; }
	//�^����_���[�W�擾
	const std::list<float> GetEnemyDamage() const { return m_damage; }
	//�R�A�擾
	const std::list<int> GetDropCore() const { return m_dropCore; }
	bool SetBossRoom(bool set, int mapNumber);
	//�{�X�����񂾂��̔���
	bool GetBossDead(int mapNumber);
	//�{�X���̔��f
	const std::list<bool> GetJudg() const { return m_bossJudg; }
private:
	//�G�̐���
	void CreateEnemy(float posX, float posY, float posZ, std::string name, std::shared_ptr<MyLibrary::Physics> physics, bool tutorial, EnemyWeapon& weapon);
	//�G�̃Q�[���̎d�l��ł̏�����
	void EnemyInit(float posX, float posY, float posZ, std::string name, std::shared_ptr<MyLibrary::Physics> physics, bool tutorial, EnemyWeapon& weapon);
private:
	//�G�̊Ǘ�
	std::list<std::shared_ptr<EnemyBase>> m_pEnemys;
	//�G�̐������
	std::list<std::shared_ptr<EnemyGenerateInfo>> m_pGenerateInfo;

	std::shared_ptr<Immortal> immortal;
	std::shared_ptr<Bear> bear;
	std::shared_ptr<Assassin> assassin;

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
	//�G���v���C���[�ɍU���ł��邩����
	std::list<bool> m_enemyAttackHit;
	//�G�̍U���͎擾�p
	std::list<float> m_damage;
	//�R�A�擾
	std::list<int> m_dropCore;
	//�{�X���̔��f
	std::list<bool> m_bossJudg;
};

