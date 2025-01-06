#pragma once
#include "DxLib.h"
#include "Library/MyLibrary.h"
#include "Manager/MapManager.h"
#include "Item/Weapon.h"
#include "Item/Shield.h"
#include "Item/Armor.h"
#include "Manager/CoreManager.h"
#include "ItemManager.h"
#include "BgmManager.h"
#include<memory>

class Player;
class CollidableNpc;
class Map;
class Camera;
class EnemyManager;
class Setting;
class ItemManager;
class UI;

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
	void GameInit();
	void Update();
	void Draw();
	void ChangeStage(const char* stageName);
	void End();

	void EquipmentUpdate();
	void EquipmentDraw();

	//���݂̃}�b�v�����擾
	const int GetThisMapName() const { return m_nowMap; }

	//�v���C���[���W���擾
	const MyLibrary::LibVec3 GetPlayerPos() const;

	//�^�C�g���ɖ߂邩�̔�����擾
	bool GetTitle() { return m_title; }

private:
	//�X�}�[�g�|�C���^�Ǘ�
	std::shared_ptr<MyLibrary::Physics> m_pPhysics;
	std::shared_ptr<Player> m_pPlayer = std::make_shared<Player>();
	//std::shared_ptr<Map> m_pMap = std::make_shared<Map>();
	std::shared_ptr<MapManager> m_pMap = std::make_shared<MapManager>();
	std::shared_ptr<Camera> m_pCamera = std::make_shared<Camera>();
	std::shared_ptr<CollidableNpc> m_pNpc = std::make_shared<CollidableNpc>();
	std::shared_ptr<EnemyManager> m_pEnemy;
	std::shared_ptr<Setting> m_pSetting = std::make_shared<Setting>();
	std::shared_ptr<UI> m_pUi;
	std::shared_ptr<Weapon> m_pWeapon = std::make_shared<Weapon>();
	std::shared_ptr<Shield> m_pShield = std::make_shared<Shield>();
	std::shared_ptr<Armor> m_pArmor = std::make_shared<Armor>();
	std::shared_ptr<CoreManager> m_pCore = std::make_shared<CoreManager>();
	std::shared_ptr<ItemManager> m_pItem;
	std::shared_ptr<BgmManager> m_pBgm = std::make_shared<BgmManager>();

	//���݂̃}�b�v
	eMapName m_nowMap;
	//�X�e�[�W��
	//std::string m_stageName;

	//���S�ŏ���������
	bool m_deadInit;
	//����������
	bool m_init;
	//�^�C�g���ɖ߂�
	bool m_title;
};

