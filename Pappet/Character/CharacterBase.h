#pragma once
#include "Library/MyLibrary.h"
#include <map>
#include <string>
#define ANIMATION_MAX   30

class CharacterBase : public MyLibrary::Collidable
{
public:
	//ステータス構造体
	struct Status
	{
		float s_hp;          //体力
		float s_stamina;     //スタミナ
		float s_attack;      //攻撃力
		int s_muscle;        //筋力
		int s_skill;         //技量
		float s_defense;     //防御力
		float s_speed;       //速度
		int s_core;          //コア
	};
public:
	CharacterBase(Priority priority, ObjectTag tag);
	virtual ~CharacterBase();

	//衝突したとき
	virtual void OnCollideEnter(const std::shared_ptr<Collidable>& colider) = 0;

protected:
	//アニメーションの更新
	bool UpdateAnim(int attachNo, int max, float startTime = 0.0f);
	//アニメーションの変更
	void ChangeAnim(int animIndex, bool& one, bool (&all)[30], float animSpeed = 0.5f);

protected:
	//物理クラスのポインタ
	std::shared_ptr<MyLibrary::Physics> m_pPhysics;
	//モデルハンドル
	int m_modelHandle;
	//モデル座標
	MyLibrary::LibVec3 m_modelPos;
	//当たり判定座標
	MyLibrary::LibVec3 m_collisionPos;
	//移動ベクトル
	MyLibrary::LibVec3 m_moveVec;
	//ステータス
	Status m_status;

	//アニメーション関係
	std::map<std::string, int> m_animIdx;
	int m_nowAnimIdx;        
	int m_equipAnimNo;        //一つ前のアニメーション
	int m_nowAnimNo;          //現在のアニメーション
	int m_prevAnimNo;         //変更前のアニメーション
	float m_animBlendRate;    //アニメーションの合成割合

	float m_nowFrame;         //アニメーションを進める時間
	float m_animTime;         //アニメーション再生速度
	bool m_isAnimationFinish; //アニメーションが終わったかどうか
	float m_animSpeed;        //アニメーションの進んだタイムを入れる
	VECTOR m_nowPos;        //現在のフレームの座標を取得する

	//使う変数
	float m_angle;  //キャラのアングル
	bool m_moveflag;  //キャラが動いたか判断するフラグ
	bool m_hit;           //怯み判定
};

