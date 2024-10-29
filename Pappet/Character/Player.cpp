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

	//ダッシュを確認するための変数
	int cAbutton = 0;

	//ポジション設定
	VECTOR m_pos;

	//シングルトン
	auto& handle = HandleManager::GetInstance();
	auto& effect = EffectManager::GetInstance();
}

Player::Player() :
	CharacterBase(Collidable::Priority::High, ObjectTag::Player),
	m_xpad(),
	m_cameraAngle(0.0f),
	m_isDead(false),
	m_dashMove(false)
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
	m_pos = VGet(0.0f, 0.0f, 0.0f);

	m_pPhysics = physics;

	//当たり判定の追加
	Collidable::Init(m_pPhysics);

	//プレイヤーの初期位置設定
	rigidbody.Init(false);
	rigidbody.SetPos(MyLibrary::LibVec3(485.0f, 0.0f, -800.0f));
	rigidbody.SetNextPos(rigidbody.GetPos());
	rigidbody.SetVec(MyLibrary::LibVec3(0.0f, 40.0f, 0.0f));
	m_collisionPos = rigidbody.GetPos();
	SetModelPos();
	MV1SetPosition(m_modelHandle, m_modelPos.ConversionToVECTOR());

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
	//パッド入力取得
	GetJoypadXInputState(DX_INPUT_KEY_PAD1, &m_xpad);

	//アニメーションの更新
	m_isAnimationFinish = UpdateAnim(m_nowAnimNo);

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
		if (!m_isDead)
		{
			m_isDead = true;

			m_nowAnimIdx = m_animIdx["Death"];
			ChangeAnim(m_nowAnimIdx);

			Finalize();
		}
	}

	//座標を取得する
	m_collisionPos = rigidbody.GetPos();

	//アナログスティックを使って移動
	int analogX = 0;
	int analogY = 0;

	float SetAngleX = 0.0f;
	float SetAngleY = 0.0f;

	if (!m_isDead)
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
		m_moveflag = true;
	}
	//プレイヤーが動いてなかったら
	else if (VSquareSize(moveVec) <= 0.0f)
	{
		m_moveflag = false;
	}

	MyLibrary::LibVec3 prevVelocity = rigidbody.GetVelocity();
	MyLibrary::LibVec3 newVelocity = MyLibrary::LibVec3(m_moveVec.x, prevVelocity.y, m_moveVec.z);
	rigidbody.SetVelocity(newVelocity);

	//プレイヤーが生きている時だけ
	if (!m_isDead)
	{
		m_modelPos = m_modelPos + m_moveVec;

		Action();

		NotWeaponAnimation();
		AllAnimation();
	}
}

/// <summary>
/// プレイヤーのアクション実装
/// </summary>
void Player::Action()
{
	//Aボタンが押されたらダッシュか回避
	if (m_xpad.Buttons[12] == 1)
	{
		if (cAbutton > 50)
		{
			//ダッシ中
			m_dashMove = true;

			m_status.s_speed = 3.0f;
		}

		if (cAbutton < 51)
		{
			cAbutton++;
		}
	}
	else
	{
		m_dashMove = false;

		m_status.s_speed = 2.0f;

		cAbutton = 0;
	}
}

/// <summary>
/// 武器を持ってないときのアニメーション
/// </summary>
void Player::NotWeaponAnimation()
{
	//攻撃が当たってない時
	if (!m_hit)
	{
		//走り
		if (m_dashMove)
		{
			m_nowAnimIdx = m_animIdx["Run"];
			ChangeAnim(m_nowAnimIdx);
		}
		//歩き
		else if (m_moveflag)
		{
			m_nowAnimIdx = m_animIdx["Walk"];
			ChangeAnim(m_nowAnimIdx);
		}
	}
}

/// <summary>
/// 全ての状態の時に行うアニメーション関数
/// </summary>
void Player::AllAnimation()
{
	//プレイヤーが生きているときだけ
	if (!m_isDead)
	{
		//攻撃が当たった時
		if (m_hit)
		{

		}
		//攻撃が当たってないとき
		else if (!m_hit)
		{
			//動いてない時
			if (!m_moveflag)
			{
				m_nowAnimIdx = m_animIdx["Idle"];
				ChangeAnim(m_nowAnimIdx);
			}
		}
	}
}

/// <summary>
/// 描画処理
/// </summary>
void Player::Draw()
{
	rigidbody.SetPos(rigidbody.GetNextPos());
	m_collisionPos = rigidbody.GetPos();
	SetModelPos();
	MV1SetPosition(m_modelHandle, VSub(m_modelPos.ConversionToVECTOR(), VGet(0.0f, 12.0f, 0.0f)));

#if false
	DrawFormatString(0, 100, 0xffffff, "posx : %f", m_modelPos.x);
	DrawFormatString(0, 200, 0xffffff, "posy : %f", m_modelPos.y);
	DrawFormatString(0, 300, 0xffffff, "posz : %f", m_modelPos.z);
	DrawFormatString(0, 400, 0xffffff, "m_nowAnim : %d", m_nowAnimIdx);
	DrawFormatString(0, 500, 0xffffff, "m_nowSpeed : %f", m_animSpeed);
	DrawFormatString(0, 600, 0xffffff, "moveVecx : %f", m_moveVec.x);
	DrawFormatString(0, 700, 0xffffff, "moveVecy : %f", m_moveVec.y);
	DrawFormatString(0, 800, 0xffffff, "moveVecz : %f", m_moveVec.z);

#endif

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
}

void Player::OnTriggerEnter(const std::shared_ptr<Collidable>& collidable)
{
}

void Player::SetModelPos()
{
	m_modelPos = m_collisionPos;
	//m_collisionPos.y += 50.0f * cModelSizeScale;
	//m_modelPos.y -= 90.0f * cModelSizeScale;
}
