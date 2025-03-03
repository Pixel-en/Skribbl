#pragma once
#include "Engine/GameObject.h"

class BackGround :public GameObject
{
	int hPalletImage_;
	int hFrameImage_;
	int hEFrameImage_;
	int hCFrameImage_;
	int hPFrameImage_;
	int linesize_;

	bool Erase_;
public:

	//コンストラクタ
	//引数：parent  親オブジェクト（ObjectManager）
	BackGround(GameObject* parent);

	//デストラクタ
	~BackGround();

	//初期化
	void Initialize() override;

	//更新
	void Update() override;

	//描画
	void Draw() override;

	//開放
	void Release() override;

	void SetLineSize(int size) { linesize_ = size; };
	void SetErase(bool set) { Erase_ = set; }
	void CanvasReset();
};

