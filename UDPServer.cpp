#include "UDPServer.h"

inline bool operator == (const IPDATA& a, const IPDATA& b) {
	if (a.d1 == b.d1 && a.d2 == b.d2 && a.d3 == b.d3 && a.d4 == b.d4)return  true;
	return false;

};

UDPServer::UDPServer(GameObject* parent)
	:GameObject(parent, "UDPServer")
{
	circle0_ = { -10,-10,5,GetColor(255,0,0) };
	UDPHandle_ = MakeUDPSocket(8888);

	IP_[0] = { 192,168,43,61 };
	IP_[1] = { 192,168,43,35 };

	assert(UDPHandle_ >= 0);
}

UDPServer::~UDPServer()
{
}

void UDPServer::Initialize()
{



}

void UDPServer::Update()
{
	int port;
	IPDATA ip;
	Circle cir;
	GetMousePoint(&circle0_.x, &circle0_.y);
	for (int i=0; i < 2; i++) {
		if (CheckNetWorkRecvUDP(UDPHandle_) == TRUE)
		{
			NetWorkRecvUDP(UDPHandle_, &ip, &port, &cir, sizeof(cir), FALSE);
			
			//if (IP[i] != ip) {

			//	IP_[i] = ip;
			//	if (IP[0] != IP[1]) {
			//		circle0_[i] = cir;
			//	}
			//}
			//else {
			//	IP_[i] = ip;
			//	circle0_[i] = cir;
			//}
			if (ip == IP_[i]) {
				circle_[i] = cir;
			}

			NetWorkSendUDP(UDPHandle_, IP_[i], port, &circle0_, sizeof(circle0_));
		}
	}
}

void UDPServer::Draw()
{
	
	for (int i=0; i < 2; i++)
	{

		DrawCircle(circle_[i].x, circle_[i].y, circle_[i].size, circle_[i].color);

	}
	DrawCircle(circle0_.x, circle0_.y, circle0_.size, circle0_.color);
	//ClearDrawScreen();
	//ScreenFlip();
}

void UDPServer::Release()
{    // Clean up resources
		DeleteUDPSocket(UDPHandle_);
}
