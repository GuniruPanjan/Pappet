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
	m_dropCore(0),
	m_hpRadius(0.0f),
	m_isExist(false),
	m_isDroped(false),
	m_isDiscovery(false),
	m_centerPos()
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
	m_pAttack->Finalize(m_pPhysics);
	if (m_isDiscovery)
	{
		m_pSearch->Finalize(m_pPhysics);
	}
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
	case ObjectTag::Attack:
#if _DEBUG
		message += "攻撃";
#endif
		break;
	case ObjectTag::Search:
#if _DEBUG
		message += "索敵範囲";
#endif
		break;
	case ObjectTag::BossEnter:
#if _DEBUG
		message += "ボス部屋入口";
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
	case ObjectTag::Attack:
#if _DEBUG
		message += "攻撃";
#endif
		break;
	case ObjectTag::Search:
#if _DEBUG
		message += "索敵範囲";
#endif
		break;
	case ObjectTag::BossEnter:
#if _DEBUG
		message += "ボス部屋入口";
#endif
		break;
	}
#if _DEBUG
	message += "と当たった\n";
	printfDx(message.c_str());
#endif
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
	m_centerPos.y += (modeldefaultSize * modelSize) / 2;
}

/// <summary>
/// モデル座標を設定
/// </summary>
/// <param name="offset">差分</param>
void EnemyBase::SetModelPos(float offset)
{
	m_modelPos = m_collisionPos;
	m_modelPos.y -= offset;
}

/// <summary>
/// 索敵判定をする当たり判定を作成
/// </summary>
/// <param name="radius">半径</param>
void EnemyBase::InitSearch(float radius)
{
	m_pSearch = std::make_shared<SearchObject>(radius);
	m_pSearch->Init(m_pPhysics, m_modelPos, true);

	m_hpRadius = radius;
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

		ChangeAnim(m_nowAnimIdx, m_animOne[0], m_animOne);
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
void EnemyBase::SetDrawModelPos(float offset)
{
	rigidbody.SetPos(rigidbody.GetNextPos());
	m_collisionPos = rigidbody.GetPos();
	SetModelPos(offset);
	MV1SetPosition(m_modelHandle, m_modelPos.ConversionToVECTOR());
}
