#pragma once
#include "Engine/GameObject.h"

namespace {
	const int CONNECTMAX{ 3 };
}

class UDPServer :public GameObject
{
	int drawerhandle_;
	int hStart_;
	int hFrame_;

	int h64Font_;
	int h32Font_;
	int hNameFrame_;
	int myPoint_;

	struct User
	{
		std::string name_;
		IPDATA IpAddr_;
		int RecvUDPHandle_;
		int point_;
	};

	enum NETWORKPART
	{
		NONE,
		INFO,
		PLAY,
	};
	NETWORKPART part;

	IPDATA MyIpAddr_;
	//�\�P�b�g
	int UDPConnectHandle_;

	User user[CONNECTMAX];
	std::string name_;

	//�ڑ��ԍ�
	int connectnum_;
	bool isConnect_;


	bool isCorrect_;	//�������Ă��邩
	float timer_;		//���o�^�C�}�[
	int drawerNum_;		//�G��`���Ă���l�̔ԍ�
	int questionNum_;	//��萔
	float DrawTimer_;

	void UpdateConnect();
	void UpdatePlay();
	void UpdateClose();

	void DrawConnect();
	void DrawPlay();
	void DrawClose();

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

	void SetName(std::string _name) { name_ = _name; };
};

