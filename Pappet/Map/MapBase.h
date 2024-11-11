#pragma once
#include "Library/MyLibrary.h"
#include "Object/SearchObject.h"
#include "Object/RectObject.h"
#include <string>

/// <summary>
/// �}�b�v�̊��N���X
/// </summary>
class MapBase
{
public:
	//�R���X�g���N�^
	MapBase();
	//�f�X�g���N�^
	virtual ~MapBase();

	//������
	virtual void Init() {};
	//�I��
	virtual void Finalize(std::shared_ptr<MyLibrary::Physics> physics);
	//�X�V
	virtual void Update(MyLibrary::LibVec3 playerPos) {};
	//�`��
	virtual void Draw() {};

protected:
	//���f����ǂݍ���
	void LoadData(std::string mapPath, std::string collisionPath);
	//���G��������铖���蔻����쐬
	void InitSearch(float radius, MyLibrary::LibVec3 pos);
	//�{�X�����̓���
	void InitRect(float width, float hight, float depth, MyLibrary::LibVec3 pos);

protected:
	//�����N���X�̃|�C���^
	std::shared_ptr<MyLibrary::Physics> m_pPhysics;

	std::shared_ptr<SearchObject> m_pSearch;   //���G����
	std::shared_ptr<RectObject> m_pRect;       //��`����

	int m_handle;             //���f���̃n���h��
	int m_collisionHandle;    //���f���̓����蔻��p�̃n���h��
};

