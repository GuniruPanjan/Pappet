#pragma once
#include "EnemyBase.h"
#include "Library/MyLibrary.h"
#include "DxLib.h"
#include <map>

/// <summary>
/// ボス敵のクラス
/// </summary>
class Bear : public EnemyBase
{
public:
	//コンストラクタ
	Bear();
	//デストラクタ
	virtual ~Bear();

	//初期化
	void Init(float posX, float posY, float posZ, std::shared_ptr<MyLibrary::Physics> physics) override;
	//更新
	void Update(MyLibrary::LibVec3 playerPos, bool isChase);
	//描画
	void Draw();
};

