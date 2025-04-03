#pragma once
#include "EnemyBase.h"
#include "Library/MyLibrary.h"
#include "DxLib.h"
#include "Manager/SEManager.h"
#include <map>

/// <summary>
/// �{�X�G�̃N���X
/// </summary>
class Bear : public EnemyBase
{
public:
	//�R���X�g���N�^
	Bear();
	//�f�X�g���N�^
	virtual ~Bear();

	//������
	void Init(float posX, float posY, float posZ, std::shared_ptr<MyLibrary::Physics> physics, bool tutorial, EnemyWeapon& weapon) override;
	//�Q�[���̎d�l��ł̏�����
	void GameInit(float posX, float posY, float posZ, std::shared_ptr<MyLibrary::Physics> physics, bool tutorial, EnemyWeapon& weapon) override;
	//�X�V
	void Update(MyLibrary::LibVec3 playerPos, MyLibrary::LibVec3 shieldPos, bool isChase, SEManager& se, std::shared_ptr<MyLibrary::Physics> physics, EnemyWeapon& weapon);
	//�{�X�̍s��
	void Action(MyLibrary::LibVec3 playerPos, bool isChase, SEManager& se);
	//�`��
	void Draw(EnemyWeapon& weapon);
	//UI�`��
	void DrawUI(UI& ui) override;


private:
	MyLibrary::LibVec3 m_attackPos;    //�U������̍��W

	int m_walk;                        //���������ɗ���SE

	int m_frameRightHand[2];           //�E��̃t���[��
	int m_frameLiftHand[2];            //����̃t���[��

	VECTOR m_ligRightPos[2];           //�E��̃|�W�V����
	VECTOR m_ligLeftPos[2];            //����̃|�W�V����
};

