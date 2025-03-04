#include "UDPClient.h"
#include <cstring>
#include "Engine/SceneManager.h"
#include "Chat.h"

namespace {
	const XMINT4 IPFRAME{ 465, 100, 815, 200 };
}
struct DataPacket {
	int packetType;
	char data[256];
};

UDPClient::UDPClient(GameObject* parent)
	:GameObject(parent, "UDPClient"), UDPHandle(-1),isDrawer_(false),connectnum_(0)
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
	currentDrawerIndex_ = 0;
	h64Font_ = CreateFontToHandle("64size", 64, -1, -1);
	for (int i = 0; i < CONNECTMAX; i++) {
		users_[i] = { "", {}, -1,0 ,false}; // Initialize users
		
	}
}

UDPClient::~UDPClient()
{
	DeleteUDPSocket(UDPHandle);
	DeleteUDPSocket(hConnectCheck_);
}

void UDPClient::Initialize()
{
	score_ = GetParent()->FindGameObject<Score>();
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

void UDPClient::UpdatePlay() {
	Chat* c = GetRootJob()->FindGameObject<Chat>();
	if (c == nullptr)
		return;

	std::string str = c->GetText();
	if (str != "") {
		char text_[64];
		strcpy_s(text_, sizeof(text_), (name_ + "：" + str).c_str());
		NetWorkSendUDP(UDPHandle, IpAddr, ServerPort_, text_, sizeof(text_));
		CheckTheme(str);
	}

	if (CheckNetWorkRecvUDP(UDPHandle) == TRUE) {
		DataPacket packet = {}; // Initialize packet
		NetWorkRecvUDP(UDPHandle, NULL, NULL, &packet, sizeof(packet), FALSE);
		
		// Handle the packet based on packetType
		switch (packet.packetType) {
		case 1: 
			HandleDrawingOrder(*reinterpret_cast<int*>(packet.data), reinterpret_cast<std::string*>(packet.data + sizeof(int)));
			break;
		case 2: // Theme update
			std::memcpy(&themeToDisplay_, packet.data, sizeof(std::string)); // Update the drawer status
			break;
		case 3: // Game over
			c->AddAns("Game Over!");
			break;
		case 4:
		{
			// User data update: update all user data at once
			User users[CONNECTMAX];
			std::memcpy(users, packet.data, sizeof(users));
			for (int i = 0; i < connectnum_; i++) {
				UpdateUserData(users[i]);
				// Update score on the client side
				score_->AddPointsToPlayer(users[i].name_, users[i].isDrawer_);
			}
		}
		break;
		case 5: // Connect number update
			std::memcpy(&connectnum_, packet.data, sizeof(connectnum_));
			break;
		default: // Chat message
			c->AddAns(packet.data);
			break;
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


void UDPClient::DrawClose()
{
}


void UDPClient::DrawPlay() {


	// Draw the theme at the top center if the client is the drawer
	if (isDrawer_ && !themeToDisplay_.empty()) {

		int screenWidth = 900;
		int textWidth = GetDrawStringWidth(themeToDisplay_.c_str(), themeToDisplay_.length());
		int x = (screenWidth - textWidth) / 2;
		DrawString(x, 20, themeToDisplay_.c_str(), GetColor(255, 0, 0));
	}
}


void UDPClient::HandleDrawingOrder(int drawerIndex, const std::string* order) {
	currentDrawerIndex_ = drawerIndex;

	// Update drawingOrder_ array
	for (int i = 0; i < connectnum_; i++) {
		users_[i].name_ = order[i];
		users_[i].isDrawer_ = (i == drawerIndex);
	}
	isDrawer_ = users_[drawerIndex].name_ == name_;
	if (score_) {
		for (int i = 0; i < connectnum_; i++) {
			score_->AddPlayer(users_[i].name_);
		}
	}

}

void UDPClient::UpdateUserData(const User& userData) {
	for (int i = 0; i < connectnum_; i++) {
		
			users_[i] = userData;
			
		
	}
}

void UDPClient::CheckTheme(const std::string& text)
{

	if (text == themeToDisplay_) {
		// Notify the server of the correct answer
		DataPacket correctPacket;
		correctPacket.packetType = 0; // Chat message
		std::string correctMsg = name_ + " answered correctly!";
		strcpy_s(correctPacket.data, correctMsg.c_str());
		NetWorkSendUDP(UDPHandle, IpAddr, ServerPort_, &correctPacket, sizeof(correctPacket));

		// Update the score locally
		score_->AddPointsToPlayer(name_, false); // Guesser
	}
}
