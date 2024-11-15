#include "MapRest.h"

namespace
{
	//マップのパス
	constexpr const char* cMapName = "Data/Map/RestMap.mv1";
	//マップのコリジョンパス
	constexpr const char* cMapCollisionName = "Data/Map/RestMapCollision.mv1";
	//コアのパス
	constexpr const char* cCoreName = "Data/Object/Core.mv1";
	//休息地点の半径
	constexpr float cRestRadius = 100.0f;
	//コアの半径
	constexpr float cCoreRadius = 70.0f;
	//コアのサイズ
	constexpr float cCoreSize = 0.5f;
}

/// <summary>
/// コンストラクタ
/// </summary>
MapRest::MapRest()
{
}

/// <summary>
/// デストラクタ
/// </summary>
MapRest::~MapRest()
{
	//メモリ解放
	MV1DeleteModel(m_handle);
	MV1DeleteModel(m_collisionHandle);
	MV1DeleteModel(m_coreHandle);
	DeleteLightHandle(m_light);
}

/// <summary>
/// データの初期化
/// </summary>
void MapRest::DataInit()
{
	//データロード
	LoadData(cMapName, cMapCollisionName, cCoreName);
}

/// <summary>
/// 初期化処理
/// </summary>
/// <param name="physics"></param>
void MapRest::Init(std::shared_ptr<MyLibrary::Physics> physics)
{
	m_pPhysics = physics;

	//マップのサイズ
	m_size = 0.12f;

	//モデルのサイズ変更
	MV1SetScale(m_handle, VGet(m_size, m_size, m_size));
	MV1SetScale(m_collisionHandle, VGet(m_size, m_size, m_size));

	m_mapPos = VGet(0.0f, 0.0f, 0.0f);
	m_mapCollisionPos = VGet(0.0f, 0.0f, 0.0f);
	m_mapCorePos = VGet(-830.0f, 50.0f, 0.0f);

	//ライト関係
	ChangeLightTypeDir(VGet(-1.0f, 0.0f, 0.0f));
	m_light = CreateDirLightHandle(VGet(1.0f, 0.0f, 0.0f));
}

/// <summary>
/// 更新処理
/// </summary>
/// <returns></returns>
std::shared_ptr<MapBase> MapRest::Update(bool warp)
{
	if (warp)
	{

	}

	return std::shared_ptr<MapBase>();
}

/// <summary>
/// 当たったかの判定
/// </summary>
void MapRest::JudgeUpdate()
{
}

/// <summary>
/// ボスが死んだら出す
/// </summary>
void MapRest::CoreUpdate()
{
}

/// <summary>
/// 描画処理
/// </summary>
void MapRest::Draw()
{
	//3Dモデルのポジション設定
	MV1SetPosition(m_handle, m_mapPos);
	MV1SetPosition(m_collisionHandle, m_mapCollisionPos);

	//3Dモデル描画
	MV1DrawModel(m_handle);
}

/// <summary>
/// コア描画処理
/// </summary>
void MapRest::CoreDraw()
{
}

/// <summary>
/// 終了処理
/// </summary>
/// <param name="physics"></param>
void MapRest::End(std::shared_ptr<MyLibrary::Physics> physics)
{
	//メモリ解放
	MV1DeleteModel(m_handle);
	MV1DeleteModel(m_collisionHandle);
	MV1DeleteModel(m_coreHandle);
	DeleteLightHandle(m_light);

	Finalize(physics);
}
