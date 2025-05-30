#pragma once
#include "DxLib.h"
#include "Library/MyLibrary.h"
#include "EnemyBase.h"
#include <map>

class SEManager;

/// <summary>
/// 雑魚敵のクラス
/// </summary>
class Immortal : public EnemyBase
{
public:
	//コンストラクタ
	Immortal();
	//デストラクタ
	virtual ~Immortal();

	//初期化
	void Init(float posX, float posY, float posZ, std::shared_ptr<MyLibrary::Physics> physics, bool tutorial, EnemyWeapon& weapon) override;
	//ゲームの仕様上での初期化処理
	void GameInit(float posX, float posY, float posZ, std::shared_ptr<MyLibrary::Physics> physics, bool tutorial, EnemyWeapon& weapon) override;
	//更新
	void Update(MyLibrary::LibVec3 playerPos, MyLibrary::LibVec3 shieldPos, bool isChase, SEManager& se, std::shared_ptr<MyLibrary::Physics> physics, EnemyWeapon& weapon);
	//怯んでないときにできる行動
	void Action(MyLibrary::LibVec3 playerPos, bool isChase, SEManager& se);
	//描画
	void Draw(EnemyWeapon& weapon);
	//UI描画
	void DrawUI(UI& ui) override;

private:
	int m_frameRightHand[2];           //右手のフレーム
	VECTOR m_ligRightPos[2];           //右手のポジション
};

