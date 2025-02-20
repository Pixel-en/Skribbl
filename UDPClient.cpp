#include "UDPClient.h"
#include "Engine/SceneManager.h"

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

	SceneManager* sc = GetRootJob()->FindGameObject<SceneManager>();
	SceneManager::SCENE_ID ID = sc->GetCurrentSceneID();

	switch (ID)
	{
	case SceneManager::SCENE_ID_TITLE:
		break;
	case SceneManager::SCENE_ID_CONNECT:
		UpdateConnect();
		break;
	case SceneManager::SCENE_ID_PLAY:
		UpdatePlay();
		break;
	case SceneManager::SCENE_ID_GAMEOVER:
		UpdateClose();
		break;
	default:
		break;
	}
}

void UDPClient::Draw()
{
	DrawCircle(me.x, me.y, me.size, me.color, true);
	DrawCircle(you.x, you.y, you.size, you.color, true);
}

void UDPClient::Release()
{
}


void UDPClient::UpdateInit()
{
	NetWorkSendUDP(UDPHandle, IpAddr, 8888, nullptr, 0);
}

void UDPClient::UpdateConnect()
{
	if (CheckNetWorkRecvUDP(UDPHandle) == TRUE) {
		NetWorkRecvUDP(UDPHandle, NULL, NULL, &ServerPort_, sizeof(ServerPort_), FALSE);
	}

	me = { -1,-1,5,GetColor(0,255,153) };
	you = { -1,-1,5,GetColor(0,0,0) };
}

void UDPClient::UpdatePlay()
{
	GetMousePoint(&me.x, &me.y);
	NetWorkSendUDP(UDPHandle, IpAddr, ServerPort_, &me, sizeof(me));

	if (CheckNetWorkRecvUDP(UDPHandle) == TRUE) {
		NetWorkRecvUDP(UDPHandle, NULL, NULL, &you, sizeof(you), FALSE);
	}
}

void UDPClient::UpdateClose()
{
}