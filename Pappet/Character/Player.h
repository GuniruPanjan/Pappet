#pragma once
#include "CharacterBase.h"

class Weapon;
class Shield;
class Armor;
class EnemyManager;
class CoreManager;

class Player : public CharacterBase
{
public:
	//���x���֌W�̍\����
	struct LevelUpStatus
	{
		int sl_hp;         //�̗�
		int sl_stamina;    //�X�^�~�i
		int sl_muscle;     //�ؗ�
		int sl_skill;      //�Z��
		int sl_all;        //���ׂẴ��x��

	}m_levelStatus;

	//�A�j���[�V�����֌W�̍\����
	struct AnimationChange
	{
		bool sa_dashMove;                    //�_�b�V�����f
		bool sa_avoidance;                   //��𔻒f
		bool sa_recovery;                    //�񕜔���
		bool sa_taking;                      //�A�C�e�����擾���锻��
		bool sa_touch;                       //�M�~�b�N���쓮�����锻��
		bool sa_bossEnter;                   //�{�X�̓�����ɓ��锻��
		bool sa_enterShield;                 //�����\���锻��
		bool sa_shieldIdle;                  //�����\���Ă锻��
		bool sa_imapact;                     //���󂯂�������

	}m_animChange;

	Player();
	virtual ~Player();

	void Init(std::shared_ptr<MyLibrary::Physics> physics, Weapon& weapon, Shield& shield, Armor& armor, bool anim);
	void GameInit(std::shared_ptr<MyLibrary::Physics> physics);
	void Finalize();
	void Update(Weapon& weapon, Shield& shield, Armor& armor, EnemyManager& enemy, CoreManager& core);
	void Action();
	void WarpMap();
	void NotWeaponAnimation();
	void AllAnimation();
	void WeaponAnimation(Shield& shield);
	void Draw(Armor& armor);
	void End();

	//�Փ˂����Ƃ�
	void OnCollideEnter(const std::shared_ptr<Collidable>& collidable) override;
	void OnCollideStay(const std::shared_ptr<Collidable>& collidable) override;
	void OnTriggerEnter(const std::shared_ptr<Collidable>& collidable) override;

	//�����֌W�̊֐�
	void ArmorChange(int one, std::string path);

	//�v���C���[�������Ă��邩���擾
	const bool IsGetPlayerDead() const { return m_anim.s_isDead; }

	//�J�����֌W
	void SetCameraAngle(float angle) { m_cameraAngle = angle; }
	float SetAngle(float angle) { return m_lockAngle = angle; }
	bool GetLock() { return m_lockonTarget; }

	//���j���[�֌W
	bool GetMenu() { return m_menuOpen; }
	bool SetMenu(bool set) { return m_menuOpen = set; }

	//�}�b�v�֌W
	int SetMapNow(int set) { return m_mapNow = set; }

	//�x���֌W
	bool GetRest() { return m_rest; }
	bool GetBigRest() { return m_bigRest; }
	bool SetRest(bool set) { return m_restTouch = set; }
	bool SetNotRest(bool set) { return m_rest = set; }

	//���[�v�֌W
	bool SetWarp(bool set) { return m_warp = set; }
	bool GetWarp() { return m_warp; }

	//�{�X�����ɓ��邽�߂̔���֌W
	bool SetBossStart(bool set) { return m_bossStart = set; }
	bool GetBossStart() { return m_animChange.sa_bossEnter; }

	//���S�֌W
	bool GetDead() { return m_deadReset; }

	//�A�C�e���Ȃǂ̃Q�[���ł̕ϐ��֌W
	int GetLevel() { return m_levelStatus.sl_all; }
	int GetCore() { return m_status.s_core; }
	int GetHPLevel() { return m_levelStatus.sl_hp; }
	int GetStaminaLevel() { return m_levelStatus.sl_stamina; }
	int GetMuscleLevel() { return m_levelStatus.sl_muscle; }
	int GetSkillLevel() { return m_levelStatus.sl_skill; }

	const MyLibrary::LibVec3 GetPos() const { return rigidbody.GetPos(); }
	const MyLibrary::LibVec3 GetShieldPos() const { return m_shieldPos; }

private:
	void SetModelPos();
private:
	std::shared_ptr<AttackObject> m_pAttack;         //�U������
	std::shared_ptr<PlayerSearchObject> m_pSearch;   //���G����
	std::shared_ptr<ShieldObject> m_pShield;    //���̔���

	EnemyAttackObject* m_pEnemyAttackCol;    //�G�̍U��

	MyLibrary::LibVec3 m_shieldPos;
	MyLibrary::LibVec3::Size m_shieldSize;

	XINPUT_STATE m_xpad;                //�p�b�h����
	int m_mapNow;                       //���݂̃}�b�v
	float m_cameraAngle;                //�J�������
	float m_lockAngle;                  //���b�N�I�������Ƃ��̃A���O��
	float m_updateX;                    //�x�������Ƃ��̏������ʒuX
	float m_updateY;                    //�x�������Ƃ��̏������ʒuY
	float m_updateZ;                    //�x�������Ƃ��̏������ʒuZ
	bool m_menuOpen;                    //���j���[���J������
	bool m_restTouch;                   //�x���ł��邩�̔���
	bool m_rest;                        //�x������
	bool m_bigRest;                     //���x�����グ����x������
	bool m_lockonTarget;                //�^�[�Q�b�g����
	bool m_warp;                        //�}�b�v�����[�v���邽�߂̔���
	bool m_bossStart;                   //�{�X�����ɓ��邽�߂̔���
	bool m_shieldOne;                   //���̔��菉����
	bool m_armorOne[10];                //�h��̏�����(�K���ɍ���Ă���)
	bool m_staminaBreak;                //�X�^�~�i�؂�̏��

	//�A�j���[�V�����p�ϐ�
	int m_attackNumber;                 //���݂̍U���i�K�̑��
	bool m_avoidanceNow;                //�t���[����𒆂̔��f
	bool m_shieldNow;                   //�h�䒆�̔��f
	bool m_animReverse;                 //�A�j���[�V�������t�Đ������邽�߂̔���
	bool m_deadReset;                   //���S������������

	//�t���[���p�ϐ�
	int m_moveAnimFrameIndex;           //�A�j���[�V�����ňړ����Ă���t���[���̔ԍ�����������ϐ�
	int m_moveAnimFrameRight;
	int m_moveAnimShieldFrameIndex;     //�����\����Ƃ��̃A�j���[�V�����̃t���[���擾
	int m_moveAnimShieldFrameHandIndex;
	MATRIX m_moveWeaponFrameMatrix;     //������A�^�b�`����t���[���̃��[�J�����W�����[���h�ϊ��s����擾����
	MATRIX m_moveShieldFrameMatrix;
	VECTOR m_rollMove;                  //����ňړ����鋗��
	VECTOR m_moveVector;                //���[�V�������̈ړ�
};

