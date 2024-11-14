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
	//ゲームの仕様上での初期化
	virtual void GameInit(float posX, float posY, float posZ, std::shared_ptr<MyLibrary::Physics> physics) {};
	//終了
	virtual void Finalize(std::shared_ptr<MyLibrary::Physics> physics);
	//更新
	virtual void Update(MyLibrary::LibVec3 playerPos, bool isChange) {};
	//描画
	virtual void Draw() {};
	//終了処理
	virtual void End();

	//衝突したとき
	void OnCollideEnter(const std::shared_ptr<Collidable>& collidable) override;
	void OnCollideStay(const std::shared_ptr<Collidable>& collidable) override;
	void OnTriggerEnter(const std::shared_ptr<Collidable>& collidable) override;
	void OnTriggerStay(const std::shared_ptr<Collidable>& collidable) override;
	void OnTriggerExit(const std::shared_ptr<Collidable>& collidable) override;

	//ターゲットに当たっていることを返す
	bool GetStay() const;
	//ターゲットから外れたことを返す
	bool GetExit();

	//座標を返す
	virtual MyLibrary::LibVec3 GetPos() const { return m_collisionPos; }
	//ターゲットの有無を返す
	const bool GetTarget() const { return m_isTarget; }
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

	//ボス部屋の判定
	bool SetBossRoom(bool set) { return m_isBossDiscovery = set; }

protected:
	//ほかのオブジェクトと押し出し判定をする当たり判定を作成
	void InitCollision(MyLibrary::LibVec3 vec, float len, float radius);
	//モデルを読み込む
	void LoadModel(std::string path);
	//アニメーション情報や、ステータス情報を読み込む
	void LoadData(std::string name);
	//ステータス情報の初期化
	void LoadDataInit(std::string name);
	//物理クラスの初期化
	void InitRigidbody(float posX, float posY, float posZ, bool isUseGravity = true);
	//モデルの中心座標を計算
	void CalculationCenterPos(float modeldefaultSize, float modelSize);
	//モデル座標を設定
	void SetModelPos();
	//索敵判定をする当たり判定を作成
	void InitSearch(float radius, float y);
	//攻撃判定の初期化
	void InitAttack(float radius);
	//攻撃判定をする当たり判定を作成
	void InitAttackUpdate(float attack);
	//索敵の更新処理
	void TriggerUpdate();
	//プレイヤーとの距離を測る処理と盾との距離を測る処理
	void DistanceUpdate(MyLibrary::LibVec3 playerPos);
	//方向を向く処理
	void AngleUpdate(MyLibrary::LibVec3 playerPos);
	//移動をする処理
	void MoveUpdate();
	//アイドル状態にする
	void IdleUpdate();
	//歩く状態にする
	void WalkUpdate(std::string path, int number);
	//攻撃する状態にする
	void AttackUpdate(std::string path, int number);
	//怯み状態にする
	void HitUpdate(int number);
	//ターゲットが外れたかどうか
	void TargetNow();
	//ヒットの更新処理
	void HitTriggerUpdate();
	//ダメージを受けたとき
	void OnDamage();
	//死亡した時
	void Death();
	//アニメーションブレンドの更新
	void UpdateAnimationBlend();
	//モデルの座標設定
	void SetDrawModelPos(float posY);
protected:
	std::shared_ptr<EnemyAttackObject> m_pAttack;    //攻撃判定
	std::shared_ptr<SearchObject> m_pSearch;    //索敵判定

	AttackObject* m_col;

	int m_randomAction;       //ランダムに行動するための変数
	int m_dropCore;              //死亡時にドロップするコア

	float m_hpRadius;            //HPバーを表示する当たり判定の半径
	float m_moveTurning;         //時計周りに旋回する法線ベクトル
	float m_moveReverseTurning;  //反時計周りに旋回する法線ベクトル
	float m_difPSize;            //プレイヤーとの距離のサイズを入れる

	bool m_isDroped;             //コアをドロップしたかどうか
	bool m_isExist;              //存在するかどうか
	bool m_isDiscovery;          //プレイヤーを見つけたかどうか
	bool m_isBossDiscovery;      //ボス戦に入ったかどうか
	bool m_isTarget;             //プレイヤーにターゲットされるかどうか
	bool m_isStayTarget = false; //プレイヤーの索敵に当たってるかどうか
	bool m_isExitTarget = false; //プレイヤーの索敵から外れたかどうか
	bool m_isEnterHit = false;   //プレイヤーの攻撃が当たっているかどうか
	bool m_isExitHit = false;    //プレイヤーの攻撃が外れたかどうか

	MyLibrary::LibVec3 m_centerPos;     //中心座標
	VECTOR m_move;
	VECTOR m_difPlayer;          //プレイヤーとの距離

	int m_I;

};

