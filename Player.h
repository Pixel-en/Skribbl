#pragma once
#include "Engine/GameObject.h"
#include <string>

class Player :public GameObject
{

	XMINT2 PreMousePos_;
	XMINT2 NowMousePos_;

	unsigned int Cr_;

	bool drawOK_;

	int linesize_;
	bool Erase_;
	
	//消しゴムモードかどうか
	bool RFlag_;

	std::string theme_;

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

	void SetTheme(std::string _theme) { theme_ = _theme; };
};

