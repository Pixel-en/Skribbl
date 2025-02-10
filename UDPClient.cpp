#include "UDPClient.h"

UDPClient::UDPClient(GameObject* parent)
	:GameObject(parent,"UDPClient")
{
}

UDPClient::~UDPClient()
{
}

void UDPClient::Initialize()
{	
	//‘—Mê—p
	netUDPHandle = MakeUDPSocket(8888);
	ip = { 192,168,43,64 };
	circle = { -100,-100,5,GetColor(255,152,0) };

	NetWorkSendUDP(netUDPHandle, ip, 8888, &ip, sizeof(ip) >= 0);

	while (true)
	{
		if (CheckNetWorkRecvUDP(netUDPHandle) == TRUE) {
			IPDATA ipdata;
			int num;
			NetWorkRecvUDP(netUDPHandle, &ip, NULL, &num, sizeof(num), FALSE);
			if (num != -1)
				break;
		}
	}
}

void UDPClient::Update()
{
	GetMousePoint(&circle.x, &circle.y);
	NetWorkSendUDP(netUDPHandle, ip, 8888, &circle, sizeof(circle));

	//if (CheckNetWorkRecvUDP(netUDPHandle) == TRUE) {
	//	NetWorkRecvUDP(netUDPHandle, NULL, NULL, &recvcircle, sizeof(recvcircle), FALSE);
	//}
	//else {
	//	recvcircle = { -100,-100,5,GetColor(255,255,0) };
	//	if (ProcessMessage() < 0) {

	//		DeleteUDPSocket(netUDPHandle);
	//	}
	//}
}

void UDPClient::Draw()
{
	DrawCircle(circle.x, circle.y, circle.size, circle.color);
	DrawCircle(recvcircle.x, recvcircle.y, recvcircle.size, recvcircle.color);
}

void UDPClient::Release()
{
	DeleteUDPSocket(netUDPHandle);
}
