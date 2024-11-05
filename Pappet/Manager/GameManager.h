#pragma once
#include "DxLib.h"
#include "Library/MyLibrary.h"
#include<memory>

class Player;
class CollidableNpc;
class Map;
class Camera;
class EnemyManager;

/// <summary>
/// �Q�[���̓��e���Ǘ�����N���X
/// </summary>
class GameManager
{
public:
	enum eMapName : int
	{
		RestMap,         //�x���X�e�[�W
		FirstMap,       //�ŏ��̃X�e�[�W
		SecondMap,      //2�Ԗڂ̃X�e�[�W
		ThreeMap,       //3�Ԗڂ̃X�e�[�W
		FourMap,        //4�Ԗڂ̃X�e�[�W
		LastMap        //�Ō�̃X�e�[�W
		
	};

public:
	GameManager();
	virtual ~GameManager();

	void Init();
	void Update();
	void Draw();
	void End();

	//���݂̃}�b�v�����擾
	const int GetThisMapName() const { return m_nowMap; }

	//�v���C���[���W���擾
	const MyLibrary::LibVec3 GetPlayerPos() const;

private:
	//�X�}�[�g�|�C���^�Ǘ�
	std::shared_ptr<MyLibrary::Physics> m_pPhysics;
	std::shared_ptr<Player> m_pPlayer = std::make_shared<Player>();
	std::shared_ptr<Map> m_pMap = std::make_shared<Map>();
	std::shared_ptr<Camera> m_pCamera = std::make_shared<Camera>();
	std::shared_ptr<CollidableNpc> m_pNpc = std::make_shared<CollidableNpc>();
	std::shared_ptr<EnemyManager> m_pEnemy;

	//���݂̃}�b�v
	eMapName m_nowMap;
	//�X�e�[�W��
	//std::string m_stageName;
};

