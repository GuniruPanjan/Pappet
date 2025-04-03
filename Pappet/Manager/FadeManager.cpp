#include "FadeManager.h"
#include "DxLib.h"

namespace
{
	//�t�F�[�h�A�E�g����
	bool cOut = false;
	//�t�F�[�h�C������
	bool cIn = false;
}

FadeManager::~FadeManager()
{
	DeleteGraph(m_fadeHandle);
}

/// <summary>
/// ����������
/// </summary>
void FadeManager::Init()
{
	m_fadeOut = false;
	m_fadeIn = false;

	m_fadeHandle = LoadGraph("Data/SceneBack/BlackMini.png");
}

/// <summary>
/// �t�F�[�h�A�E�g�֐�
/// </summary>
void FadeManager::FadeOut(int fade)
{
	//��񂾂����s
	if (!cIn)
	{
		m_fade = 0;

		cIn = true;
		cOut = false;
	}

	if (m_fade < 255)
	{
		m_fade += fade;
	}
	else
	{
		m_fadeOut = true;
	}
}

/// <summary>
/// �t�F�[�h�C���֐�
/// </summary>
void FadeManager::FadeIn(int fade)
{
	//��񂾂����s
	if (!cOut)
	{
		m_fade = 255;

		cIn = false;
		cOut = true;
	}

	if (m_fade > 0)
	{
		m_fade -= fade;
	}
	else
	{
		m_fadeIn = true;
	}
}

/// <summary>
/// �`�揈��
/// </summary>
void FadeManager::Draw()
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fade);
	DrawGraph(0, 0, m_fadeHandle, false);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

/// <summary>
/// �I������
/// </summary>
void FadeManager::End()
{
	DeleteGraph(m_fadeHandle);
}



