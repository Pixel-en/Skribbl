#pragma once
#include "Engine/GameObject.h"

class UDPClient :public GameObject
{
	IPDATA ip;
	int netUDPHandle;

	struct Circle {
		int x;
		int y;
		int size;
		unsigned int color;
	};
	Circle circle;
	Circle youcircle;
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
};

