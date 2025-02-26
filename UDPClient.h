#pragma once
#include "Engine/GameObject.h"
#include"Theme.h"
#include"UDPServer.h"
#include<vector>
#include<unordered_map>
class UDPClient :public GameObject
{
	IPDATA IpAddr;
	int UDPHandle;
	int hConnectCheck_;
	int ServerPort_;
	bool isConnect_;
	struct Circle {
		int x;
		int y;
		int size;
		unsigned int color;
	};
	
	Circle me;
	Circle you;
	Theme* theme_;
	std::string name_;

	int hKeyData_;
	int h64Font_;
	bool NowKeyInput_;
	bool IPSet_;

	std::string drawingOrder_[CONNECTMAX];
	int currentDrawerIndex_;
	int playerScores_[CONNECTMAX];
	void UpdateInit();
	void UpdateConnect();
	void UpdatePlay();
	void UpdateClose();

	void DrawInit();
	void DrawConnect();
	void DrawPlay();
	void DrawClose();


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

	void SetName(std::string _name) { name_ = _name; }
	std::string GetName() { return name_; }
	void HandleDrawingOrder(const std::string& message);
	void HandleScoreUpdate(const std::string& message);
	void HandleThemeUpdate(const std::string& message);
	void DrawPlayerScores();
};

