#include "Immortal.h"

namespace
{
	//キャラクター名
	constexpr const char* cCharacterName = "Immortal";
	//モデルパス
	constexpr const char* cModelPath = "Data/Enemy/immortalModel.mv1";
	//モデルのサイズの拡大率
	constexpr float cModelSize = 0.4f;
	//モデルの長さ
	constexpr float cCapsuleLen = 40.0f;
	//モデルの半径
	constexpr float cCapsuleRadius = 14.0f;
	//モデルの座標を合わせる
	constexpr float cModelPosY = 12.0f;
	//死亡終了
	bool cDead = false;
	//死亡したときのアニメーション
	constexpr float cDeadFrame = 116.0f;
	//索敵範囲
	constexpr float cSearchRadius = 120.0f;

}

/// <summary>
/// コンストラクタ
/// </summary>
Immortal::Immortal() :
	EnemyBase(Collidable::Priority::Middle)
{
	//当たり判定の設定
	InitCollision(MyLibrary::LibVec3(0.0f, 2.0f, 0.0f), cCapsuleLen, cCapsuleRadius);

	//モデルの読み込み
	LoadModel(cModelPath);
	//アニメーションやステータスを取得
	LoadData(cCharacterName);
	//索敵範囲の設定
	m_searchRadius = cSearchRadius;
}

/// <summary>
/// デストラクタ
/// </summary>
Immortal::~Immortal()
{
	//メモリ解放
	MV1DeleteModel(m_modelHandle);
}

/// <summary>
/// 初期化
/// </summary>
/// <param name="physics">物理クラスのポインタ</param>
void Immortal::Init(float posX, float posY, float posZ, std::shared_ptr<MyLibrary::Physics> physics)
{

	//代入
	m_pPhysics = physics;

	m_isExist = true;

	Collidable::Init(m_pPhysics);

	//物理クラスの初期化
	InitRigidbody(posX, posY + 14.0f, posZ);

	//中心座標の設定
	CalculationCenterPos(1.0f, cModelSize);

	//モデルの座標を設定
	SetModelPos();
	MV1SetPosition(m_modelHandle, m_modelPos.ConversionToVECTOR());

	//索敵判定をする当たり判定を作成
	InitSearch(cSearchRadius, 0.0f);

	//モデルのサイズ設定
	MV1SetScale(m_modelHandle, VGet(cModelSize, cModelSize, cModelSize));

	//アニメーション設定
	m_nowAnimNo = MV1AttachAnim(m_modelHandle, m_animIdx["Idle"]);
	m_nowAnimIdx = m_animIdx["Idle"];

	//死をfalseにする
	m_anim.s_isDead = false;
	cDead = false;
}

/// <summary>
/// 更新
/// </summary>
/// <param name="playerPos">プレイヤー座標</param>
/// <param name="isChase">プレイヤーと戦えるかどうか</param>
void Immortal::Update(MyLibrary::LibVec3 playerPos, bool isChase)
{
	//アニメーションの更新
	if (!cDead && m_nowFrame <= cDeadFrame)
	{
		m_isAnimationFinish = UpdateAnim(m_nowAnimNo, ANIMATION_MAX);
	}

	UpdateAnimationBlend();

	//ターゲット状態
	TargetNow();
	//攻撃を受けた時
	//攻撃が当たっているとき
	if (m_isEnterHit)
	{
		m_status.s_hp -= m_col->GetAttack() - m_status.s_defense;
	}

	TriggerUpdate();
	HitTriggerUpdate();

	//判定の更新
	MyLibrary::LibVec3 centerPos = rigidbody.GetPos();
	m_pSearch->Update(centerPos);

	//死んだとき
	if (m_status.s_hp <= 0.0f)
	{
		Death();
	}
	//死亡が終了した瞬間
	if (m_anim.s_isDead && m_isAnimationFinish)
	{
		cDead = true;
	}
}

/// <summary>
/// 描画
/// </summary>
void Immortal::Draw()
{
	//当たり判定座標を取得してモデルの描画座標を設定する
	SetDrawModelPos(cModelPosY);
	//モデルの描画
	MV1DrawModel(m_modelHandle);
}
