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
	//�Q�[���̎d�l��ł̏�����
	virtual void GameInit(float posX, float posY, float posZ, std::shared_ptr<MyLibrary::Physics> physics) {};
	//�I��
	virtual void Finalize(std::shared_ptr<MyLibrary::Physics> physics);
	//�X�V
	virtual void Update(MyLibrary::LibVec3 playerPos, bool isChange) {};
	//�`��
	virtual void Draw() {};
	//�I������
	virtual void End();

	//�Փ˂����Ƃ�
	void OnCollideEnter(const std::shared_ptr<Collidable>& collidable) override;
	void OnCollideStay(const std::shared_ptr<Collidable>& collidable) override;
	void OnTriggerEnter(const std::shared_ptr<Collidable>& collidable) override;
	void OnTriggerStay(const std::shared_ptr<Collidable>& collidable) override;
	void OnTriggerExit(const std::shared_ptr<Collidable>& collidable) override;

	//�^�[�Q�b�g�ɓ������Ă��邱�Ƃ�Ԃ�
	bool GetStay() const;
	//�^�[�Q�b�g����O�ꂽ���Ƃ�Ԃ�
	bool GetExit();

	//���W��Ԃ�
	virtual MyLibrary::LibVec3 GetPos() const { return m_collisionPos; }
	//�^�[�Q�b�g�̗L����Ԃ�
	const bool GetTarget() const { return m_isTarget; }
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

	//�{�X�����̔���
	bool SetBossRoom(bool set) { return m_isBossDiscovery = set; }

protected:
	//�ق��̃I�u�W�F�N�g�Ɖ����o����������铖���蔻����쐬
	void InitCollision(MyLibrary::LibVec3 vec, float len, float radius);
	//���f����ǂݍ���
	void LoadModel(std::string path);
	//�A�j���[�V��������A�X�e�[�^�X����ǂݍ���
	void LoadData(std::string name);
	//�X�e�[�^�X���̏�����
	void LoadDataInit(std::string name);
	//�����N���X�̏�����
	void InitRigidbody(float posX, float posY, float posZ, bool isUseGravity = true);
	//���f���̒��S���W���v�Z
	void CalculationCenterPos(float modeldefaultSize, float modelSize);
	//���f�����W��ݒ�
	void SetModelPos();
	//���G��������铖���蔻����쐬
	void InitSearch(float radius, float y);
	//�U������̏�����
	void InitAttack(float radius);
	//�U����������铖���蔻����쐬
	void InitAttackUpdate(float attack);
	//���G�̍X�V����
	void TriggerUpdate();
	//�v���C���[�Ƃ̋����𑪂鏈���Ə��Ƃ̋����𑪂鏈��
	void DistanceUpdate(MyLibrary::LibVec3 playerPos);
	//��������������
	void AngleUpdate(MyLibrary::LibVec3 playerPos);
	//�ړ������鏈��
	void MoveUpdate();
	//�A�C�h����Ԃɂ���
	void IdleUpdate();
	//������Ԃɂ���
	void WalkUpdate(std::string path, int number);
	//�U�������Ԃɂ���
	void AttackUpdate(std::string path, int number);
	//���ݏ�Ԃɂ���
	void HitUpdate(int number);
	//�^�[�Q�b�g���O�ꂽ���ǂ���
	void TargetNow();
	//�q�b�g�̍X�V����
	void HitTriggerUpdate();
	//�_���[�W���󂯂��Ƃ�
	void OnDamage();
	//���S������
	void Death();
	//�A�j���[�V�����u�����h�̍X�V
	void UpdateAnimationBlend();
	//���f���̍��W�ݒ�
	void SetDrawModelPos(float posY);
protected:
	std::shared_ptr<EnemyAttackObject> m_pAttack;    //�U������
	std::shared_ptr<SearchObject> m_pSearch;    //���G����

	AttackObject* m_col;

	int m_randomAction;       //�����_���ɍs�����邽�߂̕ϐ�
	int m_dropCore;              //���S���Ƀh���b�v����R�A

	float m_hpRadius;            //HP�o�[��\�����铖���蔻��̔��a
	float m_moveTurning;         //���v����ɐ��񂷂�@���x�N�g��
	float m_moveReverseTurning;  //�����v����ɐ��񂷂�@���x�N�g��
	float m_difPSize;            //�v���C���[�Ƃ̋����̃T�C�Y������

	bool m_isDroped;             //�R�A���h���b�v�������ǂ���
	bool m_isExist;              //���݂��邩�ǂ���
	bool m_isDiscovery;          //�v���C���[�����������ǂ���
	bool m_isBossDiscovery;      //�{�X��ɓ��������ǂ���
	bool m_isTarget;             //�v���C���[�Ƀ^�[�Q�b�g����邩�ǂ���
	bool m_isStayTarget = false; //�v���C���[�̍��G�ɓ������Ă邩�ǂ���
	bool m_isExitTarget = false; //�v���C���[�̍��G����O�ꂽ���ǂ���
	bool m_isEnterHit = false;   //�v���C���[�̍U�����������Ă��邩�ǂ���
	bool m_isExitHit = false;    //�v���C���[�̍U�����O�ꂽ���ǂ���

	MyLibrary::LibVec3 m_centerPos;     //���S���W
	VECTOR m_move;
	VECTOR m_difPlayer;          //�v���C���[�Ƃ̋���

	int m_I;

};

