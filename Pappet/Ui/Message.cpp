#include "Message.h"
#include "Object/MessageObject.h"
#include "Manager/EffectManager.h"
#include "Character/Player.h"
#include "External/Font.h"

namespace
{
	//�V���O���g��
	auto& cEffect = EffectManager::GetInstance();
}

Message::Message() :
	m_xpad(),
	m_effectTime(50),
	m_official(0),
	m_one(0),
	m_two(0),
	m_three(0),
	m_draw(false),
	m_can(false)
{
	m_pFont = std::make_shared<Font>();
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
	m_aButton = MyLoadGraph("Data/UI/AButton.png", 1, 1);
	m_yButton = MyLoadGraph("Data/UI/YButton.png", 1, 1);
	m_xButton = MyLoadGraph("Data/UI/XButton.png", 1, 1);
	m_staminaBar = MyLoadGraph("Data/UI/StaminaBar.png", 1, 2);
	m_rbButton = MyLoadGraph("Data/UI/RBButton.png", 2, 2);
	m_rtButton = MyLoadGraph("Data/UI/RTButton.png", 2, 2);
	m_lbButton = MyLoadGraph("Data/UI/LBButton.png", 2, 2);
	m_startButton = MyLoadGraph("Data/UI/START.png", 2, 2);

	m_pMessage = std::make_shared<MessageObject>(50.0f);

	m_centerPos = MyLibrary::LibVec3(posX, posY, posZ);

	m_pMessage->Init(physics, m_centerPos);

	m_official = official;
	m_one = one;
	m_two = two;
	m_three = three;

	m_pFont->FontInit(50);
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
	if (m_draw)
	{
		//B�{�^���������Ɩ߂�
		if (m_xpad.Buttons[13] == 1)
		{
			m_draw = false;

			player.SetMessageRead(false);
		}
	}
}

void Message::Draw()
{
	if (m_effectTime >= 50)
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
			DrawGraph(130, 100, m_messageUI, false);

			DrawStringToHandle(400, 150, "�ڂ̑O�̑劘�ł͓���ȋx�����ł�", 0xffffff, m_pFont->GetHandle());
			DrawStringToHandle(400, 250, "�E���ɂ���擾�����R�A���g����", 0xffffff, m_pFont->GetHandle());
			DrawStringToHandle(500, 350, "���x�����グ�邱�Ƃ��\", 0xffffff, m_pFont->GetHandle());
			DrawStringToHandle(500, 450, "���x�����グ�e�n�ɋ�����", 0xffffff, m_pFont->GetHandle());
			DrawStringToHandle(400, 550, "����ȃR�A���������l�`�B��|��", 0xffffff, m_pFont->GetHandle());
			DrawStringToHandle(700, 650, "���_�ɗ���", 0xffffff, m_pFont->GetHandle());
		}
		//�ړ��`���[�g���A��
		else if (m_official == 2)
		{
			//�w�i�`��
			DrawGraph(130, 100, m_messageUI, false);

			//���X�e�B�b�N�`��
			DrawGraph(200, 150, m_lStick, true);
			DrawStringToHandle(400, 180, "L�X�e�B�b�N�@�F�@�ړ�", 0xffffff, m_pFont->GetHandle());
			
			//B�{�^���`��
			DrawGraph(180, 340, m_aButton, true);
			DrawStringToHandle(400, 350, "A�{�^���P�����@�F�@���", 0xffffff, m_pFont->GetHandle());
			DrawStringToHandle(400, 450, "A�{�^���������@�F�@����", 0xffffff, m_pFont->GetHandle());

			//�X�^�~�i�`��
			DrawGraph(370, 660, m_staminaBar, true);
			DrawStringToHandle(350, 600, "����Ƒ���s���ɂ̓X�^�~�i�������", 0xffffff, m_pFont->GetHandle());
		}
		//�J�����`���[�g���A��
		else if (m_official == 3)
		{
			//�w�i�`��
			DrawGraph(130, 100, m_messageUI, false);

			//�E�X�e�B�b�N�`��
			DrawGraph(180, 270, m_rStick, true);
			DrawStringToHandle(400, 250, "R�X�e�B�b�N�@�F�@�J�����ړ�", 0xffffff, m_pFont->GetHandle());
			DrawStringToHandle(400, 400, "R�X�e�B�b�N�������݁@�F�@�^�[�Q�b�g�W��", 0xffffff, m_pFont->GetHandle());

			DrawStringToHandle(250, 600, "�^�[�Q�b�g�W���̊Ԃ�R�X�e�B�b�N�𓮂�����\n�@�@�@�@�@�@�@�^�[�Q�b�g�ύX", 0xffffff, m_pFont->GetHandle());

		}
		//�U���`���[�g���A��
		else if (m_official == 4)
		{
			//�w�i�`��
			DrawGraph(130, 100, m_messageUI, false);

			//RB�{�^���`��
			DrawGraph(180, 200, m_rbButton, true);
			DrawStringToHandle(600, 200, "RB�{�^���@�F�@�U��", 0xffffff, m_pFont->GetHandle());
			DrawStringToHandle(500, 270, "�U�����ɂ�����x�U����������\n�@�@�ő�R��R���{�ł���", 0xffffff, m_pFont->GetHandle());

			//RT�{�^���`��
			DrawGraph(180, 450, m_rtButton, true);
			DrawStringToHandle(600, 450, "RT�{�^���@�F�@���U��", 0xffffff, m_pFont->GetHandle());

			//�X�^�~�i�`��
			DrawGraph(370, 660, m_staminaBar, true);
			DrawStringToHandle(400, 600, "�U���s���ł̓X�^�~�i�������", 0xffffff, m_pFont->GetHandle());
		}
		//�A�C�e���֌W�`���[�g���A��
		else if (m_official == 5)
		{
			//�w�i�`��
			DrawGraph(130, 100, m_messageUI, false);

			//Y�{�^���`��
			DrawGraph(180, 200, m_yButton, true);
			DrawStringToHandle(400, 200, "�}�b�v�Ō��镨�̓A�C�e���Ƃ��ē���ł���", 0xffffff, m_pFont->GetHandle());
			DrawStringToHandle(600, 300, "Y�{�^���@�F�@�A�C�e������", 0xffffff, m_pFont->GetHandle());

			//START�{�^���`��
			DrawGraph(180, 450, m_startButton, true);
			DrawStringToHandle(500, 480, "START�{�^���@�F�@���j���[���J��", 0xffffff, m_pFont->GetHandle());
			DrawStringToHandle(400, 580, "���j���[�ł͑����̕ύX��^�C�g���ɖ߂��", 0xffffff, m_pFont->GetHandle());

		}
		//�h��`���[�g���A��
		else if (m_official == 6)
		{
			//�w�i�`��
			DrawGraph(130, 100, m_messageUI, false);

			//LB�{�^���`��
			DrawGraph(180, 300, m_lbButton, true);
			DrawStringToHandle(400, 150, "���𑕔����Ă���Ƃ���\n�G�̍U����h�����Ƃ��ł���", 0xffffff, m_pFont->GetHandle());
			DrawStringToHandle(600, 320, "LB�{�^���@�F�@�K�[�h", 0xffffff, m_pFont->GetHandle());

			//�X�^�~�i�`��
			DrawGraph(370, 660, m_staminaBar, true);
			DrawStringToHandle(400, 550, "�h��s���ł̓X�^�~�i�������\n�h�䒆�̓X�^�~�i�̉񕜂��x���Ȃ�", 0xffffff, m_pFont->GetHandle());
		}
		//�x���`���[�g���A��
		else if (m_official == 7)
		{
			//�w�i�`��
			DrawGraph(130, 100, m_messageUI, false);

			//Y�{�^���`��
			DrawGraph(200, 250, m_yButton, true);
			DrawStringToHandle(400, 150, "�@�@�����̂悤�ȃI�u�W�F�N�g��\n�@�@�@�@�@�@�@�x�����\�A\n�x��������ƃ��X�|�[���n�_���Œ肷��", 0xffffff, m_pFont->GetHandle());
			DrawStringToHandle(600, 320, "Y�{�^���@�F�@�x��", 0xffffff, m_pFont->GetHandle());

			//�A�C�e���`��
			DrawGraph(200, 500, m_xButton, true);
			DrawStringToHandle(600, 520, "X�{�^���@�F�@�A�C�e���g�p", 0xffffff, m_pFont->GetHandle());
			DrawStringToHandle(600, 600, "�A�C�e���͎g�p����Ə����", 0xffffff, m_pFont->GetHandle());

		}
		//�F�̃G���A�ɗU������
		else if(m_official == 8)
		{
			//�w�i�`��
			DrawGraph(130, 100, m_messageUI, false);

			//�F�̐�����
			DrawStringToHandle(450, 400, "�F�̘��S�l�`���̐�̕��ɖ���\n�@�@�@�@�@HARIBO", 0xffffff, m_pFont->GetHandle());
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
