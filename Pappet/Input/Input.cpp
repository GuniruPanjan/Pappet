#include "Input.h"
#include <cassert>

Input* Input::m_instance = nullptr;

namespace
{
	enum PadCheckMask : int
	{
		A = 0x00000010,	//A�{�^��
		B = 0x00000020,	//B�{�^��
		X = 0x00000040,	//X�{�^��
		Y = 0x00000080,	//Y�{�^��

		L = 0x00000100,	//L�{�^��
		R = 0x00000200,	//R�{�^��

		RT = 0x00000400,	//RT�{�^��
		LT = 0x00001000,	//LT�{�^��

		RStickPush = 0x00002000,	//R�X�e�B�b�N��������
		test = 0x00008000,	//R�X�e�B�b�N��������

		P = 0x00000800,	//�z�[���{�^��

		UP = 0x00000008,	//��{�^��
		RIGHT = 0x00000004,	//�E�{�^��
		DOWN = 0x00000001,	//���{�^��
		LEFT = 0x00000002,	//���{�^��
	};

	//�g���K�[�{�^���̃f�b�h�]�[��
	constexpr int cTriggerDeadZone = 60;
}

/// <summary>
/// �R���X�g���N�^
/// </summary>
Input::Input() :
	m_padState(),
	XInputState(new XINPUT_STATE),
	m_rightTriggerPushFrameCount(0),
	m_leftTriggerPushFrameCount(0)
{
	//�R�}���h�e�[�u���ɃR�}���h��ǉ�����

	m_commandTable["Input_Jump"] = { PadCheckMask::A };
	m_commandTable["Input_Dash"] = { PadCheckMask::B };
	m_commandTable["Input_Roll"] = { PadCheckMask::B };
	m_commandTable["Input_Attack"] = { PadCheckMask::R };
	m_commandTable["Input_Shield"] = { PadCheckMask::L };
	m_commandTable["Input_StrongAttack"] = { PadCheckMask::RT };
	m_commandTable["Input_Parry"] = { PadCheckMask::LT };
	m_commandTable["X"] = { PadCheckMask::X };
	m_commandTable["Y"] = { PadCheckMask::Y };
	m_commandTable["Pause"] = { PadCheckMask::P };
	m_commandTable["Ok"] = { PadCheckMask::A };
	m_commandTable["Cancel"] = { PadCheckMask::B };

	m_commandTable["Up"] = { PadCheckMask::UP };
	m_commandTable["Down"] = { PadCheckMask::DOWN };
	m_commandTable["Left"] = { PadCheckMask::LEFT };
	m_commandTable["Right"] = { PadCheckMask::RIGHT };


}

/// <summary>
/// ���͏����X�V����
/// </summary>
void Input::Update()
{
	//���O���͂��R�s�[���Ă���(�������u�Ԃ��擾����悤)
	m_lastInputData = m_inputData;
	//�p�b�h���̎擾
	int padState = GetJoypadInputState(DX_INPUT_PAD1);

	//�o�^���ꂽ���ƃn�[�h�̏����Ƃ炵���킹�Ȃ���
	//inputData_�̓��e���X�V���Ă���
	for (const auto& cmd : m_commandTable)
	{
		//�R�}���h�̖��O������̓f�[�^�����
		auto& input = m_inputData[cmd.first];

		//�R���g���[���[�̃`�F�b�N
		input = false;
		if (padState & cmd.second)
		{
			input = true;
		}

		//�{�^���̓��͂��擾
		GetJoypadDirectInputState(DX_INPUT_PAD1, &m_padState);
		//ZR,ZL�̓��͂��擾
		GetJoypadXInputState(DX_INPUT_PAD1, XInputState);

		//ZR�̓��͏���Ԃ�
		if (XInputState->RightTrigger > cTriggerDeadZone)
		{
			m_rightTriggerPushFrameCount++;
		}
		else
		{
			m_rightTriggerPushFrameCount = 0;
		}
		//LR�̓��͏���Ԃ�
		if (XInputState->LeftTrigger > cTriggerDeadZone)
		{
			m_leftTriggerPushFrameCount++;
		}
		else
		{
			m_leftTriggerPushFrameCount = 0;
		}

	}
}

/// <summary>
/// �w��̃R�}���h�������ꂽ�u�ԂȂ̂�
/// </summary>
/// <param name="command"></param>
/// <returns></returns>
bool Input::IsTriggered(const char* command) const
{
	auto it = m_inputData.find(command);
	if (it == m_inputData.end())   //��񂪂Ȃ����false��Ԃ�
	{
		assert(0 && "�w�肳�ꂽ�R�}���h�͓o�^����Ă��܂���");
		return false;
	}
	return (m_inputData.at(command) && !m_lastInputData.at(command));
}
/// <summary>
/// �w��̃R�}���h�������������Ă��邩�ǂ���
/// </summary>
/// <param name="command"></param>
/// <returns></returns>
bool Input::IsPushed(const char* command) const
{
	auto it = m_inputData.find(command);
	if (it == m_inputData.end())  //��񂪂Ȃ����false��Ԃ�
	{
		assert(0 && "�w�肳�ꂽ�R�}���h���o�^����Ă��܂���");
		return false;
	}
	return (m_inputData.at(command) && m_lastInputData.at(command));
}

/// <summary>
/// �w��̃R�}���h�������ꂽ�u�Ԃ��Ƃ�
/// </summary>
/// <param name="command"></param>
/// <returns></returns>
bool Input::IsReleased(const char* command) const
{
	auto it = m_inputData.find(command);
	if (it == m_inputData.end())  //��񂪂Ȃ����false��Ԃ�
	{
		assert(0 && "�w�肳�ꂽ�R�}���h���o�^����Ă��܂���");
		return false;
	}
	return (!m_inputData.at(command) && m_lastInputData.at(command));
}
/// <summary>
/// �X�e�B�b�N�̓��͏����擾
/// </summary>
/// <param name="isRight"></param>
/// <returns></returns>
std::pair<float, float> Input::GetInputStick(bool isRight) const
{
	if (isRight)
	{
		//�E�X�e�B�b�N�̓��͏���Ԃ�
		return std::make_pair(static_cast<float>(m_padState.Rx), static_cast<float>(m_padState.Ry));
	}
	else
	{
		//���X�e�B�b�N�̓��͏���Ԃ�
		return std::make_pair(static_cast<float>(m_padState.X), static_cast<float>(m_padState.Y));
	}
}
/// <summary>
/// ZR.ZL�{�^���̓��͏����擾
/// </summary>
/// <param name="isRight"></param>
/// <returns></returns>
bool Input::GetIsPushedTriggerButton(bool isRight) const
{
	if (isRight)
	{
		return m_rightTriggerPushFrameCount > 0;
	}
	else
	{
		return m_leftTriggerPushFrameCount > 0;
	}
}

bool Input::GetIsTriggeredTriggerButton(bool isRight) const
{
	if (isRight)
	{
		return m_rightTriggerPushFrameCount == 1;
	}
	else
	{
		return m_leftTriggerPushFrameCount == 1;
	}
}

bool Input::IsStickNeutral(bool isRight) const
{
	if (isRight)
	{
		//�X�e�B�b�N�̓��͒l��XY���ꂼ��K��l�ȉ����ǂ���
		if (abs(m_padState.Rx) < 60 && abs(m_padState.Ry) < 60)	return true;

		return false;
	}
	else
	{
		//�X�e�B�b�N�̓��͒l��XY���ꂼ��K��l�ȉ����ǂ���
		if (abs(m_padState.X) < 60 && abs(m_padState.Y) < 60)	return true;

		return false;
	}
}
