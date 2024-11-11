#pragma once
#include "Library/MyLibrary.h"

/// <summary>
/// マップの基底クラス
/// </summary>
class MapBase : public MyLibrary::Collidable
{
public:
	MapBase();
	virtual ~MapBase();

	virtual void Init() {};
	virtual void Finalize(std::shared_ptr<MyLibrary::Physics> physics);
	virtual void Update(MyLibrary::LibVec3 playerPos) {};
	virtual void Draw() {};

	//衝突したとき
	void OnCollideStay(const std::shared_ptr<Collidable>& collidable) override;
	void OnTriggerStay(const std::shared_ptr<Collidable>& collidable) override;
};

