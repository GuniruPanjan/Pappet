#include "Weapon.h"
#include "Manager/HandleManager.h"

namespace
{
	//行列
	constexpr float cWeaponMatrixY = 5.655f;
	constexpr float cWeaponMatrixZ = 1.795f;

	//シングルトン
	auto& handle = HandleManager::GetInstance();
}

/// <summary>
/// コンストラクタ
/// </summary>
Weapon::Weapon()
{
}

/// <summary>
/// デストラクタ
/// </summary>
Weapon::~Weapon()
{
	//メモリ解放
	MV1DeleteModel(m_itemHandle);

	//メモリ解放
	handle.Clear();
}

/// <summary>
/// 初期化処理
/// </summary>
void Weapon::Init()
{
	//黒い剣初期化
	m_weapon.sw_attack = 30.0f;
	m_weapon.sw_radius = 25.0f;
	m_itemHandle = handle.GetModelHandle("Data/Weapon/Sword.mv1");
}

/// <summary>
/// 更新処理
/// </summary>
/// <param name="mat">行列</param>
void Weapon::Update(MATRIX mat)
{
	MV1SetMatrix(m_itemHandle, MGetIdent());
	//フレーム検索
	m_frameIndex = MV1SearchFrame(m_itemHandle, "2:Sphere");
	//フレームのポジション
	m_framePos = MV1GetFramePosition(m_itemHandle, m_frameIndex);
	//アタッチするモデルをフレームの座標を原点にするための平行移動行列を作成
	m_transMatrix = MGetTranslate(VScale(m_pos, -1.0f));

	m_transMatrix = MMult(m_transMatrix, MGetRotY(cWeaponMatrixY));
	m_transMatrix = MMult(m_transMatrix, MGetRotZ(cWeaponMatrixZ));

	m_mixMatrix = MMult(m_transMatrix, mat);

	MV1SetMatrix(m_itemHandle, m_mixMatrix);
}

/// <summary>
/// 描画処理
/// </summary>
/// <param name="mat">行列</param>
void Weapon::Draw(MATRIX mat)
{
	//モデルのポジション
	m_pos = VAdd(m_framePos, m_pos);
}

/// <summary>
/// 終了処理
/// </summary>
void Weapon::End()
{
	//メモリ解放
	MV1DeleteModel(m_itemHandle);

	//メモリ解放
	handle.Clear();
}
