#pragma once
#include "DxLib.h"
#include<unordered_map>
#include<map>
#include<string>

enum class InputType
{
	KeyBoard,     //�L�[�{�[�h
	Pad           //�p�b�h
};

//�G�C���A�X�錾
using InputTable_t = std::unordered_map<std::string, int>;

/// <summary>
/// ���͏��Ǘ��N���X
/// </summary>
class Input
{
private:
	//�R���X�g���N�^
	Input();
	//�f�X�g���N�^
	~Input() {};

	static Input* m_instance;   //�C���X�^���X

public:
	//�R�s�[�R���X�g���N�^���֎~����
	Input(const Input&) = delete;
	Input& operator=(const Input&) = delete;
	Input(Input&&) = delete;
	Input& operator= (const Input&&) = delete;

	/// <summary>
	/// �C���X�^���X���擾
	/// </summary>
	/// <returns>�C���X�^���X</returns>
	static Input& GetInstance()
	{
		if (!m_instance)
		{
			m_instance = new Input;
		}
		return *m_instance;
	}

	/// <summary>
	/// �C���X�^���X���폜
	/// </summary>
	static void Destroy()
	{
		delete m_instance;
		m_instance = nullptr;
	}

public:
	//���͏����X�V����
	void Update();

	//�w��̃R�}���h�������ꂽ�u�ԂȂ̂�
	bool IsTriggered(const char* command) const;
	//�w��̃R�}���h�������������Ă��邩�ǂ���
	bool IsPushed(const char* command) const;
	//�w��̃R�}���h�������ꂽ���ǂ���
	bool IsReleased(const char* command) const;
	//�X�e�B�b�N�̓��͏����擾
	std::pair<float, float> GetInputStick(bool isRight) const;
	//ZR,ZL�{�^���̓��͏����擾
	bool GetIsPushedTriggerButton(bool isRight) const;

	bool GetIsTriggeredTriggerButton(bool isRight) const;

	bool IsStickNeutral(bool isRight) const;
private:
	InputTable_t m_commandTable;	//�R�}���h�z��

	//�R�}���h�̓��͂��o���Ă������߂̔z��
	std::map < std::string, int> m_inputData;		//���݂̓���
	std::map < std::string, int> m_lastInputData;	//���O�̓���

	//�R���g���[���[�̓��͏��
	DINPUT_JOYSTATE m_padState;
	XINPUT_STATE* XInputState;

	int m_rightTriggerPushFrameCount;	//�E�g���K�[�̉����ꂽ�t���[����
	int m_leftTriggerPushFrameCount;	//���g���K�[�̉����ꂽ�t���[����

};

