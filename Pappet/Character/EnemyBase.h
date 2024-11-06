#pragma once
#include "CharacterBase.h"

/// <summary>
/// エネミーの基盤となるクラス
/// </summary>
class EnemyBase : public CharacterBase
{
public:
	//コンストラクタ
	EnemyBase(Priority priority);
	//デストラクタ
	virtual ~EnemyBase();

	//初期化処理
	virtual void Init(float posX, float posY, float posZ, std::shared_ptr<MyLibrary::Physics> physics) {};
	//終了
	virtual void Finalize(std::shared_ptr<MyLibrary::Physics> physics);
	//更新
	virtual void Update(MyLibrary::LibVec3 playerPos, bool isChange) {};
	//描画
	virtual void Draw() {};

	//衝突したとき
	void OnCollideEnter(const std::shared_ptr<Collidable>& collidable) override;
	void OnCollideStay(const std::shared_ptr<Collidable>& collidable) override;
	void OnTriggerEnter(const std::shared_ptr<Collidable>& collidable) override;

	//座標を返す
	virtual MyLibrary::LibVec3 GetPos() const { return m_collisionPos; }
	//ダメージを食らったかどうか
	bool GetIsHit();
	//死亡時に落とすコア取得
	int GetDropCore();
	//死亡しているかどうかを取得
	bool GetIsDead() const { return m_anim.s_isDead; }
	//存在しているかどうかを取得
	const bool GetIsExist() const { return m_isExist; }
	//HPを表示するための当たり判定を取得
	const float GetRadius() const;

	//レイキャストをするためにモデルハンドルを取得
	const int GetModelHandle() const { return m_modelHandle; }
	//現在のHPを取得
	const int GetHp() const { return m_status.s_hp; }

	int SetI(int &set) { return m_I = set; }

protected:
	//ほかのオブジェクトと押し出し判定をする当たり判定を作成
	void InitCollision(MyLibrary::LibVec3 vec, float len, float radius);
	//モデルを読み込む
	void LoadModel(std::string path);
	//アニメーション情報や、ステータス情報を読み込む
	void LoadData(std::string name);
	//物理クラスの初期化
	void InitRigidbody(float posX, float posY, float posZ, bool isUseGravity = true);
	//モデルの中心座標を計算
	void CalculationCenterPos(float modeldefaultSize, float modelSize);
	//モデル座標を設定
	void SetModelPos();
	//索敵判定をする当たり判定を作成
	void InitSearch(float radius, float y);
	//死亡した時
	void Death();
	//アニメーションブレンドの更新
	void UpdateAnimationBlend();
	//モデルの座標設定
	void SetDrawModelPos(float posY);
protected:
	std::shared_ptr<AttackObject> m_pAttack;    //攻撃判定
	std::shared_ptr<SearchObject> m_pSearch;    //索敵判定

	int m_dropCore;      //死亡時にドロップするコア

	float m_hpRadius;    //HPバーを表示する当たり判定の半径

	bool m_isDroped;     //コアをドロップしたかどうか
	bool m_isExist;      //存在するかどうか
	bool m_isDiscovery;  //プレイヤーを見つけたかどうか

	MyLibrary::LibVec3 m_centerPos;     //中心座標

	int m_I;

};

