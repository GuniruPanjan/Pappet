#include "CollidableDataShield.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="isTrigger"></param>
MyLibrary::CollidableDataShield::CollidableDataShield(bool isTrigger) :
	CollidableData(CollidableData::Kind::ShieldRect, isTrigger),
	m_size()
{
}
