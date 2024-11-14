#include "EnemyBase.h"
#include "Manager/HandleManager.h"
#include "External/CsvLoad.h"

namespace
{
	//アニメーションの切り替えにかかるフレーム数
	constexpr float cAnimChangeFrame = 5.0f;
	//アニメーションの切り替え速度
	constexpr float cAnimChangeRateSpeed = 1.0f / cAnimChangeFrame;

	//アニメーションブレンド率の最大
	constexpr float cAnimBlendRateMax = 1.0f;

	//シングルトン
	auto& handle = HandleManager::GetInstance();
}

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="priority">優先度</param>
EnemyBase::EnemyBase(Priority priority) :
	CharacterBase(priority, ObjectTag::Enemy),
	m_randomAction(0),
	m_dropCore(0),
	m_hpRadius(0.0f),
	m_moveTurning(0.0f),
	m_moveReverseTurning(0.0f),
	m_difPSize(0.0f),
	m_move(VGet(0.0f,0.0f,0.0f)),
	m_difPlayer(VGet(0.0f,0.0f,0.0f)),
	m_isExist(false),
	m_isDroped(false),
	m_isDiscovery(false),
	m_isBossDiscovery(false),
	m_isTarget(false),
	m_isStayTarget(false),
	m_isExitTarget(false),
	m_isEnterHit(false),
	m_isExitHit(false),
	m_centerPos(),
	m_I(0)
{
}

/// <summary>
/// デストラクタ
/// </summary>
EnemyBase::~EnemyBase()
{
	//メモリ解放
	MV1DeleteModel(m_modelHandle);
}

void EnemyBase::Finalize(std::shared_ptr<MyLibrary::Physics> physics)
{
	Collidable::Finalize(physics);

	m_pSearch->Finalize(physics);
}

/// <summary>
/// 終了処理
/// </summary>
void EnemyBase::End()
{
	//メモリ解放
	MV1DeleteModel(m_modelHandle);

	handle.Clear();
}

void EnemyBase::OnCollideEnter(const std::shared_ptr<Collidable>& collidable)
{
#if _DEBUG
	std::string message = "敵が";
#endif
	auto tag = collidable->GetTag();
	switch (tag)
	{
	case ObjectTag::Player:
#if _DEBUG
		message += "プレイヤー";
#endif
		break;
	case ObjectTag::Enemy:
#if _DEBUG
		message += "敵";
#endif
		break;
	}
#if _DEBUG
	message += "と当たった\n";
	printfDx(message.c_str());
#endif
}

void EnemyBase::OnCollideStay(const std::shared_ptr<Collidable>& collidable)
{
#if _DEBUG
	std::string message = "敵が";
#endif
	auto tag = collidable->GetTag();
	switch (tag)
	{
	case ObjectTag::Player:
#if _DEBUG
		message += "プレイヤー";
#endif
		break;
	case ObjectTag::Enemy:
#if _DEBUG
		message += "敵";
#endif
		break;
	}
#if _DEBUG
	message += "と当たった\n";
	printfDx(message.c_str());
#endif
}

void EnemyBase::OnTriggerEnter(const std::shared_ptr<Collidable>& collidable)
{
#if _DEBUG
	std::string message = "敵が";
#endif
	auto tag = collidable->GetTag();
	switch (tag)
	{
	case ObjectTag::Attack:
#if _DEBUG
		message += "攻撃";
#endif
		m_col = dynamic_cast<AttackObject*>(collidable.get());
		m_isEnterHit = true;
		break;
	case ObjectTag::Search:
#if _DEBUG
		message += "索敵範囲";
#endif
		m_isStayTarget = true;
		break;
	case ObjectTag::Rect:
#if _DEBUG
		message += "ボス部屋入口";
#endif
		break;
	default:
		break;
	}

#if _DEBUG
	message += "と当たった\n";
	printfDx(message.c_str());
#endif
}

void EnemyBase::OnTriggerStay(const std::shared_ptr<Collidable>& collidable)
{
#if _DEBUG
	std::string message = "敵が";
#endif
	auto tag = collidable->GetTag();
	switch (tag)
	{
	case ObjectTag::Attack:
#if _DEBUG
		message += "攻撃";
#endif
		break;
	case ObjectTag::Search:
#if _DEBUG
		message += "索敵範囲";
#endif
		m_isStayTarget = true;
		break;
	case ObjectTag::Rect:
#if _DEBUG
		message += "ボス部屋入口";
#endif
		break;
	default:
		break;
	}

#if _DEBUG
	message += "と当たった\n";
	printfDx(message.c_str());
#endif
}

void EnemyBase::OnTriggerExit(const std::shared_ptr<Collidable>& collidable)
{
#if _DEBUG
	std::string message = "敵が";
#endif
	auto tag = collidable->GetTag();
	switch (tag)
	{
	case ObjectTag::Attack:
#if _DEBUG
		message += "攻撃";
#endif
		m_isExitHit = true;
		break;
	case ObjectTag::Search:
#if _DEBUG
		message += "索敵範囲";
#endif
		m_isExitTarget = true;
		break;
	case ObjectTag::Rect:
#if _DEBUG
		message += "ボス部屋入口";
#endif
		break;
	default:
		break;
	}

#if _DEBUG
	message += "が外れた\n";
	printfDx(message.c_str());
#endif
}

bool EnemyBase::GetStay() const
{
	return m_isStayTarget;
}

bool EnemyBase::GetExit()
{
	return m_isExitTarget;
}

bool EnemyBase::GetIsHit()
{
	bool log = m_anim.s_hit;
	m_anim.s_hit = false;
	return log;
}

int EnemyBase::GetDropCore()
{
	m_isDroped = true;
	return m_dropCore;
}

const float EnemyBase::GetRadius() const
{
	//もし半径0(索敵範囲なし)ならとりあえずの値を返す
	if (m_hpRadius == 0.0f)
	{
		return 5.0f;
	}

	//0でなければ索敵範囲の半径をそのまま返す
	return m_hpRadius;
}

/// <summary>
/// 他のオブジェクトと押し出し判定をする当たり判定を作成
/// </summary>
/// <param name="vec">ベクター</param>
/// <param name="len">長さ</param>
/// <param name="radius">半径</param>
void EnemyBase::InitCollision(MyLibrary::LibVec3 vec, float len, float radius)
{
	auto collider = Collidable::AddCollider(MyLibrary::CollidableData::Kind::Capsule, false);
	auto capsuleCol = dynamic_cast<MyLibrary::CollidableDataCapsule*>(collider.get());
	capsuleCol->m_len = len;
	capsuleCol->m_radius = radius;
	capsuleCol->m_vec = MyLibrary::LibVec3(vec.x, capsuleCol->m_vec.y + vec.y, vec.z);
}

/// <summary>
/// モデルを読み込む
/// </summary>
/// <param name="path">モデルパス</param>
void EnemyBase::LoadModel(std::string path)
{
	m_modelHandle = handle.GetModelHandle(path);
}

/// <summary>
/// アニメーション情報や、ステータス情報を読み込む
/// </summary>
/// <param name="name">キャラクター名</param>
void EnemyBase::LoadData(std::string name)
{
	CsvLoad::GetInstance().AnimDataLoad(name, m_animIdx);
	CsvLoad::GetInstance().StatusLoad(m_status, name.c_str());
	m_dropCore = m_status.s_core;
}

/// <summary>
/// ステータス情報の初期化
/// </summary>
/// <param name="name">キャラクター名</param>
void EnemyBase::LoadDataInit(std::string name)
{
	CsvLoad::GetInstance().StatusLoad(m_status, name.c_str());
}

/// <summary>
/// 物理クラスの初期化
/// </summary>
/// <param name="posX">X座標</param>
/// <param name="posY">Y座標</param>
/// <param name="posZ">Z座標</param>
/// <param name="isUseGravity">重力を与えるか与えないか</param>
void EnemyBase::InitRigidbody(float posX, float posY, float posZ, bool isUseGravity)
{
	rigidbody.Init(isUseGravity);
	rigidbody.SetPos(MyLibrary::LibVec3(posX, posY, posZ));
	rigidbody.SetNextPos(rigidbody.GetPos());
	m_collisionPos = rigidbody.GetPos();
}

/// <summary>
/// モデルの中心座標を計算
/// </summary>
/// <param name="modeldefaultSize">モデルのもともとのサイズ</param>
/// <param name="modelSize">モデルの拡大率</param>
void EnemyBase::CalculationCenterPos(float modeldefaultSize, float modelSize)
{
	m_centerPos = rigidbody.GetPos();
	m_centerPos.y -= (modeldefaultSize * modelSize) / 2;
}

/// <summary>
/// モデル座標を設定
/// </summary>
void EnemyBase::SetModelPos()
{
	m_modelPos = m_collisionPos;
}

/// <summary>
/// 索敵判定をする当たり判定を作成
/// </summary>
/// <param name="radius">半径</param>
void EnemyBase::InitSearch(float radius, float y)
{
	m_pSearch = std::make_shared<SearchObject>(radius);
	m_pSearch->Init(m_pPhysics, MyLibrary::LibVec3(m_modelPos.x, m_modelPos.y + y, m_modelPos.z), true);

	m_hpRadius = radius;
}

/// <summary>
/// 攻撃を初期化する
/// </summary>
/// <param name="radius">半径</param>
void EnemyBase::InitAttack(float radius)
{
	m_pAttack = std::make_shared<EnemyAttackObject>(radius);
}

/// <summary>
/// 攻撃判定を初期化する
/// </summary>
/// <param name="attack">攻撃力</param>
void EnemyBase::InitAttackUpdate(float attack)
{
	m_pAttack->SetAttack(attack);
	m_pAttack->Init(m_pPhysics);
}

void EnemyBase::TriggerUpdate()
{
	m_isStayTarget = false;
}

/// <summary>
/// 距離を測る処理
/// </summary>
/// <param name="playerPos">プレイヤーのポジション</param>
void EnemyBase::DistanceUpdate(MyLibrary::LibVec3 playerPos)
{
	//プレイヤーとの距離
	m_difPlayer = VSub(playerPos.ConversionToVECTOR(), m_modelPos.ConversionToVECTOR());

	//プレイヤーとの距離のサイズ
	m_difPSize = VSize(m_difPlayer);
}

/// <summary>
/// 索敵で発見した処理
/// </summary>
/// <param name="playerPos">プレイヤーのポジション</param>
void EnemyBase::AngleUpdate(MyLibrary::LibVec3 playerPos)
{
	float X = m_modelPos.x - playerPos.ConversionToVECTOR().x;
	float Z = m_modelPos.z - playerPos.ConversionToVECTOR().z;

	//プレイヤーの方を向く
	m_angle = atan2f(X, Z);

	//法線ベクトル
	m_moveTurning = atan2f(-X, Z);
	m_moveReverseTurning = atan2f(X, -Z);
}

/// <summary>
/// 移動をする処理
/// </summary>
void EnemyBase::MoveUpdate()
{
	MyLibrary::LibVec3 prevVelocity = rigidbody.GetVelocity();
	MyLibrary::LibVec3 newVelocity = MyLibrary::LibVec3(m_moveVec.x, prevVelocity.y, m_moveVec.z);
	rigidbody.SetVelocity(newVelocity);
}

/// <summary>
/// アイドル状態に遷移する
/// </summary>
void EnemyBase::IdleUpdate()
{
	m_nowAnimIdx = m_animIdx["Idle"];
	ChangeAnim(m_nowAnimIdx, m_animOne[0], m_animOne);
}

/// <summary>
/// 歩いている状態に遷移する
/// </summary>
/// <param name="walk">パス</param>
/// <param name="number">番号</param>
void EnemyBase::WalkUpdate(std::string path, int number)
{
	if (m_anim.s_moveflag)
	{
		m_nowAnimIdx = m_animIdx[path];
		ChangeAnim(m_nowAnimIdx, m_animOne[number], m_animOne);
	}
}

/// <summary>
/// 攻撃状態に遷移する
/// </summary>
/// <param name="path">パス</param>
/// <param name="number">番号</param>
void EnemyBase::AttackUpdate(std::string path, int number)
{
	if (m_anim.s_attack)
	{
		m_nowAnimIdx = m_animIdx[path];
		ChangeAnim(m_nowAnimIdx, m_animOne[number], m_animOne);
	}
}

/// <summary>
/// 怯み状態に遷移する
/// </summary>
/// <param name="number">番号</param>
void EnemyBase::HitUpdate(int number)
{
	if (m_anim.s_hit)
	{
		m_nowAnimIdx = m_animIdx["Hit"];
		ChangeAnim(m_nowAnimIdx, m_animOne[number], m_animOne);
	}
}

void EnemyBase::TargetNow()
{
	//ターゲットできる時
	if (GetStay())
	{
		m_isTarget = true;
	}
	//ターゲットできない時
	else if (GetExit())
	{
		m_isTarget = false;
	}
}

void EnemyBase::HitTriggerUpdate()
{
	m_isEnterHit = false;
}

/// <summary>
/// 死亡した時
/// </summary>
void EnemyBase::Death()
{
	if (!m_anim.s_isDead)
	{
		m_anim.s_isDead = true;
		EnemyBase::Finalize(m_pPhysics);

		m_nowAnimIdx = m_animIdx["Death"];
		ChangeAnim(m_nowAnimIdx, m_animOne[1], m_animOne);
	}
}

/// <summary>
/// アニメーションブレンドの更新
/// </summary>
void EnemyBase::UpdateAnimationBlend()
{
	//アニメーションの切り替え
	if (m_prevAnimNo != -1)
	{
		//フレームでアニメーションを切り替える
		m_animBlendRate += cAnimChangeRateSpeed;
		if (m_animBlendRate >= cAnimBlendRateMax)
		{
			m_animBlendRate = cAnimBlendRateMax;
		}

		//アニメーションのブレンド率を設定する
		MV1SetAttachAnimBlendRate(m_modelHandle, m_prevAnimNo, cAnimBlendRateMax - m_animBlendRate);
		MV1SetAttachAnimBlendRate(m_modelHandle, m_nowAnimNo, m_animBlendRate);
	}
}

/// <summary>
/// モデルの座標設定
/// </summary>
/// <param name="offset"></param>
void EnemyBase::SetDrawModelPos(float posY)
{
	rigidbody.SetPos(rigidbody.GetNextPos());
	m_collisionPos = rigidbody.GetPos();
	SetModelPos();
	MV1SetPosition(m_modelHandle, VSub(m_modelPos.ConversionToVECTOR(), VGet(0.0f, posY, 0.0f)));
}
