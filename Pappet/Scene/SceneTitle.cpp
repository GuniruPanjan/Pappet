#include "SceneTitle.h"
#include "SceneGame.h"
#include "Manager/HandleManager.h"
#include "Manager/SelectManager.h"
#include "Map/Map.h"
#include "Manager/MapManager.h"
#include "Manager/EffectManager.h"
#include "Manager/FadeManager.h"
#include "External/Font.h"

namespace
{
	int selectDecision = 0;  //�I�����A���肵������

	float cCameraTargetx = -250.0f;
	float cCameraTargety = 115.0f;
	float cCameraTargetz = -270.0f;

	constexpr int cFontSize = 35;

	bool cCameraTrun = false;

	int cHandY = 600;

	//��񂾂��s��
	bool cOne = false;
	bool cOneLoad = false;

	//���[�h��ʃ����_��
	int cLoad = 0;

	//�V���O���g��
	auto& handle = HandleManager::GetInstance();
	auto& cEffect = EffectManager::GetInstance();
}

/// <summary>
/// �R���X�g���N�^
/// </summary>
SceneTitle::SceneTitle() :
	m_start(0),
	m_setting(0),
	m_end(0),
	m_button(0),
	m_hand(0),
	m_BButton(0),
	m_AButton(0),
	m_loadNow1(-1),
	m_loadNow2(-1),
	m_one(false),
	m_blend(false),
	m_setButton(false),
	m_decisionButton(false),
	m_waitTime(0),
	m_cameraPos(VGet(0.0f, 0.0f, 0.0f)),
	m_playerHandle(0),
	m_anim(0),
	m_animation(0),
	m_totalAnimationTime(0.0f),
	m_playTime(0.0f),
	m_pos(VGet(0.0f, 0.0f, 0.0f)),
	m_cameraTarget(VGet(0.0f, 0.0f, 0.0f)),
	m_xpad()
{
	for (int i = 0; i < 3; i++)
	{
		m_select[i] = 0;
		m_pal[i] = 0;
	}

	m_pFont = std::make_shared<Font>();
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
SceneTitle::~SceneTitle()
{
	//���������
	DeleteGraph(m_backScene);
	DeleteGraph(m_start);
	DeleteGraph(m_setting);
	DeleteGraph(m_end);
	DeleteGraph(m_hand);
	DeleteGraph(m_BButton);
	DeleteGraph(m_AButton);
	DeleteGraph(m_loadNow1);
	DeleteGraph(m_loadNow2);
	MV1DeleteModel(m_playerHandle);
	MV1DeleteModel(m_anim);
	m_pSetting->End();
	m_pBgm->End();
	pse->End();

	handle.Clear();
}

/// <summary>
/// ����������
/// </summary>
void SceneTitle::Init()
{
	//�������ǂݍ���
	m_backScene = m_pUi->MyLoadGraph("Data/SceneBack/PuppetGraves�^�C�g��mini.png", 1, 1);     //144 KB (147,793 �o�C�g)
	m_start = m_pUi->MyLoadGraph("Data/UI/STARTButtonMini.png", 1, 1);                         //27.1 KB (27,851 �o�C�g)
	m_setting = m_pUi->MyLoadGraph("Data/UI/SettingButtonMini.png", 1, 1);                     //29.4 KB (30,170 �o�C�g)
	m_end = m_pUi->MyLoadGraph("Data/UI/EndButtonMini.png", 1, 1);                             //22.5 KB (23,109 �o�C�g)
	m_hand = m_pUi->MyLoadGraph("Data/UI/PuppetHand.png", 2, 2);                         
	m_BButton = m_pUi->MyLoadGraph("Data/UI/BButton.png", 3, 3);
	m_AButton = m_pUi->MyLoadGraph("Data/UI/AButton.png", 3, 3);

	m_loadNow1 = m_pUi->MyLoadGraph("Data/SceneBack/NowLoading1.png", 1, 1);
	m_loadNow2 = m_pUi->MyLoadGraph("Data/SceneBack/NowLoading2.png", 1, 1);

	m_playerHandle = handle.GetModelHandle("Data/Player/PuppetPlayerModel.mv1");
	m_anim = handle.GetModelHandle("Data/PlayerAnimation/JumpingDown.mv1");

	m_pMap->DataInit(0);
	cEffect.Init();

	m_pPhysics = std::make_shared<MyLibrary::Physics>(m_pMap->GetCollisionMap());

	//pmap->Init();
	m_pMap->Init(m_pPhysics);

	selectDecision = 0;

	//�A�j���[�V�����A�^�b�`
	m_animation = MV1AttachAnim(m_playerHandle, 1, m_anim, TRUE);

	//����
	m_totalAnimationTime = MV1GetAttachAnimTotalTime(m_playerHandle, m_animation);

	m_playTime = 28.0f;

	m_pos = VGet(-145.0f, 0.0f, -270.0f);

	m_select[0] = 1;
	m_select[1] = 0;
	m_select[2] = 0;

	for (int i = 0; i < 3; i++)
	{
		m_pal[i] = 255;
	}

	cCameraTrun = false;


	//m_cameraPos = VGet(550.0f, 20.0f, -450.0f);
	m_cameraPos = VGet(-80.0f, 35.0f, 80.0f);
	m_cameraTarget = VGet(cCameraTargetx, cCameraTargety, cCameraTargetz);

	//�t�F�[�h�A�E�g�C��������
	m_pFade->Init();

	//�t�H���g������
	m_pFont->FontInit(cFontSize);

	//�ݒ�֌W
	m_pSetting->Init();
	m_pBgm->TitleInit();
	m_pBgm->TitleBGM();
	pse->SceneInit();

	//��񂾂��s��
	if (!cOne)
	{
		//�񓯊����������Ȃ�
		m_isLoading = false;

		cOne = true;
	}
	else
	{
		//�񓯊��������s��
		m_isLoading = true;

		m_load = 20;
	}

	m_one = false;
	m_blend = false;
	m_decisionButton = false;
}

/// <summary>
/// �X�V����
/// </summary>
/// <returns>�V�[����Ԃ�</returns>
std::shared_ptr<SceneBase> SceneTitle::Update()
{
	if (m_isLoading)
	{
		//�񓯊����I��点��
		if (m_load == 0)
		{
			m_isLoading = false;
		}
	}
	else
	{
		cEffect.Update();
		m_pMap->Update(m_pPhysics, false, false, false);

		//�J�����㉺�ɓ����p
		if (cCameraTargety >= 140.0f)
		{
			cCameraTrun = false;
		}
		else if (cCameraTargety <= 100.0f)
		{
			cCameraTrun = true;
		}

		if (!cCameraTrun)
		{
			cCameraTargety -= 0.1f;
		}
		else if (cCameraTrun)
		{
			cCameraTargety += 0.1f;
		}

		m_cameraTarget = VGet(cCameraTargetx, cCameraTargety, cCameraTargetz);

		if (m_pSetting->GetSettingScene() == false)
		{
			//�p�b�h���͏���
			GetJoypadXInputState(DX_INPUT_KEY_PAD1, &m_xpad);

			//��
			if (m_xpad.ThumbLY > 2000 || m_xpad.Buttons[0] == 1 && !m_decisionButton)
			{
				m_button++;
			}
			//��
			else if (m_xpad.ThumbLY < 0 || m_xpad.Buttons[1] == 1 && !m_decisionButton)
			{
				m_button--;
			}
			else
			{
				//������
				m_button = 0;

				m_one = false;
			}

			m_playTime += 0.5f;

			pselect->Menu_Update(m_button, m_one, m_xpad.Buttons[12], selectDecision, pselect->Eight);

			if (pselect->NowSelect == 7)
			{
				cHandY = 600;
			}
			else if (pselect->NowSelect == 8)
			{
				cHandY = 750;
			}
			else if (pselect->NowSelect == 9)
			{
				cHandY = 900;
			}

			if (m_waitTime > 50)
			{
				//A�{�^������������
				if (m_xpad.Buttons[12] == 1 || CheckHitKey(KEY_INPUT_A) == 1)
				{
					if (!m_decisionButton)
					{
						PlaySoundMem(pse->GetButtonSE(), DX_PLAYTYPE_BACK, true);
					}

					//�Q�[���X�^�[�g
					if (selectDecision == 8)
					{
						//�{�^�������肵������
						m_decisionButton = true;
					}
					//�ݒ�
					if (selectDecision == 9)
					{
						m_setButton = true;

						m_waitTime = 0;

						m_pSetting->SetSettingScene(m_setButton);
					}
					//�I��
					if (selectDecision == 10)
					{
						SetEnd(true);
					}
				}
				//B�{�^������������
				else if (m_xpad.Buttons[13] == 1 && !m_decisionButton)
				{
					SetEnd(true);
				}
			}
			else if (m_pSetting->GetSettingScene() == false)
			{
				m_waitTime++;
			}


			SelectBlend(7, 0, 1, 2);
			SelectBlend(8, 1, 0, 2);
			SelectBlend(9, 2, 1, 0);

		}
		//�ݒ���J��
		else if (m_pSetting->GetSettingScene() == true)
		{
			m_pSetting->Update();
		}

		if (m_decisionButton)
		{
			//�t�F�[�h�J�n
			m_pFade->FadeOut(2);
		}

		//�t�F�[�h�A�E�g����
		if (m_pFade->GetOut())
		{
			m_pMap->End(m_pPhysics, true);
			m_pSetting->End();

			return std::make_shared<SceneGame>();
		}



		if (m_playTime >= m_totalAnimationTime && m_animation != -1)
		{
			m_playTime = 80.0f;
		}
		if (m_animation != -1)
		{
			MV1SetAttachAnimTime(m_playerHandle, m_animation, m_playTime);
		}

		m_pBgm->Update(m_pSetting->GetVolume());
		pse->Update(m_pSetting->GetVolume());

		SetCameraPositionAndTarget_UpVecY(m_cameraPos, m_cameraTarget);
	}

	return shared_from_this();  //���g�̃|�C���^��Ԃ�
}

/// <summary>
/// �I�𒆂̏ꍇ
/// </summary>
/// <param name="select">�񋓌^</param>
/// <param name="now">�I�����Ă����</param>
/// <param name="other1">����ȊO�P</param>
/// <param name="other2">����ȊO�Q</param>
void SceneTitle::SelectBlend(int select, int now, int other1, int other2)
{
	if (pselect->NowSelect == select)
	{
		if (m_blend == false)
		{
			if (m_pal[now] < 256)
			{
				m_pal[now] += 2;
			}
			else
			{
				m_blend = true;
			}

		}
		if (m_blend == true)
		{
			if (m_pal[now] > 125)
			{
				m_pal[now] -= 2;
			}
			else
			{
				m_blend = false;
			}
		}


		m_pal[other1] = 255;
		m_pal[other2] = 255;
	}
}

/// <summary>
/// �`�揈��
/// </summary>
void SceneTitle::Draw()
{
	if (m_isLoading)
	{
		//��񂾂��s��
		if (!cOneLoad)
		{
			cLoad = GetRand(1);

			cOneLoad = true;
		}

		//�����_���Ń��[�h��ʂ�ς���
		if (cLoad == 0)
		{
			DrawGraph(0, 0, m_loadNow1, true);
		}
		else if (cLoad == 1)
		{
			DrawGraph(-100, 0, m_loadNow2, true);
		}
		

		DrawStringToHandle(0, 0, "NowLoading...", 0xffffff, m_pFont->GetHandle());

		// ���[�h�̐i�s�󋵂��v�Z
		int totalLoadTasks = m_load; // �����[�h�^�X�N�����擾����֐��i����j

		m_load--;

		int remainingLoadTasks = m_load; // �c��̃��[�h�^�X�N�����擾
		float progress = 1.0f - (static_cast<float>(remainingLoadTasks) / static_cast<float>(totalLoadTasks));

		// �o�[�̕`��
		int barWidth = 600; // �o�[�̕�
		int barHeight = 20; // �o�[�̍���
		int barX = 100; // �o�[��X���W
		int barY = 50; // �o�[��Y���W
		DrawBox(barX, barY, barX + static_cast<int>(barWidth * progress), barY + barHeight, 0x00ff00, TRUE); // �v���O���X�o�[
		DrawBox(barX, barY, barX + barWidth, barY + barHeight, 0xffffff, false);
	}
	else
	{
		cOneLoad = false;

		m_pMap->Draw();
		cEffect.Draw();

		MV1SetPosition(m_playerHandle, m_pos);

		MV1DrawModel(m_playerHandle);

		//3D���f���̉�]�n���Z�b�g����
		MV1SetRotationXYZ(m_playerHandle, VGet(0.0f, 160.0f, 0.0f));


		DrawGraph(120, 0, m_backScene, TRUE);
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_pal[0]);
		DrawGraph(500, 350, m_start, TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_pal[1]);
		DrawGraph(500, 500, m_setting, TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_pal[2]);
		DrawGraph(500, 650, m_end, TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		DrawRotaGraph(620, cHandY, 1.0f, DX_PI_F - (DX_PI_F / 4), m_hand, true);
		DrawRotaGraph(1050, cHandY, 1.0f, DX_PI_F + (DX_PI_F / 4), m_hand, true, true);

		SetFontSize(35);

		DrawGraph(1150, 900, m_AButton, true);
		DrawStringToHandle(1210, 910, "����", 0xffffff, m_pFont->GetHandle());
		DrawGraph(1350, 900, m_BButton, true);
		DrawStringToHandle(1410, 910, "�L�����Z��", 0xffffff, m_pFont->GetHandle());

		SetFontSize(40);

		//�ݒ��ʂ�`��
		if (m_pSetting->GetSettingScene() == true)
		{
			m_pSetting->Draw();
		}

		m_pSetting->SettingDraw(m_pSetting->GetVolume());

		if (m_pSetting->GetSettingScene() == false)
		{
			pselect->Draw();
		}

		//�t�F�[�h�A�E�g�C���`��
		m_pFade->Draw();
	}
}

/// <summary>
/// �I������
/// </summary>
void SceneTitle::End()
{
	//���������
	DeleteGraph(m_backScene);
	DeleteGraph(m_start);
	DeleteGraph(m_setting);
	DeleteGraph(m_end);
	DeleteGraph(m_hand);
	DeleteGraph(m_BButton);
	DeleteGraph(m_AButton);
	DeleteGraph(m_loadNow1);
	DeleteGraph(m_loadNow2);
	MV1DeleteModel(m_playerHandle);
	MV1DeleteModel(m_anim);
	m_pSetting->End();
	m_pBgm->End();
	pse->End();
	m_pMap->End(m_pPhysics, true);
	cEffect.End();

	handle.Clear();
}
