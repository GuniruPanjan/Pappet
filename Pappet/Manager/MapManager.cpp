#include "MapManager.h"
#include "Map/MapFirst.h"

/// <summary>
/// コンストラクタ
/// </summary>
MapManager::MapManager() :
	m_pMap(nullptr)
{

}

/// <summary>
/// デストラクタ
/// </summary>
MapManager::~MapManager()
{
}

/// <summary>
/// データの初期化
/// </summary>
void MapManager::DataInit()
{
	//最初のシーンのメモリを解放する
	m_pMap = std::make_shared<MapFirst>();

	m_pMap->DataInit();
}

/// <summary>
/// 初期化処理
/// </summary>
void MapManager::Init(std::shared_ptr<MyLibrary::Physics> physics)
{
	m_pMap->Init(physics);
}

/// <summary>
/// 更新処理
/// </summary>
void MapManager::Update(std::shared_ptr<MyLibrary::Physics> physics)
{
	std::shared_ptr<MapBase> pNext = m_pMap->Update();
	if (pNext != m_pMap)
	{
		//現在のマップの終了処理
		m_pMap->End(physics);

		//Updateが返した新しいシーンの開始処理を行う
		m_pMap = pNext;
		m_pMap->DataInit();
		m_pMap->Init(physics);
	}
}

/// <summary>
/// 当たったかの判定更新処理
/// </summary>
void MapManager::JudgeUpdate()
{
	m_pMap->JudgeUpdate();
}

/// <summary>
/// 描画処理
/// </summary>
void MapManager::Draw()
{
	m_pMap->Draw();
}

/// <summary>
/// 終了処理
/// </summary>
void MapManager::End(std::shared_ptr<MyLibrary::Physics> physics)
{
	m_pMap->End(physics);
}
