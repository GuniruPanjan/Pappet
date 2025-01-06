#include "Player.h"
#include "Manager/EffectManager.h"
#include "Manager/HandleManager.h"
#include "Manager/EnemyManager.h"
#include "Manager/CoreManager.h"
#include "External/CsvLoad.h"
#include "Item/Weapon.h"
#include "Item/Shield.h"
#include "Item/Armor.h"

#include <cassert>

namespace
{
	//モデルサイズ
	constexpr float cModelSizeScale = 0.4f;
	//アニメーションの切り替えにかかるフレーム数
	constexpr float cAnimChangeFrame = 5.0f;
	constexpr float cAnimChangeSpeed = 1.0f / cAnimChangeFrame;
	//アニメーションブレンド率の最大
	constexpr float cAnimBlendRateMax = 1.0f;
	//歩きにより代入される速度
	constexpr float cWalkSpeed = 2.0f;
	//ダッシュにより代入される速度
	constexpr float cDashSpeed = 4.0f;
	//歩くモーションのプレイタイム
	constexpr float cAnimWalkTime = 0.5f;
	//歩きアニメーションのトータルタイム
	constexpr float cAnimWalkReverseTimeInit = 35.0f;
	//ボタンが押されているかの確認用変数
	int cAbutton = 0;
	int cRbutton = 0;
	bool cRstickButton = false;
	//スティックの入力を得る
	int cAnX = 0;
	int cAnY = 0;
	//索敵範囲
	constexpr float cSearchRadius = 200.0f;
	//回避での移動距離
	float cAvoidanceMove = 0.0f;
	//回避の方向を一回入れる
	bool cOneAvoidance = false;
	//攻撃での追加攻撃時間
	int cAddAttackTime = 0;
	//行動での移動距離
	float cMove = 0.0f;
	//拳の攻撃範囲
	constexpr float cFistAttackRadius = 18.0f;
	//強攻撃の攻撃範囲
	constexpr float cStrengthAttackRadius = 50.0f;
	//盾の幅
	constexpr float cShieldWidth = 20.0f;
	//盾の横
	constexpr float cShieldHight = 60.0f;
	//盾の奥行
	constexpr float cShieldDepht = 10.0f;
	//現在のアタックのナンバーを入れる
	int cNowAttackNumber = 0;
	//攻撃の終了判定
	int cIsEndAttack = 0;
	//装備の攻撃力所得
	float cEquipmentAttack = 0.0f;
	//敵からのダメージ取得
	float cDamage = 0.0f;
	//人形のモデルパス
	constexpr const char* cPath = "Data/Player/PlayerModelPuppet.mv1";

	//シングルトン
	auto& handle = HandleManager::GetInstance();
	auto& effect = EffectManager::GetInstance();


	int cAnimIdx;

	bool cOne = false;
	bool cTwo = false;

	bool cHit = false;
}

Player::Player() :
	CharacterBase(Collidable::Priority::Low, ObjectTag::Player),
	m_xpad(),
	m_attackNumber(0),
	m_mapNow(0),
	m_updateX(0.0f),
	m_updateY(0.0f),
	m_updateZ(0.0f),
	m_menuOpen(false),
	m_restTouch(false),
	m_rest(false),
	m_itemPick(false),
	m_bigRest(false),
	m_lockonTarget(false),
	m_warp(false),
	m_bossStart(false),
	m_moveAnimFrameIndex(0),
	m_moveAnimFrameRight(0),
	m_moveAnimShieldFrameIndex(0),
	m_moveAnimShieldFrameHandIndex(0),
	m_cameraAngle(0.0f),
	m_lockAngle(0.0f),
	m_avoidanceNow(false),
	m_shieldNow(false),
	m_shieldOne(false),
	m_animReverse(false),
	m_deadReset(false),
	m_moveWeaponFrameMatrix(),
	m_moveShieldFrameMatrix(),
	m_rollMove(VGet(0.0f,0.0f,0.0f)),
	m_moveVector(VGet(0.0f,0.0f,0.0f)),
	m_shieldPos(),
	m_shieldSize()
{

	//カプセル型
	auto collider = Collidable::AddCollider(MyLibrary::CollidableData::Kind::Capsule, false);
	auto capsuleCol = dynamic_cast<MyLibrary::CollidableDataCapsule*>(collider.get());
	capsuleCol->m_len = 50.0f;
	capsuleCol->m_radius = 12.0f;
	capsuleCol->m_vec = MyLibrary::LibVec3(0.0f, capsuleCol->m_vec.y + 2.0f, 0.0f);

	//レベルの初期化
	m_levelStatus.sl_hp = 1;
	m_levelStatus.sl_stamina = 1;
	m_levelStatus.sl_muscle = 1;
	m_levelStatus.sl_skill = 1;

	//アニメーションの判定初期化
	m_anim.s_attack = false;
	m_anim.s_hit = false;
	m_anim.s_isDead = false;
	m_anim.s_moveflag = false;
	m_animChange.sa_avoidance = false;
	m_animChange.sa_dashMove = false;
	m_animChange.sa_recovery = false;
	m_animChange.sa_taking = false;
	m_animChange.sa_touch = false;
	m_animChange.sa_bossEnter = false;
	m_animChange.sa_shieldIdle = false;
	m_animChange.sa_enterShield = false;
	m_animChange.sa_imapact = false;
	m_animChange.sa_strengthAttack = false;
	
	//エフェクト読み込み
	effect.EffectLoad("Rest", "Data/Effect/Benediction.efkefc", 210, 10.0f);
	effect.EffectLoad("Heal", "Data/Effect/AnotherEffect/Sylph13.efkefc", 160, 20.0f);
	effect.EffectLoad("Imapct", "Data/Effect/HitEffect.efkefc", 30, 7.0f);

	//モデル読み込み
	m_modelHandle = handle.GetModelHandle(cPath);

	//モデルのサイズ設定
	MV1SetScale(m_modelHandle, VGet(cModelSizeScale, cModelSizeScale, cModelSizeScale));

	//Csvによるデータの読み込み
	CsvLoad::GetInstance().AnimDataLoad("Player", m_animIdx);
	CsvLoad::GetInstance().StatusLoad(m_status, "Player");

	for (int i = 0; i < 10; i++)
	{
		m_armorOne[i] = false;
	}
}

Player::~Player()
{
	//メモリ解放
	MV1DeleteModel(m_modelHandle);
	//メモリ削除
	handle.Clear();
}

/// <summary>
/// 初期化処理
/// </summary>
/// <param name="physics">物理クラスのポインタ</param>
void Player::Init(std::shared_ptr<MyLibrary::Physics> physics, Weapon& weapon, Shield& shield, Armor& armor, bool anim)
{
	m_pPhysics = physics;

	//当たり判定の追加
	Collidable::Init(m_pPhysics);

	//プレイヤーの初期位置設定
	rigidbody.Init(false);
	//rigidbody.SetPos(MyLibrary::LibVec3(485.0f, 12.0f, -800.0f));
	// ↓色々試すための初期化位置
	rigidbody.SetPos(MyLibrary::LibVec3(40.0f, 12.0f, 0.0f));
	rigidbody.SetNextPos(rigidbody.GetPos());
	rigidbody.SetVec(MyLibrary::LibVec3(0.0f, 40.0f, 0.0f));
	m_collisionPos = rigidbody.GetPos();
	SetModelPos();
	MV1SetPosition(m_modelHandle, m_modelPos.ConversionToVECTOR());

	m_updateX = 485.0f;
	m_updateY = 12.0f;
	m_updateZ = -800.0f;

	//メニューを閉じる
	m_menuOpen = false;

	//拳だった場合
	m_attackRadius = cFistAttackRadius;

	m_pAttack = std::make_shared<AttackObject>(m_attackRadius);
	m_pStrengthAttack = std::make_shared<AttackObject>(cStrengthAttackRadius);

	m_pSearch = std::make_shared<PlayerSearchObject>(cSearchRadius);
	m_pSearch->Init(m_pPhysics, rigidbody.GetPos());

	m_shieldSize = MyLibrary::LibVec3::Size(cShieldWidth, cShieldHight, cShieldDepht);

	m_pShield = std::make_shared<ShieldObject>(cShieldWidth, cShieldHight, cShieldDepht);

	if (anim)
	{
		//待機アニメーション設定
		m_nowAnimNo = MV1AttachAnim(m_modelHandle, m_animIdx["Idle"]);
		m_nowAnimIdx = m_animIdx["Idle"];
	}

	//HPの最大回復量
	m_maxHeel = 80;

	//移動距離
	cMove = 0.5f;

	m_staminaBreak = false;

	//装備初期化
	weapon.SetFist(true);
	shield.SetFist(true);
	armor.SetBody(true);
	m_armorOne[0] = true;
}

/// <summary>
/// ゲームの仕様上で初期化処理
/// </summary>
/// <param name="physics"></param>
void Player::GameInit(std::shared_ptr<MyLibrary::Physics> physics)
{
	m_pPhysics = physics;

	//死んでいた場合
	if (m_anim.s_isDead)
	{
		Collidable::Init(m_pPhysics);
		m_pSearch->Init(m_pPhysics, rigidbody.GetPos());

		m_anim.s_isDead = false;
	}

	CsvLoad::GetInstance().StatusLoad(m_status, "Player");

	//プレイヤーの初期位置設定
	rigidbody.Init(false);
	rigidbody.SetPos(MyLibrary::LibVec3(m_updateX, m_updateY, m_updateZ));
	rigidbody.SetNextPos(rigidbody.GetPos());
	rigidbody.SetVec(MyLibrary::LibVec3(0.0f, 40.0f, 0.0f));
	m_collisionPos = rigidbody.GetPos();
	SetModelPos();
	MV1SetPosition(m_modelHandle, m_modelPos.ConversionToVECTOR());

	m_deadReset = false;
	m_staminaBreak = false;

	//HPの最大回復量
	m_maxHeel = 80;

}

void Player::Finalize()
{
	Collidable::Finalize(m_pPhysics);
	m_pSearch->Finalize(m_pPhysics);
}

void Player::Update(Weapon& weapon, Shield& shield, Armor& armor, EnemyManager& enemy, CoreManager& core)
{
	//とりあえずやっとく
	m_status.s_core = core.GetCore();
	m_levelStatus.sl_all = m_levelStatus.sl_hp + m_levelStatus.sl_muscle + m_levelStatus.sl_skill + m_levelStatus.sl_stamina;

	//アニメーションで移動しているフレームの番号を検索する
	m_moveAnimFrameIndex = MV1SearchFrame(m_modelHandle, "mixamorig:Hips");
	m_moveAnimFrameRight = MV1SearchFrame(m_modelHandle, "mixamorig:RightHandThumb2");

	//盾を構える時のアニメーションフレーム取得
	m_moveAnimShieldFrameIndex = MV1SearchFrame(m_modelHandle, "mixamorig:LeftShoulder");
	m_moveAnimShieldFrameHandIndex = MV1SearchFrame(m_modelHandle, "mixamorig:LeftHand");

	//武器や盾をアタッチするフレームのローカル→ワールド変換行列を取得する
	m_moveWeaponFrameMatrix = MV1GetFrameLocalWorldMatrix(m_modelHandle, m_moveAnimFrameRight);
	m_moveShieldFrameMatrix = MV1GetFrameLocalWorldMatrix(m_modelHandle, m_moveAnimShieldFrameHandIndex);


	//パッド入力取得
	GetJoypadXInputState(DX_INPUT_KEY_PAD1, &m_xpad);

	//アニメーションの切り替え
	if (m_prevAnimNo != -1)
	{
		//フレームでアニメーションを切り替える
		m_animBlendRate += cAnimChangeSpeed;
		if (m_animBlendRate >= cAnimBlendRateMax)
		{
			m_animBlendRate = cAnimBlendRateMax;
		}

		//アニメーションのブレンド率を設定する
		MV1SetAttachAnimBlendRate(m_modelHandle, m_prevAnimNo, cAnimBlendRateMax - m_animBlendRate);
		MV1SetAttachAnimBlendRate(m_modelHandle, m_nowAnimNo, m_animBlendRate);
	}

	//死んだ時のアニメーション
	if (m_status.s_hp <= 0)
	{
		if (!m_anim.s_isDead)
		{
			m_anim.s_isDead = true;

			m_nowAnimIdx = m_animIdx["Death"];

			ChangeAnim(m_nowAnimIdx, m_animOne[0], m_animOne);
			m_lockonTarget = false;

			Finalize();
		}
	}

	//死亡アニメーションが終了したら
	if (m_anim.s_isDead && m_nowFrame >= 68.0f)
	{
		m_deadReset = true;
	}

	//アナログスティックを使って移動
	int analogX = 0;
	int analogY = 0;

	float SetAngleX = 0.0f;
	float SetAngleY = 0.0f;

	if (!m_anim.s_isDead && !m_animChange.sa_avoidance && !m_anim.s_attack && !m_animChange.sa_recovery && !m_anim.s_hit && !m_animChange.sa_bossEnter
		&& !m_animChange.sa_imapact && !m_rest && !m_animChange.sa_taking && !m_animChange.sa_strengthAttack)
	{
		GetJoypadAnalogInput(&analogX, &analogY, DX_INPUT_PAD1);

		cOneAvoidance = false;
	}

	if (m_animChange.sa_avoidance && !cOneAvoidance)
	{
		GetJoypadAnalogInput(&analogX, &analogY, DX_INPUT_PAD1);

		cOneAvoidance = true;
	}

	//アナログスティックの入力を格納
	cAnX = analogX;
	cAnY = analogY;

	//移動方向を設定する
	auto moveVec = VGet(static_cast<float>(-analogX), 0.0f, static_cast<float>(analogY));    //ベクトルの長さ

	//ベクトルの長さを取得する
	float len = VSize(moveVec);
	//ベクトルの長さを0.0～1.0の割合に変換する
	float rate = len / 1000.0f;
	//アナログスティック無効な範囲を除外する
	rate = (rate - 0.1f) / (0.8f - 0.1f);
	rate = min(rate, 1.0f);
	rate = max(rate, 0.0f);

	//速度が決定できるので移動ベクトルに反映する
	moveVec = VNorm(moveVec);
	float speed = m_status.s_speed * rate;

	moveVec = VScale(moveVec, speed);

	//cameraの角度から
	//コントローラーによる移動方向を決定する
	MATRIX mtx = MGetRotY(m_cameraAngle + DX_PI_F);
	moveVec = VTransform(moveVec, mtx);

	//ライブラリのベクターに変換する
	m_moveVec = MyLibrary::LibVec3(static_cast<float>(moveVec.x), static_cast<float>(moveVec.y), static_cast<float>(moveVec.z));

	//移動方向からプレイヤーへの向く方向を決定する
	//移動していない場合(ゼロベクトル)の場合は変更しない
	if (VSquareSize(moveVec) > 0.0f)
	{
		//ロックオンしてない時と走ったときのアングル
		if (!m_lockonTarget || m_animChange.sa_dashMove && !m_animChange.sa_avoidance)
		{
			//アングルを決定
			m_angle = atan2f(-moveVec.z, moveVec.x) - DX_PI_F / 2;
		}
		//ロックオンした時のアングル
		else if (m_lockonTarget && !m_animChange.sa_avoidance)
		{
			//アングルを決定
			m_angle = m_lockAngle;
		}
		//回避するとき
		else if (m_animChange.sa_avoidance)
		{
			//アングルを決定
			m_angle = atan2f(-moveVec.z, moveVec.x) - DX_PI_F / 2;
		}

		//プレイヤーが動いたら
		m_anim.s_moveflag = true;
	}
	//プレイヤーが動いてなかったら
	else if (VSquareSize(moveVec) <= 0.0f)
	{
		m_anim.s_moveflag = false;
	}
	

	//回避してないとき
	if (!m_animChange.sa_avoidance)
	{
		MyLibrary::LibVec3 prevVelocity = rigidbody.GetVelocity();
		MyLibrary::LibVec3 newVelocity = MyLibrary::LibVec3(m_moveVec.x, prevVelocity.y, m_moveVec.z);
		rigidbody.SetVelocity(newVelocity);
	}
	//回避してるとき
	else if(m_animChange.sa_avoidance)
	{
		//回避で移動する距離
		m_rollMove = VScale(VGet(sinf(m_angle), 0.0f, cosf(m_angle)), cAvoidanceMove);

		//アングルの方向に一定距離移動させたい
		MyLibrary::LibVec3 prevVelocity = rigidbody.GetVelocity();
		MyLibrary::LibVec3 newVelocity = MyLibrary::LibVec3(-m_rollMove.x, prevVelocity.y, -m_rollMove.z);
		rigidbody.SetVelocity(newVelocity);
	}

	//装備していないとき
	if (weapon.GetFist() && shield.GetFist())
	{
		//後ろ歩き
		if (m_lockonTarget && !m_animChange.sa_dashMove && cAnY > 0 && cAnX < 500 && cAnX > -500)
		{
			//逆再生を進める
			m_animReverse = true;
		}
		else
		{
			m_animReverse = false;
		}
	}
	//装備しているとき
	else if (!weapon.GetFist() || !shield.GetFist())
	{
		//後ろ歩き
		if (m_lockonTarget && !m_animChange.sa_dashMove && cAnY > 0 && cAnX < 500 && cAnX > -500)
		{
			//逆再生を進める
			m_animReverse = true;
		}
		//右歩き
		else if (m_lockonTarget && !m_animChange.sa_dashMove && cAnX > 500)
		{
			//逆再生を進める
			m_animReverse = true;
		}
		else
		{
			m_animReverse = false;
		}
	}

	//プレイヤーが生きている時だけ
	if (!m_anim.s_isDead)
	{
		//盾の判定
		if (m_shieldNow)
		{
			if (!m_shieldOne)
			{
				m_pShield->Init(m_pPhysics, m_shieldPos);

				m_shieldOne = true;
			}

			for (auto enemy : enemy.GetEnemyAttackHit())
			{
				if (!enemy)
				{
					//盾受けした時
					if (m_pShield->GetIsStay())
					{
						m_animChange.sa_imapact = true;
					}
				}
			}

		}
		else
		{
			m_pShield->CollisionEnd();
			m_shieldOne = false;
		}

		//ダメージを食らう処理
		if (cHit)
		{
			for (auto damage : enemy.GetEnemyDamage())
			{
				m_status.s_hp -= damage - (m_status.s_defense / 10);
			}

			cHit = false;
		}


		//メニューを開いている間はアクションできない
		if (!m_menuOpen)
		{
			//アクションをできなくする
			if (!m_animChange.sa_avoidance && !m_anim.s_hit && !m_animChange.sa_recovery && !m_animChange.sa_bossEnter && !m_animChange.sa_imapact
				&& !m_staminaBreak && !m_rest && !m_animChange.sa_strengthAttack)
			{
				Action();
			}
		}

		//装備をしていない時のアニメーション
		if (weapon.GetFist() && shield.GetFist())
		{
			NotWeaponAnimation();
		}
		//装備したときのアニメーション
		else if (!weapon.GetFist() || !shield.GetFist())
		{
			WeaponAnimation(shield);
		}

		AllAnimation();

		//防具をしていない時の処理
		if (armor.GetBody())
		{
			ArmorChange(0, cPath);	
		}
		//防具をした時の処理
		else if (!armor.GetBody())
		{
			ArmorChange(1, armor.GetPath());	
		}
		
	}

	if (!m_anim.s_isDead)
	{
		//アニメーションの更新
		m_isAnimationFinish = UpdateAnim(m_nowAnimNo, ANIMATION_MAX);
	}
	//死亡したときのアニメーション更新
	else if (m_anim.s_isDead && m_nowFrame <= 68.0f)
	{
		m_isAnimationFinish = UpdateAnim(m_nowAnimNo, ANIMATION_MAX);
	}

	//プレイヤーのポジションを入れる
	SetModelPos();
	
	//装備の更新
	weapon.Update(m_moveWeaponFrameMatrix);
	shield.Update(m_moveShieldFrameMatrix);

	//装備でのステータス上昇をプラスする
	if (weapon.GetFist())
	{
		cEquipmentAttack = 0.0f;
	}
	else if (weapon.GetBlack())
	{
		cEquipmentAttack = weapon.GetBlackAttack();
	}

	if (armor.GetBody())
	{
		m_status.s_defense = 0.0f;
	}
	else if (armor.GetCommon())
	{
		m_status.s_defense = armor.GetCommonDefence();
	}

	//スタミナ回復
	if (m_status.s_stamina < ms_maxStatus.sm_stamina && !m_animChange.sa_avoidance && !m_anim.s_attack && !m_animChange.sa_dashMove && !m_animChange.sa_strengthAttack)
	{
		m_status.s_stamina += 0.5f;
	}

	//スタミナ切れ
	if (m_status.s_stamina <= 0.0f)
	{
		m_staminaBreak = true;
		m_animChange.sa_dashMove = false;
		m_status.s_speed = cWalkSpeed;
	}
	else if (m_status.s_stamina >= 50.0f)
	{
		m_staminaBreak = false;
	}

	//判定のポジション更新
	MyLibrary::LibVec3 centerPos = rigidbody.GetPos();
	MyLibrary::LibVec3 attackPos = MyLibrary::LibVec3(rigidbody.GetPos().x + sinf(m_angle) * -25.0f, rigidbody.GetPos().y + 15.0f, rigidbody.GetPos().z - cosf(m_angle) * 25.0f);
	MyLibrary::LibVec3 StrengthAttackPos = MyLibrary::LibVec3(rigidbody.GetPos().x, rigidbody.GetPos().y, rigidbody.GetPos().z);
	m_shieldPos = MyLibrary::LibVec3(rigidbody.GetPos().x + sinf(m_angle) * -15.0f, rigidbody.GetPos().y + 25.0f, rigidbody.GetPos().z - cosf(m_angle) * 15.0f);

	//sinでX軸のwidthのサイズを出す
	if (sinf(m_angle) > 0)
	{
		m_shieldSize.width = 30.0f + sinf(m_angle) * -15.0f;
	}
	else if (sinf(m_angle) < 0)
	{
		m_shieldSize.width = 30.0f - sinf(m_angle) * -15.0f;
	}

	//cosでZ軸のdepthサイズを出す
	if (cosf(m_angle) > 0)
	{
		m_shieldSize.depth = 30.0f + cosf(m_angle) * -15.0f;
	}
	else if (cosf(m_angle) < 0)
	{
		m_shieldSize.depth = 30.0f - cosf(m_angle) * -15.0f;
	}

	//盾受け終了
	if (m_animChange.sa_imapact && m_isAnimationFinish)
	{
		m_animChange.sa_imapact = false;
	}


	//判定の更新
	m_pSearch->Update(centerPos);
	m_pAttack->Update(attackPos);
	m_pStrengthAttack->Update(StrengthAttackPos);
	m_pShield->Update(m_shieldPos, m_shieldSize);

	//怯み中
	if (m_anim.s_hit)
	{
		//攻撃判定リセット
		m_pAttack->CollisionEnd();
	}

	//怯みを終わらせる
	if (m_anim.s_hit && m_isAnimationFinish)
	{
		m_anim.s_hit = false;
	}

	//走った時のスタミナ消費
	if (m_animChange.sa_dashMove)
	{
		m_status.s_stamina -= 0.1f;
	}

	//回避行動中
	if (!m_isAnimationFinish && m_animChange.sa_avoidance && !m_anim.s_hit)
	{
		//フレーム回避
		if (m_nowFrame >= 0.0f && m_nowFrame <= 20.0f)
		{
			m_avoidanceNow = true;

			cAvoidanceMove = 4.0f;
		}
		else if (m_nowFrame >= 20.0f && m_nowFrame <= 30.0f)
		{
			m_avoidanceNow = false;

			cAvoidanceMove = 1.0f;
		}
		else
		{
			m_avoidanceNow = false;

			cAvoidanceMove = 0.2f;
		}
	}
	//回避終了
	else if (m_isAnimationFinish && m_animChange.sa_avoidance)
	{
		m_animChange.sa_avoidance = false;
	}

	//攻撃中
	if (!m_isAnimationFinish && m_anim.s_attack)
	{
		//一段階目の攻撃
		if (m_nowFrame <= 40.0f)
		{
			//現在のアタックナンバー
			cNowAttackNumber = 1;

			m_pAttack->SetAttack(m_status.s_attack + cEquipmentAttack);

			//攻撃判定発生フレーム
			if (m_nowFrame == 25.0f)
			{
				m_status.s_stamina -= 25.0f;
				m_pAttack->Init(m_pPhysics);
			}
			else if (m_nowFrame >= 35.0f && m_nowFrame < 40.0f)
			{
				//判定をリセット
				m_pAttack->CollisionEnd();
			}
			//攻撃終了
			else if (m_nowFrame >= 40.0f && m_attackNumber == 1)
			{
				cIsEndAttack = 1;
			}

		}
		//二段階目の攻撃
		else if (m_nowFrame <= 70.0f && cIsEndAttack == 1)
		{
			//現在のアタックナンバー
			cNowAttackNumber = 2;

			m_pAttack->SetAttack((m_status.s_attack + cEquipmentAttack) * 1.1);

			//攻撃判定発生フレーム
			if (m_nowFrame == 55.0f)
			{
				m_status.s_stamina -= 25.0f;
				m_pAttack->Init(m_pPhysics);
			}
			else if (m_nowFrame >= 65.0f && m_nowFrame < 70.0f)
			{
				//攻撃判定リセット
				m_pAttack->CollisionEnd();
			}
			//攻撃終了
			else if (m_nowFrame >= 70.0f && m_attackNumber == 2)
			{
				cIsEndAttack = 2;
			}
		}
		//三段階目の攻撃
		else if (m_nowFrame <= 110.0f && cIsEndAttack == 2)
		{
			//現在のアタックナンバー
			cNowAttackNumber = 3;

			m_pAttack->SetAttack((m_status.s_attack + cEquipmentAttack) * 1.2);

			//攻撃判定発生フレーム
			if (m_nowFrame == 85.0f)
			{
				m_status.s_stamina -= 25.0f;
				m_pAttack->Init(m_pPhysics);
			}
			else if (m_nowFrame >= 95.0f && m_nowFrame < 110.0f)
			{
				//攻撃判定リセット
				m_pAttack->CollisionEnd();
			}
			//攻撃終了
			else if (m_nowFrame >= 110.0f)
			{
				cIsEndAttack = 0;
			}
		}
		else
		{
			m_anim.s_attack = false;

			//攻撃段階を初期化する
			m_attackNumber = 0;
			//攻撃終了
			cIsEndAttack = 0;
			//攻撃判定リセット
			m_pAttack->CollisionEnd();
		}
	}
	//攻撃終了
	else if (m_isAnimationFinish && m_anim.s_attack)
	{
		m_anim.s_attack = false;

		//攻撃段階を初期化する
		m_attackNumber = 0;
		//攻撃終了
		cIsEndAttack = 0;
	}

	//強攻撃
	if (!m_isAnimationFinish && m_animChange.sa_strengthAttack)
	{
		m_pStrengthAttack->SetAttack(80.0f);

		//フレーム中に攻撃を発生
		if (m_nowFrame == 58.0f)
		{
			m_pStrengthAttack->Init(m_pPhysics);
		}
		else if (m_nowFrame >= 68.0f)
		{
			//判定リセット
			m_pStrengthAttack->CollisionEnd();
		}
	}
	//強攻撃終了
	if (m_animChange.sa_strengthAttack && m_isAnimationFinish)
	{
		m_animChange.sa_strengthAttack = false;
	}


	//回復する
	if (!m_isAnimationFinish && m_animChange.sa_recovery)
	{
		//HPの最大値を超えないように回復する
		if (ms_maxStatus.sm_hp > m_status.s_hp && m_heel < m_maxHeel)
		{
			m_heel++;

			m_status.s_hp++;
		}
	}
	//回復終了
	else if (m_isAnimationFinish && m_animChange.sa_recovery)
	{
		m_animChange.sa_recovery = false;

		m_heel = 0;
	}

	
	//ボス部屋に入るモーション終了
	if (m_isAnimationFinish && m_animChange.sa_bossEnter)
	{
		m_animChange.sa_bossEnter = false;
	}
	//ボス部屋に入るモーション中
	else if (m_animChange.sa_bossEnter)
	{
		//とりあえず妥協点で後で直す
		//主にプレイヤーの当たり判定を消し、モデルだけを動かしモデルの最終地点に当たり判定を出す

		//回避で移動する距離
		m_moveVector = VScale(VGet(sinf(m_angle), 0.0f, cosf(m_angle)), cMove);

		//アングルの方向に一定距離移動させたい
		MyLibrary::LibVec3 prevVelocity = rigidbody.GetVelocity();
		MyLibrary::LibVec3 newVelocity = MyLibrary::LibVec3(-m_moveVector.x, prevVelocity.y, -m_moveVector.z);
		rigidbody.SetVelocity(newVelocity);
	}

	//アイテム取得終了
	if (m_animChange.sa_taking)
	{
		if (m_nowFrame >= 60.0f)
		{
			m_animChange.sa_taking = false;
		}
		
	}
}

/// <summary>
/// プレイヤーのアクション実装
/// </summary>
void Player::Action()
{
	//ターゲットできる時
	if (!m_lockonTarget && m_pSearch->GetIsStay())
	{
		//一回だけ押す
		if (m_xpad.Buttons[7] == 1 && !cRstickButton)
		{
			m_lockonTarget = true;
			cRstickButton = true;
		}
		else if(m_xpad.Buttons[7] == 0)
		{
			cRstickButton = false;
		}
	}
	//ターゲットを外す
	else if (m_lockonTarget == true && m_pSearch->GetIsStay())
	{
		//一回だけ押す
		if (m_xpad.Buttons[7] == 1 && !cRstickButton)
		{
			m_lockonTarget = false;
			cRstickButton = true;
		}
		else if (m_xpad.Buttons[7] == 0)
		{
			cRstickButton = false;
		}
	}
	//ターゲットを無理やり外す
	else if (m_pSearch->GetIsExit())
	{
		m_lockonTarget = false;
		cRstickButton = false;
	}

	//Aボタンが押されたらダッシュか回避
	if (m_xpad.Buttons[12] == 1 && !m_anim.s_attack)
	{
		//ダッシュ
		if (cAbutton > 30)
		{
			m_animChange.sa_avoidance = false;

			//ダッシ中
			m_animChange.sa_dashMove = true;

			m_status.s_speed = cDashSpeed;
		}

		if (cAbutton < 31)
		{
			cAbutton++;
		}
	}
	else
	{
		m_animChange.sa_dashMove = false;

		m_status.s_speed = cWalkSpeed;

		//回避に必要なスタミナがある場合
		if (m_status.s_stamina >= 20)
		{
			//回避
		    //離した瞬間
			if (cAbutton > 0 && cAbutton < 30 && m_animChange.sa_avoidance == false)
			{
				m_status.s_stamina -= 20;

				m_animChange.sa_avoidance = true;
			}
		}
		

		cAbutton = 0;
	}

	//攻撃に必要なスタミナがある場合
	if (m_status.s_stamina >= 25)
	{
		//攻撃
		//コントローラが無いとき用
		if (CheckHitKey(KEY_INPUT_R) == 1)
		{
			cRbutton++;

			//一回だけ反応するようにする
			if (cRbutton == 1)
			{
				m_anim.s_attack = true;

				//追加攻撃受付
				if (cAddAttackTime <= 30 && cAddAttackTime > 0)
				{
					//二段階目の攻撃
					if (cNowAttackNumber == 1)
					{
						m_attackNumber = 1;
					}
					//三段階目の攻撃
					else if (cNowAttackNumber == 2)
					{
						m_attackNumber = 2;
					}
				}

				//追加攻撃時間を初期化
				cAddAttackTime = 40;
			}
		}
		else
		{
			cRbutton = 0;
		}

		//攻撃
	    //Rボタンを押すことで攻撃
		if (m_xpad.Buttons[9] == 1)
		{
			cRbutton++;
			
			//一回だけ反応するようにする
			if (cRbutton == 1)
			{
				m_anim.s_attack = true;

				//追加攻撃受付
				if (cAddAttackTime <= 30 && cAddAttackTime > 0)
				{
					//二段階目の攻撃
					if (cNowAttackNumber == 1)
					{
						m_attackNumber = 1;
					}
					//三段階目の攻撃
					else if (cNowAttackNumber == 2)
					{
						m_attackNumber = 2;
					}
				}

				//追加攻撃時間を初期化
				cAddAttackTime = 40;
			}
		}
		else
		{
			cRbutton = 0;
		}
	}
	
	//追加攻撃受付時間を減らす
	if (cAddAttackTime <= 40 && cAddAttackTime > 0 && --cAddAttackTime > -1);

	//強攻撃
	//ZRボタン
	if (m_xpad.RightTrigger)
	{
		m_animChange.sa_strengthAttack = true;
	}

	//コントローラーが無いとき用だから後で消す
	//強攻撃
	//if (CheckHitKey(KEY_INPUT_R) == 1)
	//{
	//	m_animChange.sa_strengthAttack = true;
	//}

	//行動中は防御できない
	if (!m_anim.s_attack && !m_animChange.sa_avoidance && !m_animChange.sa_recovery)
	{
		//Lボタンで防御
		if (m_xpad.Buttons[8] == 1)
		{
			m_shieldNow = true;

			if (!m_animChange.sa_shieldIdle)
			{
				m_animChange.sa_enterShield = true;
			}
			
			//盾の構えが終了したとき
			if (m_animChange.sa_enterShield && m_isAnimationFinish)
			{
				m_animChange.sa_enterShield = false;
				m_animChange.sa_shieldIdle = true;
			}
		}
		else
		{
			//Lボタンを離した瞬間
			if (m_animChange.sa_enterShield || m_animChange.sa_shieldIdle)
			{
				m_animChange.sa_enterShield = false;
				m_animChange.sa_shieldIdle = false;
			}

			m_shieldNow = false;

			FrameEndAnim(cAnimIdx, cOne, cTwo, m_moveAnimShieldFrameIndex);
			
		}
	}
	else
	{
		m_animChange.sa_enterShield = false;
		m_animChange.sa_shieldIdle = false;
		m_shieldNow = false;

		FrameEndAnim(cAnimIdx, cOne, cTwo, m_moveAnimShieldFrameIndex);

	}

	//回復
	//Xボタンが押されたら
	if (m_xpad.Buttons[14] == 1 && !m_anim.s_attack)
	{
		m_animChange.sa_recovery = true;
	}
	
	//休息
	//休息できたら
	if (m_restTouch && m_mapNow != 0)
	{
		//初期化位置更新
		m_updateX = m_modelPos.x;
		m_updateY = m_modelPos.y;
		m_updateZ = m_modelPos.z;

		//Yボタンが押されたら
		if (m_xpad.Buttons[15] == 1)
		{
			m_rest = true;
		}
	}
	else if(m_mapNow != 0)
	{
		m_rest = false;
	}

	//レベルを上げられる休息
	//マップで分ける
	if (m_restTouch && m_mapNow == 0)
	{
		//初期化位置更新
		m_updateX = m_modelPos.x;
		m_updateY = m_modelPos.y;
		m_updateZ = m_modelPos.z;

		//Yボタンが押されたら
		if (m_xpad.Buttons[15] == 1)
		{
			m_rest = true;
			m_bigRest = true;
		}
	}
	else if(m_mapNow == 0)
	{
		m_rest = false;
		m_bigRest = false;
	}

	//ボスの部屋に入る
	if (m_bossStart)
	{
		//Yボタンを押したら
		if (m_xpad.Buttons[15] == 1)
		{
			m_animChange.sa_bossEnter = true;
		}
	}
	
	//アイテムを拾う
	if (m_itemPick)
	{
		//Yボタンを押したら
		if (m_xpad.Buttons[15] == 1)
		{
			m_animChange.sa_taking = true;
		}
	}


	//メニューを開く
	if (m_xpad.Buttons[4] == 1)
	{
		m_menuOpen = true;
	}
}

/// <summary>
/// マップをワープするための関数
/// </summary>
void Player::WarpMap()
{
	//Yボタンが押されたら
	if (m_xpad.Buttons[15] == 1)
	{
		//マップを変える
		m_warp = true;
	}
	else
	{
		m_warp = false;
	}
}

/// <summary>
/// 武器を持ってないときのアニメーション
/// </summary>
void Player::NotWeaponAnimation()
{
	//攻撃が当たってない時
	//ボス部屋に入っていない時
	if (!m_anim.s_hit && !m_animChange.sa_bossEnter && !m_animChange.sa_imapact)
	{
		//走り
		if (m_animChange.sa_dashMove && m_anim.s_moveflag)
		{
			m_nowAnimIdx = m_animIdx["Run"];
			ChangeAnim(m_nowAnimIdx, m_animOne[1], m_animOne);
			NotInitAnim(false);

		}
		//歩き
		else if (m_anim.s_moveflag)
		{
			//ターゲットしているとき
			if (!m_lockonTarget)
			{
				m_nowAnimIdx = m_animIdx["Walk"];
				ChangeAnim(m_nowAnimIdx, m_animOne[2], m_animOne);
				NotInitAnim(false);

			}
			//ターゲットしているとき
			else if (m_lockonTarget)
			{
				//左歩き
				if (cAnX < -500)
				{
					m_nowAnimIdx = m_animIdx["LeftWalk"];
					ChangeAnim(m_nowAnimIdx, m_animOne[3], m_animOne);
					NotInitAnim(false);

				}
				//右歩き
				else if (cAnX > 500)
				{
					m_nowAnimIdx = m_animIdx["RightWalk"];
					ChangeAnim(m_nowAnimIdx, m_animOne[4], m_animOne);
					NotInitAnim(false);

				}
				//後ろ歩きor歩き
				if (cAnX < 500 && cAnX > -500)
				{
					m_nowAnimIdx = m_animIdx["Walk"];
					ChangeAnim(m_nowAnimIdx, m_animOne[2], m_animOne, cAnimWalkTime, m_animReverse, cAnimWalkReverseTimeInit);
					NotInitAnim(false);

				}
			}
			
		}
	}
}

/// <summary>
/// 全ての状態の時に行うアニメーション関数
/// </summary>
void Player::AllAnimation()
{
	//プレイヤーが生きているときだけ
	if (!m_anim.s_isDead)
	{
		//攻撃が当たった時
		//ボス部屋に入った時
		if (m_anim.s_hit && !m_animChange.sa_bossEnter && !m_animChange.sa_imapact)
		{
			m_nowAnimIdx = m_animIdx["Hit"];
			ChangeAnim(m_nowAnimIdx, m_animOne[5], m_animOne);
			NotInitAnim(false);

		}
		//攻撃が当たってないとき
		//ボス部屋に入った時
		else if (!m_anim.s_hit && !m_animChange.sa_bossEnter && !m_animChange.sa_imapact)
		{
			//動いてない時
			if (!m_anim.s_moveflag && !m_animChange.sa_avoidance && !m_anim.s_attack && !m_animChange.sa_recovery && 
				!m_shieldNow && !m_animChange.sa_taking && !m_animChange.sa_strengthAttack)
			{
				m_nowAnimIdx = m_animIdx["Idle"];
				ChangeAnim(m_nowAnimIdx, m_animOne[6], m_animOne);
				NotInitAnim(false);

			}
			//回避
			else if (m_animChange.sa_avoidance)
			{
				m_nowAnimIdx = m_animIdx["Roll"];
				ChangeAnim(m_nowAnimIdx, m_animOne[7], m_animOne, 0.8f);
				NotInitAnim(true);
			}
			//攻撃
			else if (m_anim.s_attack && !m_animChange.sa_avoidance && !m_animChange.sa_recovery)
			{
				m_nowAnimIdx = m_animIdx["Attack1"];
				ChangeAnim(m_nowAnimIdx, m_animOne[8], m_animOne, 1.0f);
				NotInitAnim(false);
			}
			//強攻撃
			else if (m_animChange.sa_strengthAttack && !m_animChange.sa_avoidance && !m_animChange.sa_recovery)
			{
				m_nowAnimIdx = m_animIdx["Attack2"];
				ChangeAnim(m_nowAnimIdx, m_animOne[9], m_animOne, 0.5f);
				NotInitAnim(false);
			}
			//回復
			else if (m_animChange.sa_recovery)
			{
				m_nowAnimIdx = m_animIdx["Recovery"];
				ChangeAnim(m_nowAnimIdx, m_animOne[10], m_animOne);
				NotInitAnim(false);

			}
			//アイテムを取得するとき
			else if (m_animChange.sa_taking)
			{
				m_nowAnimIdx = m_animIdx["Taking"];
				ChangeAnim(m_nowAnimIdx, m_animOne[11], m_animOne);
				NotInitAnim(false);

			}
			//ギミックを作動させるとき
			else if (m_animChange.sa_touch)
			{
				m_nowAnimIdx = m_animIdx["Touch"];
				ChangeAnim(m_nowAnimIdx, m_animOne[12], m_animOne);
				NotInitAnim(false);
			}
		}
		//ボス部屋入り口に入るとき
		else if (m_animChange.sa_bossEnter && !m_anim.s_hit && !m_animChange.sa_imapact)
		{
			m_nowAnimIdx = m_animIdx["BossEnter"];
			ChangeAnim(m_nowAnimIdx, m_animOne[13], m_animOne);
			NotInitAnim(true);
		}
	}
}

/// <summary>
/// 武器を持ってる状態の時に行うアニメーション
/// </summary>
void Player::WeaponAnimation(Shield& shield)
{
	//とりあえず
	//m_animChange.sa_imapact = false;

	//プレイヤーが生きている時だけ
	if (!m_anim.s_isDead)
	{
		//盾受けしたとき
		if (m_animChange.sa_imapact)
		{
			m_nowAnimIdx = m_animIdx["ShieldImpact"];
			ChangeAnim(m_nowAnimIdx, m_animOne[14], m_animOne);
			NotInitAnim(true);
		}
		//攻撃が当たってない時盾受けしていないとき
		else if (!m_anim.s_hit && !m_animChange.sa_imapact)
		{
			//走り
			if (m_animChange.sa_dashMove && m_anim.s_moveflag)
			{
				m_nowAnimIdx = m_animIdx["ShieldRun"];
				ChangeAnim(m_nowAnimIdx, m_animOne[15], m_animOne);
				NotInitAnim(false);
			}
			//歩き
			else if (m_anim.s_moveflag)
			{
				//ターゲットしていないとき
				if (!m_lockonTarget)
				{
					m_nowAnimIdx = m_animIdx["ShieldWalk"];
					ChangeAnim(m_nowAnimIdx, m_animOne[16], m_animOne);
					NotInitAnim(false);
				}
				//ターゲットしているとき
				else if (m_lockonTarget)
				{
					//左右歩き
					if (cAnX < -500 || cAnX > 500)
					{
						m_nowAnimIdx = m_animIdx["ShieldSideWalk"];
						ChangeAnim(m_nowAnimIdx, m_animOne[17], m_animOne, cAnimWalkTime, m_animReverse);
						NotInitAnim(false);

					}
					//後ろ歩きor歩き
					else if (cAnX < 500 && cAnX > -500)
					{
						m_nowAnimIdx = m_animIdx["ShieldWalk"];
						ChangeAnim(m_nowAnimIdx, m_animOne[16], m_animOne, cAnimWalkTime, m_animReverse, cAnimWalkReverseTimeInit);
						NotInitAnim(false);

					}
				}
			}
			//盾があるとき
			if (!shield.GetFist())
			{
				//防御開始と防御終了
				if (m_animChange.sa_enterShield)
				{
					//キャラが動いていない時
					if (!m_anim.s_moveflag)
					{
						m_nowAnimIdx = m_animIdx["ShieldStart"];
						ChangeAnim(m_nowAnimIdx, m_animOne[18], m_animOne, 1.0f);
						NotInitAnim(true);
					}
					//キャラが動いているとき
					else
					{
						
					}
					
				}
				//防御中
				if (m_animChange.sa_shieldIdle)
				{
					//キャラが動いていないとき
					if (!m_anim.s_moveflag)
					{
						FrameEndAnim(cAnimIdx, cOne, cTwo, m_moveAnimShieldFrameIndex);
						m_nowAnimIdx = m_animIdx["ShieldIdle"];
						ChangeAnim(m_nowAnimIdx, m_animOne[19], m_animOne);
						NotInitAnim(false);
					}
					//キャラが動いているとき
					else
					{
						
						cAnimIdx = m_animIdx["ShieldTransition"];
						FrameChangeAnim(cAnimIdx, cOne, cTwo, m_moveAnimShieldFrameIndex);
					}
					
				}
				
			}
		}
	}
}

/// <summary>
/// 描画処理
/// </summary>
void Player::Draw(Armor& armor)
{
	rigidbody.SetPos(rigidbody.GetNextPos());
	m_collisionPos = rigidbody.GetPos();

#if false
	DrawFormatString(200, 600, 0xffffff, "colPosx : %f", m_collisionPos.x);
	DrawFormatString(200, 700, 0xffffff, "colPosy : %f", m_collisionPos.y);
	DrawFormatString(200, 800, 0xffffff, "colPosz : %f", m_collisionPos.z);
	DrawFormatString(200, 100, 0xffffff, "HP : %f LevelHp : %d", m_status.s_hp, m_levelStatus.sl_hp);
	DrawFormatString(200, 150, 0xffffff, "Stamina : %f LevelStamina : %d", m_status.s_stamina, m_levelStatus.sl_stamina);
	DrawFormatString(200, 200, 0xffffff, "Muscle : %d LevelMuscle : %d", m_status.s_muscle, m_levelStatus.sl_muscle);
	DrawFormatString(200, 250, 0xffffff, "Skill : %d LevelSkill : %d", m_status.s_skill, m_levelStatus.sl_skill);
	DrawFormatString(200, 300, 0xffffff, "Core : %d", m_status.s_core);
#endif
#if false
	DrawFormatString(0, 400, 0xffffff, "m_nowAnim : %d", m_nowAnimIdx);
	DrawFormatString(0, 500, 0xffffff, "m_nowSpeed : %f", m_nowFrame);
	DrawFormatString(200, 100, 0xffffff, "m_blend : %f", m_animBlendRate);
	DrawFormatString(200, 200, 0xffffff, "addattacktime : %d", cAddAttackTime);
	DrawFormatString(200, 300, 0xffffff, "avoidancemove : %f", cAvoidanceMove);
	DrawFormatString(200, 400, 0xffffff, "attackNumber : %d", m_attackNumber);
	DrawFormatString(200, 500, 0xffffff, "nowAttackNumber : %d", cNowAttackNumber);

#endif
#if false
	//DrawFormatString(200, 100, 0xffffff, "animtime : %f", cAnimWalkTime);
	DrawFormatString(200, 600, 0xffffff, "weapon : %d", m_pWeapon->GetBlack());
	DrawFormatString(200, 650, 0xffffff, "shield : %d", m_pShield->GetUgly());
	DrawFormatString(200, 700, 0xffffff, "fist : %d", m_pWeapon->GetFist());
	DrawFormatString(200, 750, 0xffffff, "fist : %d", m_pShield->GetFist());
#endif

	MV1SetPosition(m_modelHandle, VSub(m_modelPos.ConversionToVECTOR(), VGet(0.0f, 12.0f, 0.0f)));
	//モデルの回転地
	MV1SetRotationXYZ(m_modelHandle, VGet(0.0f, m_angle, 0.0f));
	//描画
	MV1DrawModel(m_modelHandle);
	
}

void Player::End()
{
	//メモリ解放
	MV1DeleteModel(m_modelHandle);
	//メモリ削除
	handle.Clear();
}

void Player::OnCollideEnter(const std::shared_ptr<Collidable>& collidable)
{
#if _DEBUG
	std::string message = "プレイヤーが";
#endif
	auto tag = collidable->GetTag();
	switch (tag)
	{
	case ObjectTag::Enemy:
#if _DEBUG
		message += "敵";
#endif
		break;
	}
#if _DEBUG
	message += "と当たった\n";
	printfDx(message.c_str());
#endif
}

void Player::OnCollideStay(const std::shared_ptr<Collidable>& collidable)
{
#if _DEBUG
	std::string message = "プレイヤーが";
#endif
	auto tag = collidable->GetTag();
	switch (tag)
	{
	case ObjectTag::Enemy:
#if _DEBUG
		message += "敵";
#endif
		break;
	case ObjectTag::Rect:
#if _DEBUG
		message += "ボス部屋の入り口";
#endif
		break;
	}
#if _DEBUG
	message += "と当たった\n";
	printfDx(message.c_str());
#endif
}

void Player::OnTriggerEnter(const std::shared_ptr<Collidable>& collidable)
{
#if _DEBUG
	std::string message = "プレイヤーが";
#endif
	auto tag = collidable->GetTag();
	switch (tag)
	{
	case ObjectTag::EnemyAttack:
#if _DEBUG
		message += "攻撃";
#endif
		//HPが0以上だとヒットする
		if (m_status.s_hp > 0.0f)
		{
			//回避中とヒット中とボス部屋に入っている時は攻撃が当たらない
			if (!m_avoidanceNow && !m_anim.s_hit && !m_animChange.sa_bossEnter && !m_animChange.sa_imapact)
			{
				cHit = true;

				m_anim.s_hit = true;
			}
		}
		break;
	case ObjectTag::EnemySearch:
#if _DEBUG
		message += "索敵";
#endif
		break;
	case ObjectTag::Item:
#if _DEBUG
		message += "アイテム";
#endif
		break;
	case ObjectTag::Rest:
#if _DEBUG
		message += "休息";
#endif
		break;
	case ObjectTag::Rect:
#if _DEBUG
		message += "ボスの入口";
#endif
		break;
	}
#if _DEBUG
	message += "と当たった\n";
	printfDx(message.c_str());
#endif
}

/// <summary>
/// 防具を変える
/// </summary>
/// <param name="one">防具のナンバー指定</param>
/// <param name="path">パス</param>
void Player::ArmorChange(int one, std::string path)
{
	//防具を変えた時にアニメーションのバグが発生する

	//一回だけ実行
	if (!m_armorOne[one])
	{
		//メモリ解放
		MV1DeleteModel(m_modelHandle);
		//モデル読み込み
		m_modelHandle = MV1LoadModel(path.c_str());

		//モデルのサイズ設定
		MV1SetScale(m_modelHandle, VGet(cModelSizeScale, cModelSizeScale, cModelSizeScale));

		//待機アニメーション設定
		m_nowAnimIdx = m_animIdx["Idle"];

		ChangeAnim(m_nowAnimIdx, m_animOne[20], m_animOne);


		m_armorOne[one] = true;

	}
	//他の配列をfalseにする
	for (int i = 0; i < 10; i++)
	{
		if (i != one)
		{
			m_armorOne[i] = false;
		}
	}
}

/// <summary>
/// レベルアップによるステータス上昇
/// </summary>
void Player::ChangeStatus()
{
	CsvLoad::GetInstance().StatusLoad(m_status, "Player");

	//とりあえず適当に補正しておく
	m_status.s_hp = m_status.s_hp + (m_levelStatus.sl_hp * 30);
	m_status.s_stamina = m_status.s_stamina + (m_levelStatus.sl_stamina * 10);

	//これでいい
	m_status.s_muscle = m_levelStatus.sl_muscle;
	m_status.s_skill = m_levelStatus.sl_skill;
	//ステータスの最大値を入れる
	ms_maxStatus.sm_hp = m_status.s_hp;
	ms_maxStatus.sm_stamina = m_status.s_stamina;
}

void Player::SetModelPos()
{
	m_modelPos = m_collisionPos;
}
