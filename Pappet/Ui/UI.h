#pragma once
#include "UIBase.h"

class Player;
class Enemy;
class Equipment;
class Map;
class ItemManager;

/// <summary>
/// UI関係のクラス
/// </summary>
class UI : public UIBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	UI();

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~UI();

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="player">プレイヤーの呼び出し</param>
	/// <param name="enemy">エネミーの呼び出し</param>
	void Init(Player& player, Enemy& enemy);

	/// <summary>
	/// 描画処理
	/// </summary>
	/// <param name="player">プレイヤーの呼び出し</param>
	/// <param name="enemy">エネミーの呼び出し</param>
	/// <param name="eq">装備の呼び出し</param>
	/// <param name="map">マップの呼び出し</param>
	/// <param name="item">アイテムの呼び出し</param>
	void Draw(Player& player, Enemy& enemy, Equipment& eq, Map& map, ItemManager& item);

	/// <summary>
	/// 死んだときの描画
	/// </summary>
	void DiedDraw();

	/// <summary>
	/// 終了処理
	/// </summary>
	void End();

private:
	unsigned int m_hpColor = 0xff0000;    //HPカラー
	unsigned int m_staminaColor = 0x00ff00;       //スタミナカラー
	//プレイヤーのUI関係変数
	float m_hpCover;
	float m_staminaCover;
	float m_hpExpressionDivide1;
	float m_hpExpressionDivide2;
	float m_staminaExpressionDivide1;
	float m_staminaExpressionDivide2;
	float m_hpPosX1;
	float m_hpPosY1;
	float m_hpPosX2;
	float m_hpPosY2;
	float m_hpPosY3;
	float m_hpPosY4;
	float m_staminaPosX1;
	float m_staminaPosY1;
	float m_staminaPosX2;
	float m_staminaPosY2;
	float m_staminaPosY3;
	float m_staminaPosY4;
	int m_equipmentUI;
	int m_youDied;

	//敵のUI変数
	float m_enemyBossHpCover;
	float m_enemyBossHpExpressionDivide1;
	float m_enemyBossHpExpressionDivide2;
	float m_enemyBossHpPosX1;
	float m_enemyBossHpPosY1;
	float m_enemyBossHpPosX2;
	float m_enemyBossHpPosY2;
	float m_enemyBossHpPosY3;
	float m_enemyBossHpPosY4;
	const char* m_enemyBossName;
};


