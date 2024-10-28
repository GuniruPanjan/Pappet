#pragma once
#include "DxLib.h"
#include "Library/MyLibrary.h"
#include<memory>

class Player;
class Map;
class Camera;

/// <summary>
/// ゲームの内容を管理するクラス
/// </summary>
class GameManager
{
public:
	GameManager();
	virtual ~GameManager();

	void Init();
	void Update();
	void Draw();
	void End();

private:
	//スマートポインタ管理
	std::shared_ptr<MyLibrary::Physics> pPhysics;
	std::shared_ptr<Player> pPlayer = std::make_shared<Player>();
	std::shared_ptr<Map> pMap = std::make_shared<Map>();
	std::shared_ptr<Camera> pCamera = std::make_shared<Camera>();

};

