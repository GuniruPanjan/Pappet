#pragma once
#include "DxLib.h"
#include "Library/MyLibrary.h"
#include "EnemyBase.h"
#include <map>

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
	void Init(float posX, float posY, float posZ, std::shared_ptr<MyLibrary::Physics> physics) override;
	//�X�V
	void Update(MyLibrary::LibVec3 playerPos, bool isChase);
	//�`��
	void Draw();
};
