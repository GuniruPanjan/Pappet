#include "ItemBase.h"

/// <summary>
/// �I������
/// </summary>
/// <param name="physics"></param>
void ItemBase::ItemFinalize(std::shared_ptr<MyLibrary::Physics> physics)
{
	m_pSearch->Finalize(physics);
}

/// <summary>
/// �I������
/// </summary>
void ItemBase::ItemEnd()
{
}

/// <summary>
/// ���G��������铖���蔻����쐬
/// </summary>
/// <param name="pos"></param>
/// <param name="radius"></param>
void ItemBase::InitSearch(MyLibrary::LibVec3 pos, float radius)
{
	m_pSearch = std::make_shared<SearchObject>(radius);
	m_pSearch->Init(m_pPhysics, pos, true);
}
