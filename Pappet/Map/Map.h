#pragma once
#include "DxLib.h"
#include<memory>

class Player;
class Effect;

/// <summary>
/// �}�b�v���Ǘ�����N���X
/// </summary>
class Map
{
public:
	//�R���X�g���N�^
	Map();
	//�f�X�g���N�^
	virtual ~Map();

	//����������
	void Init();

	//�`�揈��
	void Draw();

	//�I������
	void End();

	//�}�b�v�̃R���W�����n���h����Ԃ�
	int GetCollisionMap() { return m_collisionHandle; }

	//�}�b�v�̃{�X������ɓ��������Ԃ�
	bool GetRoomEntered() { return m_bossRoomEntered; }

	//�}�b�v�̋x���ꏊ��Ԃ�
	bool GetSavePossible() { return m_saveSpot; }

	//�}�b�v�̃|�W�V������Ԃ�
	VECTOR GetVectorMapPos() { return m_MapPosition; }

	//�}�b�v�̋x���|�C���g��Ԃ�
	VECTOR GetRestPos() { return m_restPos; }

private:
	int m_handle;   //�}�b�v�̃f�[�^������ϐ�
	int m_collisionHandle;   //�}�b�v�̃R���W�����̃f�[�^������ϐ�
	int m_light;    //���C�g�����ϐ�
	float m_size;   //���f���̃T�C�Y�ϐ�
	float m_Xposition;    //X�|�W�V����
	float m_Yposition;    //Y�|�W�V����
	float m_Zposition;    //Z�|�W�V����
	float m_XCollisionposition;    //X�|�W�V����
	float m_YCollisionposition;    //Y�|�W�V����
	float m_ZCollisionposition;    //Z�|�W�V����
	bool m_bossRoomEntered;    //�{�X�����ɓ���������
	bool m_saveSpot;        //�񕜃X�|�b�g���g���锻��
	bool m_oneInit;         //��񂾂�����������
	VECTOR m_MapPosition;   //�}�b�v�̃|�W�V����
	VECTOR m_collisionMapPosition;   //�R���W�����̃|�W�V�����ݒ�
	VECTOR m_restPos;        //�x������|�W�V����

};




