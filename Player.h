#pragma once
#include "Engine/GameObject.h"
#include <string>

class Player :public GameObject
{
public:
	struct Pencil
	{

		XMINT2 PreMousePos_;	//1フレーム前のマウスのポジション
		XMINT2 NowMousePos_;	//現在のマウスのポジション
		unsigned int Cr_;		//色

		int linesize_;			//ラインの大きさ
		bool Erase_;			//消しゴムを使っているか
	};
private:

	Pencil pen;
	Pencil youPen;


	bool drawOK_;

	
	//消しゴムモードかどうか
	bool RFlag_;

	std::string theme_;

	bool isDrawer;

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

	Pencil GetPencil() { return pen; }
	void RecvPencil(Pencil _pen) { youPen = _pen; }
};

