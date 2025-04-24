#pragma once
#include "Library/MyLibrary.h"
#include "Object/AttackObject.h"
#include "Object/SearchObject.h"
#include "Object/PlayerSearchObject.h"
#include "Object/EnemyAttackObject.h"
#include "Object/ShieldObject.h"
#include "Object/AttackLigObject.h"
#include "Object/EnemyAttackLigObject.h"
#include "StateBase.h"
#include <map>
#include <string>
#define ANIMATION_MAX   30
#define D2R(deg) ((deg)*DX_PI_F/180.0f)

class CharacterBase : public MyLibrary::Collidable
{
public:
	//�X�e�[�^�X�\����
	struct Status
	{
		float s_hp;          //�̗�
		float s_stamina;     //�X�^�~�i
		float s_attack;      //�U����
		float s_attack1;     //�U����2
		float s_attack2;     //�U����
		int s_muscle;        //�ؗ�
		int s_skill;         //�Z��
		float s_defense;     //�h���
		float s_speed;       //���x
		int s_core;          //�R�A
	};

	//�A�j���[�V�����֌W�̍\����
	struct AnimationChange
	{
		bool s_moveflag;      //�L�����������������f����t���O
		bool s_isDead;        //�L�����̎��S����
		bool s_hit;           //���ݔ���
		bool s_attack;        //�L�������U���������̔���
	};

public:
	CharacterBase(Priority priority, ObjectTag tag);
	virtual ~CharacterBase();

	//�Փ˂����Ƃ�
	virtual void OnCollideEnter(const std::shared_ptr<Collidable>& colider) = 0;

public:
	//�A�j���[�V�����̍X�V
	bool UpdateAnim(int attachNo, int max, float startTime = 0.0f);
	//�A�j���[�V�����̍X�V
	bool UpdateStateAnim(int attachNo, float startTime = 0.0f, float resetTime = 0.0f, bool loop = false);
	//�A�j���[�V�����̕ύX
	void ChangeAnim(int animIndex, bool& one, bool(&all)[30], float animSpeed = 0.5f, bool reverse = false, float resetTime = 0.0f);
	//State�ł̃A�j���[�V�����ύX
	void ChangeStateAnim(int animIndex, bool init, float animSpeed = 0.5f, bool reverse = false, float resetTime = 0.0f);
	//�A�j���[�V�����̃t���[���u�����h�ύX
	void FrameChangeAnim(int animIndex, bool& one, bool& two, int frame);
	//�A�j���[�V�����̃t���[���u�����h�ύX(�X�e�[�g)
	void FrameStateChangeAnim(int animIndex, int frame, bool& one);
	//�A�j���[�V�����̃t���[���u�����h�ύX(�X�e�[�g)
	void FrameEndStateAnim(int animIndex, int frame, bool& one);
	//�A�j���[�V�����̃t���[���u�����h�ύX
	void FrameEndAnim(int animIndex, bool& one, bool& two, int frame);
	//�A�j���[�V�����̖�������
	void NotInitAnim(bool init = false);
	//���g�̃X�e�[�g�N���X����ĂԐ�p
	void ChangeState(std::shared_ptr<StateBase> next);
	//�L�����N�^�[�����擾
	const std::string GetCharacterName() const { return m_characterName; }

	//�X�e�[�^�X�擾
	Status GetStatus() { return m_status; }
	//�t���[�����擾����
	float GetFrame() { return m_nowFrame; }
	//�A�j���[�V�����I�����擾����
	bool GetEndAnim() { return m_isAnimationFinish; }
	//�L�����N�^�[�̃q�b�g��Ԃ𓾂�
	bool GetHit() { return m_anim.s_hit; }
	//�J�v�Z����Y���W�̑傫���𓾂�
	float GetCapsuleY() { return m_capsuleY; }
	//�J�v�Z���̔��a�𓾂�
	float GetCapsuleRadius() { return m_capsuleRadius; }

	//�����֌W
	bool GetSword() { return m_sword; }
	bool GetFist() { return m_fist; }
	bool GetEquipment() { return m_equipment; }
	bool GetShield() { return m_shield; }

	//�X�s�[�h��ݒ肷��
	float SetSpeed(float set) { return m_status.s_speed = set; }
	//�A�j���[�V�����^�C����ݒ肷��
	float SetAnimSpeed(float set) { return m_animTime = set; }

	//�W�����v�֌W
	bool GetJumpCan() { return m_jumpCan; }                            //�W�����v�U�����\�ɂ���

	//�K�[�h�֌W
	bool GetGuard() { return m_guardTransition; }                      //���݂̃��O��Ԃ��m�ۂ���
	bool SetGuard(bool set) { return m_guardTransition = set; }        //���O��Ԃ��l������


	//�����f�[�^���擾
	std::shared_ptr<MyLibrary::Rigidbody> GetRigidbody() { return rigidbody; }

protected:
	//�L�����N�^�[�� 
	std::string m_characterName;	
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
	//�A�j���[�V����
	AnimationChange m_anim;

	//�A�j���[�V�����֌W
	std::map<std::string, int> m_animIdx;
	int m_nowAnimIdx;        
	int m_equipAnimNo;        //��O�̃A�j���[�V����
	int m_nowAnimNo;          //���݂̃A�j���[�V����
	int m_prevAnimNo;         //�ύX�O�̃A�j���[�V����
	int m_frameAnimNo;        //�t���[���������s���A�j���[�V����
	float m_animBlendRate;    //�A�j���[�V�����̍�������
	bool m_animOne[ANIMATION_MAX];      //�A�j���[�V�����֌W�����Z�b�g���邽�߂̔���
	float m_nowFrame;         //�A�j���[�V������i�߂鎞��
	float m_animTime;         //�A�j���[�V�����Đ����x
	bool m_isAnimationFinish; //�A�j���[�V�������I��������ǂ���
	float m_animSpeed;        //�A�j���[�V�����̐i�񂾃^�C��������
	bool m_animInit;          //�A�j���[�V���������������邩�ǂ���
	bool m_animReverse;       //�A�j���[�V�������t�Đ������邩�ǂ���
	VECTOR m_nowPos;        //���݂̃t���[���̍��W���擾����

	int m_currentAnimNo;		//���݂̃A�j���[�V����
	int m_preAnimIdx;

	//�����֌W
	bool m_equipment;
	bool m_fist;
	bool m_shield;
	bool m_sword;

	//�g���ϐ�
	float m_angle;            //�L�����̃A���O��
	float m_attackRadius;     //�U���̓����蔻��
	float m_searchRadius;     //���G�̓����蔻��
	float m_capsuleY;         //�J�v�Z����Y���W
	float m_capsuleRadius;    //�J�v�Z���̔��a
	int m_heel;               //HP�ɒǉ�����񕜕�
	int m_maxHeel;            //HP�̍ő�񕜗�
	float m_cameraAngle;      //�J�������
	bool m_jumpCan;           //�W�����v�U���\�ɂ���
	bool m_guardTransition;   //���O���K�[�h��ԂɂȂ��Ă��邩�𔻒肷��

	//State�p�^�[��
	std::shared_ptr<StateBase> m_pState;      

};

