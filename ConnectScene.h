#pragma once
#include "Engine/GameObject.h"

class ConnectScene : public GameObject
{
	int hImage_;
	int hBack_;
public:
	// コンストラクタ
	// 引数：parent 親オブジェクト（SceneManager）
	ConnectScene(GameObject* parent);

	// 初期化
	void Initialize() override;

	// 更新
	void Update() override;

	// 描画
	void Draw() override;

	// 開放
	void Release() override;
};
