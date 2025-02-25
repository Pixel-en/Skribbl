#pragma once
#include "Engine/GameObject.h"

class UDPClient :public GameObject
{
	IPDATA IpAddr;
	int UDPHandle;
	int hConnectCheck_;
	int ServerPort_;
	bool isConnect_;
	struct Circle {
		int x;
		int y;
		int size;
		unsigned int color;
	};
	
	Circle me;
	Circle you;

	std::string name_;

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

	void SetName(std::string _name) { name_ = _name; }
	std::string GetName() { return name_; }
};

