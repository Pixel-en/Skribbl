#pragma once
#include "Engine/GameObject.h"
#include <string>

class Player :public GameObject
{

	XMINT2 PreMousePos_;
	XMINT2 NowMousePos_;

	unsigned int Cr_;

	bool drawOK_;

	int linesize_;
	bool Erase_;
	
	//�����S�����[�h���ǂ���
	bool RFlag_;

	std::string theme_;

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
};

