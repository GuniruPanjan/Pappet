#pragma once
#include "SceneBase.h"
#include<memory>

class Map;
class SelectManager;
class FadeManager;
class MapManager;
class Font;

//�V�[���^�C�g���̃N���X
class SceneTitle : public SceneBase
{
public:
	//�R���X�g���N�^
	SceneTitle();
	//�f�X�g���N�^
	virtual ~SceneTitle();

	//����������
	virtual void Init();
	//�X�V����
	virtual std::shared_ptr<SceneBase> Update();

	/// <summary>
	/// �I�𒆂̏ꍇ
	/// </summary>
	/// <param name="select">�񋓌^</param>
	/// <param name="now">�I�����Ă����</param>
	/// <param name="other1">����ȊO�P</param>
	/// <param name="other2">����ȊO�Q</param>
	void SelectBlend(int select, int now, int other1, int other2);

	//�`�揈��
	virtual void Draw();

	//�I������
	virtual void End();

private:
	//�C�[�W���O�֐�(���`���)
	float Lerp(float start, float end, float t);
	//�C�[�W���O�g�p���ăJ�����̈ʒu�ƃ^�[�Q�b�g�ʒu���X�V
	void UpdateCameraPositionAndTarget(float deltaTime, VECTOR cameraPos, VECTOR cameraPos1, VECTOR cameraTarget, VECTOR cameraTarget1);

private:
	XINPUT_STATE m_xpad;  //�p�b�h����
	int m_start;      //�X�^�[�g�{�^���摜�i�[�ϐ�
	int m_setting;    //�ݒ�{�^���摜�i�[�ϐ�
	int m_end;        //�I���{�^���摜�i�[�ϐ�
	int m_hand;       //�I�����Ă�摜�`��
	int m_BButton;    //B�{�^���摜�i�[�ϐ�
	int m_AButton;    //A�{�^���摜�i�[�ϐ�
	int m_button;    //���������Ԃ𓾂�
	int m_select[3];    //�I��p�̕ϐ�
	int m_pal[3];      //�u�����h�`��p
	int m_waitTime;      //�҂��߂̎���
	int m_load;          //���[�h����
	int m_loadNow1;      //���[�h�摜�i�[�ϐ�
	int m_loadNow2;      //���[�h�摜�i�[�ϐ�
	bool m_one;      //�P������
	bool m_blend;    //�u�����h����
	bool m_decisionButton;  //���蔻��
	VECTOR m_cameraPos;    //�J�����|�W�V����
	VECTOR m_cameraTarget;     //�J�����̃^�[�Q�b�g�|�W�V����
	VECTOR m_cameraTargetGraves;    //�J�����̃^�[�Q�b�g���Ɍ�����

	//���f���p�ϐ�
	int m_playerHandle;          //�v���C���[�̃��f���i�[�ϐ�
	int m_anim;                  //�v���C���[�̃A�j���[�V�����i�[�ϐ�
	int m_animation;
	float m_totalAnimationTime;
	float m_playTime;
	float m_time;                //��ԌW��
	VECTOR m_pos;

	//�X�}�[�g�|�C���^
	std::shared_ptr<SEManager> pse = std::make_shared<SEManager>();
	std::shared_ptr<SelectManager> pselect = std::make_shared<SelectManager>();
	std::shared_ptr<FadeManager> m_pFade = std::make_shared<FadeManager>();
	std::shared_ptr<MapManager> m_pMap = std::make_shared<MapManager>();
	std::shared_ptr<Font> m_pFont;

};