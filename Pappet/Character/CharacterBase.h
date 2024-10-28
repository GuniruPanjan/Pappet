#pragma once
#include "Library/MyLibrary.h"
#include <map>
#include <string>

class CharacterBase : public MyLibrary::Collidable
{
public:
	//�X�e�[�^�X�\����
	struct Status
	{
		float s_hp;          //�̗�
		float s_stamina;     //�X�^�~�i
		float s_attack;      //�U����
		int s_muscle;        //�ؗ�
		int s_skill;         //�Z��
		float s_defense;     //�h���
		float s_speed;       //���x
		int s_core;          //�R�A
	};
public:
	CharacterBase(Priority priority, ObjectTag tag);
	virtual ~CharacterBase();

	//�Փ˂����Ƃ�
	virtual void OnCollideEnter(const std::shared_ptr<Collidable>& colider) = 0;

protected:
	//�A�j���[�V�����̍X�V
	bool UpdateAnim(int attachNo, float startTime = 0.0f);
	//�A�j���[�V�����̕ύX
	void ChangeAnim(int animIndex, float animSpeed = 0.5f);

protected:
	//�����N���X�̃|�C���^
	std::shared_ptr<MyLibrary::Physics> m_pPhysics;
	//���f���n���h��
	int m_modelHandle;
	//���f�����W
	MyLibrary::LibVec3 m_modelPos;
	//�����蔻����W
	MyLibrary::LibVec3 m_collisionPos;
	//�ړ��x�N�g��
	MyLibrary::LibVec3 m_moveVec;
	//�X�e�[�^�X
	Status m_status;

	//�A�j���[�V�����֌W
	std::map<std::string, int> m_animIdx;
	int m_nowAnimIdx;        
	int m_equipAnimNo;        //��O�̃A�j���[�V����
	int m_nowAnimNo;          //���݂̃A�j���[�V����
	int m_prevAnimNo;         //�ύX�O�̃A�j���[�V����
	float m_animBlendRate;    //�A�j���[�V�����̍�������

	float m_animTime;         //�A�j���[�V�����Đ����x
	bool m_isAnimationFinish; //�A�j���[�V�������I��������ǂ���
	float m_animSpeed;        //�A�j���[�V�����̐i�񂾃^�C��������

	//�g���ϐ�
	float m_angle;  //�L�����̃A���O��
	bool m_moveflag;  //�L�����������������f����t���O
	bool m_hit;           //���ݔ���
	int m_moveAnimFrameIndex;       //�A�j���[�V�����ňړ����Ă���t���[�������
	int m_moveAnimFrameRight;       //�E��ɑ�������t���[�������
	int m_moveAnimLeftFrameIndex;   //�����̃t���[�������
	int m_moveAnimFrameLeft;        //����ɑ�������t���[�������
	MATRIX m_moveWeaponFrameMatrix; //������A�^�b�`����t���[���̃��[�J�����W�����[���h���W�ɕς���
};

