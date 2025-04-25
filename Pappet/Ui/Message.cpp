#include "Message.h"
#include "Object/MessageObject.h"
#include "Manager/EffectManager.h"
#include "Character/Player.h"
#include "External/Font.h"

namespace
{
	//�V���O���g��
	auto& cEffect = EffectManager::GetInstance();

	//���b�Z�[�W�̔��a
	constexpr float cRadius = 50.0f;

	//�萔�̒�`
	const int cEffectTimeThreshold = 50;
	const int cBackgroundX = 130;
	const int cBackgroundY = 100;
	const int cTextColor = 0xffffff;
	const int cFontSize = 50;
	const int cLStickX = 200;
	const int cLStickY = 150;
	const int cAButtonX = 180;
	const int cAButtonY = 340;
	const int cStaminaBarX = 370;
	const int cStaminaBarY = 660;
	const int cRStickX = 180;
	const int cRStickY = 270;
	const int cRBButtonX = 180;
	const int cRBButtonY = 200;
	const int cRTButtonX = 180;
	const int cRTButtonY = 450;
	const int cYButtonX = 180;
	const int cYButtonY = 200;
	const int cStartButtonX = 180;
	const int cStartButtonY = 450;
	const int cLBButtonX = 180;
	const int cLBButtonY = 300;
	const int cXButtonX = 200;
	const int cXButtonY = 250;
	const int cItemX = 200;
	const int cItemY = 500;
	const int cBearTextX = 450;
	const int cBearTextY = 400;
	const int cDrawStringX1 = 400;
	const int cDrawStringY1 = 150;
	const int cDrawStringY2 = 250;
	const int cDrawStringX2 = 500;
	const int cDrawStringY3 = 350;
	const int cDrawStringY4 = 450;
	const int cDrawStringY5 = 550;
	const int cDrawStringX3 = 700;
	const int cDrawStringY6 = 650;
	const int cDrawStringY7 = 180;
	const int cDrawStringX4 = 350;
	const int cDrawStringY8 = 600;
	const int cDrawStringY9 = 400;
	const int cDrawStringX5 = 250;
	const int cDrawStringX6 = 600;
	const int cDrawStringY10 = 200;
	const int cDrawStringY11 = 270;
	const int cDrawStringY12 = 300;
	const int cDrawStringY13 = 480;
	const int cDrawStringY14 = 580;
	const int cDrawStringY15 = 320;
	const int cDrawStringY16 = 520;
}

Message::Message() :
	m_xpad(),
	m_effectTime(50),
	m_official(0),
	m_one(0),
	m_two(0),
	m_three(0),
	m_draw(false),
	m_can(false),
	m_stop(false)
{
	m_pFont = std::make_shared<Font>();

	for (int i = 0; i < 7; i++)
	{
		m_tutorial[i] = false;
	}
}

Message::~Message()
{
}

void Message::DataInit()
{
	
}

void Message::Init(float posX, float posY, float posZ, int official, int one, int two, int three, std::shared_ptr<MyLibrary::Physics> physics)
{
	m_messageUI = MyLoadGraph("Data/UI/Operation.png", 3, 3);
	m_lStick = MyLoadGraph("Data/UI/LStick.png", 1, 1);
	m_rStick = MyLoadGraph("Data/UI/RStick.png", 1, 1);
	m_bButton = MyLoadGraph("Data/UI/BButton.png", 1, 1);
	m_yButton = MyLoadGraph("Data/UI/YButton.png", 1, 1);
	m_xButton = MyLoadGraph("Data/UI/XButton.png", 1, 1);
	m_aButton = MyLoadGraph("Data/UI/AButton.png", 1, 1);
	m_staminaBar = MyLoadGraph("Data/UI/StaminaBar.png", 1, 2);
	m_rbButton = MyLoadGraph("Data/UI/RBButton.png", 2, 2);
	m_rtButton = MyLoadGraph("Data/UI/RTButton.png", 2, 2);
	m_lbButton = MyLoadGraph("Data/UI/LBButton.png", 2, 2);
	m_startButton = MyLoadGraph("Data/UI/START.png", 2, 2);

	m_pMessage = std::make_shared<MessageObject>(cRadius);

	m_centerPos = MyLibrary::LibVec3(posX, posY, posZ);

	m_pMessage->Init(physics, m_centerPos);

	m_official = official;
	m_one = one;
	m_two = two;
	m_three = three;

	m_pFont->FontInit(cFontSize);
}

void Message::Update(Player& player)
{
	//�p�b�h���͎擾
	GetJoypadXInputState(DX_INPUT_KEY_PAD1, &m_xpad);

	m_pMessage->Update(m_centerPos);

	//���b�Z�[�W��ǂ�
	if (player.GetMessageRead())
	{
		m_draw = true;
	}
	//���b�Z�[�W��ǂ߂Ȃ�����
	else if(!m_pMessage->GetIsStay())
	{
		m_draw = false;
	}

	if (m_draw)
	{
		//B�{�^���������Ɩ߂�
		if (m_xpad.Buttons[13] == 1)
		{
			m_draw = false;

			player.SetMessageRead(false);
			//���[�����O�ł��Ȃ��悤�ɂ���
			player.SetAction(false);
		}
	}
}

void Message::Draw()
{
	if (m_effectTime >= cEffectTimeThreshold)
	{
		cEffect.EffectCreate("Message", m_centerPos.ConversionToVECTOR());

		m_effectTime = 0;
	}
	else
	{
		m_effectTime++;
	}
}

void Message::DrawString()
{
	//�����`��
	if (m_draw)
	{
		//���㉽�����邩�̐���
		if (m_official == 1)
		{
			//�w�i�`��
			DrawGraph(cBackgroundX, cBackgroundY, m_messageUI, false);

			DrawStringToHandle(cDrawStringX1, cDrawStringY1, "�ڂ̑O�̑劘�ł͓���ȋx�����ł�", cTextColor, m_pFont->GetHandle());
			DrawStringToHandle(cDrawStringX1, cDrawStringY2, "�E���ɂ���擾�����R�A���g����", cTextColor, m_pFont->GetHandle());
			DrawStringToHandle(cDrawStringX2, cDrawStringY3, "���x�����グ�邱�Ƃ��\", cTextColor, m_pFont->GetHandle());
			DrawStringToHandle(cDrawStringX2, cDrawStringY4, "���x�����グ�e�n�ɋ�����", cTextColor, m_pFont->GetHandle());
			DrawStringToHandle(cDrawStringX1, cDrawStringY5, "����ȃR�A���������l�`�B��|��", cTextColor, m_pFont->GetHandle());
			DrawStringToHandle(cDrawStringX3, cDrawStringY6, "���_�ɗ���", cTextColor, m_pFont->GetHandle());
		}
		//�ړ��`���[�g���A��
		else if (m_official == 2)
		{
			//�w�i�`��
			DrawGraph(cBackgroundX, cBackgroundY, m_messageUI, false);

			//���X�e�B�b�N�`��
			DrawGraph(cLStickX, cLStickY, m_lStick, true);
			DrawStringToHandle(cDrawStringX1, cDrawStringY7, "L�X�e�B�b�N�@�F�@�ړ�", cTextColor, m_pFont->GetHandle());
			
			//B�{�^���`��
			DrawGraph(cAButtonX, cAButtonY, m_bButton, true);
			DrawStringToHandle(cDrawStringX1, cDrawStringY3, "B�{�^���P�����@�F�@���", cTextColor, m_pFont->GetHandle());
			DrawStringToHandle(cDrawStringX1, cDrawStringY4, "B�{�^���������@�F�@����", cTextColor, m_pFont->GetHandle());

			//�X�^�~�i�`��
			DrawGraph(cStaminaBarX, cStaminaBarY, m_staminaBar, true);
			DrawStringToHandle(cDrawStringX4, cDrawStringY8, "����Ƒ���s���ɂ̓X�^�~�i�������", cTextColor, m_pFont->GetHandle());
		}
		//�W�����v�`���[�g���A��
		else if (m_official == 9)
		{
			//�w�i�`��
			DrawGraph(cBackgroundX, cBackgroundY, m_messageUI, false);

			//A�{�^���`��
			DrawGraph(cAButtonX, cAButtonY, m_aButton, true);
			DrawStringToHandle(cDrawStringX1, cDrawStringY3, "A�{�^���P�����@�F�@�W�����v", cTextColor, m_pFont->GetHandle());
			DrawStringToHandle(cDrawStringX1, cDrawStringY4, "�W�����v���͋��U�����\", cTextColor, m_pFont->GetHandle());

			//�X�^�~�i�`��
			DrawGraph(cStaminaBarX, cStaminaBarY, m_staminaBar, true);
			DrawStringToHandle(cDrawStringX4, cDrawStringY8, "�W�����v�s���ɂ̓X�^�~�i�������", cTextColor, m_pFont->GetHandle());
		}
		//�J�����`���[�g���A��
		else if (m_official == 3)
		{
			//�w�i�`��
			DrawGraph(cBackgroundX, cBackgroundY, m_messageUI, false);

			//�E�X�e�B�b�N�`��
			DrawGraph(cRStickX, cRStickY, m_rStick, true);
			DrawStringToHandle(cDrawStringX1, cDrawStringY2, "R�X�e�B�b�N�@�F�@�J�����ړ�", cTextColor, m_pFont->GetHandle());
			DrawStringToHandle(cDrawStringX1, cDrawStringY9, "R�X�e�B�b�N�������݁@�F�@�^�[�Q�b�g�W��", cTextColor, m_pFont->GetHandle());

			DrawStringToHandle(cDrawStringX5, cDrawStringY8, "�^�[�Q�b�g�W���̊Ԃ�R�X�e�B�b�N�𓮂�����\n�@�@�@�@�@�@�@�^�[�Q�b�g�ύX", cTextColor, m_pFont->GetHandle());

		}
		//�U���`���[�g���A��
		else if (m_official == 4)
		{
			//�w�i�`��
			DrawGraph(cBackgroundX, cBackgroundY, m_messageUI, false);

			//RB�{�^���`��
			DrawGraph(cRBButtonX, cRBButtonY, m_rbButton, true);
			DrawStringToHandle(cDrawStringX6, cDrawStringY10, "RB�{�^���@�F�@�U��", cTextColor, m_pFont->GetHandle());
			DrawStringToHandle(cDrawStringX2, cDrawStringY11, "�U�����ɂ�����x�U����������\n�@�@�ő�R��R���{�ł���", cTextColor, m_pFont->GetHandle());

			//RT�{�^���`��
			DrawGraph(cRTButtonX, cRTButtonY, m_rtButton, true);
			DrawStringToHandle(cDrawStringX6, cDrawStringY4, "RT�{�^���@�F�@���U��", cTextColor, m_pFont->GetHandle());

			//�X�^�~�i�`��
			DrawGraph(cStaminaBarX, cStaminaBarY, m_staminaBar, true);
			DrawStringToHandle(cDrawStringX1, cDrawStringY8, "�U���s���ł̓X�^�~�i�������", cTextColor, m_pFont->GetHandle());
		}
		//�A�C�e���֌W�`���[�g���A��
		else if (m_official == 5)
		{
			//�w�i�`��
			DrawGraph(cBackgroundX, cBackgroundY, m_messageUI, false);

			//Y�{�^���`��
			DrawGraph(cYButtonX, cYButtonY, m_yButton, true);
			DrawStringToHandle(cDrawStringX1, cDrawStringY10, "�}�b�v�Ō��镨�̓A�C�e���Ƃ��ē���ł���", cTextColor, m_pFont->GetHandle());
			DrawStringToHandle(cDrawStringX6, cDrawStringY12, "Y�{�^���@�F�@�A�C�e������", cTextColor, m_pFont->GetHandle());

			//START�{�^���`��
			DrawGraph(cStartButtonX, cStartButtonY, m_startButton, true);
			DrawStringToHandle(cDrawStringX2, cDrawStringY13, "START�{�^���@�F�@���j���[���J��", cTextColor, m_pFont->GetHandle());
			DrawStringToHandle(cDrawStringX1, cDrawStringY14, "���j���[�ł͑����̕ύX��^�C�g���ɖ߂��", cTextColor, m_pFont->GetHandle());

		}
		//�h��`���[�g���A��
		else if (m_official == 6)
		{
			//�w�i�`��
			DrawGraph(cBackgroundX, cBackgroundY, m_messageUI, false);

			//LB�{�^���`��
			DrawGraph(cLBButtonX, cLBButtonY, m_lbButton, true);
			DrawStringToHandle(cDrawStringX1, cDrawStringY1, "���𑕔����Ă���Ƃ���\n�G�̍U����h�����Ƃ��ł���", cTextColor, m_pFont->GetHandle());
			DrawStringToHandle(cDrawStringX6, cDrawStringY15, "LB�{�^���@�F�@�K�[�h", cTextColor, m_pFont->GetHandle());

			//�X�^�~�i�`��
			DrawGraph(cStaminaBarX, cStaminaBarY, m_staminaBar, true);
			DrawStringToHandle(cDrawStringX1, cDrawStringY5, "�h��s���ł̓X�^�~�i�������\n�h�䒆�̓X�^�~�i�̉񕜂��x���Ȃ�", cTextColor, m_pFont->GetHandle());
		}
		//�x���`���[�g���A��
		else if (m_official == 7)
		{
			//�w�i�`��
			DrawGraph(cBackgroundX, cBackgroundY, m_messageUI, false);

			//Y�{�^���`��
			DrawGraph(cXButtonX, cXButtonY, m_yButton, true);
			DrawStringToHandle(cDrawStringX1, cDrawStringY1, "�@�@�����̂悤�ȃI�u�W�F�N�g��\n�@�@�@�@�@�@�@�x�����\�A\n�x��������ƃ��X�|�[���n�_���Œ肷��", cTextColor, m_pFont->GetHandle());
			DrawStringToHandle(cDrawStringX6, cDrawStringY15, "Y�{�^���@�F�@�x��", cTextColor, m_pFont->GetHandle());

			//�A�C�e���`��
			DrawGraph(cItemX, cItemY, m_xButton, true);
			DrawStringToHandle(cDrawStringX6, cDrawStringY16, "X�{�^���@�F�@�A�C�e���g�p", cTextColor, m_pFont->GetHandle());
			DrawStringToHandle(cDrawStringX6, cDrawStringY8, "�A�C�e���͎g�p����Ə����", cTextColor, m_pFont->GetHandle());

		}
		//�F�̃G���A�ɗU������
		else if(m_official == 8)
		{
			//�w�i�`��
			DrawGraph(cBackgroundX, cBackgroundY, m_messageUI, false);

			//�F�̐�����
			DrawStringToHandle(cBearTextX, cBearTextY, "�F�̘��S�l�`���̐�̕��ɖ���\n�@�@�@�@�@HARIBO", cTextColor, m_pFont->GetHandle());
		}

		
	}
}

/// <summary>
/// �`���[�g���A���`��
/// </summary>
void Message::DrawTutorial(Player& player)
{
	//�v���C���[�����̏ꏊ�ɂ����ƃ`���[�g���A����\������
	//�ړ��`���[�g���A��
	if (player.GetPos().x >= -1800.0f && !m_tutorial[0] && m_official == 2)
	{
		//�w�i�`��
		DrawGraph(cBackgroundX, cBackgroundY, m_messageUI, false);

		//���X�e�B�b�N�`��
		DrawGraph(cLStickX, cLStickY, m_lStick, true);
		DrawStringToHandle(cDrawStringX1, cDrawStringY7, "L�X�e�B�b�N�@�F�@�ړ�", cTextColor, m_pFont->GetHandle());

		//B�{�^���`��
		DrawGraph(cAButtonX, cAButtonY, m_bButton, true);
		DrawStringToHandle(cDrawStringX1, cDrawStringY3, "B�{�^���P�����@�F�@���", cTextColor, m_pFont->GetHandle());
		DrawStringToHandle(cDrawStringX1, cDrawStringY4, "B�{�^���������@�F�@����", cTextColor, m_pFont->GetHandle());

		//�X�^�~�i�`��
		DrawGraph(cStaminaBarX, cStaminaBarY, m_staminaBar, true);
		DrawStringToHandle(cDrawStringX4, cDrawStringY8, "����Ƒ���s���ɂ̓X�^�~�i�������", cTextColor, m_pFont->GetHandle());

		m_stop = true;

		//B�{�^���������Ɩ߂�
		if (m_xpad.Buttons[13] == 1)
		{
			m_tutorial[0] = true;
			m_stop = false;
			//���[�����O�ł��Ȃ��悤�ɂ���
			player.SetAction(false);
		}
	}
	//�W�����v�`���[�g���A��
	else if (player.GetPos().x >= -1600.0f && !m_tutorial[6] && m_official == 9)
	{
		//�w�i�`��
		DrawGraph(cBackgroundX, cBackgroundY, m_messageUI, false);

		//A�{�^���`��
		DrawGraph(cAButtonX, cAButtonY, m_aButton, true);
		DrawStringToHandle(cDrawStringX1, cDrawStringY3, "A�{�^���P�����@�F�@�W�����v", cTextColor, m_pFont->GetHandle());
		DrawStringToHandle(cDrawStringX1, cDrawStringY4, "�W�����v���͋��U�����\", cTextColor, m_pFont->GetHandle());

		//�X�^�~�i�`��
		DrawGraph(cStaminaBarX, cStaminaBarY, m_staminaBar, true);
		DrawStringToHandle(cDrawStringX4, cDrawStringY8, "�W�����v�s���ɂ̓X�^�~�i�������", cTextColor, m_pFont->GetHandle());

		m_stop = true;

		//B�{�^���������Ɩ߂�
		if (m_xpad.Buttons[13] == 1)
		{
			m_tutorial[6] = true;
			m_stop = false;
			//���[�����O�ł��Ȃ��悤�ɂ���
			player.SetAction(false);
		}
	}
	//�J�����`���[�g���A��
	else if (player.GetPos().x >= -1500.0f && !m_tutorial[1] && m_official == 3)
	{
		//�w�i�`��
		DrawGraph(cBackgroundX, cBackgroundY, m_messageUI, false);

		//�E�X�e�B�b�N�`��
		DrawGraph(cRStickX, cRStickY, m_rStick, true);
		DrawStringToHandle(cDrawStringX1, cDrawStringY2, "R�X�e�B�b�N�@�F�@�J�����ړ�", cTextColor, m_pFont->GetHandle());
		DrawStringToHandle(cDrawStringX1, cDrawStringY9, "R�X�e�B�b�N�������݁@�F�@�^�[�Q�b�g�W��", cTextColor, m_pFont->GetHandle());

		DrawStringToHandle(cDrawStringX5, cDrawStringY8, "�^�[�Q�b�g�W���̊Ԃ�R�X�e�B�b�N�𓮂�����\n�@�@�@�@�@�@�@�^�[�Q�b�g�ύX", cTextColor, m_pFont->GetHandle());

		m_stop = true;

		//B�{�^���������Ɩ߂�
		if (m_xpad.Buttons[13] == 1)
		{
			m_tutorial[1] = true;
			m_stop = false;
			//���[�����O�ł��Ȃ��悤�ɂ���
			player.SetAction(false);
		}
	}
	//�U���`���[�g���A��
	else if (player.GetPos().x >= -1200.0f && !m_tutorial[2] && m_official == 4)
	{
		//�w�i�`��
		DrawGraph(cBackgroundX, cBackgroundY, m_messageUI, false);

		//RB�{�^���`��
		DrawGraph(cRBButtonX, cRBButtonY, m_rbButton, true);
		DrawStringToHandle(cDrawStringX6, cDrawStringY10, "RB�{�^���@�F�@�U��", cTextColor, m_pFont->GetHandle());
		DrawStringToHandle(cDrawStringX2, cDrawStringY11, "�U�����ɂ�����x�U����������\n�@�@�ő�R��R���{�ł���", cTextColor, m_pFont->GetHandle());

		//RT�{�^���`��
		DrawGraph(cRTButtonX, cRTButtonY, m_rtButton, true);
		DrawStringToHandle(cDrawStringX6, cDrawStringY4, "RT�{�^���@�F�@���U��", cTextColor, m_pFont->GetHandle());

		//�X�^�~�i�`��
		DrawGraph(cStaminaBarX, cStaminaBarY, m_staminaBar, true);
		DrawStringToHandle(cDrawStringX1, cDrawStringY8, "�U���s���ł̓X�^�~�i�������", cTextColor, m_pFont->GetHandle());

		m_stop = true;

		//B�{�^���������Ɩ߂�
		if (m_xpad.Buttons[13] == 1)
		{
			m_tutorial[2] = true;
			m_stop = false;
			//���[�����O�ł��Ȃ��悤�ɂ���
			player.SetAction(false);
		}
	}
	//�A�C�e���`���[�g���A��
	else if (player.GetPos().x >= -900.0f && !m_tutorial[3] && m_official == 5)
	{
		//�w�i�`��
		DrawGraph(cBackgroundX, cBackgroundY, m_messageUI, false);

		//Y�{�^���`��
		DrawGraph(cYButtonX, cYButtonY, m_yButton, true);
		DrawStringToHandle(cDrawStringX1, cDrawStringY10, "�}�b�v�Ō��镨�̓A�C�e���Ƃ��ē���ł���", cTextColor, m_pFont->GetHandle());
		DrawStringToHandle(cDrawStringX6, cDrawStringY12, "Y�{�^���@�F�@�A�C�e������", cTextColor, m_pFont->GetHandle());

		//START�{�^���`��
		DrawGraph(cStartButtonX, cStartButtonY, m_startButton, true);
		DrawStringToHandle(cDrawStringX2, cDrawStringY13, "START�{�^���@�F�@���j���[���J��", cTextColor, m_pFont->GetHandle());
		DrawStringToHandle(cDrawStringX1, cDrawStringY14, "���j���[�ł͑����̕ύX��^�C�g���ɖ߂��", cTextColor, m_pFont->GetHandle());

		m_stop = true;

		//B�{�^���������Ɩ߂�
		if (m_xpad.Buttons[13] == 1)
		{
			m_tutorial[3] = true;
			m_stop = false;
			//���[�����O�ł��Ȃ��悤�ɂ���
			player.SetAction(false);
		}
	}
	//���`���[�g���A��
	else if (player.GetPos().x >= -600.0f && !m_tutorial[4] && m_official == 6)
	{
		//�w�i�`��
		DrawGraph(cBackgroundX, cBackgroundY, m_messageUI, false);

		//LB�{�^���`��
		DrawGraph(cLBButtonX, cLBButtonY, m_lbButton, true);
		DrawStringToHandle(cDrawStringX1, cDrawStringY1, "���𑕔����Ă���Ƃ���\n�G�̍U����h�����Ƃ��ł���", cTextColor, m_pFont->GetHandle());
		DrawStringToHandle(cDrawStringX6, cDrawStringY15, "LB�{�^���@�F�@�K�[�h", cTextColor, m_pFont->GetHandle());

		//�X�^�~�i�`��
		DrawGraph(cStaminaBarX, cStaminaBarY, m_staminaBar, true);
		DrawStringToHandle(cDrawStringX1, cDrawStringY5, "�h��s���ł̓X�^�~�i�������\n�h�䒆�̓X�^�~�i�̉񕜂��x���Ȃ�", cTextColor, m_pFont->GetHandle());
		m_stop = true;

		//B�{�^���������Ɩ߂�
		if (m_xpad.Buttons[13] == 1)
		{
			m_tutorial[4] = true;
			m_stop = false;
			//���[�����O�ł��Ȃ��悤�ɂ���
			player.SetAction(false);
		}
	}
	//�x���`���[�g���A��
	else if (player.GetPos().x >= -300.0f && !m_tutorial[5] && m_official == 7)
	{
		//�w�i�`��
		DrawGraph(cBackgroundX, cBackgroundY, m_messageUI, false);

		//Y�{�^���`��
		DrawGraph(cXButtonX, cXButtonY, m_yButton, true);
		DrawStringToHandle(cDrawStringX1, cDrawStringY1, "�@�@�����̂悤�ȃI�u�W�F�N�g��\n�@�@�@�@�@�@�@�x�����\�A\n�x��������ƃ��X�|�[���n�_���Œ肷��", cTextColor, m_pFont->GetHandle());
		DrawStringToHandle(cDrawStringX6, cDrawStringY15, "Y�{�^���@�F�@�x��", cTextColor, m_pFont->GetHandle());

		//�A�C�e���`��
		DrawGraph(cItemX, cItemY, m_xButton, true);
		DrawStringToHandle(cDrawStringX6, cDrawStringY16, "X�{�^���@�F�@�A�C�e���g�p", cTextColor, m_pFont->GetHandle());
		DrawStringToHandle(cDrawStringX6, cDrawStringY8, "�A�C�e���͎g�p����Ə����", cTextColor, m_pFont->GetHandle());
		m_stop = true;

		//B�{�^���������Ɩ߂�
		if (m_xpad.Buttons[13] == 1)
		{
			m_tutorial[5] = true;
			m_stop = false;
			//���[�����O�ł��Ȃ��悤�ɂ���
			player.SetAction(false);
		}
		}
}

void Message::End()
{
	DeleteGraph(m_messageUI);
	DeleteGraph(m_lStick);
	DeleteGraph(m_aButton);
	DeleteGraph(m_yButton);
	DeleteGraph(m_xButton);
	DeleteGraph(m_staminaBar);
	DeleteGraph(m_rbButton);
	DeleteGraph(m_rtButton);
	DeleteGraph(m_lbButton);
	DeleteGraph(m_startButton);
}

bool Message::GetMessageStay()
{
	return m_pMessage->GetIsStay();
}
