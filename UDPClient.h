#pragma once
#include "Engine/GameObject.h"

class UDPClient :public GameObject
{
	int hFrame_;

	IPDATA IpAddr;
	int UDPHandle;
	int hConnectCheck_;
	int ServerPort_;
	bool isConnect_;

	bool isCorrect_;

	int drawerhandle_;

	int playernum_;

	std::string name_;

	bool end_;

	int hKeyData_;
	int h64Font_;
	bool NowKeyInput_;
	bool IPSet_;

	void UpdateInit();
	void UpdateConnect();
	void UpdatePlay();
	void UpdateClose();

	void DrawInit();
	void DrawConnect();
	void DrawPlay();
	void DrawClose();

	struct User
	{
		std::string name_;
		int point_;
		bool drawer_;
	};
	User user[4];
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

