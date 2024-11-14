#pragma once
#include "Library/MyLibrary.h"
#include "Map/MapBase.h"
#include <memory>

/// <summary>
/// マップを管理するクラス
/// </summary>
class MapManager
{
public:
	//コンストラクタ
	MapManager();
	//デストラクタ
	virtual ~MapManager();

	//データの初期化
	void DataInit();
	//初期化処理
	void Init(std::shared_ptr<MyLibrary::Physics> physics);
	//更新処理
	void Update(std::shared_ptr<MyLibrary::Physics> physics);
	//当たったかの判定処理
	void JudgeUpdate();
	//判定を初期化する
	void TriggerReset();
	//描画処理
	void Draw();
	//終了処理
	void End(std::shared_ptr<MyLibrary::Physics> physics);

	//コリジョンマップを返す
	int GetCollisionMap() { return m_pMap->GetCollisionMap(); }

	//休息地点を返す
	bool GetRest() { return m_pMap->GetRest(); }

	//ボス部屋
	bool GetBossRoom() { return m_pMap->GetBossRoom(); }
private:
	//スマートポインタ
	std::shared_ptr<MapBase> m_pMap;
};

