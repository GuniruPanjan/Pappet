#include "Bear.h"

namespace
{
	//�L�����N�^�[��
	constexpr const char* cCharacterName = "bear";
	//���f���p�X
	constexpr const char* cModelPath = "Data/Enemy/BearModel.mv1";
	//���f���̃T�C�Y�̊g�嗦
	constexpr float cModelSize = 0.7f;
	//���f���̒���
	constexpr float cCapsuleLen = 40.0f;
	//���f���̔��a
	constexpr float cCapsuleRadius = 25.0f;
	//���f���̍��W�����킹��
	constexpr float cModelPosY = 24.0f;
	//���G�͈�
	constexpr float cSearchRadius = 120.0f;
}

/// <summary>
/// �R���X�g���N�^
/// </summary>
Bear::Bear() :
	EnemyBase(Collidable::Priority::High)
{
	//�����蔻��̐ݒ�
	InitCollision(MyLibrary::LibVec3(0.0f, 2.0f, 0.0f), cCapsuleLen, cCapsuleRadius);
	//���f���̓ǂݍ���
	LoadModel(cModelPath);
	//�A�j���[�V������X�e�[�^�X���擾
	LoadData(cCharacterName);
	//���G�͈͂̐ݒ�
	m_searchRadius = cSearchRadius;
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Bear::~Bear()
{
	//���������
	MV1DeleteModel(m_modelHandle);
}

/// <summary>
/// ������
/// </summary>
/// <param name="posX"></param>
/// <param name="posY"></param>
/// <param name="posZ"></param>
/// <param name="physics"></param>
void Bear::Init(float posX, float posY, float posZ, std::shared_ptr<MyLibrary::Physics> physics)
{
	//���
	m_pPhysics = physics;

	m_isExist = true;

	Collidable::Init(m_pPhysics);

	//�����N���X�̏�����
	InitRigidbody(posX, posY + cCapsuleRadius, posZ);

	//���S���W�̐ݒ�
	CalculationCenterPos(1.0f, cModelSize);

	//���f���̍��W��ݒ�
	SetModelPos();
	MV1SetPosition(m_modelHandle, m_modelPos.ConversionToVECTOR());

	//���G��������铖���蔻����쐬
	InitSearch(cSearchRadius, 35.0f);

	//���f���̃T�C�Y�ݒ�
	MV1SetScale(m_modelHandle, VGet(cModelSize, cModelSize, cModelSize));

	//�A�j���[�V�����ݒ�
	m_nowAnimNo = MV1AttachAnim(m_modelHandle, m_animIdx["Idle"]);
	m_nowAnimIdx = m_animIdx["Idle"];
}

/// <summary>
/// �X�V����
/// </summary>
/// <param name="playerPos"></param>
/// <param name="isChase"></param>
void Bear::Update(MyLibrary::LibVec3 playerPos, bool isChase)
{
	//�A�j���[�V�����̍X�V
	m_isAnimationFinish = UpdateAnim(m_nowAnimNo, ANIMATION_MAX);

	//rigidbody�̃|�W�V������������Ă�
	// ���������ŏ��ɓ�����̂���������
	//m_modelPos�͖���
	//m_collisionPos������

	//����̍X�V
	MyLibrary::LibVec3 centerPos = rigidbody.GetPos();
	m_pSearch->Update(centerPos);
}

/// <summary>
/// �`�揈��
/// </summary>
void Bear::Draw()
{
	//�����蔻����W���擾���ă��f���̕`����W��ݒ肷��
	SetDrawModelPos(cModelPosY);
	//���f���̕`��
	MV1DrawModel(m_modelHandle);
}
