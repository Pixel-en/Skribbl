#pragma once
#include "Engine/GameObject.h"

class BackGround :public GameObject
{
public:

	//�R���X�g���N�^
	//�����Fparent  �e�I�u�W�F�N�g�iObjectManager�j
	BackGround(GameObject* parent);

	//�f�X�g���N�^
	~BackGround();

	//������
	void Initialize() override;

	//�X�V
	void Update() override;

	//�`��
	void Draw() override;

	//�J��
	void Release() override;
};

