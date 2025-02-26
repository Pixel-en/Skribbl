#include "UDPClient.h"
#include <cstring>
#include "Engine/SceneManager.h"
#include "Chat.h"

namespace {
	const XMINT4 IPFRAME{ 465, 100, 815, 200 };
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
			if (CheckNetWorkRecvUDP(UDPHandle) == TRUE) {
				NetWorkRecvUDP(UDPHandle, NULL, NULL, &ServerPort_, sizeof(ServerPort_), FALSE);
				SceneManager* sc = GetParent()->FindGameObject<SceneManager>();
				sc->ChangeScene(SceneManager::SCENE_ID_PLAY);
			}
		}
	}

}

void UDPClient::UpdatePlay()
{

	Chat* c = GetRootJob()->FindGameObject<Chat>();
	if (c == nullptr)
		return;

	std::string str = c->GetText();
	if (str != "") {
		char text_[64];
		strcpy_s(text_, sizeof(text_), (name_+"：" + str).c_str());
		NetWorkSendUDP(UDPHandle, IpAddr, ServerPort_, text_, sizeof(text_));
	}

	if (CheckNetWorkRecvUDP(UDPHandle) == TRUE) {
		char text[64] = "";
		NetWorkRecvUDP(UDPHandle, NULL, NULL, &text, sizeof(text), FALSE);
		text[std::strlen(text)] = '\0';
		std::string receivedText(text);
		if (receivedText.find("Next drawer:") != std::string::npos) {
			HandleDrawingOrder(receivedText);
		}
		else if (receivedText.find("Game Over!") != std::string::npos) {
			c->AddAns(receivedText);
		}
		else if (receivedText.find("Current theme:") != std::string::npos) {
			HandleThemeUpdate(receivedText);
		}
		else {
			c->AddAns(receivedText);
		}
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
		DrawKeyInputString(IPFRAME.x+10, IPFRAME.y+18, hKeyData_);
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
			DrawStringToHandle(IPFRAME.x+10, IPFRAME.y+18, ip.c_str(), GetColor(200, 200, 200), h64Font_);

			static int count = 0;
			if (count >= 240)
				count = 0;


			std::string bDot = "";
			for (int i = 0; i < count / 60; i++) {
				bDot += ".";
			}

			if (isConnect_) {
				DrawStringToHandle(IPFRAME.x+35, 300, ("待機中" + bDot).c_str(), GetColor(0, 0, 0), h64Font_);
			}
			else {
				DrawStringToHandle(IPFRAME.x+35, 300, ("接続中" + bDot).c_str(), GetColor(0, 0, 0), h64Font_);
			}
			count++;
		}
		else {
			DrawStringToHandle(IPFRAME.x+10, IPFRAME.y+18, "ルーム番号", GetColor(180, 180, 180), h64Font_);
		}
	}


}

void UDPClient::DrawPlay()
{
}

void UDPClient::DrawClose()
{
}



void UDPClient::HandleDrawingOrder(const std::string& message) {
	size_t pos = message.find(":");
	if (pos != std::string::npos) {
		std::string drawer = message.substr(pos + 1);
		currentDrawerIndex_ = std::distance(drawingOrder_, std::find(std::begin(drawingOrder_), std::end(drawingOrder_), drawer));
		Chat* c = GetRootJob()->FindGameObject<Chat>();
		if (c != nullptr && name_ != drawer) {
			c->AddAns("Next drawer: " + drawer);
		}
	}
}
void UDPClient::HandleScoreUpdate(const std::string& message) {
	size_t pos = message.find(":");
	if (pos != std::string::npos) {
		std::string playerScoreInfo = message.substr(pos + 1);
		pos = playerScoreInfo.find(",");
		while (pos != std::string::npos) {
			std::string playerInfo = playerScoreInfo.substr(0, pos);
			size_t sep = playerInfo.find(" ");
			std::string playerName = playerInfo.substr(0, sep);
			int score = std::stoi(playerInfo.substr(sep + 1));
			for (int i = 0; i < CONNECTMAX; i++) {
				if (drawingOrder_[i] == playerName) {
					playerScores_[i] = score;
					break;
				}
			}
			playerScoreInfo = playerScoreInfo.substr(pos + 1);
			pos = playerScoreInfo.find(",");
		}
	}
}

void UDPClient::HandleThemeUpdate(const std::string& message)
{
	size_t pos = message.find(":");
	if (pos != std::string::npos) {
		std::string currentTheme = message.substr(pos + 1);
		Chat* c = GetRootJob()->FindGameObject<Chat>();
		if (c != nullptr) {
			c->AddAns("Your theme: " + currentTheme);
		}
	}
}

