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
	void OnTriggerEnter(const std::shared_ptr<Collidable>& collidable) override;

	//�v���C���[�������Ă��邩���擾
	const bool IsGetPlayerDead() const { return m_anim.s_isDead; }

	void SetCameraAngle(float angle) { m_cameraAngle = angle; }

	const MyLibrary::LibVec3 GetPos() const { return rigidbody.GetPos(); }

private:
	void SetModelPos();
private:
	XINPUT_STATE m_xpad;                //�p�b�h����
	float m_cameraAngle;                //�J�������

	//�A�j���[�V�����p�ϐ�
	int m_attackNumber;                 //���݂̍U���i�K�̑��
	bool m_avoidanceNow;                //�t���[����𒆂̔��f

	//�t���[���p�ϐ�
	int m_moveAnimFrameIndex;           //�A�j���[�V�����ňړ����Ă���t���[���̔ԍ�����������ϐ�
	int m_moveAnimFrameRight;
	int m_moveAnimShieldFrameIndex;     //�����\����Ƃ��̃A�j���[�V�����̃t���[���擾
	int m_moveAnimShieldFrameHandIndex;
	MATRIX m_moveWeaponFrameMatrix;     //������A�^�b�`����t���[���̃��[�J�����W�����[���h�ϊ��s����擾����
	MATRIX m_moveShieldFrameMatrix;
	VECTOR m_rollMove;                  //����ňړ����鋗��
};

