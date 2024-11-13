#pragma once
#include "CharacterBase.h"
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

	}m_animChange;

	Player();
	virtual ~Player();

	void Init(std::shared_ptr<MyLibrary::Physics> physics);
	void GameInit(std::shared_ptr<MyLibrary::Physics> physics);
	void Finalize();
	void Update();
	void Action();
	void NotWeaponAnimation();
	void AllAnimation();
	void WeaponAnimation();
	void Draw();
	void End();

	//�Փ˂����Ƃ�
	void OnCollideEnter(const std::shared_ptr<Collidable>& collidable) override;
	void OnCollideStay(const std::shared_ptr<Collidable>& collidable) override;
	void OnTriggerEnter(const std::shared_ptr<Collidable>& collidable) override;

	//�v���C���[�������Ă��邩���擾
	const bool IsGetPlayerDead() const { return m_anim.s_isDead; }

	//�J�����֌W
	void SetCameraAngle(float angle) { m_cameraAngle = angle; }
	float SetAngle(float angle) { return m_lockAngle = angle; }
	bool GetLock() { return m_lockonTarget; }

	//���j���[�֌W
	bool GetMenu() { return m_menuOpen; }
	bool SetMenu(bool set) { return m_menuOpen = set; }

	//�x���֌W
	bool GetRest() { return m_rest; }
	bool SetRest(bool set) { return m_restTouch = set; }

	const MyLibrary::LibVec3 GetPos() const { return rigidbody.GetPos(); }

private:
	void SetModelPos();
private:
	std::shared_ptr<AttackObject> m_pAttack;   //�U������
	std::shared_ptr<PlayerSearchObject> m_pSearch;   //���G����

	XINPUT_STATE m_xpad;                //�p�b�h����
	float m_cameraAngle;                //�J�������
	float m_lockAngle;                  //���b�N�I�������Ƃ��̃A���O��
	bool m_menuOpen;                    //���j���[���J������
	bool m_restTouch;                   //�x���ł��邩�̔���
	bool m_rest;                        //�x������
	bool m_lockonTarget;                //�^�[�Q�b�g����

	//�A�j���[�V�����p�ϐ�
	int m_attackNumber;                 //���݂̍U���i�K�̑��
	bool m_avoidanceNow;                //�t���[����𒆂̔��f
	bool m_animReverse;                 //�A�j���[�V�������t�Đ������邽�߂̔���

	//�t���[���p�ϐ�
	int m_moveAnimFrameIndex;           //�A�j���[�V�����ňړ����Ă���t���[���̔ԍ�����������ϐ�
	int m_moveAnimFrameRight;
	int m_moveAnimShieldFrameIndex;     //�����\����Ƃ��̃A�j���[�V�����̃t���[���擾
	int m_moveAnimShieldFrameHandIndex;
	MATRIX m_moveWeaponFrameMatrix;     //������A�^�b�`����t���[���̃��[�J�����W�����[���h�ϊ��s����擾����
	MATRIX m_moveShieldFrameMatrix;
	VECTOR m_rollMove;                  //����ňړ����鋗��
};

