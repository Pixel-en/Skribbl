#pragma once
#include "Engine/GameObject.h"


class Player :public GameObject
{
	struct INT2
	{
		int x;
		int y;
	};

	INT2 PreMousePos_;
	INT2 NowMousePos_;

	unsigned int Cr_;

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
};

