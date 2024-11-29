#pragma once

class EnemyManager;

class CoreManager
{
public:
	CoreManager();
	virtual ~CoreManager();

	void Init();
	void Update(int core);
	void DeadUpdate();
	void End();

	//コア計算
	int Core(int core) { return m_core += core; }
	//コア入手
	int GetCore() { return m_core; }

private:
	int m_core;     //プレイヤーが入手したコアを全て入れる
};

