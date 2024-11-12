#include "MapFirst.h"

namespace
{
	//マップのパス
	constexpr const char* cMapName = "Data/Map/Map.mv1";
	//マップのコリジョンパス
	constexpr const char* cMapCollisionName = "Data/Map/Collision.mv1";
	//休息地点の半径
	constexpr float cRestRadius = 50.0f;
}

/// <summary>
/// コンストラクタ
/// </summary>
MapFirst::MapFirst()
{
}

/// <summary>
/// デストラクタ
/// </summary>
MapFirst::~MapFirst()
{
}

/// <summary>
/// データ初期化
/// </summary>
void MapFirst::DataInit()
{
	//データロード
	LoadData(cMapName, cMapCollisionName);
}

/// <summary>
/// 初期化処理
/// </summary>
void MapFirst::Init(std::shared_ptr<MyLibrary::Physics> physics)
{
	m_pPhysics = physics;

	//マップのサイズ
	m_size = 0.12f;
	m_width = 5.0f;
	m_hight = 50.0f;
	m_depth = 70.0f;

	//モデルのサイズ変更
	MV1SetScale(m_handle, VGet(m_size, m_size, m_size));
	MV1SetScale(m_collisionHandle, VGet(m_size, m_size, m_size));

	//ポジション設定
	m_mapPos = VGet(0.0f, 250.0f, 0.0f);
	m_mapCollisionPos = VGet(-241.0f, -277.0f, -173.0f);
	m_mapRestPos = MyLibrary::LibVec3(100.0f, 0.0f, -75.0f);
	m_mapBossEnterPos = MyLibrary::LibVec3(-10.0f, 50.0f, 0.0f);

	//ライト関係
	ChangeLightTypeDir(VGet(-1.0f, 0.0f, 0.0f));
	m_light = CreateDirLightHandle(VGet(1.0f, 0.0f, 0.0f));

	//索敵判定初期化
	InitSearch(cRestRadius, m_mapRestPos);
	//ボス部屋入り口
	InitRect(m_width, m_hight, m_depth, m_mapBossEnterPos);
}

/// <summary>
/// 更新処理
/// </summary>
/// <returns>現在のマップ</returns>
std::shared_ptr<MapBase> MapFirst::Update()
{
	m_pSearch->Update(m_mapRestPos);
	m_pRect->Update(m_mapBossEnterPos);

	return shared_from_this();   //自身のポインタ
}

/// <summary>
/// 当たったかの判定
/// </summary>
void MapFirst::JudgeUpdate()
{
	if (m_pSearch->GetIsStay())
	{
		int a = 0;
	}
}

/// <summary>
/// 描画処理
/// </summary>
void MapFirst::Draw()
{
	//3Dモデルのポジション設定
	MV1SetPosition(m_handle, m_mapPos);
	MV1SetPosition(m_collisionHandle, m_mapCollisionPos);

	//3Dモデル描画
	MV1DrawModel(m_handle);

	DrawFormatString(200, 100, 0xffffff, "rest : %d", m_pSearch->GetIsStay());
}

/// <summary>
/// 終了処理
/// </summary>
void MapFirst::End(std::shared_ptr<MyLibrary::Physics> physics)
{
	//メモリ解放
	MV1DeleteModel(m_handle);
	MV1DeleteModel(m_collisionHandle);
	DeleteLightHandle(m_light);

	Finalize(physics);
}
