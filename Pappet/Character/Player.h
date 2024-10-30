#pragma once
#include "CharacterBase.h"
class Player : public CharacterBase
{
public:
	//レベル関係の構造体
	struct LevelUpStatus
	{
		int sl_hp;         //体力
		int sl_stamina;    //スタミナ
		int sl_muscle;     //筋力
		int sl_skill;      //技量
		int sl_all;        //すべてのレベル

	}m_levelStatus;

	Player();
	virtual ~Player();

	void Init(std::shared_ptr<MyLibrary::Physics> physics);
	void Finalize();
	void Update();
	void Action();
	void NotWeaponAnimation();
	void AllAnimation();
	void Draw();
	void End();

	//衝突したとき
	void OnCollideEnter(const std::shared_ptr<Collidable>& collidable) override;
	void OnTriggerEnter(const std::shared_ptr<Collidable>& collidable) override;

	void SetCameraAngle(float angle) { m_cameraAngle = angle; }

	const MyLibrary::LibVec3 GetPos() const { return rigidbody.GetPos(); }

private:
	void SetModelPos();
private:
	XINPUT_STATE m_xpad;                //パッド入力
	float m_cameraAngle;                //カメラ情報

	//アニメーション用変数
	bool m_animOne[ANIMATION_MAX];      //アニメーション関係をリセットするための判定
	bool m_isDead;                      //死亡判定
	bool m_dashMove;                    //ダッシュ判断
	bool m_avoidance;                   //回避判断
	bool m_avoidanceNow;                //フレーム回避中の判断

	//フレーム用変数
	int m_moveAnimFrameIndex;           //アニメーションで移動しているフレームの番号を検索する変数
	int m_moveAnimFrameRight;
	int m_moveAnimShieldFrameIndex;     //盾を構えるときのアニメーションのフレーム取得
	int m_moveAnimShieldFrameHandIndex;
	MATRIX m_moveWeaponFrameMatrix;     //武器をアタッチするフレームのローカル座標をワールド変換行列を取得する
	MATRIX m_moveShieldFrameMatrix;
};

