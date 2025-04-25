#include "Setting.h"
#include "Manager/SelectManager.h"
#include "UI/UIBase.h"
#include "Item/Shield.h"
#include "Item/Weapon.h"
#include "Item/Armor.h"
#include "Character/Player.h"
#include "Manager/CoreManager.h"
#include "Manager/ItemManager.h"
#include "Manager/SEManager.h"
#include "External/Font.h"

namespace
{
	int selectDecision = 0;     //�I���������̂����肷��ϐ�
	int brightDecision = 0;     //���邳�����肷��ϐ�
	int volumeDecision = 0;     //���ʂ����肷��ϐ�
	int seDecision = 0;         //SE�����肷��ϐ�
	int cameraDecision = 0;     //�J���������肷��ϐ�

	//a�l��������ϐ�
	int cBlenda = 10;  

	//�摜�̔{��
	float cGraphSize = 1.0f;

	//�Ԉ���ĉ����Ȃ��悤�ɂ���
	int cWaitTime = 0;
	//�A���ŉ����Ȃ��悤�ɂ���
	int cPush = 0;
	//���Ԃ����Ə�����悤�ɂ���
	int cTime = 0;
	//������������܂ł̎���
	constexpr int cTimeMax = 80;

	//�Â�
	int cBrightnessBlack = 0;
	float cBrightnessBar = 0.5f;
	int cBrightLine = 800;
	//���邳
	int cBrightnessWhite = 0;
	//����
	int cVolume = 125;
	float cVolumeBar = 0.6f;
	int cVolumeLine = 900;
	//SE����
	int cSe = 150;
	float cSeBar = 0.7f;
	int cSeLine = 1000;
	//�J�����̊��x
	float cCamera = 2.0f;
	float cCameraBar = 0.0f;
	int cCameraLine = 400;

	// �o�[�̕`��
	constexpr int cBarWidth = 900; //�o�[�̕�
	constexpr int cBarHeight = 50; //�o�[�̍���

	//�ύX����ϐ�
	constexpr int cEquipmentOneX = 630;
	constexpr int cEquipmentOneY = 0;
	constexpr int cEquipmentSecondX = 800;
	constexpr int cEquipmentSecondY = 170;

	constexpr int cItemOneX = 800;
	constexpr int cItemOneY = 0;
	constexpr int cItemSecondX = 970;
	constexpr int cItemSecondY = 170;

	constexpr int cRightEquipmentOneX = 107;
	constexpr int cRightEquipmentOneY = 298;
	constexpr int cRightEquipmentSecondX = 276;
	constexpr int cRightEquipmentSecondY = 467;

	constexpr int cLeftEquipmentOneX = 308;
	constexpr int cLeftEquipmentOneY = 298;
	constexpr int cLeftEquipmentSecondX = 477;
	constexpr int cLeftEquipmentSecondY = 467;

	constexpr int cArmorEquipmentOneX = 509;
	constexpr int cArmorEquipmentOneY = 298;
	constexpr int cArmorEquipmentSecondX = 678;
	constexpr int cArmorEquipmentSecondY = 467;

	constexpr int cOneX = 75;
	constexpr int cOneY = 60;
	constexpr int cSecondX = 218;
	constexpr int cSecondY = 200;
	constexpr int cDifferenceY = 155;

	// �萔�̒�`
	constexpr int cWaitTimeThreshold = 10;
	constexpr int cWaitTimePush = 30;
	constexpr int cThumbstickThreshold = 2000;
	constexpr int cButtonA = 12;
	constexpr int cButtonB = 13;
	constexpr int cSelectDecisionBrightness = 6;
	constexpr int cSelectDecisionVolume = 7;
	constexpr int cSelectDecisionSe = 8;
	constexpr int cSelectDecisionCamera = 9;
	constexpr int cSelectDecisionReturn = 10;
	constexpr int cBrightness1 = 125;
	constexpr int cBrightness2 = 100;
	constexpr int cBrightness3 = 75;
	constexpr int cBrightness4 = 50;
	constexpr int cBrightness5 = 25;
	constexpr int cSeVolume1 = 225;
	constexpr int cSeVolume2 = 200;
	constexpr int cSeVolume3 = 175;
	constexpr int cSeVolume4 = 150;
	constexpr int cSeVolume5 = 125;
	constexpr int cSeVolume6 = 100;
	constexpr int cSeVolume7 = 75;
	constexpr int cSeVolume8 = 50;
	constexpr int cSeVolume9 = 25;
	constexpr int cSeVolume10 = 0;
	constexpr float cCamera1 = 4.0f;
	constexpr float cCamera2 = 3.8f;
	constexpr float cCamera3 = 3.6f;
	constexpr float cCamera4 = 3.4f;
	constexpr float cCamera5 = 3.2f;
	constexpr float cCamera6 = 3.0f;
	constexpr float cCamera7 = 2.7f;
	constexpr float cCamera8 = 2.5f;
	constexpr float cCamera9 = 2.3f;
	constexpr float cCamera10 = 2.0f;

	constexpr int cMenuSelectDecisionEquipment = 7;
	constexpr int cMenuSelectDecisionSetting = 8;
	constexpr int cMenuSelectDecisionReturn = 9;
	constexpr int cMenuSelectDecisionTitle = 10;

	constexpr int cEquipmentSelectDecisionRight = 8;
	constexpr int cEquipmentSelectDecisionLeft = 9;
	constexpr int cEquipmentSelectDecisionArmor = 10;

	constexpr int cSelectDecisionRest = 7;
	constexpr int cSelectDecisionNormalRest = 8;
	constexpr int cSelectDecisionLevelUpRest = 8;
	constexpr int cSelectDecisionTransferRest = 9;
	constexpr int cSelectDecisionLeaveRest = 10;

	constexpr int cBlendAlpha = 150;
	constexpr int cSelectColor = 0xffff00;
	constexpr int cDefaultColor = 0xffffff;
	constexpr int cSelectX = 50;
	constexpr int cSelectY1 = -100;
	constexpr int cSelectY2 = 100;
	constexpr int cSelectY3 = 280;
	constexpr int cSelectY4 = 460;
	constexpr int cSelectY5 = 640;
	constexpr int cSelectY6 = 820;
	constexpr int cBarX = 500;
	constexpr int cColorBarX = 500;
	constexpr int cBarY1 = -60;
	constexpr int cBarY2 = 120;
	constexpr int cBarY3 = 300;
	constexpr int cBarY4 = 480;
	constexpr int cColorBarY1 = 185;
	constexpr int cColorBarY2 = 365;
	constexpr int cColorBarY3 = 545;
	constexpr int cColorBarY4 = 725;
	constexpr int cEdgeX1 = 400;
	constexpr int cEdgeX2 = 1300;
	constexpr int cEdgeY1 = 130;
	constexpr int cEdgeY2 = 310;
	constexpr int cEdgeY3 = 490;
	constexpr int cEdgeY4 = 670;
	constexpr int cLineX1 = 550;
	constexpr int cLineX2 = 630;
	constexpr int cLineX3 = 710;
	constexpr int cLineX4 = 800;
	constexpr int cLineX5 = 900;
	constexpr int cLineX6 = 1000;
	constexpr int cLineX7 = 1100;
	constexpr int cLineX8 = 1200;
	constexpr int cArrowY1 = 130;
	constexpr int cArrowY2 = 310;
	constexpr int cArrowY3 = 490;
	constexpr int cArrowY4 = 670;
	constexpr int cArrowY5 = 850;

	constexpr int cBlendAlphaMax = 100;
	constexpr int cBlendAlphaMin = 10;
	constexpr int cMenuDrawX1 = 900;
	constexpr int cMenuDrawY1 = 100;
	constexpr float cMenuDrawScale1 = 0.5f;
	constexpr int cMenuDrawX2 = 935;
	constexpr int cMenuDrawY2 = 0;
	constexpr int cMenuDrawX3 = 1550;
	constexpr int cMenuDrawY3 = 100;
	constexpr int cMenuDrawX4 = 1225;
	constexpr int cMenuDrawY4 = 200;
	constexpr float cMenuDrawScale2 = 0.8f;
	constexpr int cMenuTextX = 1130;
	constexpr int cMenuTextY = 190;
	constexpr int cMenuTextColor = 0xffffff;
	constexpr int cMenuBoxColor = 0x000fff;

	constexpr int cDrawAlpha = 100;
	constexpr int cLevelUpX = 1100;
	constexpr int cLevelUpY = 400;
	constexpr int cStatusX = 1150;
	constexpr int cStatusY1 = 515;
	constexpr int cStatusY2 = 600;
	constexpr int cStatusY3 = 680;
	constexpr int cStatusY4 = 760;
	constexpr int cStatusLabelX = 1300;
	constexpr int cStatusLabelY1 = 515;
	constexpr int cStatusLabelY2 = 615;
	constexpr int cStatusLabelY3 = 715;
	constexpr int cStatusValueY1 = 565;
	constexpr int cStatusValueY2 = 665;
	constexpr int cStatusValueY3 = 765;

	constexpr int cRestSelectX = 300;
	constexpr int cRestSelectY1 = 330;
	constexpr int cRestSelectY2 = 430;
	constexpr int cRestSelectY3 = 530;
	constexpr int cRestSelectY4 = 630;
	constexpr float cSelectUiScale = 0.7f;
	constexpr float cRestScale = 1.5f;
	constexpr int cRestX = 300;
	constexpr int cRestY = 450;
	constexpr int cRestTextX = 720;
	constexpr int cRestTextY = 350;
	constexpr int cRestTitleX = 150;
	constexpr int cRestTitleY = 50;
	constexpr int cRestOptionX = 150;
	constexpr int cRestOptionY1 = 300;
	constexpr int cRestOptionY2 = 400;
	constexpr int cRestOptionY3 = 500;
	constexpr int cRestOptionY4 = 600;
	constexpr int cRestColor = 0x000000;
	constexpr int cRestNotColor = 0xffffff;

	constexpr int cLevelUpGraphX = -50;
	constexpr int cLevelUpGraphY = 0;
	constexpr int cLevelSelectX1 = 310;
	constexpr int cLevelSelectY1 = 540;
	constexpr int cLevelSelectY2 = 635;
	constexpr int cLevelSelectY3 = 730;
	constexpr int cLevelSelectY4 = 825;
	constexpr int cLevelSelectX2 = 825;
	constexpr int cLevelSelectY5 = 940;
	constexpr float cGraphSize1 = 0.6f;
	constexpr float cGraphSize2 = 0.9f;
	constexpr int cLevelTextX = 90;
	constexpr int cLevelTextY1 = 150;
	constexpr int cLevelTextY2 = 250;
	constexpr int cLevelTextY3 = 300;
	constexpr int cStatusTextX = 290;
	constexpr int cStatusTextY1 = 510;
	constexpr int cStatusTextY2 = 605;
	constexpr int cStatusTextY3 = 700;
	constexpr int cStatusTextY4 = 795;
	constexpr int cMaxStatusTextX = 600;
	constexpr int cMaxStatusTextY1 = 200;
	constexpr int cMaxStatusTextY2 = 400;
	constexpr int cMaxStatusTextY3 = 600;
	constexpr int cDecisionTextX = 750;
	constexpr int cDecisionTextY = 900;
	constexpr int cLevelUpTextColor = 0x000000;
	constexpr int cLevelUpTextColor2 = 0xffffff;

	constexpr int cBlueColor = 0x000fff;

	constexpr int cCaveatGraphX = 600;
	constexpr int cCaveatGraphY = 200;
	constexpr int cCaveatTextX = 720;
	constexpr int cCaveatTextY = 350;
	constexpr int cCaveatTextColor = 0xffffff;

	constexpr int cWeaponNameX = 400;
	constexpr int cWeaponNameY = 100;
	constexpr int cWeaponDescX = 400;
	constexpr int cWeaponDescY1 = 300;
	constexpr int cWeaponDescY2 = 450;
	constexpr int cWeaponTextColor = 0xffffff;

	constexpr int cShieldNameX = 400;
	constexpr int cShieldNameY = 100;
	constexpr int cShieldDescX = 400;
	constexpr int cShieldDescY1 = 300;
	constexpr int cShieldDescY2 = 450;
	constexpr int cShieldTextColor = 0xffffff;

	constexpr int cArmorNameX = 400;
	constexpr int cArmorNameY = 100;
	constexpr int cArmorDescX = 400;
	constexpr int cArmorDescY1 = 300;
	constexpr int cArmorDescY2 = 400;
	constexpr int cArmorTextColor = 0xffffff;
}

/// <summary>
/// �R���X�g���N�^
/// </summary>
Setting::Setting() :
	m_black(0),
	m_white(0),
	m_back(0),
	m_selectX(0),
	m_selectY(0),
	m_blackPal(0),
	m_whitePal(0),
	m_button(0),
	m_thumb(0),
	m_waitTime(0),
	m_arrowX(0),
	m_arrowY(0),
	m_right(0),
	m_left(0),
	m_armor(0),
	m_one(false),
	m_settingScene(false),
	m_brightness(false),
	m_volume(false),
	m_se(false),
	m_camera(false),
	m_volumeSize(0),
	m_seSize(0),
	m_cameraSize(0.0f),
	m_brightBar(0.0f),
	m_volumeBar(0.0f),
	m_seBar(0.0f),
	m_cameraBar(0.0f),
	m_equipmentMenu(false),
	m_itemMenu(false),
	m_decisionEquipment(false),
	m_blend(false),
	m_returnMenu(true),
	m_titleMenu(false),
	m_statusLevel(false),
	m_up(false),
	m_menuDecision(false),
	m_restWarp(false),
	m_caveat(false),
	m_reset(false),
	m_change(),
	m_core(0),
	m_brightLineX(0),
	m_volumeLineX(0),
	m_seLineX(0),
	m_cameraLineX(0),
	m_equipmentColorPos(),
	m_selectObject(),
	ms_levelUP(),
	m_xpad()
{

	for (int i = 0; i < 5; i++)
	{
		m_menuSelect[i] = 0;
		m_menuColor[i] = 0;
	}

	m_brightBar = cBrightnessBar;
	m_brightLineX = cBrightLine;
	m_volumeBar = cVolumeBar;
	m_volumeLineX = cVolumeLine;
	m_seBar = cSeBar;
	m_seLineX = cSeLine;
	m_cameraBar = cCameraBar;
	m_cameraLineX = cCameraLine;

	m_pFont = std::make_shared<Font>();
	m_pSmallFont = std::make_shared<Font>();
	m_pBigFont = std::make_shared<Font>();
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Setting::~Setting()
{
	//���������
	DeleteGraph(m_black);
	DeleteGraph(m_back);
	DeleteGraph(m_white);
	DeleteGraph(m_arrowHandle);
}

/// <summary>
/// ����������
/// </summary>
void Setting::Init()
{
	//�摜�ǂݍ���
	m_black = LoadGraph("Data/SceneBack/BlackMini.png");         //14.1 KB (14,532 �o�C�g)
	m_back = MyLoadGraph("Data/SceneBack/BlackMini.png", 1, 1);
	m_white = LoadGraph("Data/SceneBack/WhiteMini.png");         //14.1 KB (14,529 �o�C�g)

	selectDecision = 0;

	m_blackPal = cBrightnessBlack;
	m_whitePal = cBrightnessWhite;
	m_volumeSize = cVolume;
	m_seSize = cSe;
	m_cameraSize = cCamera;

	m_menuSelect[0] = 1;
	m_menuSelect[1] = 0;
	m_menuSelect[2] = 0;

	m_button = 0;
	m_thumb = 0;
	m_one = false;
	m_settingScene = false;
	m_brightness = false;
	m_volume = false;
	m_equipmentMenu = false;
	m_returnMenu = true;
	m_titleMenu = false;
	m_menuDecision = false;

	m_equipment = MyLoadGraph("Data/UI/�V�������UI.png", 1, 1);
	//m_itemBox = MyLoadGraph("Data/UI/�A�C�e�����UI.png", 1, 1);
	m_selectEquipment = MyLoadGraph("Data/UI/�����I�����UI.png", 1, 1);
	m_rest = MyLoadGraph("Data/UI/�l�p�̔��������`�̈͂�.png", 1, 1);
	m_selectUi = MyLoadGraph("Data/UI/�I��UI.png", 1, 1);
	m_levelUp = MyLoadGraph("Data/UI/���x���X�e�[�^�XUI.png", 1, 1);
	m_menu = MyLoadGraph("Data/UI/MenuUI.png", 1, 1);

	m_settingHandle = MyLoadGraph("Data/UI/Setting.png", 1, 1);
	m_brightnessHandle = MyLoadGraph("Data/UI/brightness.png", 1, 1);
	m_bgmHandle = MyLoadGraph("Data/UI/BGM.png", 1, 1);
	m_seHandle = MyLoadGraph("Data/UI/SE.png", 1, 1);
	m_cameraHandle = MyLoadGraph("Data/UI/Camera.png", 1, 1);
	m_returnHandle = MyLoadGraph("Data/UI/Return.png", 1, 1);
	m_edgeHandle = MyLoadGraph("Data/UI/Edge.png", 2, 2);
	m_barHandle = MyLoadGraph("Data/UI/SettingBar.png", 2, 2);
	m_lineHandle = MyLoadGraph("Data/UI/Guideline.png", 2, 2);
	m_arrowHandle = MyLoadGraph("Data/UI/Select.png", 3, 3);

	m_pFont->FontInit(50);
	m_pSmallFont->FontInit(30);
	m_pBigFont->FontInit(80);
}

/// <summary>
/// �X�V����
/// </summary>
void Setting::Update(SEManager& se)
{
	GetJoypadXInputState(DX_INPUT_KEY_PAD1, &m_xpad);

	if (m_waitTime > cWaitTimeThreshold)
	{
		// ��
		if (m_xpad.ThumbLY > cThumbstickThreshold || m_xpad.Buttons[0] == 1)
		{
			m_button++;
		}
		// ��
		else if (m_xpad.ThumbLY < 0 || m_xpad.Buttons[1] == 1)
		{
			m_button--;
		}
		// �E
		else if (m_xpad.ThumbLX > cThumbstickThreshold || m_xpad.Buttons[3] == 1)
		{
			m_thumb--;
		}
		// ��
		else if (m_xpad.ThumbLX < 0 || m_xpad.Buttons[2] == 1)
		{
			m_thumb++;
		}
		else
		{
			// ������
			m_button = 0;
			m_thumb = 0;
			m_one = false;
		}

		// �I��
		if (!m_brightness && !m_volume && !m_se && !m_camera)
		{
			m_pSelect->Menu_Update(m_button, m_one, m_xpad.Buttons[cButtonA], selectDecision, m_pSelect->Six);
		}
		// ���邳�̑I��
		if (m_brightness && !m_volume && !m_se && !m_camera)
		{
			m_pSelect->Menu_Update(m_thumb, m_one, m_xpad.Buttons[cButtonB], brightDecision, m_pSelect->One);
		}
		// ���ʂ̑I��
		if (!m_brightness && m_volume && !m_se && !m_camera)
		{
			m_pSelect->Menu_Update(m_thumb, m_one, m_xpad.Buttons[cButtonB], volumeDecision, m_pSelect->One);
		}
		// SE�̑I��
		if (!m_brightness && !m_volume && m_se && !m_camera)
		{
			m_pSelect->Menu_Update(m_thumb, m_one, m_xpad.Buttons[cButtonB], seDecision, m_pSelect->One);
		}
		// �J�������x�̑I��
		if (!m_brightness && !m_volume && !m_se && m_camera)
		{
			m_pSelect->Menu_Update(m_thumb, m_one, m_xpad.Buttons[cButtonB], cameraDecision, m_pSelect->One);
		}

		if (m_brightness)
		{
			// �Â������߂�
			SettingChange(m_blackPal, cBrightnessBlack, cBrightness1, cBrightness2, cBrightness3, cBrightness4, 0, 0, 0, 0, 0, 0);
			// ���邳�����߂�
			SettingChange(m_whitePal, cBrightnessWhite, 0, 0, 0, 0, 0, cBrightness5, cBrightness4, cBrightness3, cBrightness2, cBrightness1);
		}
		else if (m_volume)
		{
			// ���ʂ����߂�
			SettingChange(m_volumeSize, cVolume, cSeVolume10, cSeVolume9, cSeVolume8, cSeVolume7, cSeVolume6, cSeVolume5, cSeVolume4, cSeVolume3, cSeVolume2, cSeVolume1);
		}
		else if (m_se)
		{
			// SE�����߂�
			SettingChange(m_seSize, cSe, cSeVolume10, cSeVolume9, cSeVolume8, cSeVolume7, cSeVolume6, cSeVolume5, cSeVolume4, cSeVolume3, cSeVolume2, cSeVolume1);
		}
		else if (m_camera)
		{
			// �J�������x�����߂�
			CameraChange(m_cameraSize, cCamera, cCamera10, cCamera9, cCamera8, cCamera7, cCamera6, cCamera5, cCamera4, cCamera3, cCamera2, cCamera1);
		}

		// A�{�^������������
		if (m_xpad.Buttons[cButtonA] == 1 && !m_brightness && !m_volume && !m_se && !m_camera)
		{
			PlaySoundMem(se.GetButtonSE(), DX_PLAYTYPE_BACK, true);

			// ���邳�ݒ�
			if (selectDecision == cSelectDecisionBrightness)
			{
				m_brightness = true;
				// ������
				m_waitTime = 0;
			}
			// BGM�ݒ�
			if (selectDecision == cSelectDecisionVolume)
			{
				m_volume = true;
				// ������
				m_waitTime = 0;
			}
			// SE�ݒ�
			if (selectDecision == cSelectDecisionSe)
			{
				m_se = true;
				// ������
				m_waitTime = 0;
			}
			// �J�������x
			if (selectDecision == cSelectDecisionCamera)
			{
				m_camera = true;
				// ������
				m_waitTime = 0;
			}
			// ���̉�ʂɖ߂�
			if (selectDecision == cSelectDecisionReturn)
			{
				m_settingScene = false;
				// ������
				m_waitTime = 0;
			}
		}

		// ���邳�ݒ����������
		if (m_brightness && m_waitTime != 0)
		{
			// AB�{�^������������
			if (m_xpad.Buttons[cButtonA] == 1 || m_xpad.Buttons[cButtonB] == 1)
			{
				PlaySoundMem(se.GetButtonSE(), DX_PLAYTYPE_BACK, true);
				m_pSelect->NowSelect = 5;
				// ������
				m_waitTime = 0;
				m_brightness = false;
			}
		}
		// ���ʐݒ����������
		else if (m_volume && m_waitTime != 0)
		{
			// B�{�^������������
			if (m_xpad.Buttons[cButtonA] == 1 || m_xpad.Buttons[cButtonB] == 1)
			{
				PlaySoundMem(se.GetButtonSE(), DX_PLAYTYPE_BACK, true);
				m_pSelect->NowSelect = 6;
				// ������
				m_waitTime = 0;
				m_volume = false;
			}
		}
		// SE�ݒ����������
		else if (m_se && m_waitTime != 0)
		{
			// B�{�^������������
			if (m_xpad.Buttons[cButtonA] == 1 || m_xpad.Buttons[cButtonB] == 1)
			{
				PlaySoundMem(se.GetButtonSE(), DX_PLAYTYPE_BACK, true);
				m_pSelect->NowSelect = 7;
				// ������
				m_waitTime = 0;
				m_se = false;
			}
		}
		// �J�������x��ݒ�
		else if (m_camera && m_waitTime != 0)
		{
			// B�{�^������������
			if (m_xpad.Buttons[cButtonA] == 1 || m_xpad.Buttons[cButtonB] == 1)
			{
				PlaySoundMem(se.GetButtonSE(), DX_PLAYTYPE_BACK, true);
				m_pSelect->NowSelect = 8;
				// ������
				m_waitTime = 0;
				m_camera = false;
			}
		}
		// ��ʂɖ߂�
		else
		{
			// B�{�^������������
			if (m_xpad.Buttons[cButtonB] == 1)
			{
				PlaySoundMem(se.GetButtonSE(), DX_PLAYTYPE_BACK, true);
				m_pSelect->NowSelect = 7;
				// ������
				m_waitTime = 0;
				m_settingScene = false;
			}
		}
	}
	else
	{
		m_waitTime++;
	}

	se.Update(m_seSize);
}

/// <summary>
/// ���j���[�X�V����
/// </summary>
void Setting::MenuUpdate(Player& player, SEManager& se)
{
	//�p�b�h���͏���
	GetJoypadXInputState(DX_INPUT_KEY_PAD1, &m_xpad);

	//�E
	if (m_xpad.Buttons[9] == 1 && !m_menuDecision)
	{
		m_button--;
	}
	//��
	else if (m_xpad.Buttons[8] == 1 && !m_menuDecision)
	{
		m_button++;
	}
	else
	{
		//������
		m_button = 0;

		m_one = false;
	}

	

	m_pSelect->Menu_Update(m_button, m_one, m_xpad.Buttons[12], selectDecision, m_pSelect->Seven);

	if (cWaitTime >= cWaitTimeThreshold)
	{
		//A�{�^���������ꂽ��
		if (m_xpad.Buttons[12] == 1)
		{
			PlaySoundMem(se.GetButtonSE(), DX_PLAYTYPE_BACK, true);

			//�����I��
			if (selectDecision == cMenuSelectDecisionEquipment)
			{
				m_equipmentMenu = true;

				//���Z�b�g
				cWaitTime = 0;
			}
			//�ݒ�
			if (selectDecision == cMenuSelectDecisionSetting)
			{
				//����������
				m_waitTime = 0;

				m_settingScene = true;

				//���Z�b�g
				cWaitTime = 0;
			}
			//���̉�ʂɖ߂�
			if (selectDecision == cMenuSelectDecisionReturn)
			{
				m_returnMenu = false;

				//�W�����v�ł��Ȃ��悤�ɂ���
				player.SetAction(false);
			}
			//�^�C�g���ɖ߂�
			if (selectDecision == cMenuSelectDecisionTitle)
			{
				m_titleMenu = true;

				m_menuDecision = true;
			}

			//���Z�b�g
			cWaitTime = 0;
		}
		//B�{�^���������ꂽ��
		else if (m_xpad.Buttons[13] == 1)
		{
			PlaySoundMem(se.GetButtonSE(), DX_PLAYTYPE_BACK, true);

			//���̉�ʂɖ߂�
			m_returnMenu = false;
			//���Z�b�g
			cWaitTime = 0;

			//���[�����O�ł��Ȃ��悤�ɂ���
			player.SetAction(false);

		}
	}
	else
	{
		cWaitTime++;
	}

}

/// <summary>
/// ������ʍX�V����
/// </summary>
void Setting::EquipmentUpdate()
{
	//�p�b�h���͏���
	GetJoypadXInputState(DX_INPUT_KEY_PAD1, &m_xpad);

	m_change.oneX = cEquipmentOneX;
	m_change.oneY = cEquipmentOneY;
	m_change.secondX = cEquipmentSecondX;
	m_change.secondY = cEquipmentSecondY;

	m_select.right = false;
	m_select.left = false;
	m_select.armor = false;

	//�E
	if (m_xpad.Buttons[3] == 1)
	{
		m_thumb--;
	}
	//��
	else if (m_xpad.Buttons[2] == 1)
	{
		m_thumb++;
	}
	else
	{
		//������
		m_thumb = 0;

		m_one = false;
	}

	m_pSelect->Menu_Update(m_thumb, m_one, m_xpad.Buttons[12], selectDecision, m_pSelect->Eight);


	if (cWaitTime >= cWaitTimeThreshold)
	{
		//B�{�^���������ꂽ��
		if (m_xpad.Buttons[13] == 1)
		{
			//������ʂ���߂�
			m_equipmentMenu = false;
			//���Z�b�g
			cWaitTime = 0;
		}


		//A�{�^������������
		if (m_xpad.Buttons[12] == 1)
		{
			//�E�����I��
			//�E�A�C�e���I��
			if (selectDecision == cEquipmentSelectDecisionRight)
			{
				m_select.right = true;
			}
			//�������I��
			//�E�w�֑I��
			//�^�񒆃A�C�e���I��
			else if (selectDecision == cEquipmentSelectDecisionLeft)
			{
				m_select.left = true;
			}
			//�h��I��
			//���w�֑I��
			//���A�C�e���I��
			else if (selectDecision == cEquipmentSelectDecisionArmor)
			{
				m_select.armor = true;
			}

			//�������J��
			m_decisionEquipment = true;

			//���Z�b�g
			cWaitTime = 0;
		}
	}
	else
	{
		cWaitTime++;
	}

}

/// <summary>
/// �x���̍X�V����
/// </summary>
void Setting::RestUpdate(Player& player, CoreManager& core, bool rest, SEManager& se)
{
	//�p�b�h���͏���
	GetJoypadXInputState(DX_INPUT_KEY_PAD1, &m_xpad);

	//��
	if (m_xpad.Buttons[0] == 1)
	{
		m_button++;
	}
	//��
	else if (m_xpad.Buttons[1] == 1)
	{
		m_button--;
	}
	else
	{
		//������
		m_button = 0;

		m_one = false;
	}


	//���ʂ̋x���������ꍇ
	if (!player.GetBigRest())
	{
		m_pSelect->Menu_Update(m_button, m_one, m_xpad.Buttons[12], selectDecision, m_pSelect->Eight);

		if (cWaitTime >= cWaitTimeThreshold)
		{
			//A�{�^���������ꂽ��
			if (m_xpad.Buttons[12] == 1)
			{
				PlaySoundMem(se.GetButtonSE(), DX_PLAYTYPE_BACK, true);

				//�x������
				if (selectDecision == cSelectDecisionNormalRest)
				{
					m_reset = true;
				}
				//�]��
				if (selectDecision == cSelectDecisionTransferRest)
				{
					//�x���n�_�ɓ]������
					if (rest)
					{
						m_restWarp = true;
					}
					//�]���ł��Ȃ�
					else
					{
						m_caveat = true;
					}
				}
				//�x������߂�
				if (selectDecision == cSelectDecisionLeaveRest)
				{
					player.SetNotRest(false);

					//�W�����v�ł��Ȃ��悤�ɂ���
					player.SetAction(false);
				}

				//���Z�b�g
				cWaitTime = 0;
			}

			//B�{�^���������ꂽ��x������߂�
			if (m_xpad.Buttons[13] == 1)
			{
				PlaySoundMem(se.GetButtonSE(), DX_PLAYTYPE_BACK, true);
				//���[�����O�ł��Ȃ��悤�ɂ���
				player.SetAction(false);
				player.SetNotRest(false);
			}
		}
		else
		{
			cWaitTime++;
		}
		
	}
	//���x�����グ����x���������ꍇ
	else
	{
		m_pSelect->Menu_Update(m_button, m_one, m_xpad.Buttons[12], selectDecision, m_pSelect->Seven);

		if (cWaitTime >= cWaitTimeThreshold)
		{
			//A�{�^���������ꂽ��
			if (m_xpad.Buttons[12] == 1)
			{
				PlaySoundMem(se.GetButtonSE(), DX_PLAYTYPE_BACK, true);

				//�x������
				if (selectDecision == cSelectDecisionRest)
				{
					m_reset = true;
				}
				//���x�����グ��
				if (selectDecision == cSelectDecisionLevelUpRest)
				{
					m_statusLevel = true;

					ms_levelUP.sl_hp = player.GetHPLevel();
					ms_levelUP.sl_stamina = player.GetStaminaLevel();
					ms_levelUP.sl_muscle = player.GetMuscleLevel();
					ms_levelUP.sl_skill = player.GetSkillLevel();

					m_core = core.GetCore();

				}
				//�]��
				if (selectDecision == cSelectDecisionTransferRest)
				{
					//�x���n�_�ɓ]������
					if (rest)
					{
						m_restWarp = true;
					}
					//�]���ł��Ȃ�
					else
					{
						m_caveat = true;
					}

				}
				//�x������߂�
				if (selectDecision == cSelectDecisionLeaveRest)
				{
					player.SetNotRest(false);

					//�W�����v�ł��Ȃ��悤�ɂ���
					player.SetAction(false);
				}

				//���Z�b�g
				cWaitTime = 0;
			}

			//B�{�^���������ꂽ��x������߂�
			if (m_xpad.Buttons[13] == 1)
			{
				PlaySoundMem(se.GetButtonSE(), DX_PLAYTYPE_BACK, true);

				//���[�����O�ł��Ȃ��悤�ɂ���
				player.SetAction(false);
				player.SetNotRest(false);
			}

		}
		else
		{
			cWaitTime++;
		}
		
	}

	
}

/// <summary>
/// ���x���A�b�v����
/// </summary>
/// <param name="player"></param>
void Setting::LevelUpdate(Player& player, CoreManager& core)
{

	ms_levelUP.sl_all = ms_levelUP.sl_hp + ms_levelUP.sl_muscle + ms_levelUP.sl_skill + ms_levelUP.sl_stamina;

	//�p�b�h���͏���
	GetJoypadXInputState(DX_INPUT_KEY_PAD1, &m_xpad);

	//��
	if (m_xpad.Buttons[0] == 1)
	{
		m_button++;
	}
	//��
	else if (m_xpad.Buttons[1] == 1)
	{
		m_button--;
	}
	else
	{
		//������
		m_button = 0;

		m_one = false;
	}

	m_pSelect->Menu_Update(m_button, m_one, m_xpad.Buttons[12], selectDecision, m_pSelect->Six);

	//�K�v�R�A��葽�������烌�x�����グ����
	if (core.NeedCore(ms_levelUP.sl_all) <= m_core)
	{
		m_up = true;
	}
	else
	{
		m_up = false;
	}

	if (cWaitTime >= cWaitTimeThreshold)
	{
		//B�{�^���������ꂽ��
		if (m_xpad.Buttons[13] == 1)
		{
			//�X�e�[�^�X���x����ʂ����
			m_statusLevel = false;

			cWaitTime = 0;
		}
		
		LevelUp(core, player.GetHPLevel(), ms_levelUP.sl_hp, m_pSelect->Six);   //HP
		LevelUp(core, player.GetStaminaLevel(), ms_levelUP.sl_stamina, m_pSelect->Seven); //Stamina
		LevelUp(core, player.GetMuscleLevel(), ms_levelUP.sl_muscle, m_pSelect->Eight); //Muscle
		LevelUp(core, player.GetSkillLevel(), ms_levelUP.sl_skill, m_pSelect->Nine);  //Skill
		
		
		//A�{�^���������ꂽ��
		if (m_xpad.Buttons[12] == 1)
		{
			//�グ�����x���ƃR�A�����肷��
			if (selectDecision == 10)
			{
				m_statusLevel = false;

				player.SetHPLevel(ms_levelUP.sl_hp);
				player.SetStaminaLevel(ms_levelUP.sl_stamina);
				player.SetMuscleLevel(ms_levelUP.sl_muscle);
				player.SetSkillLevel(ms_levelUP.sl_skill);

				core.SetCore(m_core);

				cWaitTime = 0;
			}
		}

	}
	else
	{
		cWaitTime++;
	}
}

void Setting::LevelUp(CoreManager& core, int origin, int& level, int now)
{
	if (cPush >= cWaitTimePush)
	{
		if (m_pSelect->NowSelect == now)
		{
			//��(���x����߂�)
			if (m_xpad.Buttons[2] == 1)
			{
				//���x��������������߂���
				if (origin < level)
				{
					m_core += core.NeedCore(ms_levelUP.sl_all - 1);

					level--;
				}

				cPush = 0;
			}
			//�E(���x�����グ��)
			else if (m_xpad.Buttons[3] == 1)
			{
				//���x�����グ�邱�Ƃ��\�Ȃ�
				if (m_up)
				{
					m_core -= core.NeedCore(ms_levelUP.sl_all);

					level++;
				}

				cPush = 0;
			}
		}
	}
	else
	{
		cPush++;
	}
	
}

/// <summary>
/// �A�C�e���{�b�N�X��ʍX�V����
/// </summary>
void Setting::ItemBoxUpdate()
{
	//�p�b�h���͏���
	GetJoypadXInputState(DX_INPUT_KEY_PAD1, &m_xpad);

	m_change.oneX = cItemOneX;
	m_change.oneY = cItemOneY;
	m_change.secondX = cItemSecondX;
	m_change.secondY = cItemSecondY;

	//B�{�^���������ꂽ��
	if (m_xpad.Buttons[13] == 1)
	{
		//������ʂ���߂�
		m_itemMenu = false;
	}
}

/// <summary>
/// �����I����ʍX�V����
/// </summary>
void Setting::EquipmentDecisionUpdate(Weapon& weapon, Shield& shield, Armor& armor, ItemManager& item)
{
	int Right = 0;
	int Left = 0;
	int Armor = 0;

	m_weaponList.clear();
	m_shieldList.clear();
	m_armorList.clear();

	//�p�b�h���͏���
	GetJoypadXInputState(DX_INPUT_KEY_PAD1, &m_xpad);

	//��
	if (m_xpad.Buttons[0] == 1)
	{
		m_button++;
	}
	//��
	else if (m_xpad.Buttons[1] == 1)
	{
		m_button--;
	}
	else
	{
		//������
		m_button = 0;

		m_one = false;
	}

	//�E�����������ꍇ
	if (m_select.right)
	{
		for (const auto& itemName : item.GetItemOrder())
		{
			if (itemName == "BlackSword")
			{
				Right++;

				m_weaponList.push_back("BlackSword");
			}
			else if (itemName == "Bat")
			{
				Right++;

				m_weaponList.push_back("Bat");
			}

		}

		if (Right == 0)
		{
			//���킪0�������ꍇ
			m_pSelect->Menu_Update(m_button, m_one, m_xpad.Buttons[12], selectDecision, m_pSelect->Ten);
		}
		else if (Right == 1)
		{
			//���킪1�������ꍇ
			m_pSelect->Menu_Update(m_button, m_one, m_xpad.Buttons[12], selectDecision, m_pSelect->Nine);
		}
		else if (Right == 2)
		{
			//���킪2�������ꍇ
			m_pSelect->Menu_Update(m_button, m_one, m_xpad.Buttons[12], selectDecision, m_pSelect->Eight);
		}

	}
	//�������������ꍇ
	else if (m_select.left)
	{
		for (const auto& itemName : item.GetItemOrder())
		{
			if (itemName == "Distorted")
			{
				Left++;

				m_shieldList.push_back("Distorted");
			}
			else if (itemName == "WoodShield")
			{
				Left++;

				m_shieldList.push_back("WoodShield");

			}

		}

		if (Left == 0)
		{
			//����0�������ꍇ
			m_pSelect->Menu_Update(m_button, m_one, m_xpad.Buttons[12], selectDecision, m_pSelect->Ten);
		}
		else if (Left == 1)
		{
			//����1�������ꍇ
			m_pSelect->Menu_Update(m_button, m_one, m_xpad.Buttons[12], selectDecision, m_pSelect->Nine);
		}
		else if (Left == 2)
		{
			//����2�������ꍇ
			m_pSelect->Menu_Update(m_button, m_one, m_xpad.Buttons[12], selectDecision, m_pSelect->Eight);
		}
	}
	//�h������ꍇ
	else if (m_select.armor)
	{
		for (const auto& itemName : item.GetItemOrder())
		{
			if (itemName == "ArmorNormal")
			{
				Armor++;

				m_armorList.push_back("ArmorNormal");
			}

		}

		if (Armor == 0)
		{
			//�h�0�������ꍇ
			m_pSelect->Menu_Update(m_button, m_one, m_xpad.Buttons[12], selectDecision, m_pSelect->Ten);
		}
		else if (Armor == 1)
		{
			//�h�1�������ꍇ
			m_pSelect->Menu_Update(m_button, m_one, m_xpad.Buttons[12], selectDecision, m_pSelect->Nine);
		}
	}


	if (cWaitTime >= cWaitTimeThreshold)
	{
		//B�{�^���������ꂽ��
		if (m_xpad.Buttons[13] == 1)
		{
			//������ʂ���߂�
			m_decisionEquipment = false;

			cWaitTime = 0;
		}

		//A�{�^������������
		if (m_xpad.Buttons[12] == 1)
		{
			//�E�����������ꍇ
			if (m_select.right)
			{
				WeaponUpdate(m_weaponList, weapon, m_right);
			}
			//�������������ꍇ
			else if (m_select.left)
			{
				ShieldUpdate(m_shieldList, shield, m_left);
			}
			//�h������ꍇ
			else if (m_select.armor)
			{
				ArmorUpdate(m_armorList, armor, m_armor);
			}

			//�������J��
			m_decisionEquipment = false;

			//���Z�b�g
			cWaitTime = 0;
		}
	}
	else
	{
		cWaitTime++;
	}
}

/// <summary>
/// �`��
/// </summary>
void Setting::Draw()
{
	//�w�i�̐F�𔖂�����
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, cBlendAlpha);
	DrawGraph(0, 0, m_back, false);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//�I�𒆂̐F��ς���
	if (m_brightness == false && m_volume == false && m_se == false && m_camera == false)
	{
		m_arrowX = 0;

		if (m_pSelect->NowSelect == m_pSelect->Six)
		{
			m_arrowY = cArrowY1;
		}
		else if (m_pSelect->NowSelect == m_pSelect->Seven)
		{
			m_arrowY = cArrowY2;

		}
		else if (m_pSelect->NowSelect == m_pSelect->Eight)
		{
			m_arrowY = cArrowY3;

		}
		else if (m_pSelect->NowSelect == m_pSelect->Nine)
		{
			m_arrowY = cArrowY4;

		}
		else if (m_pSelect->NowSelect == m_pSelect->Ten)
		{
			m_arrowY = cArrowY5;

		}
	}
	//���邳��I��
	if (m_brightness == true)
	{
		SettingBarChange(m_pSelect->NowSelect, m_brightBar, cBrightnessBar, cBrightLine);
		m_brightLineX = cBrightLine;
	}

	//���ʂ�I��
	if (m_volume == true)
	{
		SettingBarChange(m_pSelect->NowSelect, m_volumeBar, cVolumeBar, cVolumeLine);
		m_volumeLineX = cVolumeLine;
	}

	//SE��I��
	if (m_se == true)
	{
		SettingBarChange(m_pSelect->NowSelect, m_seBar, cSeBar, cSeLine);
		m_seLineX = cSeLine;
	}

	//�J�������x��I��
	if (m_camera == true)
	{
		SettingBarChange(m_pSelect->NowSelect, m_cameraBar, cCameraBar, cCameraLine);
		m_cameraLineX = cCameraLine;
	}

	//�摜�ɍ����ւ���


	DrawGraph(cSelectX, cSelectY1, m_settingHandle, true);


	DrawGraph(cSelectX, cSelectY2, m_brightnessHandle, true);
	DrawGraph(cSelectX, cSelectY3, m_bgmHandle, true);
	DrawGraph(cSelectX, cSelectY4, m_seHandle, true);
	DrawGraph(cSelectX, cSelectY5, m_cameraHandle, true);
	DrawGraph(cSelectX, cSelectY6, m_returnHandle, true);


	//�o�[���
	DrawGraph(cBarX, cBarY1, m_barHandle, true);
	SettingBarDraw(m_brightBar, cColorBarX, cColorBarY1);
	DrawGraph(cEdgeX1, cEdgeY1, m_edgeHandle, true);
	DrawGraph(cEdgeX2, cEdgeY1, m_edgeHandle, true);
	DrawGraph(m_brightLineX, cEdgeY1, m_lineHandle, true);

	DrawGraph(cBarX, cBarY2, m_barHandle, true);
	SettingBarDraw(m_volumeBar, cColorBarX, cColorBarY2);
	DrawGraph(cEdgeX1, cEdgeY2, m_edgeHandle, true);
	DrawGraph(cEdgeX2, cEdgeY2, m_edgeHandle, true);
	DrawGraph(m_volumeLineX, cEdgeY2, m_lineHandle, true);

	DrawGraph(cBarX, cBarY3, m_barHandle, true);
	SettingBarDraw(m_seBar, cColorBarX, cColorBarY3);
	DrawGraph(cEdgeX1, cEdgeY3, m_edgeHandle, true);
	DrawGraph(cEdgeX2, cEdgeY3, m_edgeHandle, true);
	DrawGraph(m_seLineX, cEdgeY3, m_lineHandle, true);

	DrawGraph(cBarX, cBarY4, m_barHandle, true);
	SettingBarDraw(m_cameraBar, cColorBarX, cColorBarY4);
	DrawGraph(cEdgeX1, cEdgeY4, m_edgeHandle, true);
	DrawGraph(cEdgeX2, cEdgeY4, m_edgeHandle, true);
	DrawGraph(m_cameraLineX, cEdgeY4, m_lineHandle, true);

	DrawGraph(m_arrowX, m_arrowY, m_arrowHandle, true);
	
	m_pSelect->Draw();
}

/// <summary>
/// �ݒ�̕`�揈��
/// </summary>
/// <param name="volume">����</param>
void Setting::SettingDraw(SEManager& se)
{
	se.Update(m_seSize);

	//��ʂ��Â�����
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_blackPal);
	DrawGraph(0, 0, m_black, false);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	//��ʂ𖾂邭����
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_whitePal);
	DrawGraph(0, 0, m_white, false);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

//���j���[�`��
void Setting::MenuDraw(int rb, int lb, int box)
{
	int x1 = 0;
	int y1 = 45;
	int x2 = 0;
	int y2 = 155;
	const char* menu = "";

	// �A���t�@�l��������
	if (!m_blend)
	{
		if (cBlenda < cBlendAlphaMax)
		{
			cBlenda++;
		}
		else
		{
			m_blend = true;
		}
	}
	else if (m_blend)
	{
		if (cBlenda > cBlendAlphaMin)
		{
			cBlenda--;
		}
		else
		{
			m_blend = false;
		}
	}


	if (m_pSelect->NowSelect == m_pSelect->Seven)
	{
		x1 = 970;
		x2 = 1085;

		menu = "�����ƃA�C�e��";
	}
	if (m_pSelect->NowSelect == m_pSelect->Eight)
	{
		x1 = 1100;
		x2 = 1215;

		menu = "�ݒ�ύX";
	}
	if (m_pSelect->NowSelect == m_pSelect->Nine)
	{
		x1 = 1235;
		x2 = 1350;

		menu = "�߂�";
	}
	if (m_pSelect->NowSelect == m_pSelect->Ten)
	{
		x1 = 1365;
		x2 = 1480;

		menu = "�^�C�g���ɖ߂�";
	}
	
	
	DrawRotaGraph(cMenuDrawX1, cMenuDrawY1, cMenuDrawScale1, 0.0f, lb, true);
	DrawGraph(cMenuDrawX2, cMenuDrawY2, m_menu, true);
	DrawRotaGraph(cMenuDrawX3, cMenuDrawY3, cMenuDrawScale1, 0.0f, rb, true);
	DrawRotaGraph(cMenuDrawX4, cMenuDrawY4, cMenuDrawScale2, 0.0f, box, true);

	DrawStringToHandle(cMenuTextX, cMenuTextY, menu, cMenuTextColor, m_pSmallFont->GetHandle());

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, cBlenda);
	DrawBox(x1, y1, x2, y2, cMenuBoxColor, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

//�w�i���Â�����
void Setting::MenuBackDraw()
{
	//��ʂ��Â�����
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, cBlendAlpha);
	DrawGraph(0, 0, m_black, false);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

/// <summary>
/// �������
/// </summary>
void Setting::EquipmentDraw(Player& player)
{
	DrawGraph(0, 0, m_equipment, true);

	if (m_pSelect->NowSelect == m_pSelect->Eight)
	{
		m_equipmentColorPos.oneX = cRightEquipmentOneX;
		m_equipmentColorPos.oneY = cRightEquipmentOneY;
		m_equipmentColorPos.secondX = cRightEquipmentSecondX;
		m_equipmentColorPos.secondY = cRightEquipmentSecondY;
	}
	else if (m_pSelect->NowSelect == m_pSelect->Nine)
	{
		m_equipmentColorPos.oneX = cLeftEquipmentOneX;
		m_equipmentColorPos.oneY = cLeftEquipmentOneY;
		m_equipmentColorPos.secondX = cLeftEquipmentSecondX;
		m_equipmentColorPos.secondY = cLeftEquipmentSecondY;
	}
	else if (m_pSelect->NowSelect == m_pSelect->Ten)
	{
		m_equipmentColorPos.oneX = cArmorEquipmentOneX;
		m_equipmentColorPos.oneY = cArmorEquipmentOneY;
		m_equipmentColorPos.secondX = cArmorEquipmentSecondX;
		m_equipmentColorPos.secondY = cArmorEquipmentSecondY;
	}

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, cDrawAlpha);
	DrawBox(m_equipmentColorPos.oneX, m_equipmentColorPos.oneY, m_equipmentColorPos.secondX, m_equipmentColorPos.secondY, 0x000fff, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	ms_levelUP.sl_all = player.GetLevel();

	ms_levelUP.sl_hp = player.GetHPLevel();
	ms_levelUP.sl_stamina = player.GetStaminaLevel();
	ms_levelUP.sl_muscle = player.GetMuscleLevel();
	ms_levelUP.sl_skill = player.GetSkillLevel();

	DrawFormatStringToHandle(cLevelUpX, cLevelUpY, 0xffffff, m_pFont->GetHandle(), "���x���@�@�@%d", ms_levelUP.sl_all);

	DrawFormatStringToHandle(cStatusX, cStatusY1, 0xffffff, m_pFont->GetHandle(), "%d", ms_levelUP.sl_hp);
	DrawFormatStringToHandle(cStatusX, cStatusY2, 0xffffff, m_pFont->GetHandle(), "%d", ms_levelUP.sl_stamina);
	DrawFormatStringToHandle(cStatusX, cStatusY3, 0xffffff, m_pFont->GetHandle(), "%d", ms_levelUP.sl_muscle);
	DrawFormatStringToHandle(cStatusX, cStatusY4, 0xffffff, m_pFont->GetHandle(), "%d", ms_levelUP.sl_skill);

	DrawStringToHandle(cStatusLabelX, cStatusLabelY1, "������", 0xffffff, m_pFont->GetHandle());
	DrawFormatStringToHandle(cStatusLabelX, cStatusValueY1, 0xffffff, m_pFont->GetHandle(), "%d", static_cast<int>(player.GetStatus().s_hp));
	DrawStringToHandle(cStatusLabelX, cStatusLabelY2, "�̗�", 0xffffff, m_pFont->GetHandle());
	DrawFormatStringToHandle(cStatusLabelX, cStatusValueY2, 0xffffff, m_pFont->GetHandle(), "%d", static_cast<int>(player.GetStatus().s_stamina));
	DrawStringToHandle(cStatusLabelX, cStatusLabelY3, "�U����", 0xffffff, m_pFont->GetHandle());
	DrawFormatStringToHandle(cStatusLabelX, cStatusValueY3, 0xffffff, m_pFont->GetHandle(), "%d", static_cast<int>(player.GetAttackDamage() + ((ms_levelUP.sl_muscle - 1) * player.GetAttackMuscle()) + ((ms_levelUP.sl_skill - 1) * player.GetAttackSkill())));
}

/// <summary>
/// �x�����
/// </summary>
void Setting::RestDraw(bool rest)
{
	DrawRotaGraph(cRestX, cRestY, cRestScale, DX_PI_F / 2, m_rest, true);

	m_selectX = cRestSelectX;

	DrawRotaGraph(m_selectX, m_selectY, cSelectUiScale, 0.0f, m_selectUi, true);

	//���ʂ̋x��
	if (!rest)
	{
		if (m_pSelect->NowSelect == m_pSelect->Eight)
		{
			m_selectY = cRestSelectY1;

			m_menuColor[0] = cRestColor;
			m_menuColor[1] = cRestNotColor;
			m_menuColor[2] = cRestNotColor;
		}
		else if (m_pSelect->NowSelect == m_pSelect->Nine)
		{
			m_selectY = cRestSelectY2;

			m_menuColor[0] = cRestNotColor;
			m_menuColor[1] = cRestColor;
			m_menuColor[2] = cRestNotColor;
		}
		else if (m_pSelect->NowSelect == m_pSelect->Ten)
		{
			m_selectY = cRestSelectY3;

			m_menuColor[0] = cRestNotColor;
			m_menuColor[1] = cRestNotColor;
			m_menuColor[2] = cRestColor;
		}

		DrawStringToHandle(cRestTitleX, cRestTitleY, "����", 0xffffff, m_pBigFont->GetHandle());

		DrawStringToHandle(cRestOptionX, cRestOptionY1, "�x��", m_menuColor[0], m_pFont->GetHandle());
		DrawStringToHandle(cRestOptionX, cRestOptionY2, "����֓]��", m_menuColor[1], m_pFont->GetHandle());
		DrawStringToHandle(cRestOptionX, cRestOptionY3, "��������", m_menuColor[2], m_pFont->GetHandle());
	}
	//���x���グ����x��
	else
	{
		if (m_pSelect->NowSelect == m_pSelect->Seven)
		{
			m_selectY = cRestSelectY1;

			m_menuColor[0] = cRestColor;
			m_menuColor[1] = cRestNotColor;
			m_menuColor[2] = cRestNotColor;
			m_menuColor[3] = cRestNotColor;
		}
		else if (m_pSelect->NowSelect == m_pSelect->Eight)
		{
			m_selectY = cRestSelectY2;

			m_menuColor[0] = cRestNotColor;
			m_menuColor[1] = cRestColor;
			m_menuColor[2] = cRestNotColor;
			m_menuColor[3] = cRestNotColor;
		}
		else if (m_pSelect->NowSelect == m_pSelect->Nine)
		{
			m_selectY = cRestSelectY3;

			m_menuColor[0] = cRestNotColor;
			m_menuColor[1] = cRestNotColor;
			m_menuColor[2] = cRestColor;
			m_menuColor[3] = cRestNotColor;

		}
		else if (m_pSelect->NowSelect == m_pSelect->Ten)
		{
			m_selectY = cRestSelectY4;

			m_menuColor[0] = cRestNotColor;
			m_menuColor[1] = cRestNotColor;
			m_menuColor[2] = cRestNotColor;
			m_menuColor[3] = cRestColor;
		}

		DrawStringToHandle(cRestTitleX, cRestTitleY, "����", 0xffffff, m_pBigFont->GetHandle());

		DrawStringToHandle(cRestOptionX, cRestOptionY1, "�x��", m_menuColor[0], m_pFont->GetHandle());
		DrawStringToHandle(cRestOptionX, cRestOptionY2, "���x���グ", m_menuColor[1], m_pFont->GetHandle());
		DrawStringToHandle(cRestOptionX, cRestOptionY3, "����֓]��", m_menuColor[2], m_pFont->GetHandle());
		DrawStringToHandle(cRestOptionX, cRestOptionY4, "��������", m_menuColor[3], m_pFont->GetHandle());
	}
}

/// <summary>
/// ���x���A�b�v�`��
/// </summary>
/// <param name="player"></param>
void Setting::LevelUpDraw(Player& player, CoreManager& core)
{

	DrawGraph(cLevelUpGraphX, cLevelUpGraphY, m_levelUp, true);
	DrawRotaGraph(m_selectX, m_selectY, cGraphSize, DX_PI_F, m_selectUi, true);

	if (m_pSelect->NowSelect == m_pSelect->Six)
	{
		m_selectX = cLevelSelectX1;
		m_selectY = cLevelSelectY1;
		cGraphSize = cGraphSize1;

		m_menuColor[0] = cLevelUpTextColor;
		m_menuColor[1] = cLevelUpTextColor2;
		m_menuColor[2] = cLevelUpTextColor2;
		m_menuColor[3] = cLevelUpTextColor2;
		m_menuColor[4] = cLevelUpTextColor2;
	}
	else if (m_pSelect->NowSelect == m_pSelect->Seven)
	{
		m_selectX = cLevelSelectX1;
		m_selectY = cLevelSelectY2;
		cGraphSize = cGraphSize1;

		m_menuColor[0] = cLevelUpTextColor2;
		m_menuColor[1] = cLevelUpTextColor;
		m_menuColor[2] = cLevelUpTextColor2;
		m_menuColor[3] = cLevelUpTextColor2;
		m_menuColor[4] = cLevelUpTextColor2;
	}
	else if (m_pSelect->NowSelect == m_pSelect->Eight)
	{
		m_selectX = cLevelSelectX1;
		m_selectY = cLevelSelectY3;
		cGraphSize = cGraphSize1;

		m_menuColor[0] = cLevelUpTextColor2;
		m_menuColor[1] = cLevelUpTextColor2;
		m_menuColor[2] = cLevelUpTextColor;
		m_menuColor[3] = cLevelUpTextColor2;
		m_menuColor[4] = cLevelUpTextColor2;
	}
	else if (m_pSelect->NowSelect == m_pSelect->Nine)
	{
		m_selectX = cLevelSelectX1;
		m_selectY = cLevelSelectY4;
		cGraphSize = cGraphSize1;

		m_menuColor[0] = cLevelUpTextColor2;
		m_menuColor[1] = cLevelUpTextColor2;
		m_menuColor[2] = cLevelUpTextColor2;
		m_menuColor[3] = cLevelUpTextColor;
		m_menuColor[4] = cLevelUpTextColor2;
	}
	else if (m_pSelect->NowSelect == m_pSelect->Ten)
	{
		m_selectX = cLevelSelectX2;
		m_selectY = cLevelSelectY5;
		cGraphSize = cGraphSize2;

		m_menuColor[0] = cLevelUpTextColor2;
		m_menuColor[1] = cLevelUpTextColor2;
		m_menuColor[2] = cLevelUpTextColor2;
		m_menuColor[3] = cLevelUpTextColor2;
		m_menuColor[4] = cLevelUpTextColor;
	}

	DrawFormatStringToHandle(cLevelTextX, cLevelTextY1, cLevelUpTextColor2, m_pFont->GetHandle(), "���x��      %d", ms_levelUP.sl_all);
	DrawFormatStringToHandle(cLevelTextX, cLevelTextY2, cLevelUpTextColor2, m_pFont->GetHandle(), "�����R�A  %d", m_core);
	DrawFormatStringToHandle(cLevelTextX, cLevelTextY3, cLevelUpTextColor2, m_pFont->GetHandle(), "�K�v�R�A  %d", core.NeedCore(ms_levelUP.sl_all));



	//���̕ϐ������x���グ��O�̃��x���ŉE�̕ϐ������x�����グ����̃��x��
	DrawFormatStringToHandle(cStatusTextX, cStatusTextY1, m_menuColor[0], m_pFont->GetHandle(), "%d", ms_levelUP.sl_hp);
	DrawFormatStringToHandle(cStatusTextX, cStatusTextY2, m_menuColor[1], m_pFont->GetHandle(), "%d", ms_levelUP.sl_stamina);
	DrawFormatStringToHandle(cStatusTextX, cStatusTextY3, m_menuColor[2], m_pFont->GetHandle(), "%d", ms_levelUP.sl_muscle);
	DrawFormatStringToHandle(cStatusTextX, cStatusTextY4, m_menuColor[3], m_pFont->GetHandle(), "%d", ms_levelUP.sl_skill);

	//���x�����グ�����̃X�e�[�^�X�ϓ�
	DrawFormatStringToHandle(cMaxStatusTextX, cMaxStatusTextY1, cLevelUpTextColor2, m_pFont->GetHandle(), "�ő吶���� :   %d", static_cast<int>(player.GetStatus().s_hp + ((ms_levelUP.sl_hp - 1) * 30.0f)));
	DrawFormatStringToHandle(cMaxStatusTextX, cMaxStatusTextY2, cLevelUpTextColor2, m_pFont->GetHandle(), "�ő�̗�   :   %d", static_cast<int>(player.GetStatus().s_stamina + ((ms_levelUP.sl_stamina - 1) * 10.0f)));
	DrawFormatStringToHandle(cMaxStatusTextX, cMaxStatusTextY3, cLevelUpTextColor2, m_pFont->GetHandle(), "�U����     :   %d", static_cast<int>(player.GetAttackDamage() + ((ms_levelUP.sl_muscle - 1) * player.GetAttackMuscle()) + ((ms_levelUP.sl_skill - 1) * player.GetAttackSkill())));

	DrawStringToHandle(cDecisionTextX, cDecisionTextY, "����", m_menuColor[4], m_pBigFont->GetHandle());

}

/// <summary>
/// �A�C�e�����
/// </summary>
//void Setting::ItemBoxDraw()
//{
//	DrawGraph(0, 0, m_itemBox, true);
//}

/// <summary>
/// �����I�����
/// </summary>
void Setting::EquipmentDecisionDraw(ItemManager& item)
{
	int Right = 0;
	int Left = 0;
	int Armor = 0;

	DrawGraph(0, 0, m_selectEquipment, true);

	//�E�����������ꍇ
	if (m_select.right)
	{
		//�A�C�e���擾��
		for (const auto& itemName : item.GetItemOrder())
		{
			if (itemName == "BlackSword")
			{
				Right++;
			}
			else if (itemName == "Bat")
			{
				Right++;
			}
		}

		if (Right == 0)
		{
			if (m_pSelect->NowSelect == m_pSelect->Ten)
			{
				m_selectObject.oneX = cOneX;
				m_selectObject.oneY = cOneY;
				m_selectObject.secondX = cSecondX;
				m_selectObject.secondY = cSecondY;

				//����I�����Ă��邩�̕ϐ�
				m_right = 0;
			}
		}
		else if (Right == 1)
		{
			if (m_pSelect->NowSelect == m_pSelect->Nine)
			{
				m_selectObject.oneX = cOneX;
				m_selectObject.oneY = cOneY;
				m_selectObject.secondX = cSecondX;
				m_selectObject.secondY = cSecondY;

				//����I�����Ă��邩�̕ϐ�
				m_right = 0;
			}
			else if (m_pSelect->NowSelect == m_pSelect->Ten)
			{
				m_selectObject.oneY = cOneY + cDifferenceY;
				m_selectObject.secondY = cSecondY + cDifferenceY;

				//����I�����Ă��邩�̕ϐ�
				m_right = 1;
			}
		}
		else if (Right == 2)
		{
			if (m_pSelect->NowSelect == m_pSelect->Eight)
			{
				m_selectObject.oneX = cOneX;
				m_selectObject.oneY = cOneY;
				m_selectObject.secondX = cSecondX;
				m_selectObject.secondY = cSecondY;

				//����I�����Ă��邩�̕ϐ�
				m_right = 0;
			}
			else if (m_pSelect->NowSelect == m_pSelect->Nine)
			{
				m_selectObject.oneY = cOneY + cDifferenceY;
				m_selectObject.secondY = cSecondY + cDifferenceY;

				//����I�����Ă��邩�̕ϐ�
				m_right = 1;
			}
			else if (m_pSelect->NowSelect == m_pSelect->Ten)
			{
				m_selectObject.oneY = cOneY + (cDifferenceY * 2);
				m_selectObject.secondY = cSecondY + (cDifferenceY * 2);

				//����I�����Ă��邩�̕ϐ�
				m_right = 2;
			}
		}

		//����̐����`��
		WeaponDraw(m_weaponList, m_right);
	}
	//�������������ꍇ
	else if (m_select.left)
	{
		//�A�C�e���擾��
		for (const auto& itemName : item.GetItemOrder())
		{
			if (itemName == "Distorted")
			{
				Left++;
			}
			else if (itemName == "WoodShield")
			{
				Left++;
			}
		}

		if (Left == 0)
		{
			if (m_pSelect->NowSelect == m_pSelect->Ten)
			{
				m_selectObject.oneX = cOneX;
				m_selectObject.oneY = cOneY;
				m_selectObject.secondX = cSecondX;
				m_selectObject.secondY = cSecondY;

				//����I�����Ă��邩�̕ϐ�
				m_left = 0;
			}
		}
		else if (Left == 1)
		{
			if (m_pSelect->NowSelect == m_pSelect->Nine)
			{
				m_selectObject.oneX = cOneX;
				m_selectObject.oneY = cOneY;
				m_selectObject.secondX = cSecondX;
				m_selectObject.secondY = cSecondY;

				//����I�����Ă��邩�̕ϐ�
				m_left = 0;
			}
			else if (m_pSelect->NowSelect == m_pSelect->Ten)
			{
				m_selectObject.oneY = cOneY + cDifferenceY;
				m_selectObject.secondY = cSecondY + cDifferenceY;

				//����I�����Ă��邩�̕ϐ�
				m_left = 1;
			}
		}
		else if (Left == 2)
		{
			if (m_pSelect->NowSelect == m_pSelect->Eight)
			{
				m_selectObject.oneX = cOneX;
				m_selectObject.oneY = cOneY;
				m_selectObject.secondX = cSecondX;
				m_selectObject.secondY = cSecondY;

				//����I�����Ă��邩�̕ϐ�
				m_left = 0;
			}
			else if (m_pSelect->NowSelect == m_pSelect->Nine)
			{
				m_selectObject.oneY = cOneY + cDifferenceY;
				m_selectObject.secondY = cSecondY + cDifferenceY;

				//����I�����Ă��邩�̕ϐ�
				m_left = 1;
			}
			else if (m_pSelect->NowSelect == m_pSelect->Ten)
			{
				m_selectObject.oneY = cOneY + (cDifferenceY * 2);
				m_selectObject.secondY = cSecondY + (cDifferenceY * 2);

				//����I�����Ă��邩�̕ϐ�
				m_left = 2;
			}
		}

		//���̐����`��
		ShieldDraw(m_shieldList, m_left);
	}
	//�h������ꍇ
	else if (m_select.armor)
	{
		//�A�C�e���擾��
		for (const auto& itemName : item.GetItemOrder())
		{
			if (itemName == "ArmorNormal")
			{
				Armor++;
			}
		}

		if (Armor == 0)
		{
			if (m_pSelect->NowSelect == m_pSelect->Ten)
			{
				m_selectObject.oneX = cOneX;
				m_selectObject.oneY = cOneY;
				m_selectObject.secondX = cSecondX;
				m_selectObject.secondY = cSecondY;

				//����I�����Ă��邩���f���邽�߂̕ϐ�
				m_armor = 0;
			}
		}
		else if (Armor == 1)
		{
			if (m_pSelect->NowSelect == m_pSelect->Nine)
			{
				m_selectObject.oneX = cOneX;
				m_selectObject.oneY = cOneY;
				m_selectObject.secondX = cSecondX;
				m_selectObject.secondY = cSecondY;

				//����I�����Ă��邩���f���邽�߂̕ϐ�
				m_armor = 0;
			}
			else if (m_pSelect->NowSelect == m_pSelect->Ten)
			{
				m_selectObject.oneY = cOneY + cDifferenceY;
				m_selectObject.secondY = cSecondY + cDifferenceY;

				//����I�����Ă��邩���f���邽�߂̕ϐ�
				m_armor = 1;
			}
		}

		//�h��̐����`��
		ArmorDraw(m_armorList, m_armor);
	}

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, cDrawAlpha);
	DrawBox(m_selectObject.oneX, m_selectObject.oneY, m_selectObject.secondX, m_selectObject.secondY, cBlueColor, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

/// <summary>
/// ���[�v�ł��Ȃ����̕`�揈��
/// </summary>
void Setting::CaveatDraw()
{
	if (m_caveat)
	{
		cTime++;
		//���Ԓ����ӏ�����\������
		if (cTime <= cTimeMax)
		{
			DrawGraph(cCaveatGraphX, cCaveatGraphY, m_rest, true);
			DrawStringToHandle(cCaveatTextX, cCaveatTextY, "�]���ł��Ȃ�", cCaveatTextColor, m_pFont->GetHandle());
		}
		else
		{
			m_caveat = false;
		}
	}
	else
	{
		//����������
		cTime = 0;
	}
}

/// <summary>
/// �I������
/// </summary>
void Setting::End()
{
	//���������
	DeleteGraph(m_black);
	DeleteGraph(m_back);
	DeleteGraph(m_white);
	DeleteGraph(m_equipment);
	//DeleteGraph(m_itemBox);
	DeleteGraph(m_selectEquipment);
	DeleteGraph(m_rest);
	DeleteGraph(m_selectUi);
	DeleteGraph(m_levelUp);
	DeleteGraph(m_settingHandle);
	DeleteGraph(m_brightnessHandle);
	DeleteGraph(m_bgmHandle);
	DeleteGraph(m_seHandle);
	DeleteGraph(m_cameraHandle);
	DeleteGraph(m_returnHandle);
	DeleteGraph(m_edgeHandle);
	DeleteGraph(m_barHandle);
	DeleteGraph(m_lineHandle);
	DeleteGraph(m_arrowHandle);
}

//�ݒ�֐�
void Setting::SettingChange(int& setting, int& cSetting, int one, int two, int three, int four, int five, int six, int seven, int eight, int nine, int ten)
{
	if (m_pSelect->NowSelect == 0)
	{
		setting = one;
		cSetting = one;
	}
	else if (m_pSelect->NowSelect == 1)
	{
		setting = two;
		cSetting = two;
	}
	else if (m_pSelect->NowSelect == 2)
	{
		setting = three;
		cSetting = three;
	}
	else if (m_pSelect->NowSelect == 3)
	{
		setting = four;
		cSetting = four;
	}
	else if (m_pSelect->NowSelect == 4)
	{
		setting = five;
		cSetting = five;
	}
	else if (m_pSelect->NowSelect == 5)
	{
		setting = six;
		cSetting = six;
	}
	else if (m_pSelect->NowSelect == 6)
	{
		setting = seven;
		cSetting = seven;
	}
	else if (m_pSelect->NowSelect == 7)
	{
		setting = eight;
		cSetting = eight;
	}
	else if (m_pSelect->NowSelect == 8)
	{
		setting = nine;
		cSetting = nine;
	}
	else if (m_pSelect->NowSelect == 9)
	{
		setting = ten;
		cSetting = ten;
	}
}

//�J�������x�̐ݒ�
void Setting::CameraChange(float& setting, float& cSetting, float one, float two, float three, float four, float five, float six, float seven, float eight, float nine, float ten)
{
	if (m_pSelect->NowSelect == 0)
	{
		setting = one;
		cSetting = one;
	}
	else if (m_pSelect->NowSelect == 1)
	{
		setting = two;
		cSetting = two;
	}
	else if (m_pSelect->NowSelect == 2)
	{
		setting = three;
		cSetting = three;
	}
	else if (m_pSelect->NowSelect == 3)
	{
		setting = four;
		cSetting = four;
	}
	else if (m_pSelect->NowSelect == 4)
	{
		setting = five;
		cSetting = five;
	}
	else if (m_pSelect->NowSelect == 5)
	{
		setting = six;
		cSetting = six;
	}
	else if (m_pSelect->NowSelect == 6)
	{
		setting = seven;
		cSetting = seven;
	}
	else if (m_pSelect->NowSelect == 7)
	{
		setting = eight;
		cSetting = eight;
	}
	else if (m_pSelect->NowSelect == 8)
	{
		setting = nine;
		cSetting = nine;
	}
	else if (m_pSelect->NowSelect == 9)
	{
		setting = ten;
		cSetting = ten;
	}
}

//�ݒ�̃o�[�̕`�揈��
void Setting::SettingBarChange(int Decision, float& bar, float& cBar, int& lineX)
{
	if (select)
	{
		if (Decision == 0)
		{
			bar = 0.0f;
			cBar = 0.0f;
			lineX = cEdgeX1;
		}
		else if (Decision == 1)
		{
			bar = 0.2f;
			cBar = 0.2f;
			lineX = cLineX1;
		}
		else if (Decision == 2)
		{
			bar = 0.3f;
			cBar = 0.3f;
			lineX = cLineX2;

		}
		else if (Decision == 3)
		{
			bar = 0.4f;
			cBar = 0.4f;
			lineX = cLineX3;

		}
		else if (Decision == 4)
		{
			bar = 0.5f;
			cBar = 0.5f;
			lineX = cLineX4;

		}
		else if (Decision == 5)
		{
			bar = 0.6f;
			cBar = 0.6f;
			lineX = cLineX5;

		}
		else if (Decision == 6)
		{
			bar = 0.7f;
			cBar = 0.7f;
			lineX = cLineX6;

		}
		else if (Decision == 7)
		{
			bar = 0.8f;
			cBar = 0.8f;
			lineX = cLineX7;

		}
		else if (Decision == 8)
		{
			bar = 0.9f;
			cBar = 0.9f;
			lineX = cLineX8;

		}
		else if (Decision == 9)
		{
			bar = 1.0f;
			cBar = 1.0f;
			lineX = cEdgeX2;
		}
	}

	
}

void Setting::SettingBarDraw(float bar, int x, int y)
{
	DrawBox(x, y, x + static_cast<int>(cBarWidth * bar), y + cBarHeight, 0x00ff00, TRUE); // �v���O���X�o�[
}

void Setting::WeaponUpdate(std::list<std::string> list, Weapon& weapon, int right)
{
	int select = 1;

	//�f��I��
	if (right == 0)
	{
		weapon.End();

		weapon.SetFist(true);
		weapon.SetBlack(false);
		weapon.SetBat(false);

		weapon.Init();

		m_pSelect->NowSelect = 7;
	}
	//����I��
	else if (right >= 1)
	{
		for (const auto& item : list)
		{
			if (right == select)
			{
				//�������I��
				if (item == "BlackSword")
				{
					weapon.End();

					weapon.SetBlack(true);
					weapon.SetFist(false);
					weapon.SetBat(false);

					weapon.Init();

					m_pSelect->NowSelect = 7;
				}
				//�؂̃o�b�g�I��
				else if (item == "Bat")
				{
					weapon.End();

					weapon.SetBat(true);
					weapon.SetBlack(false);
					weapon.SetFist(false);

					weapon.Init();

					m_pSelect->NowSelect = 7;
				}
			}

			select++;
		}
	}
}

/// <summary>
/// ����̐������`��
/// </summary>
/// <param name="list"></param>
/// <param name="right"></param>
void Setting::WeaponDraw(std::list<std::string> list, int right)
{
	int select = 1;

	//�f��I��
	if (right == 0)
	{
		DrawStringToHandle(cWeaponNameX, cWeaponNameY, "�f��", cWeaponTextColor, m_pBigFont->GetHandle());
		DrawStringToHandle(cWeaponDescX, cWeaponDescY1, "�M����ׂ��͌Ȃ̗͂̂�", cWeaponTextColor, m_pFont->GetHandle());
	}
	//����I��
	else if (right >= 1)
	{
		for (const auto& item : list)
		{
			if (right == select)
			{
				//�������I��
				if (item == "BlackSword")
				{
					DrawStringToHandle(cWeaponNameX, cWeaponNameY, "����", cWeaponTextColor, m_pBigFont->GetHandle());
					DrawStringToHandle(cWeaponDescX, cWeaponDescY1, "���̎����̐n�́A�܂�Ŗ邻�̂��̂�\n�b���グ�����̂悤�Ɍ����z�����݁A\n���ق̒��ɗ₽���P�����h��", cWeaponTextColor, m_pFont->GetHandle());
					DrawStringToHandle(cWeaponDescX, cWeaponDescY2, "���̌���U�邤�҂́A\n�n�ɔ�߂�ꂽ�[����`�����ނ��ƂƂȂ�\n�����Ȃ��҂ɂ͒����Ȑn�ƂȂ�A\n�S�h�炮�҂ɂ͔j�ł̉e�𗎂Ƃ�", cWeaponTextColor, m_pFont->GetHandle());
				}
				//�؂̃o�b�g�I��
				if (item == "Bat")
				{
					DrawStringToHandle(cWeaponNameX, cWeaponNameY, "�؂̞��_", cWeaponTextColor, m_pBigFont->GetHandle());
					DrawStringToHandle(cWeaponDescX, cWeaponDescY1, "�l�`�̈ꕔ�����o�������_\n������̐킢��ς��������ؔ���\n�|�ɂ����ʋ��x���ւ�", cWeaponTextColor, m_pFont->GetHandle());
					DrawStringToHandle(cWeaponDescX, cWeaponDescY2, "���Ă̐l�`�̋L�����h���A\n������̎�ɓ���ނ��̊��G�́A\n�s�v�c�Ȉ��S������^����", cWeaponTextColor, m_pFont->GetHandle());
				}
			}
			select++;
		}
	}
}

void Setting::ShieldUpdate(std::list<std::string> list, Shield& shield, int left)
{
	int select = 1;

	//�f��I��
	if (left == 0)
	{
		shield.End();

		shield.SetFist(true);
		shield.SetUgly(false);
		shield.SetWood(false);

		shield.Init();

		m_pSelect->NowSelect = 8;
	}
	//���I��
	else if (left >= 1)
	{
		for (const auto& item : list)
		{
			if (left == select)
			{
				//�X�����I��
				if (item == "Distorted")
				{
					shield.End();

					shield.SetUgly(true);
					shield.SetFist(false);
					shield.SetWood(false);

					shield.Init();

					m_pSelect->NowSelect = 8;

				}
				//�؂̏��I��
				else if (item == "WoodShield")
				{
					shield.End();

					shield.SetWood(true);
					shield.SetUgly(false);
					shield.SetFist(false);

					shield.Init();

					m_pSelect->NowSelect = 8;
				}
			}

			select++;
		}
	}
}

/// <summary>
/// ���̐����`��
/// </summary>
/// <param name="list"></param>
/// <param name="left"></param>
void Setting::ShieldDraw(std::list<std::string> list, int left)
{
	int select = 1;

	//�f��I��
	if (left == 0)
	{
		DrawStringToHandle(cShieldNameX, cShieldNameY, "�f��", cShieldTextColor, m_pBigFont->GetHandle());
		DrawStringToHandle(cShieldDescX, cShieldDescY1, "�M����ׂ��͌Ȃ̗͂̂�", cShieldTextColor, m_pFont->GetHandle());
	}
	//���I��
	else if (left >= 1)
	{
		for (const auto& item : list)
		{
			if (left == select)
			{
				//�c�񂾏��I��
				if (item == "Distorted")
				{
					DrawStringToHandle(cShieldNameX, cShieldNameY, "����", cShieldTextColor, m_pBigFont->GetHandle());
					DrawStringToHandle(cShieldDescX, cShieldDescY1, "�_�ւ̔��t�����݂��҂��������A���ꂵ��\n�G���҂̐S�ɔ����Ț����𗎂Ƃ��Ƃ���\n�n�������e�����A������̍���I��", cShieldTextColor, m_pFont->GetHandle());
					DrawStringToHandle(cShieldDescX, cShieldDescY2, "�I�Ԃ̂Ȃ�o�傹�悱�̏��́A\n���҂����̂ł͂Ȃ��A�����̂�����", cShieldTextColor, m_pFont->GetHandle());
				}
				//�؂̏��I��
				if (item == "WoodShield")
				{
					DrawStringToHandle(cShieldNameX, cShieldNameY, "�؂̏�", cShieldTextColor, m_pBigFont->GetHandle());
					DrawStringToHandle(cShieldDescX, cShieldDescY1, "�l�`�̈ꕔ�����o������\n���̖ؖڂɂ͂��Ă̗��Ɛ�΂�\n�ς��������ւ肪���܂�Ă���", cShieldTextColor, m_pFont->GetHandle());
					DrawStringToHandle(cShieldDescX, cShieldDescY2, "�������e�����S���ƁA\n���Ȃ₩�ɏՌ����󂯗����_������˔����A\n�������Â��ɁA�������m���Ɏ�葱����", cShieldTextColor, m_pFont->GetHandle());
				}
			}
			select++;
		}
	}
}

void Setting::ArmorUpdate(std::list<std::string> list, Armor& armor, int body)
{
	int select = 1;

	//���̑I��
	if (body == 0)
	{
		armor.SetBody(true);
		armor.SetCommon(false);

		armor.Init();

		m_pSelect->NowSelect = 9;
	}
	//�h��I��
	else if (body >= 1)
	{
		for (const auto& item : list)
		{
			if (body == select)
			{
				//���ʂ̊Z�I��
				if (item == "ArmorNormal")
				{
					armor.SetCommon(true);
					armor.SetBody(false);

					armor.Init();

					m_pSelect->NowSelect = 9;
				}
			}

			select++;
		}
	}
}

/// <summary>
/// �h��̐����`��
/// </summary>
/// <param name="list"></param>
/// <param name="body"></param>
void Setting::ArmorDraw(std::list<std::string> list, int body)
{
	int select = 1;

	//���̑I��
	if (body == 0)
	{
		DrawStringToHandle(cArmorNameX, cArmorNameY, "����", cArmorTextColor, m_pBigFont->GetHandle());
		DrawStringToHandle(cArmorDescX, cArmorDescY1, "�M����ׂ��͌Ȃ̗͂̂�", cArmorTextColor, m_pFont->GetHandle());
	}
	//�h��I��
	else if (body >= 1)
	{
		for (const auto& item : list)
		{
			if (body == select)
			{
				//���ʂ̊Z�I��
				if (item == "ArmorNormal")
				{
					DrawStringToHandle(cArmorNameX, cArmorNameY, "�m�N�^�[�j�X���̊Z", cArmorTextColor, m_pBigFont->GetHandle());
					DrawStringToHandle(cArmorDescX, cArmorDescY1, "�m�N�^�[�j�X�����삷�镺�����̊Z\n��]���z�����݁A�Î��Z��", cArmorTextColor, m_pFont->GetHandle());
					DrawStringToHandle(cArmorDescX, cArmorDescY2, "�b���グ��ꂽ�|�͌���e���A��������\n�邪�I���܂Ő킢������\n���ꂪ�m�N�^�[�j�X���̏h���ł���", cArmorTextColor, m_pFont->GetHandle());
				}
			}
			select++;
		}
	}
}
