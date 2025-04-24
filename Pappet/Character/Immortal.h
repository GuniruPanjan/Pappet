#pragma once
#include "DxLib.h"
#include "Library/MyLibrary.h"
#include "EnemyBase.h"
#include <map>

class SEManager;

/// <summary>
/// �G���G�̃N���X
/// </summary>
class Immortal : public EnemyBase
{
public:
	//�R���X�g���N�^
	Immortal();
	//�f�X�g���N�^
	virtual ~Immortal();

	//������
	void Init(float posX, float posY, float posZ, std::shared_ptr<MyLibrary::Physics> physics, bool tutorial, EnemyWeapon& weapon) override;
	//�Q�[���̎d�l��ł̏���������
	void GameInit(float posX, float posY, float posZ, std::shared_ptr<MyLibrary::Physics> physics, bool tutorial, EnemyWeapon& weapon) override;
	//�X�V
	void Update(MyLibrary::LibVec3 playerPos, MyLibrary::LibVec3 shieldPos, bool isChase, SEManager& se, std::shared_ptr<MyLibrary::Physics> physics, EnemyWeapon& weapon);
	//����łȂ��Ƃ��ɂł���s��
	void Action(MyLibrary::LibVec3 playerPos, bool isChase, SEManager& se);
	//�`��
	void Draw(EnemyWeapon& weapon);
	//UI�`��
	void DrawUI(UI& ui) override;

private:
	int m_frameRightHand[2];           //�E��̃t���[��
	VECTOR m_ligRightPos[2];           //�E��̃|�W�V����
};

