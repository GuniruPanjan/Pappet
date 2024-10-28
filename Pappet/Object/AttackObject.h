#pragma once
#include "ObjectBase.h"
class AttackObject : public ObjectBase
{
public:
	AttackObject(float radius);
	~AttackObject();

	void Init(std::shared_ptr<MyLibrary::Physics> physics, MyLibrary::LibVec3 pos, bool isEnemy = false);
	void Update(MyLibrary::LibVec3 pos);

	void OnCollideEnter(const std::shared_ptr<Collidable>& collidable) {};

	void Finalize(std::shared_ptr<MyLibrary::Physics> physics) override;

	void CollisionEnd();

	void OnTriggerEnter(const std::shared_ptr<Collidable>& collidable) override;

	bool GetIsTrigger();

	const int GetAttack() const { return m_attack; }

private:
	std::shared_ptr<MyLibrary::Physics> m_pPhysics;

	bool m_isEnemy = false;

	//“–‚½‚è”»’è‚ÌØ‚è‘Ö‚¦
	bool m_isCollisionOn = false;

	int m_attack;
};

