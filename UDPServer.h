#pragma once
#include "Engine/GameObject.h"

class UDPServer :public GameObject
{

	struct Circle {
		int x;
		int y;
		int size;
		unsigned int color;
	};   // Structure to store the received circle data
	Circle circle_[2];
	Circle circle0_;
	int UDPHandle_;
	IPDATA IP_[2];
public:

	//�R���X�g���N�^
	//�����Fparent  �e�I�u�W�F�N�g�iObjectManager�j
	UDPServer(GameObject* parent);

	//�f�X�g���N�^
	~UDPServer();

	//������
	void Initialize() override;

	//�X�V
	void Update() override;

	//�`��
	void Draw() override;

	//�J��
	void Release() override;
};

