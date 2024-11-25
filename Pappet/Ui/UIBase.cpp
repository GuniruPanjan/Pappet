#include "UIBase.h"

/// <summary>
/// コンストラクタ
/// </summary>
UIBase::UIBase()
{
	//画像読み込み
	m_heelStone = MyLoadGraph("Data/UI/HeelStoneMini.png", 6, 6);
	m_eqFist = MyLoadGraph("Data/UI/FistUi.png", 7, 7);
	m_sword = MyLoadGraph("Data/UI/SwordUI.png", 3, 3);
	m_eqSword = MyLoadGraph("Data/UI/SwordUI.png", 7, 7);
	m_eqShield = MyLoadGraph("Data/UI/Shield.png", 7, 7);
	m_equipment = MyLoadGraph("Data/UI/装備画面UI.png", 1, 1);
	m_itemBox = MyLoadGraph("Data/UI/アイテム画面UI.png", 1, 1);
	m_selectEquipment = MyLoadGraph("Data/UI/装備選択画面UI.png", 1, 1);

	m_blackSword = MyLoadGraph("Data/UI/黒い剣UI.png", 3, 3);
	m_fist = MyLoadGraph("Data/UI/拳UI.png", 3, 3);
	m_body = MyLoadGraph("Data/UI/裸体UI.png", 3, 3);
	m_uglyShield = MyLoadGraph("Data/UI/醜い盾UI.png", 4, 4);
	m_commonArmor = MyLoadGraph("Data/UI/平凡な鎧UI.png", 3, 3);
}

/// <summary>
/// デストラクタ
/// </summary>
UIBase::~UIBase()
{
	//メモリ解放
	DeleteGraph(m_heelStone);
	DeleteGraph(m_sword);
	DeleteGraph(m_eqFist);
	DeleteGraph(m_eqSword);
	DeleteGraph(m_eqShield);
	DeleteGraph(m_equipment);
	DeleteGraph(m_itemBox);
	DeleteGraph(m_selectEquipment);
	DeleteGraph(m_blackSword);
	DeleteGraph(m_fist);
	DeleteGraph(m_body);
	DeleteGraph(m_uglyShield);
	DeleteGraph(m_commonArmor);
}

/// <summary>
/// 画像のサイズを変えてロードする関数
/// </summary>
/// <param name="FileName">画像のFileName</param>
/// <param name="XSize">画像の横幅を割る数</param>
/// <param name="YSize">画像の縦幅を割る数</param>
/// <returns>サイズを変えた画像を返す</returns>
int UIBase::MyLoadGraph(const char* FileName, int XSize, int YSize)
{
	int handle = LoadGraph(FileName);  //画像のロード
	if (handle != -1)  //画像のロードに成功した場合
	{
		int SizeX, SizeY;  //画像サイズを格納するための変数を用意

		GetGraphSize(handle, &SizeX, &SizeY);  //ロードした画像のサイズ取得

		int NowScreen = GetDrawScreen();    //現在の描画対象画面を一時保存

		SizeX /= XSize;
		SizeY /= YSize;

		int miniHandle = MakeScreen(SizeX, SizeY, TRUE);  //サイズ変更後のグラフィックハンドルを作成
		if (miniHandle == -1)   //ハンドルの作成に失敗した場合
		{
			DeleteGraph(handle);   //メモリリークしないように、本来のグラフィックハンドルを削除
		}
		SetDrawScreen(miniHandle);   //描画対象画面を先ほど作ったサイズのグラフィックハンドルに変更
		DrawExtendGraph(0, 0, SizeX, SizeY, handle, TRUE);   //ロードした画像を描画
		DeleteGraph(handle);   //不要になった本来のサイズのグラフィックハンドルを削除
		SetDrawScreen(NowScreen);  //現在の描画対象画面を元の画面に戻す

		return miniHandle;  //縮めた画像のグラフィックハンドルを返す
	}
	else   //画像のロードに失敗した場合
	{
		return -1;   //エラー発生
	}
}


