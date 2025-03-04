#pragma once
#include "Engine/GameObject.h"

namespace {
	const int CONNECTMAX{ 3 };
}

class UDPServer :public GameObject
{
	int drawerhandle_;
	int hStart_;
	int hFrame_;

	int h64Font_;
	int h32Font_;
	int hNameFrame_;
	int myPoint_;

	struct User
	{
		std::string name_;
		IPDATA IpAddr_;
		int RecvUDPHandle_;
		int point_;
	};

	enum NETWORKPART
	{
		NONE,
		INFO,
		PLAY,
	};
	NETWORKPART part;

	IPDATA MyIpAddr_;
	//ソケット
	int UDPConnectHandle_;

	User user[CONNECTMAX];
	std::string name_;

	//接続番号
	int connectnum_;
	bool isConnect_;


	bool isCorrect_;	//正解しているか
	float timer_;		//演出タイマー
	int drawerNum_;		//絵を描いている人の番号
	int questionNum_;	//問題数
	float DrawTimer_;

	void UpdateConnect();
	void UpdatePlay();
	void UpdateClose();

	void DrawConnect();
	void DrawPlay();
	void DrawClose();

public:

	//コンストラクタ
	//引数：parent  親オブジェクト（ObjectManager）
	UDPServer(GameObject* parent);

	//デストラクタ
	~UDPServer();

	//初期化
	void Initialize() override;

	//更新
	void Update() override;

	//描画
	void Draw() override;

	//開放
	void Release() override;

	void SetName(std::string _name) { name_ = _name; };
};

