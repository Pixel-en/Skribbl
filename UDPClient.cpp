#include "UDPClient.h"
#include <cstring>
#include "Engine/SceneManager.h"
#include "Chat.h"
#include "Player.h"

namespace {
	const XMINT4 IPFRAME{ 400, 100, 750, 200 };
	const int CONNECTMAX{ 3 };

}

UDPClient::UDPClient(GameObject* parent)
	:GameObject(parent, "UDPClient"), UDPHandle(-1)
{
	IpAddr = { 192,168,0,0 };
	UDPHandle = MakeUDPSocket(8888);
	hConnectCheck_ = MakeUDPSocket(9876);
	HandleCheck(UDPHandle, "ソケットが作られてない");
	HandleCheck(hConnectCheck_, "ソケットが作られてない9876");
	isConnect_ = false;
	NowKeyInput_ = false;
	IPSet_ = false;
	name_ = "";

	h64Font_ = CreateFontToHandle("64size", 64, -1, -1);
}

UDPClient::~UDPClient()
{
	DeleteUDPSocket(UDPHandle);
	DeleteUDPSocket(hConnectCheck_);
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
	//IPの入力
	if (IPSet_ == false) {
		//IPの入力枠
		if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0) {
			XMINT2 pos;
			GetMousePoint(&pos.x, &pos.y);

			if (pos.x >= 400 && pos.x <= 800 &&
				pos.y >= 100 && pos.y <= 200) {
				IPSet_ = false;
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
			if (std::strlen(ip) == 6) {
				int d3 = ((ip[0] - '0') * 100) + ((ip[1] - '0') * 10) + ((ip[2] - '0') * 1);
				int d4 = ((ip[3] - '0') * 100) + ((ip[4] - '0') * 10) + ((ip[5] - '0') * 1);
				if (d3 <= 255 && d4 <= 255) {
					IpAddr.d3 = d3;
					IpAddr.d4 = d4;
					IPSet_ = true;
					char sendname[256];
					strcpy_s(sendname, sizeof(sendname), name_.c_str());
					NetWorkSendUDP(UDPHandle, IpAddr, 8888, &sendname, sizeof(sendname));
					DeleteKeyInput(hKeyData_);
				}
			}
			NowKeyInput_ = false;
			break;
		case 2:
			NowKeyInput_ = false;
			break;
		default:
			break;
		}
	}

	//受信待ち
	else {
		//接続確認を受信
		if (isConnect_ == false) {
			if (CheckNetWorkRecvUDP(hConnectCheck_) == TRUE) {
				isConnect_ = true;
			}
		}
		else {
			//ポートを受信
			if (CheckNetWorkRecvUDP(UDPHandle) == TRUE) {
				struct Data
				{
					int port;
					int num;
				};
				Data recvdata;
				NetWorkRecvUDP(UDPHandle, NULL, NULL, &recvdata, sizeof(recvdata), FALSE);
				ServerPort_ = recvdata.port;
				playernum_ = recvdata.num;

				data = new UserData[playernum_];
				SceneManager* sc = GetParent()->FindGameObject<SceneManager>();
				sc->ChangeScene(SceneManager::SCENE_ID_PLAY);
			}
		}
	}

}

void UDPClient::UpdatePlay()
{

	if (CheckNetWorkRecvUDP(hConnectCheck_) == TRUE) {
		NetWorkRecvUDP(hConnectCheck_, NULL, NULL, &nstate, sizeof(nstate), FALSE);
	}

	Player* player = GetRootJob()->FindGameObject<Player>();
	Chat* c = GetRootJob()->FindGameObject<Chat>();

	struct Data
	{
		int port;
		char text[64] = "";
		Player::Pencil pen;
	};

	Data data;
	data.port = ServerPort_;
	strcpy_s(data.text, sizeof(data.text), (name_ + ":" + c->GetText()).c_str());
	data.pen = player->GetPencil();
	//チャットとペンデータを送る
	NetWorkSendUDP(UDPHandle, IpAddr, ServerPort_, &data, sizeof(data));

	switch (nstate)
	{
	case UDPClient::NONE:
		break;
	case UDPClient::INFO:
		break;
	case UDPClient::PLAY:
	{
		Data data[CONNECTMAX];
		//初期化
		for (int i = 0; i < playernum_; i++) {
			data[i].pen = { {-10,-10},{-10,-10},0,-1,false };
		}

		if (CheckNetWorkRecvUDP(UDPHandle) == TRUE) {
			NetWorkRecvUDP(UDPHandle, NULL, NULL, data, sizeof(data), FALSE);
		}

		for (int i = 0; i < playernum_; i++) {
			if (data[i].port == ServerPort_)
				continue;

			data[i].text[std::strlen(data[i].text)] = '\0';
			std::string str(data[i].text);
			
			if (str != "")
				c->AddAns(str);
			if (data[i].pen.NowMousePos_.x != -10) {
				player->RecvPencil(data[i].pen);
			}
		}
	}
	break;
	default:
		break;
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
	DrawBox(IPFRAME.x, IPFRAME.y, IPFRAME.z, IPFRAME.w, GetColor(100, 100, 100), true); // TypeBox
	DrawBox(IPFRAME.x, IPFRAME.y, IPFRAME.z, IPFRAME.w, GetColor(0, 0, 0), false);

	if (NowKeyInput_) {
		SetKeyInputStringFont(h64Font_);
		DrawKeyInputString(410, 110, hKeyData_);
		SetKeyInputStringFont(-1);
	}
	else {
		if (IPSet_) {
			//0埋め
			std::string d3 = std::to_string(IpAddr.d3);
			std::string d4 = std::to_string(IpAddr.d4);
			d3.insert(0, 3 - d3.length(), '0');
			d4.insert(0, 3 - d4.length(), '0');
			std::string ip = d3 + d4;
			DrawStringToHandle(IPFRAME.x, IPFRAME.y, ip.c_str(), GetColor(200, 200, 200), h64Font_);

			static int count = 0;
			if (count >= 240)
				count = 0;

			std::string bDot = "";
			for (int i = 0; i < count / 60; i++) {
				bDot += ".";
			}

			if (isConnect_) {
				DrawStringToHandle(IPFRAME.x, 300, ("待機中" + bDot).c_str(), GetColor(0, 0, 0), h64Font_);
			}
			else {
				DrawStringToHandle(IPFRAME.x, 300, ("接続中" + bDot).c_str(), GetColor(0, 0, 0), h64Font_);
			}
			count++;
		}
		else {
			DrawStringToHandle(IPFRAME.x, IPFRAME.y, "ルーム番号", GetColor(180, 180, 180), h64Font_);
		}
	}


}

void UDPClient::DrawPlay()
{
}

void UDPClient::DrawClose()
{
}

