#pragma once
#include "Library/MyLibrary.h"
#include "Object/SearchObject.h"
#include "Object/RectObject.h"
#include <string>

/// <summary>
/// マップの基底クラス
/// </summary>
class MapBase : public std::enable_shared_from_this<MapBase>
{
public:
	//コンストラクタ
	MapBase();
	//デストラクタ
	virtual ~MapBase();

	//データ初期化
	virtual void DataInit() = 0;
	//初期化
	virtual void Init(std::shared_ptr<MyLibrary::Physics> physics) = 0;
	//終了
	virtual void Finalize(std::shared_ptr<MyLibrary::Physics> physics);
	//更新
	virtual std::shared_ptr<MapBase> Update() = 0;
	//当たったかの判定更新
	virtual void JudgeUpdate() = 0;
	//描画
	virtual void Draw() = 0;
	//終了
	virtual void End(std::shared_ptr<MyLibrary::Physics> physics) = 0;

	//コリジョンのマップハンドルを返す
	int GetCollisionMap() { return m_collisionHandle; }

	//休息できるように当たっているかを返す
	bool GetRest() { return m_pSearch->GetIsStay(); }

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

	int m_light;                            //ライト

	int m_handle;                           //モデルのハンドル
	int m_collisionHandle;                  //モデルの当たり判定用のハンドル
	float m_size;                           //マップのサイズ
	float m_width;                          //幅
	float m_hight;                          //高さ
	float m_depth;                          //奥行

	VECTOR m_mapPos;                        //マップのポジション
	VECTOR m_mapCollisionPos;               //マップのコリジョンのポジション
	MyLibrary::LibVec3 m_mapRestPos;        //休息ポイントのポジション
	MyLibrary::LibVec3 m_mapBossEnterPos;   //ボス部屋入り口のポジション
};

