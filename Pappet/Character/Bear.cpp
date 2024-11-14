#include "Bear.h"

namespace
{
	//キャラクター名
	constexpr const char* cCharacterName = "bear";
	//モデルパス
	constexpr const char* cModelPath = "Data/Enemy/BearModel.mv1";
	//モデルのサイズの拡大率
	constexpr float cModelSize = 0.7f;
	//モデルの長さ
	constexpr float cCapsuleLen = 40.0f;
	//モデルの半径
	constexpr float cCapsuleRadius = 25.0f;
	//モデルの座標を合わせる
	constexpr float cModelPosY = 24.0f;
	//死亡終了
	bool cDead = false;
	//死亡したときのアニメーション
	constexpr float cDeadFrame = 137.0f;
	//索敵範囲
	constexpr float cSearchRadius = 120.0f;
}

/// <summary>
/// コンストラクタ
/// </summary>
Bear::Bear() :
	EnemyBase(Collidable::Priority::High)
{
	//当たり判定の設定
	InitCollision(MyLibrary::LibVec3(0.0f, 2.0f, 0.0f), cCapsuleLen, cCapsuleRadius);
	//モデルの読み込み
	LoadModel(cModelPath);
	//アニメーションやステータスを取得
	LoadData(cCharacterName);
	//索敵範囲の設定
	m_searchRadius = cSearchRadius;

	//アニメーションの判定初期化
	m_anim.s_attack = false;
	m_anim.s_moveflag = false;
	m_anim.s_hit = false;
}

/// <summary>
/// デストラクタ
/// </summary>
Bear::~Bear()
{
	//メモリ解放
	MV1DeleteModel(m_modelHandle);
}

/// <summary>
/// 初期化
/// </summary>
/// <param name="posX"></param>
/// <param name="posY"></param>
/// <param name="posZ"></param>
/// <param name="physics"></param>
void Bear::Init(float posX, float posY, float posZ, std::shared_ptr<MyLibrary::Physics> physics)
{
	//代入
	m_pPhysics = physics;

	m_isExist = true;

	Collidable::Init(m_pPhysics);

	//物理クラスの初期化
	InitRigidbody(posX, posY + cCapsuleRadius, posZ);

	//中心座標の設定
	CalculationCenterPos(1.0f, cModelSize);

	//モデルの座標を設定
	SetModelPos();
	MV1SetPosition(m_modelHandle, m_modelPos.ConversionToVECTOR());

	//索敵判定をする当たり判定を作成
	InitSearch(cSearchRadius, 35.0f);

	//モデルのサイズ設定
	MV1SetScale(m_modelHandle, VGet(cModelSize, cModelSize, cModelSize));

	//アニメーション設定
	m_nowAnimNo = MV1AttachAnim(m_modelHandle, m_animIdx["Idle"]);
	m_nowAnimIdx = m_animIdx["Idle"];

	m_anim.s_isDead = false;
	cDead = false;

	m_status.s_hp = 1.0f;
}

/// <summary>
/// ゲームの仕様上での初期化処理
/// </summary>
/// <param name="posX">X座標</param>
/// <param name="posY">Y座標</param>
/// <param name="posZ">Z座標</param>
/// <param name="physics">物理クラス</param>
void Bear::GameInit(float posX, float posY, float posZ, std::shared_ptr<MyLibrary::Physics> physics)
{
	m_pPhysics = physics;

	Collidable::Init(m_pPhysics);

	//索敵判定をする当たり判定を作成
	InitSearch(cSearchRadius, 35.0f);

	//中心座標の設定
	CalculationCenterPos(1.0f, cModelSize);
	
	//物理クラスの初期化
	InitRigidbody(posX, posY + cCapsuleRadius, posZ);

	//モデルの座標を設定
	SetModelPos();
	MV1SetPosition(m_modelHandle, m_modelPos.ConversionToVECTOR());

	//モデルのサイズ設定
	MV1SetScale(m_modelHandle, VGet(cModelSize, cModelSize, cModelSize));

	//アニメーション設定
	m_nowAnimNo = MV1AttachAnim(m_modelHandle, m_animIdx["Idle"]);
	m_nowAnimIdx = m_animIdx["Idle"];

	m_anim.s_isDead = false;
	cDead = false;

}

/// <summary>
/// 更新処理
/// </summary>
/// <param name="playerPos"></param>
/// <param name="isChase"></param>
void Bear::Update(MyLibrary::LibVec3 playerPos, bool isChase)
{
	//アニメーションの更新
	if (!cDead)
	{
		m_isAnimationFinish = UpdateAnim(m_nowAnimNo, ANIMATION_MAX);
	}
	//死亡したときのアニメーション更新
	else if (cDead && m_nowFrame <= cDeadFrame)
	{
		m_isAnimationFinish = UpdateAnim(m_nowAnimNo, ANIMATION_MAX);
	}

	UpdateAnimationBlend();

	DistanceUpdate(playerPos);

	//移動処理
	MoveUpdate();

	//ターゲット状態
	TargetNow();
	//攻撃を受けた時
	if (m_isEnterHit)
	{
		m_status.s_hp -= m_col->GetAttack() - m_status.s_defense;
	}
	//プレイヤーがボス部屋に入ったら
	if (m_isBossDiscovery && !cDead)
	{
		Action(playerPos, isChase);
	}



	TriggerUpdate();
	HitTriggerUpdate();

	//判定の更新
	MyLibrary::LibVec3 centerPos = rigidbody.GetPos();
	m_pSearch->Update(centerPos);

	//死んだ時
	if (m_status.s_hp <= 0.0f)
	{
		//アニメーションを初期化
		m_anim.s_attack = false;
		m_anim.s_hit = false;
		m_anim.s_moveflag = false;

		Death();
		cDead = true;
	}

}

/// <summary>
/// ボスの行動
/// </summary>
/// <param name="playerPos">プレイヤーのポジション</param>
/// <param name="isChase">プレイヤー</param>
void Bear::Action(MyLibrary::LibVec3 playerPos, bool isChase)
{
	//敵がプレイヤーの位置によって方向を補正する
	float Cx = m_modelPos.x - playerPos.x;
	float Cz = m_modelPos.z - playerPos.z;

	//攻撃してない時
	if (!m_anim.s_attack)
	{
		//方向を決める
		AngleUpdate(playerPos);
	}

	//プレイヤーが範囲外だった時
	if (!m_pSearch->GetIsStay())
	{
		WalkUpdate("Walk", 2);

		//攻撃してない時
		if (!m_anim.s_attack)
		{
			//歩くアニメーション
			m_anim.s_moveflag = true;
			//スピード
			m_status.s_speed = 0.01f;

			m_move = VScale(m_difPlayer, m_status.s_speed);

			//移動方向
			m_moveVec = MyLibrary::LibVec3(m_move.x, m_move.y, m_move.z);
		}
		
	}
	//射程圏内に入った
	else if (m_pSearch->GetIsStay())
	{
		//ランダム行動で0が出たら
		if (m_randomAction == 0)
		{
			m_anim.s_moveflag = false;
			m_anim.s_attack = true;

			AttackUpdate("Attack1", 3);
		}
		//ランダム行動で1が出たら
		else if (m_randomAction == 1)
		{
			m_anim.s_moveflag = false;
			m_anim.s_attack = true;

			AttackUpdate("Attack2", 4);
		}
		//ランダム行動で2が出たら
		else if (m_randomAction == 2)
		{
			m_anim.s_moveflag = false;
			m_anim.s_attack = true;

			AttackUpdate("Attack3", 5);
		}

		m_moveVec = MyLibrary::LibVec3(0.0f, 0.0f, 0.0f);
	}


	//アニメーションが終わる度にランダムな行動を行う
	if (m_isAnimationFinish)
	{
		m_anim.s_attack = false;
		m_randomAction = GetRand(2);
	}

}

/// <summary>
/// 描画処理
/// </summary>
void Bear::Draw()
{
	//当たり判定座標を取得してモデルの描画座標を設定する
	SetDrawModelPos(cModelPosY);
	//3Dモデルの回転地をセットする
	MV1SetRotationXYZ(m_modelHandle, VGet(0.0f, m_angle, 0.0f));
	//モデルの描画
	MV1DrawModel(m_modelHandle);
}
