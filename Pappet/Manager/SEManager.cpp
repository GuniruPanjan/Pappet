#include "SEManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
SEManager::SEManager():
	m_buttonSE(0),
	m_selectSE(0),
	m_hitSE(0),
	m_playerHitSE(0),
	m_attackSE(0),
	m_walkSE(0),
	m_playerHeelSE(0),
	m_playerRestSE(0),
	m_bossVoiceSE(0),
	m_bossWalkSE(0),
	m_diedSE(0),
	m_itemGetSE(0),
	m_kickSE(0),
	m_knifeSE(0),
	m_loserSE(0),
	m_victorySE(0),
	m_volumePal(0),
	m_guardSE(0),
	m_impactSE(0),
	m_stormSE(0),
	m_jumpSE(0)
{
	for (int i = 0; i < 3; i++)
	{
		m_bossAttackSE[i] = 0;
	}
}

/// <summary>
/// デストラクタ
/// </summary>
SEManager::~SEManager()
{
	//メモリ解放
	//SE
	DeleteSoundMem(m_buttonSE);
	DeleteSoundMem(m_selectSE);
	DeleteSoundMem(m_hitSE);
	DeleteSoundMem(m_playerHitSE);
	DeleteSoundMem(m_attackSE);
	DeleteSoundMem(m_walkSE);
	DeleteSoundMem(m_playerHeelSE);
	DeleteSoundMem(m_playerRestSE);
	DeleteSoundMem(m_bossVoiceSE);
	DeleteSoundMem(m_bossAttackSE[0]);
	DeleteSoundMem(m_bossAttackSE[1]);
	DeleteSoundMem(m_bossAttackSE[2]);
	DeleteSoundMem(m_bossWalkSE);
	DeleteSoundMem(m_diedSE);
	DeleteSoundMem(m_victorySE);
	DeleteSoundMem(m_loserSE);
	DeleteSoundMem(m_guardSE);
	DeleteSoundMem(m_impactSE);
	DeleteSoundMem(m_stormSE);
	DeleteSoundMem(m_jumpSE);
}

/// <summary>
/// シーンSEの初期化処理
/// </summary>
void SEManager::SceneInit()
{
	//SE読み込み
	m_buttonSE = LoadSoundMem("Data/SE/ButtonSE.mp3");
	m_selectSE = LoadSoundMem("Data/SE/SelectSE.mp3");
	m_loserSE = LoadSoundMem("Data/SE/LoserSE.mp3");
	m_victorySE = LoadSoundMem("Data/SE/VictorySE.mp3");
}

/// <summary>
/// キャラSEの初期化処理
/// </summary>
void SEManager::CharaInit()
{
	//SE読み込み
	m_hitSE = LoadSoundMem("Data/SE/HitSE.mp3");
	m_playerHitSE = LoadSoundMem("Data/SE/PlayerToBossHitSE.mp3");
	m_attackSE = LoadSoundMem("Data/SE/AttackSE.mp3");
	m_walkSE = LoadSoundMem("Data/SE/WalkSE.mp3");
	m_playerHeelSE = LoadSoundMem("Data/SE/HeelSE.mp3");
	m_playerRestSE = LoadSoundMem("Data/SE/RestSE.mp3");
	m_diedSE = LoadSoundMem("Data/SE/DiedSE.mp3");
	m_itemGetSE = LoadSoundMem("Data/SE/GetItemSE.mp3");
	m_kickSE = LoadSoundMem("Data/SE/KickSE.mp3");
	m_knifeSE = LoadSoundMem("Data/SE/KnifeSE.mp3");
	m_guardSE = LoadSoundMem("Data/SE/GuardSE.mp3");
	m_impactSE = LoadSoundMem("Data/SE/ImpactSE.mp3");
	m_stormSE = LoadSoundMem("Data/SE/StormSE.mp3");
	m_jumpSE = LoadSoundMem("Data/SE/JumpSE.mp3");
	
}

/// <summary>
/// ボスSEの初期化処理
/// </summary>
void SEManager::BossInit()
{
	//SE読み込み
	m_bossVoiceSE = LoadSoundMem("Data/SE/BossVoiceSE.mp3");
	m_bossAttackSE[0] = LoadSoundMem("Data/SE/BossAttack1SE.mp3");
	m_bossAttackSE[1] = LoadSoundMem("Data/SE/BossAttack2SE.mp3");
	m_bossAttackSE[2] = LoadSoundMem("Data/SE/BossAttack3SE.mp3");
	m_bossWalkSE = LoadSoundMem("Data/SE/BossWalkSE.mp3");
}

/// <summary>
/// 更新処理
/// </summary>
/// <param name="volume">音量</param>
void SEManager::Update(int volume)
{
	m_volumePal = volume;

	//SE
	ChangeVolumeSoundMem(m_volumePal, m_buttonSE);
	ChangeVolumeSoundMem(m_volumePal, m_selectSE);
	ChangeVolumeSoundMem(m_volumePal, m_hitSE);
	ChangeVolumeSoundMem(m_volumePal, m_playerHitSE);
	ChangeVolumeSoundMem(m_volumePal, m_attackSE);
	ChangeVolumeSoundMem(m_volumePal, m_walkSE);
	ChangeVolumeSoundMem(m_volumePal, m_playerHeelSE);
	ChangeVolumeSoundMem(m_volumePal, m_playerRestSE);
	ChangeVolumeSoundMem(m_volumePal, m_bossVoiceSE);
	ChangeVolumeSoundMem(m_volumePal, m_bossAttackSE[0]);
	ChangeVolumeSoundMem(m_volumePal, m_bossAttackSE[1]);
	ChangeVolumeSoundMem(m_volumePal, m_bossAttackSE[2]);
	ChangeVolumeSoundMem(m_volumePal, m_bossWalkSE);
	ChangeVolumeSoundMem(m_volumePal, m_itemGetSE);
	ChangeVolumeSoundMem(m_volumePal, m_kickSE);
	ChangeVolumeSoundMem(m_volumePal, m_knifeSE);
	ChangeVolumeSoundMem(m_volumePal, m_diedSE);
	ChangeVolumeSoundMem(m_volumePal, m_victorySE);
	ChangeVolumeSoundMem(m_volumePal, m_loserSE);
	ChangeVolumeSoundMem(m_volumePal, m_guardSE);
	ChangeVolumeSoundMem(m_volumePal, m_impactSE);
	ChangeVolumeSoundMem(m_volumePal, m_stormSE);
	ChangeVolumeSoundMem(m_volumePal, m_jumpSE);

}

/// <summary>
/// 終了処理
/// </summary>
void SEManager::End()
{
	//SE
	DeleteSoundMem(m_buttonSE);
	DeleteSoundMem(m_selectSE);
	DeleteSoundMem(m_hitSE);
	DeleteSoundMem(m_playerHitSE);
	DeleteSoundMem(m_attackSE);
	DeleteSoundMem(m_walkSE);
	DeleteSoundMem(m_playerHeelSE);
	DeleteSoundMem(m_playerRestSE);
	DeleteSoundMem(m_bossVoiceSE);
	DeleteSoundMem(m_bossAttackSE[0]);
	DeleteSoundMem(m_bossAttackSE[1]);
	DeleteSoundMem(m_bossAttackSE[2]);
	DeleteSoundMem(m_bossWalkSE);
	DeleteSoundMem(m_itemGetSE);
	DeleteSoundMem(m_kickSE);
	DeleteSoundMem(m_knifeSE);
	DeleteSoundMem(m_diedSE);
	DeleteSoundMem(m_victorySE);
	DeleteSoundMem(m_loserSE);
	DeleteSoundMem(m_guardSE);
	DeleteSoundMem(m_impactSE);
	DeleteSoundMem(m_stormSE);
	DeleteSoundMem(m_jumpSE);
}
