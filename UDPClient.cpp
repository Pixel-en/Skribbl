#include "UDPClient.h"

UDPClient::UDPClient(GameObject* parent)
	:GameObject(parent,"UDPClient"),UDPHandle(-1)
{
	IpAddr = { 192,168,42,6 };
	UDPHandle = MakeUDPSocket(8888);
}

UDPClient::~UDPClient()
{
	DeleteUDPSocket(UDPHandle);
}

void UDPClient::Initialize()
{	
	assert(UDPHandle >= 0);
}

void UDPClient::Update()
{
	switch (state_)
	{
	case UDPClient::INIT:
		UpdateInit();
		break;
	case UDPClient::CONNECT:
		UpdateConnect();
		break;
	case UDPClient::PLAY:
		UpdatePlay();
		break;
	case UDPClient::CLOSE:
		UpdateClose();
		break;
	case UDPClient::END:
		break;
	default:
		break;
	}
}

void UDPClient::Draw()
{
}

void UDPClient::Release()
{
}


void UDPClient::UpdateInit()
{
	NetWorkSendUDP(UDPHandle, IpAddr, 8888, nullptr, 0);
	state_ = CONNECT;
}

void UDPClient::UpdateConnect()
{
	if (CheckNetWorkRecvUDP(UDPHandle) == TRUE) {
		NetWorkRecvUDP(UDPHandle, NULL, NULL, &ServerPort_, sizeof(ServerPort_), FALSE);
		state_ = PLAY;
	}
}

void UDPClient::UpdatePlay()
{
}

void UDPClient::UpdateClose()
{
}