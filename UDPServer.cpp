#include "UDPServer.h"

inline bool operator == (const IPDATA& a, const IPDATA& b) {
	if (a.d1 == b.d1 && a.d2 == b.d2 && a.d3 == b.d3 && a.d4 == b.d4)return  true;
	return false;

};

namespace {
	const int SERVERPORT{ 9850 };
	const int CLIENTPORT{ 8888 };
}

UDPServer::UDPServer(GameObject* parent)
	:GameObject(parent, "UDPServer")
{
	connectnum_ = 0;

	for (int i = 0; i < CONNECTMAX; i++) {
		UDPHandle_[i] = MakeUDPSocket(SERVERPORT + i);
		assert(UDPHandle_[i] >= 0);

		IpAddr_[i] = { 0,0,0,0 };
	}

	UDPConnectHandle_ = MakeUDPSocket(8888);
	assert(UDPConnectHandle_ >= 0);
}

UDPServer::~UDPServer()
{

}

void UDPServer::Initialize()
{

}

void UDPServer::Update()
{

	switch (state_)
	{
	case UDPServer::INIT:
		UpdateInit();
		break;
	case UDPServer::CONNECT:
		UpdateConnect();
		break;
	case UDPServer::PLAY:
		UpdatePlay();
		break;
	case UDPServer::CLOSE:
		UpdateClose();
		break;
	case UDPServer::END:
		break;
	default:
		break;
	}

}

void UDPServer::Draw()
{
	DrawCircle(me.x, me.y, me.size, me.color, true);
	DrawCircle(you.x, you.y, you.size, you.color, true);

}

void UDPServer::Release()
{   
	// Clean up resources
	for (int i = 0; i < CONNECTMAX; i++) {
		DeleteUDPSocket(UDPHandle_[i]);
	}
}


void UDPServer::UpdateInit()
{
	bool check = false;

	if (CheckNetWorkRecvUDP(UDPConnectHandle_) == TRUE) {
		IPDATA ip;
		int rPort;
		
		NetWorkRecvUDP(UDPConnectHandle_, &ip, &rPort, nullptr, 0, FALSE);
		for (int i = 0; i < CONNECTMAX; i++) {
			if (ip == IpAddr_[i]) {
				check = true;
				break;
			}
		}

		if (!check) {
			IpAddr_[connectnum_] = ip;
			connectnum_++;
		}

	}

	if (connectnum_ == CONNECTMAX) {
		state_ = CONNECT;
	}
}

void UDPServer::UpdateConnect()
{
	for (int i = 0; i < CONNECTMAX; i++) {
		int data = SERVERPORT + i;
		NetWorkSendUDP(UDPConnectHandle_, IpAddr_[i], CLIENTPORT, &data, sizeof(data));
	}
}

void UDPServer::UpdatePlay()
{

	GetMousePoint(&me.x, &me.y);
	

}

void UDPServer::UpdateClose()
{
}