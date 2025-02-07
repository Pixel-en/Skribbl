#pragma once
#include "Engine/GameObject.h"
#include "Theme.h"
#include "Chat.h"
class Theme;
class Chat;
class Player :public GameObject
{

	XMINT2 PreMousePos_;
	XMINT2 NowMousePos_;

	unsigned int Cr_;

	bool drawOK_;

	int linesize_;
	bool Erase_;

	bool RFlag_;
	bool LFlag_;
	bool Lcount_;
	Theme* theme_;
	Chat* chat_;

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

	void DrawCurrentTheme();
};

