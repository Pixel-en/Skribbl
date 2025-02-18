#include "UDPClient.h"

UDPClient::UDPClient(GameObject* parent)
	:GameObject(parent,"UDPClient"),netUDPHandle(-1)
{
	ip = { 192,168,43,64 };
	netUDPHandle = MakeUDPSocket(8888);
	circle = { -10,-10,5,GetColor(255,255,0) };
}

UDPClient::~UDPClient()
{
	DeleteUDPSocket(netUDPHandle);
}

void UDPClient::Initialize()
{	
	assert(netUDPHandle >= 0);
}

void UDPClient::Update()
{
	GetMousePoint(&circle.x, &circle.y);
	NetWorkSendUDP(netUDPHandle, ip, 8888, &circle, sizeof(circle));
	if (CheckNetWorkRecvUDP(netUDPHandle) == TRUE) {
		NetWorkRecvUDP(netUDPHandle, NULL, NULL, &youcircle, sizeof(youcircle),FALSE);
	}
}

void UDPClient::Draw()
{
	//ClearDrawScreen();
	DrawCircle(circle.x, circle.y, circle.size, circle.color, true);
	DrawCircle(youcircle.x, youcircle.y, youcircle.size, youcircle.color, true);
}

void UDPClient::Release()
{

}
