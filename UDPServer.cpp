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
		user[i].RecvUDPHandle_ = MakeUDPSocket(SERVERPORT + i);
		HandleCheck(user[i].RecvUDPHandle_, "�\�P�b�g�����ĂȂ�");

		user[i].IpAddr_ = { 0,0,0,0 };
	}

	UDPConnectHandle_ = MakeUDPSocket(8888);
	HandleCheck(UDPConnectHandle_, "�\�P�b�g�����ĂȂ�");

	IPDATA ip[2];
	GetMyIPAddress(ip, 2, NULL);
	MyIpAddr_ = ip[1];

	me = { -1,-1,5,GetColor(155,155,0) };
	you = { -1,-1,5,GetColor(0,0,0) };
}

UDPServer::~UDPServer()
{

}

void UDPServer::Initialize()
{
	SetFontSize(64);
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
	//DrawCircle(me.x, me.y, me.size, me.color, true);
	//DrawCircle(you.x, you.y, you.size, you.color, true);

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

void UDPServer::Release()
{
}

void UDPServer::UpdateConnect()
{
	bool check = false;

	//�|�[�g8888�i�ڑ�����e�X�g�j�ɒʐM��������
	if (CheckNetWorkRecvUDP(UDPConnectHandle_) == TRUE) {
		IPDATA ip;
		int rPort;
		std::string Recvname;
		//�ߋ��ɐڑ������l�łȂ����
		NetWorkRecvUDP(UDPConnectHandle_, &ip, &rPort, &Recvname, sizeof(Recvname), FALSE);
		for (int i = 0; i < CONNECTMAX; i++) {
			if (ip == user[i].IpAddr_) {
				check = true;
				break;
			}
		}
		//IP��ۑ����Ă���
		if (!check) {
			user[connectnum_].IpAddr_ = ip;
			user[connectnum_].name_ = Recvname;
			connectnum_++;
		}

	}
	//�ڑ��l�����m�肵�����|�[�g�ԍ��𑗂�
	if (connectnum_ == CONNECTMAX) {
		for (int i = 0; i < CONNECTMAX; i++) {
			int data = SERVERPORT + i;
			NetWorkSendUDP(UDPConnectHandle_, user[i].IpAddr_, CLIENTPORT, &data, sizeof(data));
		}
	}

}

void UDPServer::UpdatePlay()
{

	GetMousePoint(&me.x, &me.y);
	for (int i = 0; i < CONNECTMAX; i++) {
		if (CheckNetWorkRecvUDP(user[i].RecvUDPHandle_) == TRUE) {
			NetWorkRecvUDP(user[i].RecvUDPHandle_, NULL, NULL, &you, sizeof(you), FALSE);
		}
	}

	for (int i = 0; i < CONNECTMAX; i++) {
		NetWorkSendUDP(user[i].RecvUDPHandle_, user[i].IpAddr_, CLIENTPORT, &me, sizeof(me));
	}

}

void UDPServer::UpdateClose()
{
	// Clean up resources
	for (int i = 0; i < CONNECTMAX; i++) {
		DeleteUDPSocket(user[i].RecvUDPHandle_);
	}
}

void UDPServer::DrawConnect()
{
	std::string pass = "���[���ԍ��F";
	std::string d3 = std::to_string(MyIpAddr_.d3);
	std::string d4 = std::to_string(MyIpAddr_.d4);
	d3.insert(0, 3 - d3.length(), '0');
	d4.insert(0, 3 - d4.length(), '0');
	pass += d3 + d4;
	DrawString(200, 100, pass.c_str(), GetColor(0, 0, 0));

	for (int i = 0; i < connectnum_ + 1; i++) {
		if (i == 0) {
			DrawString(500, 500 / (connectnum_ + 1) *i+200, name_.c_str(), GetColor(255, 255, 255));
		}
		else {
			DrawString(500, 500 / (connectnum_ + 1) * i + 200, user[i-1].name_.c_str(), GetColor(255, 255, 255));
		}
	}
}

void UDPServer::DrawPlay()
{
}

void UDPServer::DrawClose()
{
}
