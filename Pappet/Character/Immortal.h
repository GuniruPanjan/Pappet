#pragma once
#include "DxLib.h"
#include "Library/MyLibrary.h"
#include "EnemyBase.h"
#include <map>

/// <summary>
/// 雑魚敵のクラス
/// </summary>
class Immortal : public EnemyBase
{
public:
	//コンストラクタ
	Immortal();
	//デストラクタ
	virtual ~Immortal();

	//初期化
	void Init(float posX, float posY, float posZ, std::shared_ptr<MyLibrary::Physics> physics) override;
	//更新
	void Update(MyLibrary::LibVec3 playerPos, bool isChase);
	//描画
	void Draw();
};

