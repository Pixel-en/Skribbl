#pragma once
#include "Engine/GameObject.h"
#include"Theme.h"
#include"Score.h"
#include"UDPServer.h"
#include<array>
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

	struct User
	{
		std::string name_;
		IPDATA IpAddr_;
		int RecvUDPHandle_;
		int score;
		bool isDrawer_;
	};
	Circle me;
	Circle you;
	Theme* theme_;
	Score* score_;
	std::string name_;

	int hKeyData_;
	int h64Font_;
	bool NowKeyInput_;
	bool IPSet_;
	int connectnum_;
	std::array<User, CONNECTMAX> users_;
	std::string drawingOrder_[CONNECTMAX];
	//std::array<int, MAX_PLAYERS> scores_;
	bool isDrawer_;
	int currentDrawerIndex_;
	std::string themeToDisplay_;
	void UpdateInit();
	void UpdateConnect();
	void UpdatePlay();
	void UpdateClose();

	void DrawInit();
	void DrawConnect();
	void DrawPlay();
	void DrawClose();


public:

	//コンストラクタ
	//引数：parent  親オブジェクト（ObjectManager）
	UDPClient(GameObject* parent);

	//デストラクタ
	~UDPClient();

	//初期化
	void Initialize() override;

	//更新
	void Update() override;

	//描画
	void Draw() override;

	//開放
	void Release() override;
	void SetName(std::string _name) { name_ = _name; }
	void HandleDrawingOrder(int drawerIndex,const std::string* order);
	void UpdateUserData(const User& userData);

};

