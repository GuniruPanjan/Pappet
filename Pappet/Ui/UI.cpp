#include "UI.h"

/// <summary>
/// コンストラクタ
/// </summary>
UI::UI() :
	m_equipmentReturn(false)
{
}

/// <summary>
/// デストラクタ
/// </summary>
UI::~UI()
{
}

/// <summary>
/// 初期化処理
/// </summary>
/// <param name="player"></param>
/// <param name="enemy"></param>
void UI::Init()
{
}

/// <summary>
/// 装備の描画処理
/// </summary>
/// <param name="player"></param>
void UI::EquipmentDraw(Player& player)
{
	//拳
	DrawGraph(110, 310, m_fist, true);

	//拳
	DrawGraph(310, 310, m_fist, true);

	//裸体
	DrawGraph(565, 320, m_body, true);
}

/// <summary>
/// 右武器描画処理
/// </summary>
void UI::RightDraw()
{
	//拳
	DrawGraph(70, 56, m_fist, true);
	//黒い剣
	DrawGraph(100, 210, m_blackSword, true);
}

/// <summary>
/// 左武器描画処理
/// </summary>
void UI::LeftDraw()
{
	//拳
	DrawGraph(70, 56, m_fist, true);
}

/// <summary>
/// 防具描画処理
/// </summary>
void UI::ArmorDraw()
{
	//裸
	DrawGraph(120, 66, m_body, true);
}

/// <summary>
/// 終了処理
/// </summary>
void UI::End()
{
}
