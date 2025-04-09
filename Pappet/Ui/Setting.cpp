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
	//���邳
	int cBrightnessWhite = 0;
	//����
	int cVolume = 125;
	float cVolumeBar = 0.6f;
	//SE����
	int cSe = 150;
	float cSeBar = 0.7f;
	//�J�����̊��x
	float cCamera = 2.0f;
	float cCameraBar = 0.1f;

	// �o�[�̕`��
	constexpr int cBarWidth = 600; //�o�[�̕�
	constexpr int cBarHeight = 20; //�o�[�̍���

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

	bool cNo = false;    //�͋Z�p
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
	m_brightnessColor(0),
	m_bgmColor(0),
	m_seColor(0),
	m_cameraColor(0),
	m_returnColor(0),
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
	m_volumeBar = cVolumeBar;
	m_seBar = cSeBar;
	m_cameraBar = cCameraBar;

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

	m_brightnessColor = 0xffffff;
	m_bgmColor = 0xffffff;
	m_returnColor = 0xffffff;

	m_equipment = MyLoadGraph("Data/UI/�������UI.png", 1, 1);
	m_itemBox = MyLoadGraph("Data/UI/�A�C�e�����UI.png", 1, 1);
	m_selectEquipment = MyLoadGraph("Data/UI/�����I�����UI.png", 1, 1);
	m_rest = MyLoadGraph("Data/UI/�l�p�̔��������`�̈͂�.png", 1, 1);
	m_selectUi = MyLoadGraph("Data/UI/�I��UI.png", 2, 2);
	m_levelUp = MyLoadGraph("Data/UI/���x���X�e�[�^�XUI.png", 1, 1);
	m_menu = MyLoadGraph("Data/UI/MenuUI.png", 1, 1);

	m_pFont->FontInit(50);
	m_pSmallFont->FontInit(30);
	m_pBigFont->FontInit(80);
}

/// <summary>
/// �X�V����
/// </summary>
void Setting::Update(SEManager& se)
{
	//�p�b�h���͏���
	GetJoypadXInputState(DX_INPUT_KEY_PAD1, &m_xpad);

	if (m_waitTime > 10)
	{
		//��
		if (m_xpad.ThumbLY > 2000 || m_xpad.Buttons[0] == 1)
		{
			m_button++;
		}
		//��
		else if (m_xpad.ThumbLY < 0 || m_xpad.Buttons[1] == 1)
		{
			m_button--;
		}
		//�E
		else if (m_xpad.ThumbLX > 2000 || m_xpad.Buttons[3] == 1)
		{
			m_thumb--;
		}
		//��
		else if (m_xpad.ThumbLX < 0 || m_xpad.Buttons[2] == 1)
		{
			m_thumb++;
		}
		else
		{
			//������
			m_button = 0;
			m_thumb = 0;

			m_one = false;
		}

		//�I��
		if (m_brightness == false && m_volume == false && m_se == false && m_camera == false)
		{
			m_pSelect->Menu_Update(m_button, m_one, m_xpad.Buttons[12], selectDecision, m_pSelect->Six);
		}
		//���邳�̑I��
		if (m_brightness == true && m_volume == false && m_se == false && m_camera == false)
		{
			m_pSelect->Menu_Update(m_thumb, m_one, m_xpad.Buttons[13], brightDecision, m_pSelect->One);
		}
		//���ʂ̑I��
		if (m_brightness == false && m_volume == true && m_se == false && m_camera == false)
		{
			m_pSelect->Menu_Update(m_thumb, m_one, m_xpad.Buttons[13], volumeDecision, m_pSelect->One);
		}
		//SE�̑I��
		if (m_brightness == false && m_volume == false && m_se == true && m_camera == false)
		{
			m_pSelect->Menu_Update(m_thumb, m_one, m_xpad.Buttons[13], seDecision, m_pSelect->One);
		}
		//�J�������x�̑I��
		if (m_brightness == false && m_volume == false && m_se == false && m_camera == true)
		{
			m_pSelect->Menu_Update(m_thumb, m_one, m_xpad.Buttons[13], cameraDecision, m_pSelect->One);
		}

		if (m_brightness)
		{
			//�Â������߂�
			SettingChange(m_blackPal, cBrightnessBlack, 125, 100, 75, 50, 0, 0, 0, 0, 0, 0);
			//���邳�����߂�
			SettingChange(m_whitePal, cBrightnessWhite, 0, 0, 0, 0, 0, 25, 50, 75, 100, 125);
		}
		else if(m_volume)
		{
			//���ʂ����߂�
			SettingChange(m_volumeSize, cVolume, 0, 25, 50, 75, 100, 125, 150, 175, 200, 225);
		}
		else if (m_se)
		{
			//SE�����߂�
			SettingChange(m_seSize, cSe, 0, 25, 50, 75, 100, 125, 150, 175, 200, 225);
		}
		else if (m_camera)
		{
			//�J�������x�����߂�
			CameraChange(m_cameraSize, cCamera, 2.0f, 2.3f, 2.5f, 2.7f, 3.0f, 3.2f, 3.4f, 3.6f, 3.8f, 4.0f);
		}


		//A�{�^������������
		if (m_xpad.Buttons[12] == 1 && !m_brightness && !m_volume && !m_se && !m_camera)
		{
			PlaySoundMem(se.GetButtonSE(), DX_PLAYTYPE_BACK, true);

			//���邳�ݒ�
			if (selectDecision == 6)
			{
				m_brightness = true;

				//������
				m_waitTime = 0;
			}
			//BGM�ݒ�
			if (selectDecision == 7)
			{
				m_volume = true;

				//������
				m_waitTime = 0;
			}
			//SE�ݒ�
			if (selectDecision == 8)
			{
				m_se = true;

				//������
				m_waitTime = 0;
			}
			//�J�������x
			if (selectDecision == 9)
			{
				m_camera = true;

				//������
				m_waitTime = 0;
			}
			//���̉�ʂɖ߂�
			if (selectDecision == 10)
			{
				m_settingScene = false;

				//������
				m_waitTime = 0;
			}
		}

		//���邳�ݒ����������
		if (m_brightness == true && m_waitTime != 0)
		{
			//AB�{�^������������
			if (m_xpad.Buttons[12] == 1 || m_xpad.Buttons[13] == 1)
			{
				PlaySoundMem(se.GetButtonSE(), DX_PLAYTYPE_BACK, true);

				m_pSelect->NowSelect = 5;

				//������
				m_waitTime = 0;

				m_brightness = false;
			}
		}
		//���ʐݒ����������
		else if (m_volume == true && m_waitTime != 0)
		{
			//B�{�^������������
			if (m_xpad.Buttons[12] == 1 || m_xpad.Buttons[13] == 1)
			{
				PlaySoundMem(se.GetButtonSE(), DX_PLAYTYPE_BACK, true);

				m_pSelect->NowSelect = 6;

				//������
				m_waitTime = 0;

				m_volume = false;
			}
		}
		//SE�ݒ����������
		else if (m_se == true && m_waitTime != 0)
		{
			//B�{�^������������
			if (m_xpad.Buttons[12] == 1 || m_xpad.Buttons[13] == 1)
			{
				PlaySoundMem(se.GetButtonSE(), DX_PLAYTYPE_BACK, true);

				m_pSelect->NowSelect = 7;

				//������
				m_waitTime = 0;

				m_se = false;
			}
		}
		//�J�������x��ݒ�
		else if (m_camera == true && m_waitTime != 0)
		{
			//B�{�^������������
			if (m_xpad.Buttons[12] == 1 || m_xpad.Buttons[13] == 1)
			{
				PlaySoundMem(se.GetButtonSE(), DX_PLAYTYPE_BACK, true);

				m_pSelect->NowSelect = 8;

				//������
				m_waitTime = 0;

				m_camera = false;
			}
		}
		//��ʂɖ߂�
		else
		{
			//B�{�^������������
			if (m_xpad.Buttons[13] == 1)
			{
				PlaySoundMem(se.GetButtonSE(), DX_PLAYTYPE_BACK, true);

				m_pSelect->NowSelect = 7;

				//������
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

	if (cWaitTime >= 10)
	{
		//A�{�^���������ꂽ��
		if (m_xpad.Buttons[12] == 1)
		{
			PlaySoundMem(se.GetButtonSE(), DX_PLAYTYPE_BACK, true);

			//�����I��
			if (selectDecision == 7)
			{
				m_equipmentMenu = true;

				//���Z�b�g
				cWaitTime = 0;
			}
			//�ݒ�
			if (selectDecision == 8)
			{
				//����������
				m_waitTime = 0;

				m_settingScene = true;

				//���Z�b�g
				cWaitTime = 0;
			}
			//���̉�ʂɖ߂�
			if (selectDecision == 9)
			{
				m_returnMenu = false;

				player.SetRoll(0);
			}
			//�^�C�g���ɖ߂�
			if (selectDecision == 10)
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

			player.SetRoll(0);

		}
	}
	else
	{
		cWaitTime++;
	}

}

/// <summary>
/// �X�e�[�^�X��ʂ�ς���X�V����
/// </summary>
void Setting::MenuChange()
{
	//�p�b�h���͏���
	GetJoypadXInputState(DX_INPUT_KEY_PAD1, &m_xpad);

	//R�{�^���������ꂽ��
	if (m_xpad.Buttons[9] == 1)
	{
		//������ʂ���߂�
		m_equipmentMenu = false;
		//�A�C�e����ʂɈڂ�
		m_itemMenu = true;
	}
	//L�{�^���������ꂽ��
	else if (m_xpad.Buttons[8] == 1)
	{
		//�A�C�e����ʂ���߂�
		m_itemMenu = false;
		//������ʂɈڂ�
		m_equipmentMenu = true;
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


	if (cWaitTime >= 10)
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
			if (selectDecision == 8)
			{
				m_select.right = true;
			}
			//�������I��
			//�E�w�֑I��
			//�^�񒆃A�C�e���I��
			else if (selectDecision == 9)
			{
				m_select.left = true;
			}
			//�h��I��
			//���w�֑I��
			//���A�C�e���I��
			else if (selectDecision == 10)
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

		if (cWaitTime >= 10)
		{
			//A�{�^���������ꂽ��
			if (m_xpad.Buttons[12] == 1)
			{
				PlaySoundMem(se.GetButtonSE(), DX_PLAYTYPE_BACK, true);

				//�x������
				if (selectDecision == 8)
				{
					m_reset = true;
				}
				//�]��
				if (selectDecision == 9)
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
				if (selectDecision == 10)
				{
					player.SetNotRest(false);

					player.SetRoll(0);
				}

				//���Z�b�g
				cWaitTime = 0;
			}

			//A�{�^���������ꂽ��x������߂�
			if (m_xpad.Buttons[13] == 1)
			{
				PlaySoundMem(se.GetButtonSE(), DX_PLAYTYPE_BACK, true);

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

		if (cWaitTime >= 10)
		{
			//A�{�^���������ꂽ��
			if (m_xpad.Buttons[12] == 1)
			{
				PlaySoundMem(se.GetButtonSE(), DX_PLAYTYPE_BACK, true);

				//�x������
				if (selectDecision == 7)
				{
					m_reset = true;
				}
				//���x�����グ��
				if (selectDecision == 8)
				{
					m_statusLevel = true;

					ms_levelUP.sl_hp = player.GetHPLevel();
					ms_levelUP.sl_stamina = player.GetStaminaLevel();
					ms_levelUP.sl_muscle = player.GetMuscleLevel();
					ms_levelUP.sl_skill = player.GetSkillLevel();

					m_core = core.GetCore();

				}
				//�]��
				if (selectDecision == 9)
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
				if (selectDecision == 10)
				{
					player.SetNotRest(false);

					player.SetRoll(0);
				}

				//���Z�b�g
				cWaitTime = 0;
			}

			//A�{�^���������ꂽ��x������߂�
			if (m_xpad.Buttons[13] == 1)
			{
				PlaySoundMem(se.GetButtonSE(), DX_PLAYTYPE_BACK, true);

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

	if (cWaitTime >= 10)
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
	if (cPush >= 30)
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


	if (cWaitTime >= 10)
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
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
	DrawGraph(0, 0, m_back, false);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//�I�𒆂̐F��ς���
	if (m_brightness == false && m_volume == false && m_se == false && m_camera == false)
	{
		if (m_pSelect->NowSelect == m_pSelect->Six)
		{
			m_brightnessColor = 0xffff00;
			m_bgmColor = 0xffffff;
			m_seColor = 0xffffff;
			m_cameraColor = 0xffffff;
			m_returnColor = 0xffffff;
		}
		else if (m_pSelect->NowSelect == m_pSelect->Seven)
		{
			m_brightnessColor = 0xffffff;
			m_bgmColor = 0xffff00;
			m_seColor = 0xffffff;
			m_cameraColor = 0xffffff;
			m_returnColor = 0xffffff;
		}
		else if (m_pSelect->NowSelect == m_pSelect->Eight)
		{
			m_brightnessColor = 0xffffff;
			m_bgmColor = 0xffffff;
			m_seColor = 0xffff00;
			m_cameraColor = 0xffffff;
			m_returnColor = 0xffffff;
		}
		else if (m_pSelect->NowSelect == m_pSelect->Nine)
		{
			m_brightnessColor = 0xffffff;
			m_bgmColor = 0xffffff;
			m_seColor = 0xffffff;
			m_cameraColor = 0xffff00;
			m_returnColor = 0xffffff;
		}
		else if (m_pSelect->NowSelect == m_pSelect->Ten)
		{
			m_brightnessColor = 0xffffff;
			m_bgmColor = 0xffffff;
			m_seColor = 0xffffff;
			m_cameraColor = 0xffffff;
			m_returnColor = 0xffff00;
		}
	}
	//���邳��I��
	if (m_brightness == true)
	{
		SettingBarChange(m_pSelect->NowSelect, m_brightBar, cBrightnessBar);

	}

	//���ʂ�I��
	if (m_volume == true)
	{
		SettingBarChange(m_pSelect->NowSelect, m_volumeBar, cVolumeBar);

	}

	//SE��I��
	if (m_se == true)
	{
		SettingBarChange(m_pSelect->NowSelect, m_seBar, cSeBar);

	}

	//�J�������x��I��
	if (m_camera == true)
	{
		SettingBarChange(m_pSelect->NowSelect, m_cameraBar, cCameraBar);

	}

	DrawStringToHandle(100, 50, "�ݒ�", 0xffffff, m_pBigFont->GetHandle());

	DrawStringToHandle(100, 180, "���邳�ݒ�", m_brightnessColor, m_pFont->GetHandle());
	DrawStringToHandle(100, 360, "BGM�ݒ�", m_bgmColor, m_pFont->GetHandle());
	DrawStringToHandle(100, 540, "SE�ݒ�", m_seColor, m_pFont->GetHandle());
	DrawStringToHandle(100, 720, "�J�������x", m_cameraColor, m_pFont->GetHandle());
	DrawStringToHandle(100, 900, "�߂�", m_returnColor, m_pFont->GetHandle());

	SettingBarDraw(m_brightBar, 500, 200);
	SettingBarDraw(m_volumeBar, 500, 380);
	SettingBarDraw(m_seBar, 500, 560);
	SettingBarDraw(m_cameraBar, 500, 740);
	
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

	//�A���t�@�l��������
	if (!m_blend)
	{
		if (cBlenda < 100)
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
		if (cBlenda > 10)
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
	
	
	DrawRotaGraph(900, 100, 0.5f, 0.0f, lb, true);
	DrawGraph(935, 0, m_menu, true);
	DrawRotaGraph(1550, 100, 0.5f, 0.0f, rb, true);
	DrawRotaGraph(1225, 200, 0.8f, 0.0f, box, true);

	DrawStringToHandle(1130, 190, menu, 0xffffff, m_pSmallFont->GetHandle());

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, cBlenda);
	DrawBox(x1, y1, x2, y2, 0x000fff, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

//�w�i���Â�����
void Setting::MenuBackDraw()
{
	//��ʂ��Â�����
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
	DrawGraph(0, 0, m_black, false);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

/// <summary>
/// ���j���[��ύX���鏊�̕`�揈��
/// </summary>
void Setting::MenuChangeDraw()
{
	if (!m_blend)
	{
		if (cBlenda < 100)
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
		if (cBlenda > 10)
		{
			cBlenda--;
		}
		else
		{
			m_blend = false;
		}
	}


	SetDrawBlendMode(DX_BLENDMODE_ALPHA, cBlenda);
	DrawBox(m_change.oneX, m_change.oneY, m_change.secondX, m_change.secondY, 0x000fff, true);
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

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
	DrawBox(m_equipmentColorPos.oneX, m_equipmentColorPos.oneY, m_equipmentColorPos.secondX, m_equipmentColorPos.secondY, 0x000fff, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	ms_levelUP.sl_all = player.GetLevel();

	ms_levelUP.sl_hp = player.GetHPLevel();
	ms_levelUP.sl_stamina = player.GetStaminaLevel();
	ms_levelUP.sl_muscle = player.GetMuscleLevel();
	ms_levelUP.sl_skill = player.GetSkillLevel();

	DrawFormatStringToHandle(1100, 400, 0xffffff, m_pFont->GetHandle(), "���x���@�@�@%d", ms_levelUP.sl_all);

	DrawFormatStringToHandle(1150, 515, 0xffffff, m_pFont->GetHandle(), "%d", ms_levelUP.sl_hp);
	DrawFormatStringToHandle(1150, 600, 0xffffff, m_pFont->GetHandle(), "%d", ms_levelUP.sl_stamina);
	DrawFormatStringToHandle(1150, 680, 0xffffff, m_pFont->GetHandle(), "%d", ms_levelUP.sl_muscle);
	DrawFormatStringToHandle(1150, 760, 0xffffff, m_pFont->GetHandle(), "%d", ms_levelUP.sl_skill);


	DrawStringToHandle(1300, 515, "������", 0xffffff, m_pFont->GetHandle());
	DrawFormatStringToHandle(1300, 565, 0xffffff, m_pFont->GetHandle(), "%d", static_cast<int>(player.GetStatus().s_hp));
	DrawStringToHandle(1300, 615, "�̗�", 0xffffff, m_pFont->GetHandle());
	DrawFormatStringToHandle(1300, 665, 0xffffff, m_pFont->GetHandle(), "%d", static_cast<int>(player.GetStatus().s_stamina));
	DrawStringToHandle(1300, 715, "�U����", 0xffffff, m_pFont->GetHandle());
	DrawFormatStringToHandle(1300, 765, 0xffffff, m_pFont->GetHandle(), "%d", static_cast<int>(player.GetAttackDamage() + ((ms_levelUP.sl_muscle - 1) * player.GetAttackMuscle()) + ((ms_levelUP.sl_skill - 1) * player.GetAttackSkill())));

}

/// <summary>
/// �x�����
/// </summary>
void Setting::RestDraw(bool rest)
{
	DrawRotaGraph(300, 450, 1.5f, DX_PI_F / 2, m_rest, true);

	m_selectX = 130;

	DrawGraph(m_selectX, m_selectY, m_selectUi, true);

	//���ʂ̋x��
	if (!rest)
	{
		if (m_pSelect->NowSelect == m_pSelect->Eight)
		{
			m_selectY = 255;

			m_menuColor[0] = 0x000000;
			m_menuColor[1] = 0xffffff;
			m_menuColor[2] = 0xffffff;
		}
		else if (m_pSelect->NowSelect == m_pSelect->Nine)
		{
			m_selectY = 355;

			m_menuColor[0] = 0xffffff;
			m_menuColor[1] = 0x000000;
			m_menuColor[2] = 0xffffff;
		}
		else if (m_pSelect->NowSelect == m_pSelect->Ten)
		{
			m_selectY = 455;

			m_menuColor[0] = 0xffffff;
			m_menuColor[1] = 0xffffff;
			m_menuColor[2] = 0x000000;
		}

		//�t�H���g�̃T�C�Y�ύX
		SetFontSize(60);

		DrawString(150, 50, "����", 0xffffff);

		//�t�H���g�̃T�C�Y�ύX
		SetFontSize(40);

		DrawString(150, 300, "�x��", m_menuColor[0]);
		DrawString(150, 400, "����֓]��", m_menuColor[1]);
		DrawString(150, 500, "��������", m_menuColor[2]);
	}
	//���x���グ����x��
	else
	{
		if (m_pSelect->NowSelect == m_pSelect->Seven)
		{
			m_selectY = 255;

			m_menuColor[0] = 0x000000;
			m_menuColor[1] = 0xffffff;
			m_menuColor[2] = 0xffffff;
			m_menuColor[3] = 0xffffff;
		}
		else if (m_pSelect->NowSelect == m_pSelect->Eight)
		{
			m_selectY = 355;

			m_menuColor[0] = 0xffffff;
			m_menuColor[1] = 0x000000;
			m_menuColor[2] = 0xffffff;
			m_menuColor[3] = 0xffffff;
		}
		else if (m_pSelect->NowSelect == m_pSelect->Nine)
		{
			m_selectY = 455;

			m_menuColor[0] = 0xffffff;
			m_menuColor[1] = 0xffffff;
			m_menuColor[2] = 0x000000;
			m_menuColor[3] = 0xffffff;

		}
		else if (m_pSelect->NowSelect == m_pSelect->Ten)
		{
			m_selectY = 555;

			m_menuColor[0] = 0xffffff;
			m_menuColor[1] = 0xffffff;
			m_menuColor[2] = 0xffffff;
			m_menuColor[3] = 0x000000;
		}

		//�t�H���g�̃T�C�Y�ύX
		SetFontSize(60);

		DrawString(150, 50, "����", 0xffffff);

		//�t�H���g�̃T�C�Y�ύX
		SetFontSize(40);

		DrawString(150, 300, "�x��", m_menuColor[0]);
		DrawString(150, 400, "���x���グ", m_menuColor[1]);
		DrawString(150, 500, "����֓]��", m_menuColor[2]);
		DrawString(150, 600, "��������", m_menuColor[3]);
	}
}

/// <summary>
/// ���x���A�b�v�`��
/// </summary>
/// <param name="player"></param>
void Setting::LevelUpDraw(Player& player, CoreManager& core)
{

	DrawGraph(-50, 0, m_levelUp, true);
	DrawRotaGraph(m_selectX, m_selectY, cGraphSize, DX_PI_F, m_selectUi, true);

	if (m_pSelect->NowSelect == m_pSelect->Six)
	{
		m_selectX = 300;
		m_selectY = 540;
		cGraphSize = 1.0f;

		m_menuColor[0] = 0x000000;
		m_menuColor[1] = 0xffffff;
		m_menuColor[2] = 0xffffff;
		m_menuColor[3] = 0xffffff;
		m_menuColor[4] = 0xffffff;
	}
	else if (m_pSelect->NowSelect == m_pSelect->Seven)
	{
		m_selectX = 300;
		m_selectY = 635;
		cGraphSize = 1.0f;

		m_menuColor[0] = 0xffffff;
		m_menuColor[1] = 0x000000;
		m_menuColor[2] = 0xffffff;
		m_menuColor[3] = 0xffffff;
		m_menuColor[4] = 0xffffff;
	}
	else if (m_pSelect->NowSelect == m_pSelect->Eight)
	{
		m_selectX = 300;
		m_selectY = 730;
		cGraphSize = 1.0f;

		m_menuColor[0] = 0xffffff;
		m_menuColor[1] = 0xffffff;
		m_menuColor[2] = 0x000000;
		m_menuColor[3] = 0xffffff;
		m_menuColor[4] = 0xffffff;
	}
	else if (m_pSelect->NowSelect == m_pSelect->Nine)
	{
		m_selectX = 300;
		m_selectY = 825;
		cGraphSize = 1.0f;

		m_menuColor[0] = 0xffffff;
		m_menuColor[1] = 0xffffff;
		m_menuColor[2] = 0xffffff;
		m_menuColor[3] = 0x000000;
		m_menuColor[4] = 0xffffff;
	}
	else if (m_pSelect->NowSelect == m_pSelect->Ten)
	{
		m_selectX = 815;
		m_selectY = 930;
		cGraphSize = 1.3f;

		m_menuColor[0] = 0xffffff;
		m_menuColor[1] = 0xffffff;
		m_menuColor[2] = 0xffffff;
		m_menuColor[3] = 0xffffff;
		m_menuColor[4] = 0x000000;
	}


	//�t�H���g�̃T�C�Y�ύX
	SetFontSize(40);

	DrawFormatString(90, 150, 0xffffff, "���x��      %d", ms_levelUP.sl_all);
	DrawFormatString(90, 250, 0xffffff, "�����R�A  %d", m_core);
	DrawFormatString(90, 300, 0xffffff, "�K�v�R�A  %d", core.NeedCore(ms_levelUP.sl_all));

	//���̕ϐ������x���グ��O�̃��x���ŉE�̕ϐ������x�����グ����̃��x��
	DrawFormatString(290, 520, m_menuColor[0], "%d", ms_levelUP.sl_hp);
	DrawFormatString(290, 615, m_menuColor[1], "%d", ms_levelUP.sl_stamina);
	DrawFormatString(290, 710, m_menuColor[2], "%d", ms_levelUP.sl_muscle);
	DrawFormatString(290, 805, m_menuColor[3], "%d", ms_levelUP.sl_skill);

	//���x�����グ�����̃X�e�[�^�X�ϓ�
	DrawFormatString(600, 200, 0xffffff, "�ő吶���� :   %d", static_cast<int>(player.GetStatus().s_hp + ((ms_levelUP.sl_hp - 1) * 30.0f)));
	DrawFormatString(600, 400, 0xffffff, "�ő�̗�   :   %d", static_cast<int>(player.GetStatus().s_stamina + ((ms_levelUP.sl_stamina - 1) * 10.0f)));
	DrawFormatString(600, 600, 0xffffff, "�U����     :   %d", static_cast<int>(player.GetAttackDamage() + ((ms_levelUP.sl_muscle - 1) * player.GetAttackMuscle()) + ((ms_levelUP.sl_skill - 1) * player.GetAttackSkill())));

	//�t�H���g�̃T�C�Y�ύX
	SetFontSize(60);

	DrawString(750, 900, "����", m_menuColor[4]);

	//�t�H���g�̃T�C�Y�ύX
	SetFontSize(40);
}

/// <summary>
/// �A�C�e�����
/// </summary>
void Setting::ItemBoxDraw()
{
	DrawGraph(0, 0, m_itemBox, true);
}

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

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
	DrawBox(m_selectObject.oneX, m_selectObject.oneY, m_selectObject.secondX, m_selectObject.secondY, 0x000fff, true);
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
			DrawGraph(600, 200, m_rest, true);

			DrawStringToHandle(720, 350, "�]���ł��Ȃ�", 0xffffff, m_pFont->GetHandle());
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
	DeleteGraph(m_itemBox);
	DeleteGraph(m_selectEquipment);
	DeleteGraph(m_rest);
	DeleteGraph(m_selectUi);
	DeleteGraph(m_levelUp);
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
void Setting::SettingBarChange(int Decision, float& bar, float& cBar)
{
	if (select)
	{
		if (Decision == 0)
		{
			bar = 0.1f;
			cBar = 0.1f;
		}
		else if (Decision == 1)
		{
			bar = 0.2f;
			cBar = 0.2f;

		}
		else if (Decision == 2)
		{
			bar = 0.3f;
			cBar = 0.3f;

		}
		else if (Decision == 3)
		{
			bar = 0.4f;
			cBar = 0.4f;

		}
		else if (Decision == 4)
		{
			bar = 0.5f;
			cBar = 0.5f;

		}
		else if (Decision == 5)
		{
			bar = 0.6f;
			cBar = 0.6f;

		}
		else if (Decision == 6)
		{
			bar = 0.7f;
			cBar = 0.7f;

		}
		else if (Decision == 7)
		{
			bar = 0.8f;
			cBar = 0.8f;

		}
		else if (Decision == 8)
		{
			bar = 0.9f;
			cBar = 0.9f;

		}
		else if (Decision == 9)
		{
			bar = 1.0f;
			cBar = 1.0f;

		}
	}

	
}

void Setting::SettingBarDraw(float bar, int x, int y)
{
	DrawBox(x, y, x + static_cast<int>(cBarWidth * bar), y + cBarHeight, 0x00ff00, TRUE); // �v���O���X�o�[
	DrawBox(x, y, x + cBarWidth, y + cBarHeight, 0xffffff, false);
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
		DrawStringToHandle(400, 100, "�f��", 0xffffff, m_pBigFont->GetHandle());

		DrawStringToHandle(400, 300, "�M����ׂ��͌Ȃ̗͂̂�", 0xffffff, m_pFont->GetHandle());
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
					DrawStringToHandle(400, 100, "����", 0xffffff, m_pBigFont->GetHandle());

					DrawStringToHandle(400, 300, "���̎����̐n�́A�܂�Ŗ邻�̂��̂�\n�b���グ�����̂悤�Ɍ����z�����݁A\n���ق̒��ɗ₽���P�����h��", 0xffffff, m_pFont->GetHandle());
					DrawStringToHandle(400, 450, "���̌���U�邤�҂́A\n�n�ɔ�߂�ꂽ�[����`�����ނ��ƂƂȂ�\n�����Ȃ��҂ɂ͒����Ȑn�ƂȂ�A\n�S�h�炮�҂ɂ͔j�ł̉e�𗎂Ƃ�", 0xffffff, m_pFont->GetHandle());
				}
				//�؂̃o�b�g�I��
				if (item == "Bat")
				{
					DrawStringToHandle(400, 100, "�؂̞��_", 0xffffff, m_pBigFont->GetHandle());

					DrawStringToHandle(400, 300, "�l�`�̈ꕔ�����o�������_\n������̐킢��ς��������ؔ���\n�|�ɂ����ʋ��x���ւ�", 0xffffff, m_pFont->GetHandle());
					DrawStringToHandle(400, 450, "���Ă̐l�`�̋L�����h���A\n������̎�ɓ���ނ��̊��G�́A\n�s�v�c�Ȉ��S������^����", 0xffffff, m_pFont->GetHandle());

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
		DrawStringToHandle(400, 100, "�f��", 0xffffff, m_pBigFont->GetHandle());

		DrawStringToHandle(400, 300, "�M����ׂ��͌Ȃ̗͂̂�", 0xffffff, m_pFont->GetHandle());
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
					DrawStringToHandle(400, 100, "����", 0xffffff, m_pBigFont->GetHandle());

					DrawStringToHandle(400, 300, "�_�ւ̔��t�����݂��҂��������A���ꂵ��\n�G���҂̐S�ɔ����Ț����𗎂Ƃ��Ƃ���\n�n�������e�����A������̍���I��", 0xffffff, m_pFont->GetHandle());
					DrawStringToHandle(400, 450, "�I�Ԃ̂Ȃ�o�傹�悱�̏��́A\n���҂����̂ł͂Ȃ��A�����̂�����", 0xffffff, m_pFont->GetHandle());

				}
				//�؂̏��I��
				if (item == "WoodShield")
				{
					DrawStringToHandle(400, 100, "�؂̏�", 0xffffff, m_pBigFont->GetHandle());

					DrawStringToHandle(400, 300, "�l�`�̈ꕔ�����o������\n���̖ؖڂɂ͂��Ă̗��Ɛ�΂�\n�ς��������ւ肪���܂�Ă���", 0xffffff, m_pFont->GetHandle());
					DrawStringToHandle(400, 450, "�������e�����S���ƁA\n���Ȃ₩�ɏՌ����󂯗����_������˔����A\n�������Â��ɁA�������m���Ɏ�葱����", 0xffffff, m_pFont->GetHandle());

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
		DrawStringToHandle(400, 100, "����", 0xffffff, m_pBigFont->GetHandle());

		DrawStringToHandle(400, 300, "�M����ׂ��͌Ȃ̗͂̂�", 0xffffff, m_pFont->GetHandle());
	}
	//�h��I��
	else if (body >= 1)
	{
		for (const auto& item : list)
		{
			if (body == select)
			{
				//�c�񂾏��I��
				if (item == "ArmorNormal")
				{
					DrawStringToHandle(400, 100, "�m�N�^�[�j�X���̊Z", 0xffffff, m_pBigFont->GetHandle());

					DrawStringToHandle(400, 300, "�m�N�^�[�j�X�����삷�镺�����̊Z\n��]���z�����݁A�Î��Z��", 0xffffff, m_pFont->GetHandle());
					DrawStringToHandle(400, 400, "�b���グ��ꂽ�|�͌���e���A��������\n�邪�I���܂Ő킢������\n���ꂪ�m�N�^�[�j�X���̏h���ł���", 0xffffff, m_pFont->GetHandle());

				}

			}

			select++;
		}
	}
}
