#pragma once
#include "Engine/GameObject.h"


class Player :public GameObject
{
	struct INT2
	{
		int x;
		int y;
	};

	INT2 PreMousePos_;
	INT2 NowMousePos_;

	unsigned int Cr_;

public:

	//コンストラクタ
	//引数：parent  親オブジェクト（ObjectManager）
	Player(GameObject* parent);

	//デストラクタ
	~Player();

	//初期化
	void Initialize() override;

	//更新
	void Update() override;

	//描画
	void Draw() override;

	//開放
	void Release() override;
};

