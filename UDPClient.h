#pragma once
#include "Engine/GameObject.h"

class UDPClient :public GameObject
{
	IPDATA ip;
	int netUDPHandle;

	struct Circle {
		int x;
		int y;
		int size;
		unsigned int color;
	};
	Circle circle;
	Circle youcircle;
public:

	//�R���X�g���N�^
	//�����Fparent  �e�I�u�W�F�N�g�iObjectManager�j
	UDPClient(GameObject* parent);

	//�f�X�g���N�^
	~UDPClient();

	//������
	void Initialize() override;

	//�X�V
	void Update() override;

	//�`��
	void Draw() override;

	//�J��
	void Release() override;
};

