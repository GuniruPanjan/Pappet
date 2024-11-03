#include "Shield.h"
#include "Manager/HandleManager.h"

namespace
{
	//武器の行列
	constexpr float cShieldMatrixY = 3.142f;
	constexpr float cShieldMatrixZ = 0.0f;

	//シングルトン
	auto& handle = HandleManager::GetInstance();
}

/// <summary>
/// コンストラクタ
/// </summary>
Shield::Shield()
{
}

/// <summary>
/// デストラクタ
/// </summary>
Shield::~Shield()
{
	//メモリ解放
	MV1DeleteModel(m_itemHandle);

	handle.Clear();
}

/// <summary>
/// 初期化処理
/// </summary>
void Shield::Init()
{
	//盾の初期化
	m_itemHandle = handle.GetModelHandle("Data/Weapon/Shield.mv1");
	m_shield.ss_cut = 90.0f;
	m_shield.ss_strength = 20.0f;
}

/// <summary>
/// 更新処理
/// </summary>
/// <param name="mat">左手の行列</param>
void Shield::Update(MATRIX mat)
{
	MV1SetMatrix(m_itemHandle, MGetIdent());

	//フレームを検索
	m_frameIndex = MV1SearchFrame(m_itemHandle, "0:fbx");

	m_framePos = MV1GetFramePosition(m_itemHandle, m_frameIndex);

	m_pos = VAdd(m_framePos, m_pos);

	//アタッチするモデルをフレームの座標を原点にするための平行移動行列を作成
	m_transMatrix = MGetTranslate(VScale(m_pos, -1.0f));

	m_transMatrix = MMult(m_transMatrix, MGetRotY(cShieldMatrixY));
	m_transMatrix = MMult(m_transMatrix, MGetRotZ(cShieldMatrixZ));

	m_mixMatrix = MMult(m_transMatrix, mat);

	MV1SetMatrix(m_itemHandle, m_mixMatrix);
}

/// <summary>
/// 描画処理
/// </summary>
void Shield::Draw()
{
	//モデル描画
	MV1DrawModel(m_itemHandle);
}

void Shield::End()
{
	//メモリ解放
	MV1DeleteModel(m_itemHandle);

	handle.Clear();
}
