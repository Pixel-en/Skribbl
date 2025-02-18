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

	//ソケット
	int UDPHandle_[CONNECTMAX];
	int UDPConnectHandle_;
	//IP
	IPDATA IpAddr_[CONNECTMAX];
	//接続番号
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

