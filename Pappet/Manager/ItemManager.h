#pragma once

/// <summary>
/// �A�C�e�����Ǘ�����N���X
/// </summary>
class ItemManager
{
private:
	//�A�C�e���̍\����
	struct Item
	{
		int SmallCore = 0;       //���^�R�A
		int MediumCore = 0;      //���^�R�A
		int Rubbish = 0;         //�S�~
		int BlackSword = 0;      //������
		int Distorted = 0;       //�c�񂾏�
	};

public:
	//�R���X�g���N�^
	ItemManager();
	//�f�X�g���N�^
	virtual ~ItemManager();
	//����������
	void Init();
	//�X�V����
	void Update();

	//�`�揈��
	void Draw();

	//�I������
	void End();

private:

};

