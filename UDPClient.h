#pragma once
#include "Engine/GameObject.h"

class UDPClient :public GameObject
{
	int hFrame_;

	IPDATA IpAddr;
	int UDPHandle;
	int hConnectCheck_;
	int ServerPort_;
	bool isConnect_;

	bool isCorrect_;

	int drawerhandle_;

	int playernum_;

	std::string name_;

	bool end_;

	int hKeyData_;
	int h64Font_;
	bool NowKeyInput_;
	bool IPSet_;

	void UpdateInit();
	void UpdateConnect();
	void UpdatePlay();
	void UpdateClose();

	void DrawInit();
	void DrawConnect();
	void DrawPlay();
	void DrawClose();

	struct User
	{
		std::string name_;
		int point_;
		bool drawer_;
	};
	User user[4];
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

};

