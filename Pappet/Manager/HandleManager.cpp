#include "HandleManager.h"
#include "DxLib.h"
#include<cassert>

HandleManager* HandleManager::m_instance = nullptr;

HandleManager::HandleManager()
{
}

HandleManager::~HandleManager()
{
	for (auto& h : m_handles)
	{
		MV1DeleteModel(h.handle);
	}

	m_handles.clear();
}

int HandleManager::GetModelHandle(std::string path, bool isEternal)
{
	//���łɃ��[�h����Ă����畡���n���h����Ԃ�
	for (auto& h : m_handles)
	{
		if (h.path == path)
		{
			SetUseASyncLoadFlag(TRUE);     //�񓯊��ǂݍ��ݗL��
			return MV1DuplicateModel(h.handle);
			SetUseASyncLoadFlag(FALSE);    //�ȍ~�񓯊��ǂݍ��ݔ��肵�Ă͂����Ȃ����߉���

		}
	}

	//���[�h����Ă��Ȃ�������
	Handle add;
	SetUseASyncLoadFlag(TRUE);     //�񓯊��ǂݍ��ݗL��
	add.handle = MV1LoadModel(path.c_str());
	SetUseASyncLoadFlag(FALSE);    //�ȍ~�񓯊��ǂݍ��ݔ��肵�Ă͂����Ȃ����߉���
	add.path = path;
	add.isEternal = isEternal;

	m_handles.emplace_back(add);

	//�����p
	int a = GetASyncLoadNum();

	//���[�h�����n���h���̕����n���h����Ԃ�
	return MV1DuplicateModel(add.handle);
}

void HandleManager::Clear()
{
	//isEternal��false�̃n���h����Delete����
	for (auto& h : m_handles)
	{
		if (!h.isEternal)
		{
			MV1DeleteModel(h.handle);
			h.handle = 0;
		}
	}

	//�s�v�ɂȂ����n���h���������ō폜��������
	auto it = remove_if(m_handles.begin(), m_handles.end(), []
	(auto& v) {
			return v.handle == 0;
		       });

	m_handles.erase(it, m_handles.end());
}
