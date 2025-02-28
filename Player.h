#pragma once
#include "Engine/GameObject.h"
#include <string>

class Player :public GameObject
{
public:
	struct Pencil
	{

		XMINT2 PreMousePos_;	//1�t���[���O�̃}�E�X�̃|�W�V����
		XMINT2 NowMousePos_;	//���݂̃}�E�X�̃|�W�V����
		unsigned int Cr_;		//�F

		int linesize_;			//���C���̑傫��
		bool Erase_;			//�����S�����g���Ă��邩
	};
private:

	Pencil pen;
	Pencil youPen;


	bool drawOK_;

	
	//�����S�����[�h���ǂ���
	bool RFlag_;

	std::string theme_;

	bool isDrawer;

public:

	//�R���X�g���N�^
	//�����Fparent  �e�I�u�W�F�N�g�iObjectManager�j
	Player(GameObject* parent);

	//�f�X�g���N�^
	~Player();

	//������
	void Initialize() override;

	//�X�V
	void Update() override;

	//�`��
	void Draw() override;

	//�J��
	void Release() override;

	void SetTheme(std::string _theme) { theme_ = _theme; };

	Pencil GetPencil() { return pen; }
	void RecvPencil(Pencil _pen) { youPen = _pen; }
};

