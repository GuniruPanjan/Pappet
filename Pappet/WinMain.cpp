#include "DxLib.h"
#include "EffekseerForDXLib.h"
#include <cmath>
#include <memory>
#include "Icon/Icon.h"

// �v���O������ WinMain ����n�܂�܂�
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	//���O�ύX
	SetWindowText("PuppetNuclear");

	//��ʂ̑傫��
	// window���[�h�ݒ�
	SetGraphMode(1600, 1000, 32);
	ChangeWindowMode(true);

	SetWindowIconID(IDI_ICON1);    //�A�C�R���̐ݒ�

	// �ꕔ�̊֐���DxLib_Init()�̑O�Ɏ��s����K�v������
	ChangeWindowMode(true);

	if (DxLib_Init() == -1)		// �c�w���C�u��������������

	{

		return -1;			// �G���[���N�����璼���ɏI��

	}

	//Effekseer������������
	//�����ɂ͉�ʂɕ\������ő�p�[�e�B�N������ݒ肷��
	if (Effekseer_Init(15000) == -1)
	{
		DxLib_End();
		return -1;
	}

	Effekseer_InitDistortion();

	//Dx���C�u�����̃f�o�C�X���X�g�����Ƃ��̃R�[���o�b�N��ݒ�
	//�E�C���h�E�ƃt���X�N���[���̐؂�ւ�����������ꍇ�͕K�����s����
	Effekseer_SetGraphicsDeviceLostCallbackFunctions();

	SetUseZBuffer3D(true);
	SetWriteZBuffer3D(true);
	SetUseBackCulling(true);

	//�_�u���o�b�t�@���[�h
	SetDrawScreen(DX_SCREEN_BACK);

	// �Q�[�����[�v

	while (ProcessMessage() != -1)

	{

		// ���̃t���[���̊J�n�������o���Ă���

		LONGLONG start = GetNowHiPerformanceCount();

		// �`����s���O�ɉ�ʂ��N���A����

		ClearDrawScreen();

		// �Q�[���̏���

		Effekseer_Sync3DSetting();


		UpdateEffekseer3D();


		// ��ʂ��؂�ւ��̂�҂�

		ScreenFlip();

		// esc�L�[�ŃQ�[���I��

		if (CheckHitKey(KEY_INPUT_ESCAPE))

		{

			break;

		}

		// FPS60�ɌŒ肷��

		while (GetNowHiPerformanceCount() - start < 16667)

		{

			// 16.66�~���b(16667�}�C�N���b)�o�߂���܂ő҂�

		}

	}

	Effkseer_End();
	DxLib_End();				// �c�w���C�u�����g�p�̏I������

	return 0;				// �\�t�g�̏I�� 

}