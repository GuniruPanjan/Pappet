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
	XINPUT_STATE m_xpad;  //パッド入力
	float m_cameraAngle;  //カメラ情報
	bool m_isDead;        //死亡判定
	bool m_dashMove;      //ダッシュ判断
};

