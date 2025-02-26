#pragma once
#include "Engine/GameObject.h"
#include<vector>
#include"Theme.h"
#include"Score.h"
#include <unordered_map>
namespace {
	const int CONNECTMAX{ 3 };
	//const float DRAWING_TIME{ 90.0f };
}

class UDPServer :public GameObject
{
	int h64Font_;
	int h32Font_;
	int hNameFrame_;

	struct Circle {
		int x;
		int y;
		int size;
		unsigned int color;
	};   // Structure to store the received circle data

	struct User
	{
		std::string name_;
		IPDATA IpAddr_;
		int RecvUDPHandle_;
	};
	Theme* theme_;
	Score* score_;
	Circle me;
	Circle you;

	IPDATA MyIpAddr_;
	//�\�P�b�g
	int UDPConnectHandle_;

	User user[CONNECTMAX];
	std::string name_;

	//�ڑ��ԍ�
	int connectnum_;
	bool isConnect_;
	float elapsedTime_;

	void UpdateConnect();
	void UpdatePlay();
	void UpdateClose();

	void DrawConnect();
	void DrawPlay();
	void DrawClose();

	std::string drawingOrder_[CONNECTMAX];
	int currentDrawerIndex_;
	int playerScores_[CONNECTMAX];
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

	void SetName(std::string _name) { name_ = _name; }
	std::string GetName(){ return name_; }
	void SetDrawingOrder();
	void StartNextTurn();
	void RollAndSendTheme();
};

