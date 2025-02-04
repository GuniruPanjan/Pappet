#pragma once
#include "UIBase.h"
#include "Library/MyLibrary.h"

class MessageObject;

class Message : public UIBase
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	Message();

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~Message();

	//�f�[�^��ǂݍ��ޏ�����
	void DataInit();

	/// <summary>
	/// ����������
	/// </summary>
	void Init(float posX, float posY, float posZ, bool official, int one, int two, int three, std::shared_ptr<MyLibrary::Physics> physics);

	/// <summary>
	/// �X�V����
	/// </summary>
	void Update(bool taking);

	/// <summary>
	/// �`�揈��
	/// </summary>
	void Draw();

	/// <summary>
	/// �I������
	/// </summary>
	void End();

private:
	XINPUT_STATE m_xpad;                           //�p�b�h����
	std::shared_ptr<MessageObject> m_pMessage;     //���b�Z�[�W�̔���

	MyLibrary::LibVec3 m_centerPos;

	bool m_official;       //����

	int m_one;             //���
	int m_two;             //���
	int m_three;           //�O��

	bool m_draw;           //���b�Z�[�W��\������
};

