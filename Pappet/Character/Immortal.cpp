#include "Immortal.h"

namespace
{
	//�L�����N�^�[��
	constexpr const char* cCharacterName = "Immortal";
	//���f���p�X
	constexpr const char* cModelPath = "Data/Enemy/immortalModel.mv1";
	//���f���̃T�C�Y�̊g�嗦
	constexpr float cModelSize = 0.4f;
	//���f���̒���
	constexpr float cCapsuleLen = 40.0f;
	//���f���̔��a
	constexpr float cCapsuleRadius = 14.0f;
	//���G�͈�
	constexpr float cSearchRadius = 120.0f;

}

/// <summary>
/// �R���X�g���N�^
/// </summary>
Immortal::Immortal() :
	EnemyBase(Collidable::Priority::Middle)
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
Immortal::~Immortal()
{
	//���������
	MV1DeleteModel(m_modelHandle);
}

/// <summary>
/// ������
/// </summary>
/// <param name="physics">�����N���X�̃|�C���^</param>
void Immortal::Init(float posX, float posY, float posZ, std::shared_ptr<MyLibrary::Physics> physics)
{
	//���
	m_pPhysics = physics;

	m_isExist = true;

	Collidable::Init(m_pPhysics);

	//�����N���X�̏�����
	InitRigidbody(posX, posY, posZ, false);

	//���S���W�̐ݒ�
	CalculationCenterPos(1.0f, cModelSize);

	//���f���̍��W��ݒ�
	SetModelPos(cModelSize);
	MV1SetPosition(m_modelHandle, m_modelPos.ConversionToVECTOR());

	//���G��������铖���蔻����쐬
	InitSearch(cSearchRadius);

	//���f���̃T�C�Y�ݒ�
	MV1SetScale(m_modelHandle, VGet(cModelSize, cModelSize, cModelSize));

	//�A�j���[�V�����ݒ�
	m_nowAnimNo = MV1AttachAnim(m_modelHandle, m_animIdx["Idle"]);
	m_nowAnimIdx = m_animIdx["Idle"];
}

/// <summary>
/// �X�V
/// </summary>
/// <param name="playerPos">�v���C���[���W</param>
/// <param name="isChase">�v���C���[�Ɛ킦�邩�ǂ���</param>
void Immortal::Update(MyLibrary::LibVec3 playerPos, bool isChase)
{

}

/// <summary>
/// �`��
/// </summary>
void Immortal::Draw()
{
	//�����蔻����W���擾���ă��f���̕`����W��ݒ肷��
	SetDrawModelPos(1.0f * cModelSize);
	//���f���̕`��
	MV1DrawModel(m_modelHandle);
}