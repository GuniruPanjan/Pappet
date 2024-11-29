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

	//�R�A�v�Z
	int Core(int core) { return m_core += core; }
	//�R�A����
	int GetCore() { return m_core; }

private:
	int m_core;     //�v���C���[�����肵���R�A��S�ē����
};

