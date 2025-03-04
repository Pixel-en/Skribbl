#pragma once
#include "Engine/GameObject.h"

namespace {

	const float PLAYTIME{ 120.0f };
}

class BackGround :public GameObject
{
	int hPalletImage_;
	int hFrameImage_;
	int hEFrameImage_;
	int hCFrameImage_;
	int hPFrameImage_;

	int hTimerBackImage_;
	int hTimerImage_;

	int linesize_;

	bool Erase_;

	float timer_;
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
	void SetTiemr(float time) { timer_ = time; }
};

