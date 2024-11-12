#pragma once
#include <memory>
#include <map>
#include <string>
#include <List>
#include "Library/MyLibrary.h"

class EnemyBase;
class GameManager;

/// <summary>
/// 敵を管理するクラス
/// </summary>
class EnemyManager
{
public:
	/// <summary>
	/// 敵生成情報
	/// </summary>
	struct EnemyGenerateInfo
	{
		std::string enemyName;     //敵の名前
		int mapNumber;             //マップの番号
		int posX;                  //敵のX座標
		int posY;                  //敵のY座標
		int posZ;                  //敵のZ座標

		bool isCreated = false;    //生成済みかどうか
	};
public:
	//コンストラクタ
	EnemyManager();
	//デストラクタ
	virtual ~EnemyManager();

	//初期化
	void Init(const char* stageName);
	//更新
	void Update(std::shared_ptr<MyLibrary::Physics> physics, GameManager* gameManager, MyLibrary::LibVec3 playerPos, MyLibrary::LibVec3 playerDir, bool isPlayerChase);
	//描画
	void Draw();
	//終了処理
	void End();

	//正面の敵の現在のHPを返す
	const int GetFrontEnemyHp() const { return m_frontEnemyHp; }
	//正面の敵の最大HPを返す
	const int GetFrontEnemyMaxHp() const { return m_frontEnemyMaxHp; }
	//敵の座標を返す
	const std::list<MyLibrary::LibVec3> GetEnemyPos() const { return m_enemyPos; }
	//敵のターゲットを返す
	const std::list<bool> GetEnemyTarget() const { return m_enemyTarget; }
private:
	//敵の生成
	void CreateEnemy(float posX, float posY, float posZ, std::string name, std::shared_ptr<MyLibrary::Physics> physics);
private:
	//敵の管理
	std::list<std::shared_ptr<EnemyBase>> m_pEnemys;
	//敵の生成情報
	std::list<std::shared_ptr<EnemyGenerateInfo>> m_pGenerateInfo;

	//ステージ名
	const char* m_stageName;

	//ステージ毎の敵の生成数
	std::unordered_map<int, int> m_enemyGenerationCountPerOneMap;

	//正面の敵のHP
	int m_frontEnemyHp;
	//正面の敵の最大HP
	int m_frontEnemyMaxHp;

	//敵の座標
	std::list<MyLibrary::LibVec3> m_enemyPos;
	//敵のターゲット判定
	std::list<bool> m_enemyTarget;
};

