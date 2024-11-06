#include "Bear.h"

namespace
{
	//キャラクター名
	constexpr const char* cCharacterName = "bear";
	//モデルパス
	constexpr const char* cModelPath = "Data/Enemy/BearModel.mv1";
	//モデルのサイズの拡大率
	constexpr float cModelSize = 0.7f;
	//モデルの長さ
	constexpr float cCapsuleLen = 40.0f;
	//モデルの半径
	constexpr float cCapsuleRadius = 25.0f;
	//モデルの座標を合わせる
	constexpr float cModelPosY = 24.0f;
	//索敵範囲
	constexpr float cSearchRadius = 120.0f;
}

/// <summary>
/// コンストラクタ
/// </summary>
Bear::Bear() :
	EnemyBase(Collidable::Priority::High)
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
Bear::~Bear()
{
	//メモリ解放
	MV1DeleteModel(m_modelHandle);
}

/// <summary>
/// 初期化
/// </summary>
/// <param name="posX"></param>
/// <param name="posY"></param>
/// <param name="posZ"></param>
/// <param name="physics"></param>
void Bear::Init(float posX, float posY, float posZ, std::shared_ptr<MyLibrary::Physics> physics)
{
	//代入
	m_pPhysics = physics;

	m_isExist = true;

	Collidable::Init(m_pPhysics);

	//物理クラスの初期化
	InitRigidbody(posX, posY + cCapsuleRadius, posZ);

	//中心座標の設定
	CalculationCenterPos(1.0f, cModelSize);

	//モデルの座標を設定
	SetModelPos();
	MV1SetPosition(m_modelHandle, m_modelPos.ConversionToVECTOR());

	//索敵判定をする当たり判定を作成
	InitSearch(cSearchRadius, 35.0f);

	//モデルのサイズ設定
	MV1SetScale(m_modelHandle, VGet(cModelSize, cModelSize, cModelSize));

	//アニメーション設定
	m_nowAnimNo = MV1AttachAnim(m_modelHandle, m_animIdx["Idle"]);
	m_nowAnimIdx = m_animIdx["Idle"];
}

/// <summary>
/// 更新処理
/// </summary>
/// <param name="playerPos"></param>
/// <param name="isChase"></param>
void Bear::Update(MyLibrary::LibVec3 playerPos, bool isChase)
{
	//アニメーションの更新
	m_isAnimationFinish = UpdateAnim(m_nowAnimNo, ANIMATION_MAX);

	//rigidbodyのポジションがいかれてる
	// そもそも最初に当たるのがおかしい
	//m_modelPosは無事
	//m_collisionPosも無事

	//判定の更新
	MyLibrary::LibVec3 centerPos = rigidbody.GetPos();
	m_pSearch->Update(centerPos);
}

/// <summary>
/// 描画処理
/// </summary>
void Bear::Draw()
{
	//当たり判定座標を取得してモデルの描画座標を設定する
	SetDrawModelPos(cModelPosY);
	//モデルの描画
	MV1DrawModel(m_modelHandle);
}
