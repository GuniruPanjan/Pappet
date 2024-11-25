#include "UIBase.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
UIBase::UIBase()
{
	//�摜�ǂݍ���
	m_heelStone = MyLoadGraph("Data/UI/HeelStoneMini.png", 6, 6);
	m_eqFist = MyLoadGraph("Data/UI/FistUi.png", 7, 7);
	m_sword = MyLoadGraph("Data/UI/SwordUI.png", 3, 3);
	m_eqSword = MyLoadGraph("Data/UI/SwordUI.png", 7, 7);
	m_eqShield = MyLoadGraph("Data/UI/Shield.png", 7, 7);
	m_equipment = MyLoadGraph("Data/UI/�������UI.png", 1, 1);
	m_itemBox = MyLoadGraph("Data/UI/�A�C�e�����UI.png", 1, 1);
	m_selectEquipment = MyLoadGraph("Data/UI/�����I�����UI.png", 1, 1);

	m_blackSword = MyLoadGraph("Data/UI/������UI.png", 3, 3);
	m_fist = MyLoadGraph("Data/UI/��UI.png", 3, 3);
	m_body = MyLoadGraph("Data/UI/����UI.png", 3, 3);
	m_uglyShield = MyLoadGraph("Data/UI/�X����UI.png", 4, 4);
	m_commonArmor = MyLoadGraph("Data/UI/���}�ȊZUI.png", 3, 3);
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
UIBase::~UIBase()
{
	//���������
	DeleteGraph(m_heelStone);
	DeleteGraph(m_sword);
	DeleteGraph(m_eqFist);
	DeleteGraph(m_eqSword);
	DeleteGraph(m_eqShield);
	DeleteGraph(m_equipment);
	DeleteGraph(m_itemBox);
	DeleteGraph(m_selectEquipment);
	DeleteGraph(m_blackSword);
	DeleteGraph(m_fist);
	DeleteGraph(m_body);
	DeleteGraph(m_uglyShield);
	DeleteGraph(m_commonArmor);
}

/// <summary>
/// �摜�̃T�C�Y��ς��ă��[�h����֐�
/// </summary>
/// <param name="FileName">�摜��FileName</param>
/// <param name="XSize">�摜�̉��������鐔</param>
/// <param name="YSize">�摜�̏c�������鐔</param>
/// <returns>�T�C�Y��ς����摜��Ԃ�</returns>
int UIBase::MyLoadGraph(const char* FileName, int XSize, int YSize)
{
	int handle = LoadGraph(FileName);  //�摜�̃��[�h
	if (handle != -1)  //�摜�̃��[�h�ɐ��������ꍇ
	{
		int SizeX, SizeY;  //�摜�T�C�Y���i�[���邽�߂̕ϐ���p��

		GetGraphSize(handle, &SizeX, &SizeY);  //���[�h�����摜�̃T�C�Y�擾

		int NowScreen = GetDrawScreen();    //���݂̕`��Ώۉ�ʂ��ꎞ�ۑ�

		SizeX /= XSize;
		SizeY /= YSize;

		int miniHandle = MakeScreen(SizeX, SizeY, TRUE);  //�T�C�Y�ύX��̃O���t�B�b�N�n���h�����쐬
		if (miniHandle == -1)   //�n���h���̍쐬�Ɏ��s�����ꍇ
		{
			DeleteGraph(handle);   //���������[�N���Ȃ��悤�ɁA�{���̃O���t�B�b�N�n���h�����폜
		}
		SetDrawScreen(miniHandle);   //�`��Ώۉ�ʂ��قǍ�����T�C�Y�̃O���t�B�b�N�n���h���ɕύX
		DrawExtendGraph(0, 0, SizeX, SizeY, handle, TRUE);   //���[�h�����摜��`��
		DeleteGraph(handle);   //�s�v�ɂȂ����{���̃T�C�Y�̃O���t�B�b�N�n���h�����폜
		SetDrawScreen(NowScreen);  //���݂̕`��Ώۉ�ʂ����̉�ʂɖ߂�

		return miniHandle;  //�k�߂��摜�̃O���t�B�b�N�n���h����Ԃ�
	}
	else   //�摜�̃��[�h�Ɏ��s�����ꍇ
	{
		return -1;   //�G���[����
	}
}


