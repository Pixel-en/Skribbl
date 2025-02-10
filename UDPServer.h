#pragma once
#include "Engine/GameObject.h"

class UDPServer :public GameObject
{
	struct Net {

		IPDATA IPAddr;
		int port;
	};
	Net net[2];
	int NetUDPHandle;
	struct circle {
		int x;
		int y;
		int size;
		unsigned int color;
	};   // Structure to store the received circle data
	circle receivedCircle[2];
	bool isCircleReceived; // Flag to check if circle data is received
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

