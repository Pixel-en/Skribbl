#pragma once
#include "Engine/GameObject.h"

//テストシーンを管理するクラス
class TitleScene : public GameObject
{
	int hImage_;
	int hSButtonImage_;
	int hCButtonImage_;
	int hMoveImage_;

	int hKeyData_;      //文字入力データ
	bool NowNameInput_; //文字入力しているか
	std::string name_;  //名前
	bool nameOK;		//名前は入力できたか

	int movex;

	enum State {
		NONE,
		SERVER,
		CLIENT,
		MAX
	};
	State state_;

public:
	// コンストラクタ
	// 引数：parent 親オブジェクト（SceneManager）
	TitleScene(GameObject* parent);

	// 初期化
	void Initialize() override;

	// 更新
	void Update() override;

	// 描画
	void Draw() override;

	// 開放
	void Release() override;
};
