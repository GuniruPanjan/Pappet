#pragma once
#include "DxLib.h"
#include<memory>

/// <summary>
/// BGMを管理するクラス
/// </summary>
class BgmManager
{
public:
	//コンストラクタ
	BgmManager();
	//デストラクタ
	virtual ~BgmManager();

	//タイトルシーンBGM初期化
	void TitleInit();
	//ゲームシーンBGM初期化
	void GameInit();
	//クリアシーンBGM初期化
	void ClearInit();
	//更新処理
	void Update(int volume);
	//タイトルBGM更新処理
	void TitleBGM();
	//ゲームBGM更新処理
	void GameBGM();
	//ボスBGM更新処理
	void BossBGM();
	//クリアBGM更新処理
	void ClearBGM();
	//終了処理
	void End();

	//ボスのBGMを流すかの判定をとる
	int GetBossBGM() { return m_bossBGM; }

private:
	//BGM関係
	int m_titleBGM;
	int m_gameBGM;
	int m_bossBGM;
	int m_clearBGM;
	int m_volumePal;
};

