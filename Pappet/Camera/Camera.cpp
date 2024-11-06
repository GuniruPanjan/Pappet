#include "Camera.h"
#include "Character/Player.h"
#include "Manager/EnemyManager.h"
//度をラジアンに変換
#define D2R(deg) ((deg)*DX_PI_F/180.0f)

namespace
{
	VECTOR cPlayerPos = VGet(0.0f, 0.0f, 0.0f);
}

/// <summary>
/// コンストラクタ
/// </summary>
Camera::Camera() :
	input(),
	m_cameraPos(VGet(0.0f,0.0f,0.0f)),
	m_cameraTarget(VGet(0.0f, 0.0f, 0.0f)),
	m_cameraAngle(VGet(0.0f, 0.0f, 0.0f)),
	m_x(0.0f),
	m_z(0.0f),
	m_radius(0.0f)
{
}

/// <summary>
/// デストラクタ
/// </summary>
Camera::~Camera()
{
}

/// <summary>
/// 初期化処理
/// </summary>
void Camera::Init()
{
	m_radius = 3.0f;

	//基準となるカメラの座標
	m_cameraPos = VGet(0.0f, 50.0f, 100.0f);

	//カメラのターゲット座標初期化
	m_cameraTarget = VGet(0.0f, 5.0f, 0.0f);

	//カメラのアングル初期設定
	m_cameraAngle = VGet(D2R(-20.0f), 97.4f, 0.0f);

	SetCameraNearFar(0.001f, 1000.0f);
}

/// <summary>
/// 更新処理
/// </summary>
/// <param name="player">プレイヤーを呼び出す</param>
void Camera::Update(Player& player)
{
	GetJoypadDirectInputState(DX_INPUT_PAD1, &input);

	cPlayerPos = player.GetPos().GetVector();

	//左キー
	if (input.Rx < 0)
	{
		m_cameraAngle.y -= D2R(1.0f);

	}
	//右キー
	if (input.Rx > 0)
	{
		m_cameraAngle.y += D2R(1.0f);

	}
	//上キー
	if (input.Ry < 0)
	{
		//カメラが地面にめりこまないように
		if (m_cameraPos.y >= 15.2f + player.GetPos().y)
		{
			m_cameraAngle.x -= D2R(1.0f);
		}

	}
	//下キー
	if (input.Ry > 0)
	{
		//カメラがプレイヤーを超えないくらいまで
		if (m_cameraAngle.x <= 0.7f)
		{
			m_cameraAngle.x += D2R(1.0f);
		}
	}

	//基準のベクトル
	VECTOR Direction = VGet(0.0f, 112.0f, -112.0f);

	//X軸回転行列
	MATRIX matrixX = MGetRotX(m_cameraAngle.x);
	//Y軸回転行列
	MATRIX matrixY = MGetRotY(m_cameraAngle.y);

	//行列の合成
	MATRIX matrix = MMult(matrixX, matrixY);

	//基準ベクトルを行列で変換
	Direction = VTransform(Direction, matrix);

	//カメラ座標はプレイヤー座標から少しはなれたところ
	m_cameraPos = VAdd(cPlayerPos, Direction);

	//注視点の座標はプレイヤー座標の少し上
	m_cameraTarget = VAdd(cPlayerPos, VGet(0.0f, 50.0f, 0.0f));

	SetCameraPositionAndTarget_UpVecY(m_cameraPos, m_cameraTarget);
}

/// <summary>
/// ロックオン処理
/// </summary>
/// <param name="player">プレイヤーを呼び出す</param>
/// <param name="enemy">エネミーを呼び出す</param>
void Camera::LockUpdate(Player& player, EnemyManager& enemy)
{
	for (auto& enemyPos : enemy.GetEnemyPos())
	{

	}
}

/// <summary>
/// 描画処理
/// </summary>
void Camera::Draw()
{
}

/// <summary>
/// 終了処理
/// </summary>
void Camera::End()
{
}

/// <summary>
/// カメラが向いてる方向ベクトルを取得
/// </summary>
/// <returns>カメラが向いてる方向</returns>
const MyLibrary::LibVec3 Camera::GetDirection() const
{
	return (MyLibrary::LibVec3(m_cameraTarget.x, m_cameraTarget.y, m_cameraTarget.z)
		- MyLibrary::LibVec3(m_cameraPos.x, m_cameraPos.y, m_cameraPos.z)).Normalize();
}
