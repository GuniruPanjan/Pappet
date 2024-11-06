#pragma once
#include "CharacterBase.h"

/// <summary>
/// �G�l�~�[�̊�ՂƂȂ�N���X
/// </summary>
class EnemyBase : public CharacterBase
{
public:
	//�R���X�g���N�^
	EnemyBase(Priority priority);
	//�f�X�g���N�^
	virtual ~EnemyBase();

	//����������
	virtual void Init(float posX, float posY, float posZ, std::shared_ptr<MyLibrary::Physics> physics) {};
	//�I��
	virtual void Finalize(std::shared_ptr<MyLibrary::Physics> physics);
	//�X�V
	virtual void Update(MyLibrary::LibVec3 playerPos, bool isChange) {};
	//�`��
	virtual void Draw() {};

	//�Փ˂����Ƃ�
	void OnCollideEnter(const std::shared_ptr<Collidable>& collidable) override;
	void OnCollideStay(const std::shared_ptr<Collidable>& collidable) override;
	void OnTriggerEnter(const std::shared_ptr<Collidable>& collidable) override;

	//���W��Ԃ�
	virtual MyLibrary::LibVec3 GetPos() const { return m_collisionPos; }
	//�_���[�W��H��������ǂ���
	bool GetIsHit();
	//���S���ɗ��Ƃ��R�A�擾
	int GetDropCore();
	//���S���Ă��邩�ǂ������擾
	bool GetIsDead() const { return m_anim.s_isDead; }
	//���݂��Ă��邩�ǂ������擾
	const bool GetIsExist() const { return m_isExist; }
	//HP��\�����邽�߂̓����蔻����擾
	const float GetRadius() const;

	//���C�L���X�g�����邽�߂Ƀ��f���n���h�����擾
	const int GetModelHandle() const { return m_modelHandle; }
	//���݂�HP���擾
	const int GetHp() const { return m_status.s_hp; }

	int SetI(int &set) { return m_I = set; }

protected:
	//�ق��̃I�u�W�F�N�g�Ɖ����o����������铖���蔻����쐬
	void InitCollision(MyLibrary::LibVec3 vec, float len, float radius);
	//���f����ǂݍ���
	void LoadModel(std::string path);
	//�A�j���[�V��������A�X�e�[�^�X����ǂݍ���
	void LoadData(std::string name);
	//�����N���X�̏�����
	void InitRigidbody(float posX, float posY, float posZ, bool isUseGravity = true);
	//���f���̒��S���W���v�Z
	void CalculationCenterPos(float modeldefaultSize, float modelSize);
	//���f�����W��ݒ�
	void SetModelPos();
	//���G��������铖���蔻����쐬
	void InitSearch(float radius, float y);
	//���S������
	void Death();
	//�A�j���[�V�����u�����h�̍X�V
	void UpdateAnimationBlend();
	//���f���̍��W�ݒ�
	void SetDrawModelPos(float posY);
protected:
	std::shared_ptr<AttackObject> m_pAttack;    //�U������
	std::shared_ptr<SearchObject> m_pSearch;    //���G����

	int m_dropCore;      //���S���Ƀh���b�v����R�A

	float m_hpRadius;    //HP�o�[��\�����铖���蔻��̔��a

	bool m_isDroped;     //�R�A���h���b�v�������ǂ���
	bool m_isExist;      //���݂��邩�ǂ���
	bool m_isDiscovery;  //�v���C���[�����������ǂ���

	MyLibrary::LibVec3 m_centerPos;     //���S���W

	int m_I;

};

