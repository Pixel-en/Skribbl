#pragma once
#include "Engine/GameObject.h"

class UDPClient :public GameObject
{
	IPDATA IpAddr;
	int UDPHandle;
	int hConnectCheck_;
	int ServerPort_;
	bool isConnect_;

	struct UserData
	{
		char name[10];
		bool draw;
		int point;
	};
	UserData* data;

	int playernum_;

	enum NetData
	{
		NONE,
		INFO,
		PLAY,
	};
	NetData nstate;

	std::string name_;

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
};

