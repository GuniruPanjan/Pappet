#include "Camera.h"
#include "Character/Player.h"
//�x�����W�A���ɕϊ�
#define D2R(deg) ((deg)*DX_PI_F/180.0f)

namespace
{
	VECTOR cPlayerPos = VGet(0.0f, 0.0f, 0.0f);
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
	SetUseZBuffer3D(true);
	SetWriteZBuffer3D(true);
	SetUseBackCulling(true);

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

	cPlayerPos = VGet(player.GetPos().x, player.GetPos().y, player.GetPos().z);

	//���L�[
	if (input.Rx < 0)
	{
		m_cameraAngle.y -= D2R(1.0f);

	}
	//�E�L�[
	if (input.Rx > 0)
	{
		m_cameraAngle.y += D2R(1.0f);

	}
	//��L�[
	if (input.Ry < 0)
	{
		//�J�������n�ʂɂ߂肱�܂Ȃ��悤��
		if (m_cameraPos.y >= 15.2f + player.GetPos().y)
		{
			m_cameraAngle.x -= D2R(1.0f);
		}

	}
	//���L�[
	if (input.Ry > 0)
	{
		//�J�������v���C���[�𒴂��Ȃ����炢�܂�
		if (m_cameraAngle.x <= 0.7f)
		{
			m_cameraAngle.x += D2R(1.0f);
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