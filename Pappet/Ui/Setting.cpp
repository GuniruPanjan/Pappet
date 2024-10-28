#include "Setting.h"
#include "Manager/SelectManager.h"
#include "UI/UIBase.h"

namespace
{
	int selectDecision = 0;     //選択したものを決定する変数
	int brightDecision = 0;     //明るさを決定する変数
	int volumeDecision = 0;     //音量を決定する変数
}

/// <summary>
/// コンストラクタ
/// </summary>
Setting::Setting() :
	m_black(0),
	m_white(0),
	m_back(0),
	m_blackPal(0),
	m_whitePal(0),
	m_button(0),
	m_thumb(0),
	m_waitTime(0),
	m_brightnessColor(0),
	m_bgmColor(0),
	m_returnColor(0),
	m_one(false),
	m_settingScene(false),
	m_brightness(false),
	m_volume(false),
	m_volumeSize(0),
	m_equipmentMenu(false),
	m_returnMenu(true),
	m_titleMenu(false),
	m_xpad()
{
	for (int i = 0; i < 3; i++)
	{
		m_menuSelect[i] = 0;
		m_menuColor[i] = 0;
	}

	for (int i = 0; i < 5; i++)
	{
		m_brightColor[i] = 0;
		m_volumeColor[i] = 0;
	}
}

/// <summary>
/// デストラクタ
/// </summary>
Setting::~Setting()
{
	//メモリ解放
	DeleteGraph(m_black);
	DeleteGraph(m_back);
	DeleteGraph(m_white);
	pse->End();
}

/// <summary>
/// 初期化処理
/// </summary>
void Setting::Init()
{
	//画像読み込み
	m_black = LoadGraph("Data/SceneBack/BlackMini.png");         //14.1 KB (14,532 バイト)
	m_back = ui->MyLoadGraph("Data/SceneBack/BlackMini.png", 1, 1);
	m_white = LoadGraph("Data/SceneBack/WhiteMini.png");         //14.1 KB (14,529 バイト)

	selectDecision = 0;

	//明るさ色初期化
	m_brightColor[0] = 0xffffff;
	m_brightColor[1] = 0xffffff;
	m_brightColor[2] = 0xffff00;
	m_brightColor[3] = 0xffffff;
	m_brightColor[4] = 0xffffff;

	//音量色初期化
	m_volumeColor[0] = 0xffffff;
	m_volumeColor[1] = 0xffffff;
	m_volumeColor[2] = 0xffff00;
	m_volumeColor[3] = 0xffffff;
	m_volumeColor[4] = 0xffffff;

	m_menuSelect[0] = 1;
	m_menuSelect[1] = 0;
	m_menuSelect[2] = 0;

	//音量サイズ
	m_volumeSize = 130;

	m_button = 0;
	m_thumb = 0;
	m_one = false;
	m_settingScene = false;
	m_brightness = false;
	m_volume = false;
	m_equipmentMenu = false;
	m_returnMenu = true;
	m_titleMenu = false;

	m_brightnessColor = 0xffffff;
	m_bgmColor = 0xffffff;
	m_returnColor = 0xffffff;

	m_blackPal = 0;
	m_whitePal = 0;

	pse->SceneInit();
}

/// <summary>
/// 更新処理
/// </summary>
void Setting::Update()
{
	//パッド入力所得
	GetJoypadXInputState(DX_INPUT_KEY_PAD1, &m_xpad);

	if (m_waitTime > 10)
	{
		//上
		if (m_xpad.ThumbLY > 2000)
		{
			m_button++;
		}
		//下
		else if (m_xpad.ThumbLY < 0)
		{
			m_button--;
		}
		//右
		else if (m_xpad.ThumbLX > 2000)
		{
			m_thumb--;
		}
		//左
		else if (m_xpad.ThumbLX < 0)
		{
			m_thumb++;
		}
		else
		{
			//初期化
			m_button = 0;
			m_thumb = 0;

			m_one = false;
		}

		//選択中
		if (m_brightness == false && m_volume == false)
		{
			pselect->Menu_Update(m_button, m_one, m_xpad.Buttons[12], selectDecision, pselect->Eight);
		}
		//明るさの選択中
		if (m_brightness == true && m_volume == false)
		{
			pselect->Menu_Update(m_thumb, m_one, m_xpad.Buttons[13], brightDecision, pselect->Six);
		}
		//音量の選択中
		if (m_brightness == false && m_volume == true)
		{
			pselect->Menu_Update(m_thumb, m_one, m_xpad.Buttons[13], volumeDecision, pselect->Six);
		}

		//Aボタンを押したら
		if (m_xpad.Buttons[12] == 1)
		{
			PlaySoundMem(pse->GetButtonSE(), DX_PLAYTYPE_BACK, true);

			//明るさ設定
			if (selectDecision == 8)
			{
				m_brightness = true;
			}
			//音量設定
			if (selectDecision == 9)
			{
				m_volume = true;
			}
			//元の画面に戻る
			if (selectDecision == 10)
			{
				m_settingScene = false;
			}
		}

		//明るさ設定を押したら
		if (m_brightness == true)
		{
			//Bボタンを押したら
			if (m_xpad.Buttons[13] == 1)
			{
				PlaySoundMem(pse->GetButtonSE(), DX_PLAYTYPE_BACK, true);

				m_brightness = false;
			}
		}
		//音量設定を押したら
		if (m_volume == true)
		{
			//Bボタンを押したら
			if (m_xpad.Buttons[13] == 1)
			{
				PlaySoundMem(pse->GetButtonSE(), DX_PLAYTYPE_BACK, true);

				m_volume = false;
			}
		}
	}
	else
	{
		m_waitTime++;
	}

	pse->Update(m_volumeSize);

}

/// <summary>
/// メニュー更新処理
/// </summary>
void Setting::MenuUpdate()
{
	//パッド入力所得
	GetJoypadXInputState(DX_INPUT_KEY_PAD1, &m_xpad);

	//上
	if (m_xpad.Buttons[0] == 1)
	{
		m_button++;
	}
	//下
	else if (m_xpad.Buttons[1] == 1)
	{
		m_button--;
	}
	else
	{
		//初期化
		m_button = 0;

		m_one = false;
	}

	//上選択
	if (m_menuSelect[0] == 1 && m_button > 0 && m_one == false)
	{
		m_menuSelect[2] = 1;
		m_menuSelect[0] = 0;

		PlaySoundMem(pse->GetSelectSE(), DX_PLAYTYPE_BACK, true);

		m_one = true;
	}
	if (m_menuSelect[1] == 1 && m_button > 0 && m_one == false)
	{
		m_menuSelect[0] = 1;
		m_menuSelect[1] = 0;

		PlaySoundMem(pse->GetSelectSE(), DX_PLAYTYPE_BACK, true);

		m_one = true;
	}
	if (m_menuSelect[2] == 1 && m_button > 0 && m_one == false)
	{
		m_menuSelect[1] = 1;
		m_menuSelect[2] = 0;

		PlaySoundMem(pse->GetSelectSE(), DX_PLAYTYPE_BACK, true);

		m_one = true;
	}
	//下選択
	if (m_menuSelect[0] == 1 && m_button < 0 && m_one == false)
	{
		m_menuSelect[1] = 1;
		m_menuSelect[0] = 0;

		PlaySoundMem(pse->GetSelectSE(), DX_PLAYTYPE_BACK, true);

		m_one = true;
	}
	if (m_menuSelect[1] == 1 && m_button < 0 && m_one == false)
	{
		m_menuSelect[2] = 1;
		m_menuSelect[1] = 0;

		PlaySoundMem(pse->GetSelectSE(), DX_PLAYTYPE_BACK, true);

		m_one = true;
	}
	if (m_menuSelect[2] == 1 && m_button < 0 && m_one == false)
	{
		m_menuSelect[0] = 1;
		m_menuSelect[2] = 0;

		PlaySoundMem(pse->GetSelectSE(), DX_PLAYTYPE_BACK, true);

		m_one = true;
	}
	//Aボタン押したら
	//装備メニュー
	if (m_xpad.Buttons[12] == 1 && m_menuSelect[0] == 1)
	{
		PlaySoundMem(pse->GetButtonSE(), DX_PLAYTYPE_BACK, true);

		m_equipmentMenu = true;
	}
	//戻る
	if (m_xpad.Buttons[12] == 1 && m_menuSelect[1] == 1)
	{
		PlaySoundMem(pse->GetButtonSE(), DX_PLAYTYPE_BACK, true);

		m_returnMenu = false;
	}
	//タイトルに戻る
	if (m_xpad.Buttons[12] == 1 && m_menuSelect[2] == 1)
	{
		PlaySoundMem(pse->GetButtonSE(), DX_PLAYTYPE_BACK, true);

		m_titleMenu = true;
	}

}

/// <summary>
/// 描画
/// </summary>
void Setting::Draw()
{
	//背景の色を薄くする
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
	DrawGraph(0, 0, m_back, false);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//選択中の色を変える
	if (m_brightness == false && m_volume == false)
	{
		if (pselect->NowSelect == pselect->Eight)
		{
			m_brightnessColor = 0xffff00;
			m_bgmColor = 0xffffff;
			m_returnColor = 0xffffff;
		}
		if (pselect->NowSelect == pselect->Nine)
		{
			m_brightnessColor = 0xffffff;
			m_bgmColor = 0xffff00;
			m_returnColor = 0xffffff;
		}
		if (pselect->NowSelect == pselect->Ten)
		{
			m_brightnessColor = 0xffffff;
			m_bgmColor = 0xffffff;
			m_returnColor = 0xffff00;
		}
	}
	//明るさを選択
	if (m_brightness == true)
	{
		BrightColorDraw(5, 0, 1, 2, 3, 4, 125, 0);
		BrightColorDraw(6, 1, 0, 2, 3, 4, 125 / 2, 0);
		BrightColorDraw(7, 2, 1, 0, 3, 4, 0, 0);
		BrightColorDraw(8, 3, 1, 2, 0, 4, 0, 125 / 2);
		BrightColorDraw(9, 4, 1, 2, 3, 0, 0, 125);
	}

	//音量を選択
	if (m_volume == true)
	{
		VolumeColorDraw(5, 0, 1, 2, 3, 4, 0);
		VolumeColorDraw(6, 1, 0, 2, 3, 4, 60);
		VolumeColorDraw(7, 2, 1, 0, 3, 4, 130);
		VolumeColorDraw(8, 3, 1, 2, 0, 4, 190);
		VolumeColorDraw(9, 4, 1, 2, 3, 0, 255);
	}

	//フォントのサイズ変更
	SetFontSize(150);

	DrawString(100, 70, "設定", 0xffffff);

	SetFontSize(100);

	DrawString(100, 340, "明るさ", m_brightnessColor);
	DrawString(100, 500, "音量", m_bgmColor);
	DrawString(100, 660, "戻る", m_returnColor);

	DrawBox(500, 340, 620, 460, m_brightColor[0], true);
	DrawBox(700, 340, 820, 460, m_brightColor[1], true);
	DrawBox(900, 340, 1020, 460, m_brightColor[2], true);
	DrawBox(1100, 340, 1220, 460, m_brightColor[3], true);
	DrawBox(1300, 340, 1420, 460, m_brightColor[4], true);

	DrawBox(500, 500, 620, 620, m_volumeColor[0], true);
	DrawBox(700, 500, 820, 620, m_volumeColor[1], true);
	DrawBox(900, 500, 1020, 620, m_volumeColor[2], true);
	DrawBox(1100, 500, 1220, 620, m_volumeColor[3], true);
	DrawBox(1300, 500, 1420, 620, m_volumeColor[4], true);

	pselect->Draw();

	//フォントのサイズを戻す
	SetFontSize(40);
}

/// <summary>
/// 選択中の色を変える
/// </summary>
/// <param name="select">列挙型</param>
/// <param name="now">選択してるもの</param>
/// <param name="other1">それ以外１</param>
/// <param name="other2">それ以外２</param>
/// <param name="other3">それ以外３</param>
/// <param name="other4">それ以外４</param>
/// <param name="black">黒い画像のブレンド率</param>
/// <param name="white">白い画像のブレンド率</param>
void Setting::BrightColorDraw(int select, int now, int other1, int other2, int other3, int other4, int black, int white)
{
	if (pselect->NowSelect == select)
	{
		m_brightColor[now] = 0xffff00;
		m_brightColor[other1] = 0xffffff;
		m_brightColor[other2] = 0xffffff;
		m_brightColor[other3] = 0xffffff;
		m_brightColor[other4] = 0xffffff;

		m_blackPal = black;
		m_whitePal = white;
	}
}

/// <summary>
/// 選択中の色を変える
/// </summary>
/// <param name="select">列挙型</param>
/// <param name="now">選択してるもの</param>
/// <param name="other1">それ以外１</param>
/// <param name="other2">それ以外２</param>
/// <param name="other3">それ以外３</param>
/// <param name="other4">それ以外４</param>
/// <param name="volume">音量</param>
void Setting::VolumeColorDraw(int select, int now, int other1, int other2, int other3, int other4, int volume)
{
	if (pselect->NowSelect == select)
	{
		m_volumeColor[now] = 0xffff00;
		m_volumeColor[other1] = 0xffffff;
		m_volumeColor[other2] = 0xffffff;
		m_volumeColor[other3] = 0xffffff;
		m_volumeColor[other4] = 0xffffff;

		m_volumeSize = volume;
	}
}

/// <summary>
/// 設定の描画処理
/// </summary>
/// <param name="volume">音量</param>
void Setting::SettingDraw(int volume)
{
	pse->Update(volume);

	//明るさ
	if (brightDecision == 6)
	{
		m_blackPal = 125;
		m_whitePal = 0;
	}
	if (brightDecision == 7)
	{
		m_blackPal = 125 / 2;
		m_whitePal = 0;
	}
	if (brightDecision == 8)
	{
		m_blackPal = 0;
		m_whitePal = 0;
	}
	if (brightDecision == 9)
	{
		m_blackPal = 0;
		m_whitePal = 125 / 2;
	}
	if (brightDecision == 10)
	{
		m_blackPal = 0;
		m_whitePal = 125;
	}

	//音量
	if (volumeDecision == 6)
	{
		m_volumeSize = 0;
	}
	if (volumeDecision == 7)
	{
		m_volumeSize = 60;
	}
	if (volumeDecision == 8)
	{
		m_volumeSize = 130;
	}
	if (volumeDecision == 9)
	{
		m_volumeSize = 190;
	}
	if (volumeDecision == 10)
	{
		m_volumeSize = 255;
	}

	//画面を暗くする
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_blackPal);
	DrawGraph(0, 0, m_black, false);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	//画面を明るくする
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_whitePal);
	DrawGraph(0, 0, m_white, false);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void Setting::MenuDraw()
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
	DrawGraph(0, 0, m_back, false);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	if (m_menuSelect[0] == 1)
	{
		m_menuColor[0] = 0xffff00;
		m_menuColor[1] = 0xffffff;
		m_menuColor[2] = 0xffffff;
	}
	if (m_menuSelect[1] == 1)
	{
		m_menuColor[0] = 0xffffff;
		m_menuColor[1] = 0xffff00;
		m_menuColor[2] = 0xffffff;
	}
	if (m_menuSelect[2] == 1)
	{
		m_menuColor[0] = 0xffffff;
		m_menuColor[1] = 0xffffff;
		m_menuColor[2] = 0xffff00;
	}

	//フォントのサイズ変更
	SetFontSize(150);

	DrawString(100, 70, "メニュー", 0xffffff);

	SetFontSize(100);

	DrawString(100, 340, "装備", m_menuColor[0]);
	DrawString(100, 500, "戻る", m_menuColor[1]);
	DrawString(100, 660, "タイトルへ", m_menuColor[2]);

	//フォントのサイズを戻す
	SetFontSize(40);
}

/// <summary>
/// 終了処理
/// </summary>
void Setting::End()
{
	//メモリ解放
	DeleteGraph(m_black);
	DeleteGraph(m_back);
	DeleteGraph(m_white);
	pse->End();
}
