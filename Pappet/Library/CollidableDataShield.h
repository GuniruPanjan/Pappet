#pragma once
#include "CollidableData.h"
#include "LibVec3.h"

namespace MyLibrary
{
	class CollidableDataShield : public CollidableData
	{
	public:
		//�R���X�g���N�^
		CollidableDataShield(bool isTrigger);

		//�T�C�Y
		LibVec3::Size m_size;
	};
}



