#pragma once
#include <memory>
#include <string>
#include "Library/MyLibrary.h"

class CharacterBase;

class StateBase : public std::enable_shared_from_this<StateBase>
{
public:
	//�X�e�[�gKind
	enum class StateKind
	{
		Null,
		Idle,        //�ҋ@
		Walk,        //����
		Roll,        //���
		Dash,        //����
		Jump,        //�W�����v
		Attack,      //�U��
		StrongAttack,//���U��
		Guard,      //��

		Parry,       //�p���B   //�܂�����Ă��Ȃ�
		
		Damage,      //�_���[�W
		Item,        //�񕜂Ȃ�
		Taking,      //�A�C�e���擾
		EnterBoss,   //�{�X�����ɓ��郂�[�V����
		Death,       //���S
	};

protected:
	//���X�e�B�b�N�̓��͕���
	const enum eDir : int
	{
		Forward,
		ForwardRight,
		Right,
		BackRight,
		Back,
		BackLeft,
		Left,
		ForwardLeft,
		None
	};

public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="chara">���L��</param>
	StateBase(std::shared_ptr<CharacterBase> chara);

	virtual ~StateBase();

	//������
	virtual void Init(int md) abstract;

	//�X�V
	virtual void Update() abstract;

	//���݂�State��Ԃ�
	StateKind GetState() { return m_nowState; }
	//�J�ڗ\���State��Ԃ�
	const StateKind GetNextState() { return m_nextState; }
	//�X�e�[�g��J�ڂ���
	void ChangeState(StateKind kind);
	//������𔻒f����
	bool CheckState();
	//�����̓�̒l����p�x���v�Z����
	const eDir GetDirection(float x, float y)const;

	//�J�ڗ\���State��ݒ肷��
	void SetNextKind(StateKind state) { m_nextState = state; }
	//�^�[�Q�b�g��Ԃ��ƃA�j���[�V�������ς��
	bool SetTarget(bool set) { return m_targetState = set; }
	//������t���Ă���ƃA�j���[�V�������ς��
	bool SetEquipment(bool set) { return m_equipmentState = set; }
	//�q�b�g���̔�������߂�
	bool SetHit(bool set) { return m_hit = set; }
	//���������Ȃǂ̏ꍇ
	bool SetSword(bool set) { return m_equipmentSword = set; }

protected:
	std::weak_ptr<CharacterBase> m_pChara;   //���L�҂̃|�C���^

	int m_stageCol;    //�X�e�[�W�̓����蔻��

	StateKind m_nowState;      //���݂̃X�e�[�g
	StateKind m_nextState;     //���̃X�e�[�g

	float m_leftX, m_leftZ;
	bool m_targetState;
	bool m_equipmentState;
	//�U�����v���C���[�Ƀq�b�g�������̔���
	bool m_hit;
	//�ǂ̎�ނ̑��������Ă��邩
	bool m_equipmentFist = false;
	bool m_equipmentSword = false;
	bool m_equipmentShield = false;
	//�W�����v���̔���
	bool m_jumping = false; 

	bool m_revese = false;     //�A�j���[�V�������t�Đ������邩�̔��f
};

