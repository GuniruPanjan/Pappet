#pragma once
#include "Library/MyLibrary.h"
#include "Map/MapBase.h"
#include <memory>

/// <summary>
/// �}�b�v���Ǘ�����N���X
/// </summary>
class MapManager
{
public:
	//�R���X�g���N�^
	MapManager();
	//�f�X�g���N�^
	virtual ~MapManager();

	//�f�[�^�̏�����
	void DataInit();
	//����������
	void Init(std::shared_ptr<MyLibrary::Physics> physics);
	//�X�V����
	void Update(std::shared_ptr<MyLibrary::Physics> physics);
	//�����������̔��菈��
	void JudgeUpdate();
	//���������������
	void TriggerReset();
	//�`�揈��
	void Draw();
	//�I������
	void End(std::shared_ptr<MyLibrary::Physics> physics);

	//�R���W�����}�b�v��Ԃ�
	int GetCollisionMap() { return m_pMap->GetCollisionMap(); }

	//�x���n�_��Ԃ�
	bool GetRest() { return m_pMap->GetRest(); }

	//�{�X����
	bool GetBossRoom() { return m_pMap->GetBossRoom(); }
private:
	//�X�}�[�g�|�C���^
	std::shared_ptr<MapBase> m_pMap;
};

