#pragma once
#include "DxLib.h"
#include "Library/MyLibrary.h"
#include <cmath>

class EnemyManager;
class Player;
class Map;

/// <summary>
/// �J�������Ǘ�����N���X
/// </summary>
class Camera
{
public:
	//�R���X�g���N�^
	Camera();
	//�f�X�g���N�^
	virtual ~Camera();

	//����������
	void Init();
	//�X�V����
	void Update(Player& player);
	//���b�N�I������
	void LockUpdate(Player& player, EnemyManager& enemy);
	//�`�揈��
	void Draw();
	//�I������
	void End();
	//�J�����������Ă�����x�N�g�����擾
	const MyLibrary::LibVec3 GetDirection() const;

	VECTOR GetAngle() { return m_cameraAngle; }         //�J�����̃A���O������

private:
	DINPUT_JOYSTATE input;

	//�J�����̃|�W�V����
	VECTOR m_cameraPos = VGet(0.0f, 0.0f, 0.0f);

	//�J�����̃^�[�Q�b�g
	VECTOR m_cameraTarget = VGet(0.0f, 0.0f, 0.0f);

	//�J�����̃A���O��
	VECTOR m_cameraAngle = VGet(0.0f, 0.0f, 0.0f);

	float m_x, m_z;
	float m_radius;

};