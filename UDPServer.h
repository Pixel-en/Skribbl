#pragma once
#include "Engine/GameObject.h"

namespace {
	const int CONNECTMAX{ 1 };
}

class UDPServer :public GameObject
{

	struct Circle {
		int x;
		int y;
		int size;
		unsigned int color;
	};   // Structure to store the received circle data

	Circle me;
	Circle you;

	//�\�P�b�g
	int UDPHandle_[CONNECTMAX];
	int UDPConnectHandle_;
	//IP
	IPDATA IpAddr_[CONNECTMAX];
	//�ڑ��ԍ�
	int connectnum_;

	enum State
	{
		INIT,
		CONNECT,
		PLAY,
		CLOSE,
		END,

	};

	State state_;

	void UpdateInit();
	void UpdateConnect();
	void UpdatePlay();
	void UpdateClose();

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

