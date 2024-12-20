#pragma once
#include "Engine/GameObject.h"

//オブジェクトを作るときのコピー用テンプレート
//注意：このオブジェクトを継承しないこと
class TestObject final :public GameObject
{
public:

	//コンストラクタ
	//引数：parent  親オブジェクト（ObjectManager）
	TestObject(GameObject* parent);

	//デストラクタ
	~TestObject();

	//初期化
	void Initialize() override;

	//更新
	void Update() override;

	//描画
	void Draw() override;

	//開放
	void Release() override;
};

