#pragma once

/// <summary>
/// アイテムを管理するクラス
/// </summary>
class ItemManager
{
private:
	//アイテムの構造体
	struct Item
	{
		int SmallCore = 0;       //小型コア
		int MediumCore = 0;      //中型コア
		int Rubbish = 0;         //ゴミ
		int BlackSword = 0;      //黒い剣
		int Distorted = 0;       //歪んだ盾
	};

public:
	//コンストラクタ
	ItemManager();
	//デストラクタ
	virtual ~ItemManager();
	//初期化処理
	void Init();
	//更新処理
	void Update();

	//描画処理
	void Draw();

	//終了処理
	void End();

private:

};

