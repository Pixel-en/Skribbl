#include "UDPClient.h"
#include "Engine/SceneManager.h"

UDPClient::UDPClient(GameObject* parent)
	:GameObject(parent,"UDPClient"),UDPHandle(-1)
{
	IpAddr = { 192,168,0,0 };
	UDPHandle = MakeUDPSocket(8888);
	HandleCheck(UDPHandle, "ソケットが作られてない");
	NowKeyInput_ = false;
	IPSet_ = false;
	name_ = "";
}

UDPClient::~UDPClient()
{
	DeleteUDPSocket(UDPHandle);
}

void UDPClient::Initialize()
{

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

	SceneManager* sc = GetRootJob()->FindGameObject<SceneManager>();
	SceneManager::SCENE_ID ID = sc->GetCurrentSceneID();

	switch (ID)
	{
	case SceneManager::SCENE_ID_TITLE:
		break;
	case SceneManager::SCENE_ID_CONNECT:
		DrawConnect();
		break;
	case SceneManager::SCENE_ID_PLAY:
		DrawPlay();
		break;
	case SceneManager::SCENE_ID_GAMEOVER:
		DrawClose();
		break;
	default:
		break;
	}
}

void UDPClient::Release()
{
}


void UDPClient::UpdateInit()
{
}

void UDPClient::UpdateConnect()
{
	if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0 && !IPSet_) {
		XMINT2 pos;
		GetMousePoint(&pos.x, &pos.y);

		if (pos.x >= 400 && pos.x <= 800 &&
			pos.y >= 100 && pos.y <= 200) {
			hKeyData_ = MakeKeyInput(6, true, false, true);
			SetActiveKeyInput(hKeyData_);
		}
	}
	
	switch (CheckKeyInput(hKeyData_))
	{
	case 0:
		NowKeyInput_ = true;
		break;
	case 1:
		char ip[256];
		GetKeyInputString(ip, hKeyData_);
		if (ip != "") {
			int d3 = ((ip[0] - '0') * 100) + ((ip[1] - '0') * 10) + ((ip[2] - '0') * 1);
			int d4 = ((ip[3] - '0') * 100) + ((ip[4] - '0') * 10) + ((ip[5] - '0') * 1);
			IpAddr.d3 = d3;
			IpAddr.d4 = d4;

			IPSet_ = true;
			char sendname[256];
			strcpy_s(sendname, sizeof(sendname), name_.c_str());
			NetWorkSendUDP(UDPHandle, IpAddr, 8888, &sendname, sizeof(sendname));
		}

		break;
	case 2:

		NowKeyInput_ = false;
		break;
	default:
		break;
	}

	if (IPSet_) {
		if (CheckNetWorkRecvUDP(UDPHandle) == TRUE) {
			NetWorkRecvUDP(UDPHandle, NULL, NULL, &ServerPort_, sizeof(ServerPort_), FALSE);
		}
	}

	//me = { -1,-1,5,GetColor(0,255,153) };
	//you = { -1,-1,5,GetColor(0,0,0) };
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

void UDPClient::DrawInit()
{
}

void UDPClient::DrawConnect()
{
	DrawBox(400, 100, 800, 200, GetColor(100, 100, 100), true); // TypeBox
	DrawBox(400, 100, 800, 200, GetColor(0, 0, 0), false);

	if (NowKeyInput_) {
		DrawKeyInputString(410, 110, hKeyData_);
	}
	else {
		if (IPSet_) {
			std::string ip = std::to_string(IpAddr.d3) + std::to_string(IpAddr.d4);
			DrawString(410, 110, ip.c_str(), GetColor(255, 255, 255));
		}
		else {
			DrawString(410, 110, "ルーム番号を入力", GetColor(255, 255, 255));
		}
	}

}

void UDPClient::DrawPlay()
{
}

void UDPClient::DrawClose()
{
}
