#include "SceneTitle.h"
#include "SceneGame.h"
#include "Manager/HandleManager.h"
#include "Manager/SelectManager.h"
#include "Manager/MapManager.h"
#include "Manager/EffectManager.h"
#include "Manager/FadeManager.h"
#include "External/Font.h"

namespace
{
	int cSelectDecision = 0;  //�I�����A���肵������

	//�v���C���[�ւ̃^�[�Q�b�g�ϐ�
	float cCameraTargetx = -250.0f;
	float cCameraTargety = 115.0f;
	float cCameraTargetz = -270.0f;
	//��ւ̃^�[�Q�b�g�ϐ�
	float cCameraTargetGravesx = -200.0f;
	float cCameraTargetGravesy = 115.0f;
	float cCameraTargetGravesz = 500.0f;

	const float cDuration = 1.0f;   //�C�[�W���O�ɂ����鎞��

	float cCameraPosx = -200.0f;
	float cCameraPosy = 500.0f;
	float cCameraPosz = -400.0f;

	constexpr int cFontSize = 35;
	constexpr float cDeltaTime = 0.016f;

	bool cCameraTrun = false;
	bool cCameraEasing = false;

	int cHandY = 600;

	//��񂾂��s��
	bool cOne = false;
	bool cOneLoad = false;

	//���[�h��ʃ����_��
	int cLoad = 0;

	constexpr int cCameraTargetYMax = 140;
	constexpr int cCameraTargetYMin = 100;
	constexpr float cCameraTargetYStep = 0.1f;
	constexpr int cThumbLYThreshold = 2000;
	constexpr int cWaitTimeThreshold = 50;
	constexpr float cPlayTimeStep = 0.5f;
	constexpr int cHandYSelect7 = 600;
	constexpr int cHandYSelect8 = 750;
	constexpr int cHandYSelect9 = 900;
	constexpr int cPlayTimeMax = 80;
	constexpr int cPalMax = 256;
	constexpr int cPalMin = 125;
	constexpr int cPalStep = 2;

	constexpr int cBarWidth = 600; // �o�[�̕�
	constexpr int cBarHeight = 20; // �o�[�̍���
	constexpr int cBarX = 100; // �o�[��X���W
	constexpr int cBarY = 50; // �o�[��Y���W
	constexpr int cGraphX1 = 120;
	constexpr int cGraphY1 = 0;
	constexpr int cGraphX2 = 500;
	constexpr int cGraphY2 = 350;
	constexpr int cGraphY3 = 500;
	constexpr int cGraphY4 = 650;
	constexpr int cRotaGraphX1 = 620;
	constexpr int cRotaGraphX2 = 1050;
	constexpr float cRotaGraphScale = 1.0f;
	constexpr float cRotaGraphAngle1 = DX_PI_F - (DX_PI_F / 4);
	constexpr float cRotaGraphAngle2 = DX_PI_F + (DX_PI_F / 4);
	constexpr int cGraphX3 = 1150;
	constexpr int cGraphY5 = 900;
	constexpr int cStringX1 = 1210;
	constexpr int cStringY1 = 910;
	constexpr int cGraphX4 = 1350;
	constexpr int cStringX2 = 1410;
	constexpr int cStringY2 = 910;

	//�V���O���g��
	auto& cHandle = HandleManager::GetInstance();
	auto& cEffect = EffectManager::GetInstance();
}

/// <summary>
/// �R���X�g���N�^
/// </summary>
SceneTitle::SceneTitle() :
	m_load(0),
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
	m_decisionButton(false),
	m_waitTime(0),
	m_cameraPos(VGet(0.0f, 0.0f, 0.0f)),
	m_playerHandle(0),
	m_anim(0),
	m_animation(0),
	m_totalAnimationTime(0.0f),
	m_playTime(0.0f),
	m_time(0.0f),
	m_pos(VGet(0.0f, 0.0f, 0.0f)),
	m_cameraTarget(VGet(0.0f, 0.0f, 0.0f)),
	m_cameraTargetGraves(VGet(0.0f,0.0f,0.0f)),
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

	cHandle.Clear();
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

	m_playerHandle = cHandle.GetModelHandle("Data/Player/PuppetPlayerModel.mv1");
	m_anim = cHandle.GetModelHandle("Data/PlayerAnimation/JumpingDown.mv1");

	m_pMap->DataInit(0);
	cEffect.Init();

	m_pPhysics = std::make_shared<MyLibrary::Physics>(m_pMap->GetCollisionMap());

	m_pMap->Init(m_pPhysics);

	cSelectDecision = 0;

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

	//�J�����̈ʒu��ݒ�
	m_cameraPos = VGet(-80.0f, 35.0f, 80.0f);
	//�J�����^�[�Q�b�g���v���C���[�Ɍ�����
	m_cameraTarget = VGet(cCameraTargetx, cCameraTargety, cCameraTargetz);
	//�J�����^�[�Q�b�g���Ɍ�����
	m_cameraTargetGraves = VGet(cCameraTargetGravesx, cCameraTargetGravesy, cCameraTargetGravesz);
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
	cCameraEasing = false;
}

/// <summary>
/// �X�V����
/// </summary>
/// <returns>�V�[����Ԃ�</returns>
std::shared_ptr<SceneBase> SceneTitle::Update()
{
	static int previousSelect = -1;   //�O��̑I��

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
		if (cCameraTargety >= cCameraTargetYMax)
		{
			cCameraTrun = false;
		}
		else if (cCameraTargety <= cCameraTargetYMin)
		{
			cCameraTrun = true;
		}

		if (!cCameraTrun)
		{
			cCameraTargety -= cCameraTargetYStep;
		}
		else if (cCameraTrun)
		{
			cCameraTargety += cCameraTargetYStep;
		}

		m_cameraTarget = VGet(cCameraTargetx, cCameraTargety, cCameraTargetz);

		if (m_pSetting->GetSettingScene() == false)
		{
			//�p�b�h���͏���
			GetJoypadXInputState(DX_INPUT_KEY_PAD1, &m_xpad);

			//��
			if (m_xpad.ThumbLY > cThumbLYThreshold || m_xpad.Buttons[0] == 1 && !m_decisionButton)
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

			m_playTime += cPlayTimeStep;

			pselect->Menu_Update(m_button, m_one, m_xpad.Buttons[12], cSelectDecision, pselect->Eight);

			if (pselect->NowSelect == 7)
			{
				cHandY = cHandYSelect7;
			}
			else if (pselect->NowSelect == 8)
			{
				cHandY = cHandYSelect8;
			}
			else if (pselect->NowSelect == 9)
			{
				cHandY = cHandYSelect9;
			}

			if (m_waitTime > cWaitTimeThreshold)
			{
				//A�{�^������������
				if (m_xpad.Buttons[12] == 1 || CheckHitKey(KEY_INPUT_A) == 1)
				{
					if (!m_decisionButton)
					{
						PlaySoundMem(pse->GetButtonSE(), DX_PLAYTYPE_BACK, true);
					}

					//�Q�[���X�^�[�g
					if (cSelectDecision == 8)
					{
						//�{�^�������肵������
						m_decisionButton = true;
					}
					//�ݒ�
					if (cSelectDecision == 9)
					{
						m_waitTime = 0;

						//�҂����Ԃ�����������
						m_pSetting->SetWait(0);

						m_pSetting->SetSettingScene(true);
					}
					//�I��
					if (cSelectDecision == 10)
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
			m_pSetting->Update(*pse);
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
			m_playTime = cPlayTimeMax;
		}
		if (m_animation != -1)
		{
			MV1SetAttachAnimTime(m_playerHandle, m_animation, m_playTime);
		}

		//�I�����ύX���ꂽ�ꍇ�A�C�[�W���O�̐i�s�x�����Z�b�g����
		if (previousSelect != pselect->NowSelect)
		{
			m_time = 0.0f;
			previousSelect = pselect->NowSelect;
		}


		//�ݒ��I�����Ă��鎞�̃^�[�Q�b�g�ʒu�ƃJ�����ʒu
		if (pselect->NowSelect == 8)
		{
			//�C�[�W���O�\�ɂ���
			cCameraEasing = true;

			//�J�����̃^�[�Q�b�g���Ɍ�����
			UpdateCameraPositionAndTarget(cDeltaTime, m_cameraPos, VGet(cCameraPosx, cCameraPosy, cCameraPosz), m_cameraTarget, m_cameraTargetGraves);
		}
		else
		{
			//�C�[�W���O���\�ɂȂ�����ł���悤�ɂ���
			if (cCameraEasing)
			{
				//�v���C���[�Ƀ^�[�Q�b�g��������
				UpdateCameraPositionAndTarget(cDeltaTime, m_cameraPos, VGet(-80.0f, 35.0f, 80.0f), m_cameraTargetGraves, m_cameraTarget);

				if (m_time >= cDuration)
				{
					//�C�[�W���O���I������
					cCameraEasing = false;
				}
			}
		}

		SetCameraPositionAndTarget_UpVecY(m_cameraPos, m_cameraTarget);


		m_pBgm->Update(m_pSetting->GetVolume());
		pse->Update(m_pSetting->GetVolume());
		
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
			if (m_pal[now] < cPalMax)
			{
				m_pal[now] += cPalStep;
			}
			else
			{
				m_blend = true;
			}

		}
		if (m_blend == true)
		{
			if (m_pal[now] > cPalMin)
			{
				m_pal[now] -= cPalStep;
			}
			else
			{
				m_blend = false;
			}
		}


		m_pal[other1] = cPalMax - 1;
		m_pal[other2] = cPalMax - 1;
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
			DrawGraph(cGraphX1, cGraphY1, m_loadNow2, true);
		}


		DrawStringToHandle(cGraphX1, cGraphY1, "NowLoading...", 0xffffff, m_pFont->GetHandle());

		// ���[�h�̐i�s�󋵂��v�Z
		int totalLoadTasks = m_load; // �����[�h�^�X�N�����擾����֐��i����j

		m_load--;

		int remainingLoadTasks = m_load; // �c��̃��[�h�^�X�N�����擾
		float progress = 1.0f - (static_cast<float>(remainingLoadTasks) / static_cast<float>(totalLoadTasks));

		// �o�[�̕`��
		DrawBox(cBarX, cBarY, cBarX + static_cast<int>(cBarWidth * progress), cBarY + cBarHeight, 0x00ff00, TRUE); // �v���O���X�o�[
		DrawBox(cBarX, cBarY, cBarX + cBarWidth, cBarY + cBarHeight, 0xffffff, false);
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

		if (!m_pSetting->GetSettingScene())
		{
			DrawGraph(cGraphX1, cGraphY1, m_backScene, TRUE);
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_pal[0]);
			DrawGraph(cGraphX2, cGraphY2, m_start, TRUE);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_pal[1]);
			DrawGraph(cGraphX2, cGraphY3, m_setting, TRUE);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_pal[2]);
			DrawGraph(cGraphX2, cGraphY4, m_end, TRUE);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

			DrawRotaGraph(cRotaGraphX1, cHandY, cRotaGraphScale, cRotaGraphAngle1, m_hand, true);
			DrawRotaGraph(cRotaGraphX2, cHandY, cRotaGraphScale, cRotaGraphAngle2, m_hand, true, true);
		}

		DrawGraph(cGraphX3, cGraphY5, m_AButton, true);
		DrawStringToHandle(cStringX1, cStringY1, "����", 0xffffff, m_pFont->GetHandle());
		DrawGraph(cGraphX4, cGraphY5, m_BButton, true);
		DrawStringToHandle(cStringX2, cStringY2, "�L�����Z��", 0xffffff, m_pFont->GetHandle());

		//�ݒ��ʂ�`��
		if (m_pSetting->GetSettingScene())
		{
			m_pSetting->Draw();

		}

		m_pSetting->SettingDraw(*pse);

		if (!m_pSetting->GetSettingScene())
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

	cHandle.Clear();
}

//�C�[�W���O�֐�(���`���)
float SceneTitle::Lerp(float start, float end, float t)
{
	return start + t * (end - start);
}

//�C�[�W���O���g�p���ăJ�����̈ʒu�ƃ^�[�Q�b�g�ʒu���X�V
void SceneTitle::UpdateCameraPositionAndTarget(float deltaTime, VECTOR cameraPos, VECTOR cameraPos1, VECTOR cameraTarget, VECTOR cameraTarget1)
{
	//�C�[�W���O�̐i�s�x���X�V
	m_time += deltaTime / cDuration;
	if (m_time > cDuration) m_time = cDuration;

	//�J�����̃^�[�Q�b�g�ʒu���C�[�W���O�ōX�V
	m_cameraTarget.x = Lerp(cameraTarget.x, cameraTarget1.x, m_time);
	m_cameraTarget.y = Lerp(cameraTarget.y, cameraTarget1.y, m_time);
	m_cameraTarget.z = Lerp(cameraTarget.z, cameraTarget1.z, m_time);

	//�J�����̈ʒu���C�[�W���O�ōX�V
	m_cameraPos.x = Lerp(cameraPos.x, cameraPos1.x, m_time);
	m_cameraPos.y = Lerp(cameraPos.y, cameraPos1.y, m_time);
	m_cameraPos.z = Lerp(cameraPos.z, cameraPos1.z, m_time);
}
