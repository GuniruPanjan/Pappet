#pragma once

/// <summary>
/// �t�F�[�h�A�E�g�E�t�F�[�h�C�����Ǘ�����N���X
/// </summary>
class FadeManager
{
public:
	FadeManager() :
		m_fadeOut(false),
		m_fadeIn(false),
		m_fade(0),
		m_fadeHandle(-1)
	{

	}
	virtual ~FadeManager();

	//����������
	void Init();

	//�t�F�[�h�A�E�g�֐�
	void FadeOut(int fade);
	//�t�F�[�h�C���֐�
	void FadeIn(int fade);
	//�`�揈��
	void Draw();
	//�I������
	void End();

	int SetFade(int set) { return m_fade = set; }

	bool GetOut() { return m_fadeOut; }
	bool SetOut(bool set) { return m_fadeOut = set; }
	bool GetIn() { return m_fadeIn; }
	bool SetIn(bool set) { return m_fadeIn = set; }

private:
	bool m_fadeOut;    //�t�F�[�h�A�E�g����
	bool m_fadeIn;     //�t�F�[�h�C������

	int m_fade;        //�t�F�[�h�ϐ�
	int m_fadeHandle;  //�t�F�[�h�n���h��
};






