#include "Player.h"
#include "Manager/EffectManager.h"
#include "Manager/HandleManager.h"
#include "External/CsvLoad.h"

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
	//ボタンが押されているかの確認用変数
	int cAbutton = 0;
	int cRbutton = 0;
	bool cRstickButton = false;
	//索敵範囲
	constexpr float cSearchRadius = 200.0f;
	//回避での移動距離
	float cAvoidanceMove = 0.0f;
	//攻撃での追加攻撃時間
	int cAddAttackTime = 0;
	//拳の攻撃範囲
	constexpr float cFistAttackRadius = 18.0f;
	//現在のアタックのナンバーを入れる
	int cNowAttackNumber = 0;
	//攻撃の終了判定
	int cIsEndAttack = 0;

	//シングルトン
	auto& handle = HandleManager::GetInstance();
	auto& effect = EffectManager::GetInstance();
}

Player::Player() :
	CharacterBase(Collidable::Priority::Low, ObjectTag::Player),
	m_xpad(),
	m_attackNumber(0),
	m_menuOpen(false),
	m_lockonTarget(false),
	m_moveAnimFrameIndex(0),
	m_moveAnimFrameRight(0),
	m_moveAnimShieldFrameIndex(0),
	m_moveAnimShieldFrameHandIndex(0),
	m_cameraAngle(0.0f),
	m_avoidanceNow(false),
	m_moveWeaponFrameMatrix(),
	m_moveShieldFrameMatrix(),
	m_rollMove(VGet(0.0f,0.0f,0.0f))
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
	
	//エフェクト読み込み
	effect.EffectLoad("Rest", "Data/Effect/Benediction.efkefc", 210, 10.0f);
	effect.EffectLoad("Heal", "Data/Effect/AnotherEffect/Sylph13.efkefc", 160, 20.0f);
	effect.EffectLoad("Imapct", "Data/Effect/HitEffect.efkefc", 30, 7.0f);

	//モデル読み込み
	m_modelHandle = handle.GetModelHandle("Data/Player/PlayerModel.mv1");

	//モデルのサイズ設定
	MV1SetScale(m_modelHandle, VGet(cModelSizeScale, cModelSizeScale, cModelSizeScale));

	//Csvによるデータの読み込み
	CsvLoad::GetInstance().AnimDataLoad("Player", m_animIdx);
	CsvLoad::GetInstance().StatusLoad(m_status, "Player");
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
void Player::Init(std::shared_ptr<MyLibrary::Physics> physics)
{
	m_pPhysics = physics;

	//当たり判定の追加
	Collidable::Init(m_pPhysics);

	//プレイヤーの初期位置設定
	rigidbody.Init(false);
	rigidbody.SetPos(MyLibrary::LibVec3(485.0f, 12.0f, -800.0f));
	rigidbody.SetNextPos(rigidbody.GetPos());
	rigidbody.SetVec(MyLibrary::LibVec3(0.0f, 40.0f, 0.0f));
	m_collisionPos = rigidbody.GetPos();
	SetModelPos();
	MV1SetPosition(m_modelHandle, m_modelPos.ConversionToVECTOR());

	//メニューを閉じる
	m_menuOpen = false;

	//拳だった場合
	m_attackRadius = cFistAttackRadius;

	m_pAttack = std::make_shared<AttackObject>(m_attackRadius);

	m_pSearch = std::make_shared<SearchObject>(cSearchRadius);
	m_pSearch->Init(m_pPhysics, rigidbody.GetPos(), false, false, false, true);

	//待機アニメーション設定
	m_nowAnimNo = MV1AttachAnim(m_modelHandle, m_animIdx["Idle"]);
	m_nowAnimIdx = m_animIdx["Idle"];
}

void Player::Finalize()
{
	Collidable::Finalize(m_pPhysics);
}

void Player::Update()
{
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

	//アニメーションの更新
	m_isAnimationFinish = UpdateAnim(m_nowAnimNo, ANIMATION_MAX);

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

			Finalize();
		}
	}

	//アナログスティックを使って移動
	int analogX = 0;
	int analogY = 0;

	float SetAngleX = 0.0f;
	float SetAngleY = 0.0f;

	if (!m_anim.s_isDead && !m_animChange.sa_avoidance && !m_anim.s_attack && !m_animChange.sa_recovery)
	{
		GetJoypadAnalogInput(&analogX, &analogY, DX_INPUT_PAD1);
	}

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
		m_angle = atan2f(-moveVec.z, moveVec.x) - DX_PI_F / 2;

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

	//プレイヤーが生きている時だけ
	if (!m_anim.s_isDead)
	{
		//メニューを開いている間はアクションできない
		if (!m_menuOpen)
		{
			//アクションをできなくする
			if (!m_animChange.sa_avoidance || !m_anim.s_hit || !m_animChange.sa_recovery)
			{
				Action();
			}
		}

		NotWeaponAnimation();
		AllAnimation();
	}

	//プレイヤーのポジションを入れる
	SetModelPos();

	//判定の更新
	MyLibrary::LibVec3 centerPos = rigidbody.GetPos();
	MyLibrary::LibVec3 attackPos = MyLibrary::LibVec3(rigidbody.GetPos().x + sinf(m_angle) * -25.0f, rigidbody.GetPos().y + 15.0f, rigidbody.GetPos().z - cosf(m_angle) * 25.0f);
	m_pSearch->Update(centerPos);
	m_pAttack->Update(attackPos);

	//回避行動中
	if (!m_isAnimationFinish && m_animChange.sa_avoidance)
	{
		//フレーム回避
		if (m_nowFrame >= 0.0f && m_nowFrame <= 20.0f)
		{
			m_avoidanceNow = true;

			cAvoidanceMove = 4.0f;
		}
		else if (m_nowFrame >= 20.0f && m_nowFrame <= 30.0f)
		{
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

			//攻撃判定発生フレーム
			if (m_nowFrame == 25.0f)
			{
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

			//攻撃判定発生フレーム
			if (m_nowFrame == 55.0f)
			{
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

			//攻撃判定発生フレーム
			if (m_nowFrame == 85.0f)
			{
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

	//回復する
	if (!m_isAnimationFinish && m_animChange.sa_recovery)
	{

	}
	//回復終了
	else if (m_isAnimationFinish && m_animChange.sa_recovery)
	{
		m_animChange.sa_recovery = false;
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
	else if (!m_pSearch->GetIsStay())
	{
		m_lockonTarget = false;
		cRstickButton = false;
	}

	//Aボタンが押されたらダッシュか回避
	if (m_xpad.Buttons[12] == 1 && !m_anim.s_attack)
	{
		//ダッシュ
		if (cAbutton > 50)
		{
			m_animChange.sa_avoidance = false;

			//ダッシ中
			m_animChange.sa_dashMove = true;

			m_status.s_speed = cDashSpeed;
		}

		if (cAbutton < 51)
		{
			cAbutton++;
		}
	}
	else
	{
		m_animChange.sa_dashMove = false;

		m_status.s_speed = cWalkSpeed;

		//回避
		//離した瞬間
		if (cAbutton > 0 && cAbutton < 30 && m_animChange.sa_avoidance == false)
		{
			m_animChange.sa_avoidance = true;
		}

		cAbutton = 0;
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
	//追加攻撃受付時間を減らす
	if (cAddAttackTime <= 40 && cAddAttackTime > 0 && --cAddAttackTime > -1);

	//回復
	//Xボタンが押されたら
	if (m_xpad.Buttons[14] == 1 && !m_anim.s_attack)
	{
		m_animChange.sa_recovery = true;
	}
	
	//メニューを開く
	if (m_xpad.Buttons[4] == 1)
	{
		m_menuOpen = true;
	}
}

/// <summary>
/// 武器を持ってないときのアニメーション
/// </summary>
void Player::NotWeaponAnimation()
{
	//攻撃が当たってない時
	if (!m_anim.s_hit)
	{
		//走り
		if (m_animChange.sa_dashMove && m_anim.s_moveflag)
		{
			m_nowAnimIdx = m_animIdx["Run"];
			ChangeAnim(m_nowAnimIdx, m_animOne[1], m_animOne);
		}
		//歩き
		else if (m_anim.s_moveflag)
		{
			m_nowAnimIdx = m_animIdx["Walk"];
			ChangeAnim(m_nowAnimIdx, m_animOne[2], m_animOne);
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
		if (m_anim.s_hit)
		{
			m_nowAnimIdx = m_animIdx["Hit"];
			ChangeAnim(m_nowAnimIdx, m_animOne[3], m_animOne);
		}
		//攻撃が当たってないとき
		else if (!m_anim.s_hit)
		{
			//動いてない時
			if (!m_anim.s_moveflag && !m_animChange.sa_avoidance && !m_anim.s_attack && !m_animChange.sa_recovery)
			{
				m_nowAnimIdx = m_animIdx["Idle"];
				ChangeAnim(m_nowAnimIdx, m_animOne[4], m_animOne);
			}
			//回避
			else if (m_animChange.sa_avoidance)
			{
				m_nowAnimIdx = m_animIdx["Roll"];
				ChangeAnim(m_nowAnimIdx, m_animOne[5], m_animOne);
			}
			//攻撃
			else if (m_anim.s_attack && !m_animChange.sa_avoidance && !m_animChange.sa_recovery)
			{
				m_nowAnimIdx = m_animIdx["Attack1"];
				ChangeAnim(m_nowAnimIdx, m_animOne[6], m_animOne, 1.0f);
			}
			//回復
			else if (m_animChange.sa_recovery)
			{
				m_nowAnimIdx = m_animIdx["Recovery"];
				ChangeAnim(m_nowAnimIdx, m_animOne[7], m_animOne);
			}
			//アイテムを取得するとき
			else if (m_animChange.sa_taking)
			{
				m_nowAnimIdx = m_animIdx["Taking"];
				ChangeAnim(m_nowAnimIdx, m_animOne[8], m_animOne);
			}
			//ギミックを作動させるとき
			else if (m_animChange.sa_touch)
			{
				m_nowAnimIdx = m_animIdx["Touch"];
				ChangeAnim(m_nowAnimIdx, m_animOne[9], m_animOne);
			}
		}
	}
}

/// <summary>
/// 武器を持ってる状態の時に行うアニメーション
/// </summary>
void Player::WeaponAnimation()
{
}

/// <summary>
/// 描画処理
/// </summary>
void Player::Draw()
{
	rigidbody.SetPos(rigidbody.GetNextPos());
	m_collisionPos = rigidbody.GetPos();
	
	MV1SetPosition(m_modelHandle, VSub(m_modelPos.ConversionToVECTOR(), VGet(0.0f, 12.0f, 0.0f)));

#if false
	DrawFormatString(0, 100, 0xffffff, "posx : %f", m_modelPos.x);
	DrawFormatString(0, 200, 0xffffff, "posy : %f", m_modelPos.y);
	DrawFormatString(0, 300, 0xffffff, "posz : %f", m_modelPos.z);
	DrawFormatString(0, 400, 0xffffff, "m_nowAnim : %d", m_nowAnimIdx);
	DrawFormatString(0, 500, 0xffffff, "m_nowSpeed : %f", m_nowFrame);
	DrawFormatString(0, 600, 0xffffff, "colPosx : %f", m_collisionPos.x);
	DrawFormatString(0, 700, 0xffffff, "colPosy : %f", m_collisionPos.y);
	DrawFormatString(0, 800, 0xffffff, "colPosz : %f", m_collisionPos.z);
	DrawFormatString(200, 100, 0xffffff, "m_blend : %f", m_animBlendRate);
	DrawFormatString(200, 200, 0xffffff, "addattacktime : %d", cAddAttackTime);
	DrawFormatString(200, 300, 0xffffff, "avoidancemove : %f", cAvoidanceMove);
	DrawFormatString(200, 400, 0xffffff, "attackNumber : %d", m_attackNumber);
	DrawFormatString(200, 500, 0xffffff, "nowAttackNumber : %d", cNowAttackNumber);

#endif

	DrawFormatString(200, 100, 0xffffff, "m_lockOnTarget : %d", m_lockonTarget);
	DrawFormatString(200, 150, 0xffffff, "m_attack : %d", m_pAttack->GetIsTrigger());

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
	case ObjectTag::Attack:
#if _DEBUG
		message += "攻撃";
#endif
		break;
	case ObjectTag::Shield:
#if _DEBUG
		message += "盾";
#endif
		break;
	case ObjectTag::Search:
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
	case ObjectTag::BossEnter:
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

void Player::SetModelPos()
{
	m_modelPos = m_collisionPos;
}
