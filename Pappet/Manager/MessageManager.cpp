#include "MessageManager.h"
#include "External/CsvLoad.h"
#include "Manager/GameManager.h"

namespace
{
	bool cMessage = false;
}

MessageManager::MessageManager() :
	m_stageName(""),
	m_messagePick(false)
{
}

MessageManager::~MessageManager()
{
}

void MessageManager::Init()
{
	m_pMessage.clear();
	m_pGenerateInfo.clear();

	m_stageName = "Message";

	cMessage = false;

	//���b�Z�[�W���������擾����
	CsvLoad::GetInstance().MessageDataLoad(m_pGenerateInfo);

	//csv���Ń}�b�v���w�肵�Ă��邽�߃}�b�v�̕␳������
	for (auto& generate : m_pGenerateInfo)
	{
		//�}�b�v��1�Ȃ�
		if (generate->mapNumber == 1)
		{
			//���̃}�b�v�̏o�����郁�b�Z�[�W�̐����X�V����
			int num = m_MessageGenerationCountPerOneMap[generate->mapNumber];
			num++;
			m_MessageGenerationCountPerOneMap[generate->mapNumber];
		}
	}
}

/// <summary>
/// �X�V����
/// </summary>
/// <param name="physics"></param>
/// <param name="gameManager"></param>
void MessageManager::Update(std::shared_ptr<MyLibrary::Physics> physics, GameManager* gameManager, Player& player)
{
	//���̃}�b�v���ǂ̃}�b�v���擾����
	auto thisMapName = gameManager->GetThisMapName();

	//���b�Z�[�W���������܂킵��
	for (auto& generate : m_pGenerateInfo)
	{
		//���̃}�b�v����v�Ă���Ƃ�
		if (generate->mapNumber == thisMapName)
		{
			//�����ς݂łȂ����
			if (!generate->isCreated)
			{
				generate->isCreated = true;
				CreateMessage(generate->posx, generate->posy, generate->posz, generate->official, generate->one, generate->two, generate->three, physics);
			}
		}
	}

	for (auto& up : m_pMessage)
	{
		//�}�b�v�̃��b�Z�[�W�Ƃ��čX�V����
		up->Update(player);
	}
}

void MessageManager::Draw()
{
	if (cMessage)
	{
		for (auto& up : m_pMessage)
		{
			//���������O�ɂ������������ŗ�O�X���[�����
			up->Draw();
		}
		
	}
}

void MessageManager::DrawString()
{
	for (auto& generate : m_pGenerateInfo)
	{
		for (auto& up : m_pMessage)
		{
			if (generate->posx == up->GetPos().x && generate->posy == up->GetPos().y && generate->posz == up->GetPos().z)
			{
				if (up->GetDraw() && up->GetMessageStay())
				{
					up->DrawString();
				}
			}
		}
	}
}

void MessageManager::End()
{
	for (auto& up : m_pMessage)
	{
		up->End();
	}
}

bool MessageManager::GetDraw()
{
	if (cMessage)
	{
		for (auto& up : m_pMessage)
		{
			if (up->GetDraw())
			{
				return up->GetDraw();
			}
		}
		
	}

	return false;
}

int MessageManager::GetLB()
{
	return message->GetLB();
}

int MessageManager::GetRB()
{
	return message->GetRB();
}

void MessageManager::CreateMessage(float posx, float posy, float posz, int official, int one, int two, int three, std::shared_ptr<MyLibrary::Physics> physics)
{
	message = std::make_shared<Message>();
	message->Init(posx, posy, posz, official, one, two, three, physics);
	m_pMessage.emplace_back(message);
	cMessage = true;
}
