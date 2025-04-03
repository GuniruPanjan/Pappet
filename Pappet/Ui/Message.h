#pragma once
#include "UIBase.h"
#include "Library/MyLibrary.h"

class MessageObject;
class Player;
class Font;

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
	void Init(float posX, float posY, float posZ, int official, int one, int two, int three, std::shared_ptr<MyLibrary::Physics> physics);

	/// <summary>
	/// �X�V����
	/// </summary>
	void Update(Player& player);

	/// <summary>
	/// �`�揈��
	/// </summary>
	void Draw();

	/// <summary>
	/// �����`�揈��
	/// </summary>
	void DrawString();

	/// <summary>
	/// �I������
	/// </summary>
	void End();

	//�������ł���̂��̔��f
	bool GetCan() { return m_can; }
	bool SetCan(bool set) { return m_can = set; }

	//���b�Z�[�W�ɓ������Ă��邩�ǂ���
	bool GetMessageStay();

	//�|�W�V������Ԃ�
	VECTOR GetPos() { return m_centerPos.ConversionToVECTOR(); }

	//���b�Z�[�W��\���ł��邩�̔��f
	bool GetDraw() { return m_draw; }

private:
	XINPUT_STATE m_xpad;                           //�p�b�h����
	std::shared_ptr<MessageObject> m_pMessage;     //���b�Z�[�W�̔���
	std::shared_ptr<Font> m_pFont;                 //�t�H���g

	MyLibrary::LibVec3 m_centerPos;

	int m_official;       //����

	int m_effectTime;      //�G�t�F�N�g�̍Đ�����
	int m_one;             //���
	int m_two;             //���
	int m_three;           //�O��

	bool m_draw;           //���b�Z�[�W��\������
	bool m_can;            //�`�揈�������s�ł��邩
};

