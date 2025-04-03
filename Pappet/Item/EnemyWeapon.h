#pragma once
#include "ItemBase.h"

/// <summary>
/// �G�̕���N���X
/// </summary>
class EnemyWeapon : public ItemBase
{
public:
	//�R���X�g���N�^
	EnemyWeapon();
	//�f�X�g���N�^
	virtual ~EnemyWeapon();

	//������
	void Init(std::string path, VECTOR pos, float x, float y, float z, float size);
	//�X�V����
	void Update(MATRIX mat, float size);
	//�|�W�V�����Ƃ��Ďg���t���[��
	void WeaponFrame(const char* frame1, const char* frame2);
	//�`�揈��
	void Draw();
	//�I������
	void End();

	VECTOR GetFramePos1() { return m_weaponFramePos[0]; }
	VECTOR GetFramePos2() { return m_weaponFramePos[1];}

private:
	float m_weaponMatrixX = 0.0f;
	float m_weaponMatrixY = 0.0f;
	float m_weaponMatrixZ = 0.0f;

	int m_weaponFrame[2];           //����̃t���[��
	VECTOR m_weaponFramePos[2];     //����̃t���[���̍��W
};

