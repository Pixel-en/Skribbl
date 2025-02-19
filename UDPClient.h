#pragma once
#include "Engine/GameObject.h"

class UDPClient :public GameObject
{
	IPDATA IpAddr;
	int UDPHandle;
	int ServerPort_;

	struct Circle {
		int x;
		int y;
		int size;
		unsigned int color;
	};
	
	Circle me;
	Circle you;

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

