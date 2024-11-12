#pragma once
#include "MapBase.h"

/// <summary>
/// 最初のマップクラス
/// </summary>
class MapFirst : public MapBase
{
public:
	//コンストラクタ
	MapFirst();
	//デストラクタ
	virtual ~MapFirst();

	//データの初期化
	virtual void DataInit();
	//初期化
	virtual void Init(std::shared_ptr<MyLibrary::Physics> physics);
	//更新
	virtual std::shared_ptr<MapBase> Update();
	//当たったかの判定
	virtual void JudgeUpdate();
	//描画
	virtual void Draw();
	//終了
	virtual void End(std::shared_ptr<MyLibrary::Physics> physics);

};

