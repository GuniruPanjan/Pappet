#pragma once
#include "Library/MyLibrary.h"
#include "Object/SearchObject.h"
#include "Object/RectObject.h"
#include <string>

/// <summary>
/// マップの基底クラス
/// </summary>
class MapBase
{
public:
	//コンストラクタ
	MapBase();
	//デストラクタ
	virtual ~MapBase();

	//初期化
	virtual void Init() {};
	//終了
	virtual void Finalize(std::shared_ptr<MyLibrary::Physics> physics);
	//更新
	virtual void Update(MyLibrary::LibVec3 playerPos) {};
	//描画
	virtual void Draw() {};

protected:
	//モデルを読み込む
	void LoadData(std::string mapPath, std::string collisionPath);
	//索敵判定をする当たり判定を作成
	void InitSearch(float radius, MyLibrary::LibVec3 pos);
	//ボス部屋の入口
	void InitRect(float width, float hight, float depth, MyLibrary::LibVec3 pos);

protected:
	//物理クラスのポインタ
	std::shared_ptr<MyLibrary::Physics> m_pPhysics;

	std::shared_ptr<SearchObject> m_pSearch;   //索敵判定
	std::shared_ptr<RectObject> m_pRect;       //矩形判定

	int m_handle;             //モデルのハンドル
	int m_collisionHandle;    //モデルの当たり判定用のハンドル
};

