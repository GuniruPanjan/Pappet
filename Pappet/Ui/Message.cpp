#include "Message.h"
#include "Object/MessageObject.h"
#include "Manager/EffectManager.h"

namespace
{
	//�G�t�F�N�g�Đ�����
	int cEffectTime = 40;

	//�V���O���g��
	auto& cEffect = EffectManager::GetInstance();
}

Message::Message() :
	m_xpad(),
	m_official(false),
	m_one(0),
	m_two(0),
	m_three(0),
	m_draw(false)
{
	
}

Message::~Message()
{
}

void Message::DataInit()
{
	
}

void Message::Init(float posX, float posY, float posZ, bool official, int one, int two, int three, std::shared_ptr<MyLibrary::Physics> physics)
{
	m_pMessage = std::make_shared<MessageObject>(50.0f);

	m_centerPos = MyLibrary::LibVec3(posX, posY, posZ);

	m_pMessage->Init(physics, m_centerPos);

	m_official = official;
	m_one = one;
	m_two = two;
	m_three = three;
}

void Message::Update(bool taking)
{
	//�p�b�h���͎擾
	GetJoypadXInputState(DX_INPUT_KEY_PAD1, &m_xpad);

	m_pMessage->Update(m_centerPos);

	//���b�Z�[�W��ǂ�
	if (taking)
	{
		m_draw = true;

		//B�{�^���������Ɩ߂�
		if (m_xpad.Buttons[13] == 1)
		{
			m_draw = false;
		}
	}
}

void Message::Draw()
{
	if (cEffectTime >= 50)
	{
		cEffect.EffectCreate("Message", m_centerPos.ConversionToVECTOR());

		cEffectTime = 0;
	}
	else
	{
		cEffectTime++;
	}

	//�����`��
	if (m_draw)
	{
		if (m_official)
		{
			int a = 1;
		}
		else
		{

		}
	}
}

void Message::End()
{
}
