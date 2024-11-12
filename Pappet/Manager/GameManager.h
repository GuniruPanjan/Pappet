#pragma once
#include "DxLib.h"
#include "Library/MyLibrary.h"
#include "Manager/MapManager.h"
#include<memory>

class Player;
class CollidableNpc;
class Map;
class Camera;
class EnemyManager;
class Setting;

/// <summary>
/// ゲームの内容を管理するクラス
/// </summary>
class GameManager
{
public:
	enum eMapName : int
	{
		RestMap,         //休息ステージ
		FirstMap,       //最初のステージ
		SecondMap,      //2番目のステージ
		ThreeMap,       //3番目のステージ
		FourMap,        //4番目のステージ
		LastMap        //最後のステージ
		
	};

public:
	GameManager();
	virtual ~GameManager();

	void Init();
	void Update();
	void Draw();
	void End();

	//現在のマップ名を取得
	const int GetThisMapName() const { return m_nowMap; }

	//プレイヤー座標を取得
	const MyLibrary::LibVec3 GetPlayerPos() const;

	//タイトルに戻るかの判定を取得
	bool GetTitle() { return m_title; }

private:
	//スマートポインタ管理
	std::shared_ptr<MyLibrary::Physics> m_pPhysics;
	std::shared_ptr<Player> m_pPlayer = std::make_shared<Player>();
	//std::shared_ptr<Map> m_pMap = std::make_shared<Map>();
	std::shared_ptr<MapManager> m_pMap = std::make_shared<MapManager>();
	std::shared_ptr<Camera> m_pCamera = std::make_shared<Camera>();
	std::shared_ptr<CollidableNpc> m_pNpc = std::make_shared<CollidableNpc>();
	std::shared_ptr<EnemyManager> m_pEnemy;
	std::shared_ptr<Setting> m_pSetting = std::make_shared<Setting>();

	//現在のマップ
	eMapName m_nowMap;
	//ステージ名
	//std::string m_stageName;

	//タイトルに戻る
	bool m_title;
};

