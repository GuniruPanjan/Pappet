#pragma once
#include "DxLib.h"
#include "Library/MyLibrary.h"
#include <cmath>

class EnemyManager;
class Player;
class Map;

/// <summary>
/// カメラを管理するクラス
/// </summary>
class Camera
{
public:
	//コンストラクタ
	Camera();
	//デストラクタ
	virtual ~Camera();

	//初期化処理
	void Init();
	//更新処理
	void Update(Player& player);
	//ロックオン処理
	void LockUpdate(Player& player, EnemyManager& enemy);
	//描画処理
	void Draw();
	//終了処理
	void End();
	//カメラが向いてる方向ベクトルを取得
	const MyLibrary::LibVec3 GetDirection() const;

	VECTOR GetAngle() { return m_cameraAngle; }         //カメラのアングル所得

private:
	DINPUT_JOYSTATE input;

	//カメラのポジション
	VECTOR m_cameraPos = VGet(0.0f, 0.0f, 0.0f);

	//カメラのターゲット
	VECTOR m_cameraTarget = VGet(0.0f, 0.0f, 0.0f);

	//カメラのアングル
	VECTOR m_cameraAngle = VGet(0.0f, 0.0f, 0.0f);

	float m_x, m_z;
	float m_radius;

};