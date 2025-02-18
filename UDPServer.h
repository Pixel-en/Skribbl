#pragma once
#include "Engine/GameObject.h"

class UDPServer :public GameObject
{

	struct Circle {
		int x;
		int y;
		int size;
		unsigned int color;
	};   // Structure to store the received circle data
	Circle circle_[2];
	Circle circle0_;
	int UDPHandle_;
	IPDATA IP_[2];
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

