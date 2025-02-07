#include "Camera.h"
#include "Character/Player.h"
#include "Manager/EnemyManager.h"
//�x�����W�A���ɕϊ�
#define D2R(deg) ((deg)*DX_PI_F/180.0f)

namespace
{
	VECTOR cPlayerPos = VGet(0.0f, 0.0f, 0.0f);

	int cSize = 0;
	int cTargetSize = 0;
}

/// <summary>
/// �R���X�g���N�^
/// </summary>
Camera::Camera() :
	input(),
	m_cameraPos(VGet(0.0f,0.0f,0.0f)),
	m_cameraTarget(VGet(0.0f, 0.0f, 0.0f)),
	m_cameraAngle(VGet(0.0f, 0.0f, 0.0f)),
	m_x(0.0f),
	m_z(0.0f),
	m_radius(0.0f)
{
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Camera::~Camera()
{
}

/// <summary>
/// ����������
/// </summary>
void Camera::Init()
{
	m_radius = 3.0f;

	//��ƂȂ�J�����̍��W
	m_cameraPos = VGet(0.0f, 50.0f, 100.0f);

	//�J�����̃^�[�Q�b�g���W������
	m_cameraTarget = VGet(0.0f, 5.0f, 0.0f);

	//�J�����̃A���O�������ݒ�
	m_cameraAngle = VGet(D2R(-20.0f), 97.4f, 0.0f);

	SetCameraNearFar(0.001f, 1000.0f);
}

/// <summary>
/// �X�V����
/// </summary>
/// <param name="player">�v���C���[���Ăяo��</param>
void Camera::Update(Player& player)
{
	GetJoypadDirectInputState(DX_INPUT_PAD1, &input);

	cPlayerPos = player.GetPos().GetVector();

	//���L�[
	if (input.Rx < 0)
	{
		m_cameraAngle.y -= D2R(2.0f);

	}
	//�E�L�[
	if (input.Rx > 0)
	{
		m_cameraAngle.y += D2R(2.0f);

	}
	//��L�[
	if (input.Ry < 0)
	{
		//�J�������n�ʂɂ߂肱�܂Ȃ��悤��
		if (m_cameraPos.y >= 15.2f + player.GetPos().y)
		{
			m_cameraAngle.x -= D2R(2.0f);
		}

	}
	//���L�[
	if (input.Ry > 0)
	{
		//�J�������v���C���[�𒴂��Ȃ����炢�܂�
		if (m_cameraAngle.x <= 0.7f)
		{
			m_cameraAngle.x += D2R(2.0f);
		}
	}

	//��̃x�N�g��
	VECTOR Direction = VGet(0.0f, 112.0f, -112.0f);

	//X����]�s��
	MATRIX matrixX = MGetRotX(m_cameraAngle.x);
	//Y����]�s��
	MATRIX matrixY = MGetRotY(m_cameraAngle.y);

	//�s��̍���
	MATRIX matrix = MMult(matrixX, matrixY);

	//��x�N�g�����s��ŕϊ�
	Direction = VTransform(Direction, matrix);

	//�J�������W�̓v���C���[���W���班���͂Ȃꂽ�Ƃ���
	m_cameraPos = VAdd(cPlayerPos, Direction);

	//�����_�̍��W�̓v���C���[���W�̏�����
	m_cameraTarget = VAdd(cPlayerPos, VGet(0.0f, 50.0f, 0.0f));

	SetCameraPositionAndTarget_UpVecY(m_cameraPos, m_cameraTarget);
}

/// <summary>
/// ���b�N�I������
/// </summary>
/// <param name="player">�v���C���[���Ăяo��</param>
/// <param name="enemy">�G�l�~�[���Ăяo��</param>
void Camera::LockUpdate(Player& player, EnemyManager& enemy)
{
	m_enemyPos = VGet(0, 0, 0);
	m_cameraTarget = VGet(0, 0, 0);
	cTargetSize = 0;
	cSize = 0;

	for (auto& enemyTarget : enemy.GetEnemyTarget())
	{
		cTargetSize++;

		if (enemyTarget == true)
		{
			for (auto& enemyPos : enemy.GetEnemyPos())
			{
				cSize++;

				if (cTargetSize == cSize)
				{
					//�������^�[�Q�b�g�ɂȂ���
					m_enemyPos = VGet(enemyPos.x, enemyPos.y, enemyPos.z);

					break;
				}
			}
		}

	}

	

	VECTOR pPos = VGet(player.GetPos().x, player.GetPos().y, player.GetPos().z);

	//�����_�͓G�̍��W�ɂ���
	m_cameraTarget = VAdd(m_enemyPos, VGet(0.0, 20.0f, 0.0f));

	//�v���C���[�ƃG�l�~�[��X���W�̍������߂�
	float X = m_enemyPos.x - pPos.x;
	float lockX = pPos.x - m_enemyPos.x;

	//�v���C���[�ƃG�l�~�[��Z���W�̍������߂�
	float Z = m_enemyPos.z - pPos.z;
	float lockZ = pPos.z - m_enemyPos.z;

	//�p�x���o��
	float angle = atan2f(X, Z);

	//�v���C���[�̕������ς���
	player.SetAngle(atan2f(lockX, lockZ));

	//�G����v���C���[�ɐL�т��̃x�N�g�������߂�
	VECTOR pos = VSub(pPos, m_enemyPos);

	//�x�N�g���̐��K��
	VECTOR posTarget = VNorm(pos);

	posTarget.x *= 130.0f;
	posTarget.z *= 130.0f;

	//�J�������ǂꂾ���v���C���[�̍��W��荂������ݒ�
	posTarget.y = 80.0f;

	m_cameraAngle.y = angle;

	//�v���C���[�̍��W�ɋ��߂��x�N�g���𑫂��ăJ�����̍��W�Ƃ���
	m_cameraPos = VAdd(pPos, posTarget);

	//for (auto& enemyPos : enemy.GetEnemyPos())
	//{
	//	VECTOR ePos = VGet(enemyPos.x, enemyPos.y, enemyPos.z);
	//	VECTOR pPos = VGet(player.GetPos().x, player.GetPos().y, player.GetPos().z);

	//	//�����_�͓G�̍��W�ɂ���
	//	m_cameraTarget = VAdd(ePos, VGet(0.0, 20.0f, 0.0f));

	//	//�v���C���[�ƃG�l�~�[��X���W�̍������߂�
	//	float X = ePos.x - pPos.x;
	//	float lockX = pPos.x - ePos.x;

	//	//�v���C���[�ƃG�l�~�[��Z���W�̍������߂�
	//	float Z = ePos.z - pPos.z;
	//	float lockZ = pPos.z - ePos.z;

	//	//�p�x���o��
	//	float angle = atan2f(X, Z);

	//	//�v���C���[�̕������ς���
	//	player.SetCameraAngle(atan2f(lockX, lockZ));

	//	//�G����v���C���[�ɐL�т��̃x�N�g�������߂�
	//	VECTOR pos = VSub(pPos, ePos);

	//	//�x�N�g���̐��K��
	//	VECTOR posTarget = VNorm(pos);

	//	posTarget.x *= 130.0f;
	//	posTarget.z *= 130.0f;

	//	//�J�������ǂꂾ���v���C���[�̍��W��荂������ݒ�
	//	posTarget.y = 80.0f;

	//	m_cameraAngle.y = angle;

	//	//�v���C���[�̍��W�ɋ��߂��x�N�g���𑫂��ăJ�����̍��W�Ƃ���
	//	m_cameraPos = VAdd(pPos, posTarget);
	//}

	SetCameraPositionAndTarget_UpVecY(m_cameraPos, m_cameraTarget);
}

void Camera::LockBossUpdate(Player& player, EnemyManager& enemy)
{
	m_enemyPos = VGet(0, 0, 0);
	m_cameraTarget = VGet(0, 0, 0);
	cTargetSize = 0;
	cSize = 0;

	for (auto& enemyTarget : enemy.GetJudg())
	{
		cTargetSize++;

		if(enemyTarget == true)
		{
			for (auto& enemyPos : enemy.GetEnemyPos())
			{
				cSize++;

				if (cTargetSize == cSize)
				{
					//�������^�[�Q�b�g�ɂȂ���
					m_enemyPos = VGet(enemyPos.x, enemyPos.y, enemyPos.z);

					break;
				}
			}
		}
	}


	VECTOR pPos = VGet(player.GetPos().x, player.GetPos().y, player.GetPos().z);

	//�����_�͓G�̍��W�ɂ���
	m_cameraTarget = VAdd(m_enemyPos, VGet(0.0, 20.0f, 0.0f));

	//�v���C���[�ƃG�l�~�[��X���W�̍������߂�
	float X = m_enemyPos.x - pPos.x;
	float lockX = pPos.x - m_enemyPos.x;

	//�v���C���[�ƃG�l�~�[��Z���W�̍������߂�
	float Z = m_enemyPos.z - pPos.z;
	float lockZ = pPos.z - m_enemyPos.z;

	//�p�x���o��
	float angle = atan2f(X, Z);

	//�v���C���[�̕������ς���
	player.SetAngle(atan2f(lockX, lockZ));

	//�G����v���C���[�ɐL�т��̃x�N�g�������߂�
	VECTOR pos = VSub(pPos, m_enemyPos);

	//�x�N�g���̐��K��
	VECTOR posTarget = VNorm(pos);

	posTarget.x *= 130.0f;
	posTarget.z *= 130.0f;

	//�J�������ǂꂾ���v���C���[�̍��W��荂������ݒ�
	posTarget.y = 80.0f;

	m_cameraAngle.y = angle;

	//�v���C���[�̍��W�ɋ��߂��x�N�g���𑫂��ăJ�����̍��W�Ƃ���
	m_cameraPos = VAdd(pPos, posTarget);

	SetCameraPositionAndTarget_UpVecY(m_cameraPos, m_cameraTarget);

}

/// <summary>
/// �`�揈��
/// </summary>
void Camera::Draw()
{
	
}

/// <summary>
/// �I������
/// </summary>
void Camera::End()
{
}

/// <summary>
/// �J�����������Ă�����x�N�g�����擾
/// </summary>
/// <returns>�J�����������Ă����</returns>
const MyLibrary::LibVec3 Camera::GetDirection() const
{
	return (MyLibrary::LibVec3(m_cameraTarget.x, m_cameraTarget.y, m_cameraTarget.z)
		- MyLibrary::LibVec3(m_cameraPos.x, m_cameraPos.y, m_cameraPos.z)).Normalize();
}
