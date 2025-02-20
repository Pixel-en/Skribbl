#include "UDPServer.h"
#include "Engine/SceneManager.h"

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
		HandleCheck(UDPHandle_[i], "ソケットが作れてない");

		IpAddr_[i] = { 0,0,0,0 };
	}

	UDPConnectHandle_ = MakeUDPSocket(8888);
	HandleCheck(UDPConnectHandle_, "ソケットが作れてない");


	me = { -1,-1,5,GetColor(155,155,0) };
	you = { -1,-1,5,GetColor(0,0,0) };
}

UDPServer::~UDPServer()
{

}

void UDPServer::Initialize()
{

}

void UDPServer::Update()
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

void UDPServer::Draw()
{
	DrawCircle(me.x, me.y, me.size, me.color, true);
	DrawCircle(you.x, you.y, you.size, you.color, true);

}

void UDPServer::Release()
{   
}

void UDPServer::UpdateConnect()
{
	bool check = false;

	//ポート8888（接続相手テスト）に通信が来た時
	if (CheckNetWorkRecvUDP(UDPConnectHandle_) == TRUE) {
		IPDATA ip;
		int rPort;
		//過去に接続した人でなければ
		NetWorkRecvUDP(UDPConnectHandle_, &ip, &rPort, nullptr, 0, FALSE);
		for (int i = 0; i < CONNECTMAX; i++) {
			if (ip == IpAddr_[i]) {
				check = true;
				break;
			}
		}
		//IPを保存しておく
		if (!check) {
			IpAddr_[connectnum_] = ip;
			connectnum_++;
		}

	}
	//接続人数が確定した時ポート番号を送る
	if (connectnum_ == CONNECTMAX) {
		for (int i = 0; i < CONNECTMAX; i++) {
			int data = SERVERPORT + i;
			NetWorkSendUDP(UDPConnectHandle_, IpAddr_[i], CLIENTPORT, &data, sizeof(data));
		}
	}

}

void UDPServer::UpdatePlay()
{

	GetMousePoint(&me.x, &me.y);
	for (int i = 0; i < CONNECTMAX; i++) {
		if (CheckNetWorkRecvUDP(UDPHandle_[i]) == TRUE) {
			NetWorkRecvUDP(UDPHandle_[i], NULL, NULL, &you, sizeof(you), FALSE);
		}
	}

	for (int i = 0; i < CONNECTMAX; i++) {
		NetWorkSendUDP(UDPHandle_[i], IpAddr_[i], CLIENTPORT, &me, sizeof(me));
	}

}

void UDPServer::UpdateClose()
{
	// Clean up resources
	for (int i = 0; i < CONNECTMAX; i++) {
		DeleteUDPSocket(UDPHandle_[i]);
	}
}