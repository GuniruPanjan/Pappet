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

	Player();
	virtual ~Player();

	void Init(std::shared_ptr<MyLibrary::Physics> physics);
	void Finalize();
	void Update();
	void Action();
	void NotWeaponAnimation();
	void AllAnimation();
	void Draw();
	void End();

	//�Փ˂����Ƃ�
	void OnCollideEnter(const std::shared_ptr<Collidable>& collidable) override;
	void OnTriggerEnter(const std::shared_ptr<Collidable>& collidable) override;

	void SetCameraAngle(float angle) { m_cameraAngle = angle; }

	const MyLibrary::LibVec3 GetPos() const { return rigidbody.GetPos(); }

private:
	void SetModelPos();
private:
	XINPUT_STATE m_xpad;                //�p�b�h����
	float m_cameraAngle;                //�J�������

	//�A�j���[�V�����p�ϐ�
	bool m_animOne[ANIMATION_MAX];      //�A�j���[�V�����֌W�����Z�b�g���邽�߂̔���
	bool m_isDead;                      //���S����
	bool m_dashMove;                    //�_�b�V�����f
	bool m_avoidance;                   //��𔻒f
	bool m_avoidanceNow;                //�t���[����𒆂̔��f

	//�t���[���p�ϐ�
	int m_moveAnimFrameIndex;           //�A�j���[�V�����ňړ����Ă���t���[���̔ԍ�����������ϐ�
	int m_moveAnimFrameRight;
	int m_moveAnimShieldFrameIndex;     //�����\����Ƃ��̃A�j���[�V�����̃t���[���擾
	int m_moveAnimShieldFrameHandIndex;
	MATRIX m_moveWeaponFrameMatrix;     //������A�^�b�`����t���[���̃��[�J�����W�����[���h�ϊ��s����擾����
	MATRIX m_moveShieldFrameMatrix;
};

