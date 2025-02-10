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

	//コンストラクタ
	//引数：parent  親オブジェクト（ObjectManager）
	UDPServer(GameObject* parent);

	//デストラクタ
	~UDPServer();

	//初期化
	void Initialize() override;

	//更新
	void Update() override;

	//描画
	void Draw() override;

	//開放
	void Release() override;
};

