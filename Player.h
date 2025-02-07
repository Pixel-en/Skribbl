#pragma once
#include "Engine/GameObject.h"
#include "Theme.h"
#include "Chat.h"
class Theme;
class Chat;
class Player :public GameObject
{

	XMINT2 PreMousePos_;
	XMINT2 NowMousePos_;

	unsigned int Cr_;

	bool drawOK_;

	int linesize_;
	bool Erase_;

	bool RFlag_;
	bool LFlag_;
	bool Lcount_;
	Theme* theme_;
	Chat* chat_;

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

	void DrawCurrentTheme();
};

