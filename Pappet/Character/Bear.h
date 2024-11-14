#pragma once
#include "EnemyBase.h"
#include "Library/MyLibrary.h"
#include "DxLib.h"
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
	void Init(float posX, float posY, float posZ, std::shared_ptr<MyLibrary::Physics> physics) override;
	//�Q�[���̎d�l��ł̏�����
	void GameInit(float posX, float posY, float posZ, std::shared_ptr<MyLibrary::Physics> physics) override;
	//�X�V
	void Update(MyLibrary::LibVec3 playerPos, bool isChase);
	//�{�X�̍s��
	void Action(MyLibrary::LibVec3 playerPos, bool isChase);
	//�`��
	void Draw();
};

