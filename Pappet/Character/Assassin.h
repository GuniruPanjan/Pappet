#pragma once
#include "EnemyBase.h"

class Weapon;

/// <summary>
/// �A�T�V���N���X
/// </summary>
class Assassin : public EnemyBase
{
public:
	//�R���X�g���N�^
	Assassin();
	//�f�X�g���N�^
	virtual ~Assassin();

	//������
	void Init(float posX, float posY, float posZ, std::shared_ptr<MyLibrary::Physics> physics, bool tutorial, EnemyWeapon& weapon) override;
	//�Q�[���̎d�l��ł̏�����
	void GameInit(float posX, float posY, float posZ, std::shared_ptr<MyLibrary::Physics> physics, bool tutorial, EnemyWeapon& weapon) override;
	//�X�V
	void Update(MyLibrary::LibVec3 playerPos, MyLibrary::LibVec3 shieldPos, bool isChase, SEManager& se, std::shared_ptr<MyLibrary::Physics> physics, EnemyWeapon& weapon);
	//�s��
	void Action(MyLibrary::LibVec3 playerPos, bool isChase, SEManager& se, EnemyWeapon& weapon);
	//�{�X�̎��̍s��
	void BossAction(MyLibrary::LibVec3 playerPos, bool isChase, SEManager& se, EnemyWeapon& weapon);
	//�`��
	void Draw(EnemyWeapon& weapon);
	//UI�`��
	void DrawUI(UI& ui) override;

private:
	MyLibrary::LibVec3 m_attackPos;       //�U������̍��W

	int m_moveFrameRightHand;             //�E��̃t���[��
	int m_daggerFrame[2];                 //�_�K�[�̓����蔻��
	int m_ligLeftLegFrame[2];             //�����̃t���[��
	int m_ligRightLegFrame[2];            //�E���̃t���[��

	VECTOR m_frameRightHand;              //�E��̃x�N�^�[
	VECTOR m_daggerPos[2];                //�_�K�[�̓����蔻��
	VECTOR m_ligLeftLegPos[2];            //�����̃x�N�^�[
	VECTOR m_ligRightLegPos[2];           //�E���̃x�N�^�[

	VECTOR m_attackMove;                  //�U�����̈ړ�
	VECTOR m_avoidanceMove;               //������̈ړ�
	VECTOR m_hitMove;                     //�U���q�b�g���̈ړ�

	MATRIX m_weaponFrameMatrix;
};

