#include "UDPServer.h"
#include <cstring>
#include<ctime>
#include "Engine/SceneManager.h"
#include "Chat.h"

inline bool operator == (const IPDATA& a, const IPDATA& b) {
	if (a.d1 == b.d1 && a.d2 == b.d2 && a.d3 == b.d3 && a.d4 == b.d4)return  true;
	return false;

};

namespace {
	const int SERVERPORT{ 9850 };
	const int CLIENTPORT{ 8888 };
	const XMINT4 CONNECTFRAME{ 900,600,1200,700 };
	const float DRAWER_CHANGE_INTERVAL = 90.0f; // 90 seconds time limit for each drawer turn}
	const int START_NEXT_TURN = 1;
	const int THEME_UPDATE = 2;

}
struct DataPacket {
	int packetType;
	char data[256];
};

UDPServer::UDPServer(GameObject* parent)
	:GameObject(parent, "UDPServer"), hNameFrame_(-1),currentDrawerIndex_(0), timeElapsed_(0.0f)
{
	connectnum_ = 0;
	isConnect_ = false;
	currentDrawerIndex_ = 0;
	for (int i = 0; i < CONNECTMAX; i++) {
		user[i].RecvUDPHandle_ = MakeUDPSocket(SERVERPORT + i);
		HandleCheck(user[i].RecvUDPHandle_, "ソケットが作れてない");

		user[i].IpAddr_ = { 0,0,0,0 };
		playerScores_[i] = 0; // Initialize scores
	}

	UDPConnectHandle_ = MakeUDPSocket(8888);
	HandleCheck(UDPConnectHandle_, "ソケットが作れてない");

	//自分のIPアドレスを取得
	IPDATA ip[2];
	GetMyIPAddress(ip, 2, NULL);
	MyIpAddr_ = ip[1];

	me = { -1,-1,5,GetColor(155,155,0) };
	you = { -1,-1,5,GetColor(0,0,0) };

	h64Font_ = CreateFontToHandle("64", 64, -1, -1);
	h32Font_ = CreateFontToHandle("32", 32, -1, -1);
	Time::Init();
}

UDPServer::~UDPServer()
{
	for (int i = 0; i < CONNECTMAX; i++) {
		DeleteUDPSocket(user[i].RecvUDPHandle_);
	}
	DeleteUDPSocket(UDPConnectHandle_);
}


void UDPServer::Initialize()
{
	SceneManager* sc = GetRootJob()->FindGameObject<SceneManager>();
	SceneManager::SCENE_ID ID = sc->GetCurrentSceneID();
	theme_ = GetParent()->FindGameObject<Theme>();
	score_ = GetParent()->FindGameObject<Score>();
	switch (ID)
	{
	case SceneManager::SCENE_ID_TITLE:
		break;
	case SceneManager::SCENE_ID_CONNECT:
		hNameFrame_ = LoadGraph("Assets\\Image\\NameFrame.png");
		HandleCheck(hNameFrame_, "ネームフレーム画像がない");
		break;
	case SceneManager::SCENE_ID_PLAY:
		SetDrawingOrder();  // Set the initial drawing order when starting the game
		StartNextTurn();    // Start the first turn
		
		break;
	case SceneManager::SCENE_ID_GAMEOVER:
		break;
	default:
		break;
	}
}

void UDPServer::Update()
{

	SceneManager* sc = GetRootJob()->FindGameObject<SceneManager>();
	SceneManager::SCENE_ID ID = sc->GetCurrentSceneID();
	Time::Refresh();
	timeElapsed_ += Time::DeltaTime();
	switch (ID)
	{
	case SceneManager::SCENE_ID_TITLE:
		break;
	case SceneManager::SCENE_ID_CONNECT:
		UpdateConnect();
		break;
	case SceneManager::SCENE_ID_PLAY:
		if (timeElapsed_ >= DRAWER_CHANGE_INTERVAL) {
			StartNextTurn();
			timeElapsed_ = 0.0f;
		}
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

	if (connectnum_ < CONNECTMAX) {
		//ポート8888（接続相手テスト）に通信が来た時
		if (CheckNetWorkRecvUDP(UDPConnectHandle_) == TRUE) {
			IPDATA ip = { 0,0,0,0 };
			char Recvname[256];
			//過去に接続した人でなければ
			NetWorkRecvUDP(UDPConnectHandle_, &ip, NULL, Recvname, sizeof(Recvname), FALSE);
			for (int i = 0; i < CONNECTMAX; i++) {
				if (ip == user[i].IpAddr_) {
					check = true;
					break;
				}
			}
			//IPを保存しておく
			if (!check) {
				user[connectnum_].IpAddr_ = ip;
				Recvname[std::strlen(Recvname)] = '\0';
				std::string _name(Recvname);
				user[connectnum_].name_ = _name;
				connectnum_++;

				//接続できたことを送信
				NetWorkSendUDP(UDPConnectHandle_, ip, 9876, NULL, 0);
			}

		}
	}

	if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0) {
		XMINT2 pos;
		GetMousePoint(&pos.x, &pos.y);

		if (pos.x >= CONNECTFRAME.x && pos.x <= CONNECTFRAME.z &&
			pos.y >= CONNECTFRAME.y && pos.y <= CONNECTFRAME.w && connectnum_ > 0) {
			isConnect_ = true;
		}
	}

	//接続人数が確定した時ポート番号を送る
	if (isConnect_ == true) {
		for (int i = 0; i < connectnum_; i++) {
			int data = SERVERPORT + i;
			NetWorkSendUDP(UDPConnectHandle_, user[i].IpAddr_, CLIENTPORT, &data, sizeof(data));
		}
		SceneManager* sc = GetParent()->FindGameObject<SceneManager>();
		sc->ChangeScene(SceneManager::SCENE_ID_PLAY);
	}

}

void UDPServer::UpdatePlay() {
	Chat* c = GetParent()->FindGameObject<Chat>();
	if (c == nullptr)
		return;

	for (int i = 0; i < connectnum_; i++) {
		if (CheckNetWorkRecvUDP(user[i].RecvUDPHandle_) == TRUE) {
			char text[64] = "";
			NetWorkRecvUDP(user[i].RecvUDPHandle_, NULL, NULL, text, sizeof(text), FALSE);
			text[std::strlen(text)] = '\0';
			std::string str(text);
			if (str != "") {
				DataPacket packet;
				packet.packetType = 0; // Chat message
				strcpy_s(packet.data, str.c_str());

				c->AddAns(str);

				for (int j = 0; j < connectnum_; j++) {
					if (i != j) {
						NetWorkSendUDP(user[j].RecvUDPHandle_, user[j].IpAddr_, CLIENTPORT, &packet, sizeof(packet));
					}
				}
			}
		}
	}

	std::string str = c->GetText();
	if (str != "") {
		DataPacket packet;
		packet.packetType = 0; // Chat message
		strcpy_s(packet.data, (name_ + "：" + str).c_str());

		for (int i = 0; i < connectnum_; i++) {
			NetWorkSendUDP(user[i].RecvUDPHandle_, user[i].IpAddr_, CLIENTPORT, &packet, sizeof(packet));
		}
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
	std::string pass = "ルーム番号：";
	std::string d3 = std::to_string(MyIpAddr_.d3);
	std::string d4 = std::to_string(MyIpAddr_.d4);
	d3.insert(0, 3 - d3.length(), '0');
	d4.insert(0, 3 - d4.length(), '0');
	pass += d3 + d4;
	//じぶんのIP表示
	DrawStringToHandle(320, 60,pass.c_str(), GetColor(0, 0, 0),h64Font_);

	//参加者の名前表示
	for (int i = 0; i < connectnum_ + 1; i++) {
		DrawGraph(350, 200+i*90, hNameFrame_, true);
		if (i == 0) {
			DrawStringToHandle(520, 200 + i * 90+10, name_.c_str(), GetColor(0, 0, 0),h64Font_);
		}
		else {
			DrawStringToHandle(520, 200 + i * 90+10, user[i - 1].name_.c_str(), GetColor(0, 0, 0),h64Font_);
		}
	}

	//スタートボタン表示
	DrawBox(CONNECTFRAME.x, CONNECTFRAME.y, CONNECTFRAME.z, CONNECTFRAME.w, GetColor(200, 200, 200), true);
	DrawBox(CONNECTFRAME.x, CONNECTFRAME.w - 3, CONNECTFRAME.z, CONNECTFRAME.w, GetColor(150, 150, 150), true);
	DrawBox(CONNECTFRAME.z - 3, CONNECTFRAME.y, CONNECTFRAME.z, CONNECTFRAME.w, GetColor(150, 150, 150), true);

}

void UDPServer::DrawPlay()
{
	// Draw the theme at the top center for the drawer
	if (name_ == drawingOrder_[currentDrawerIndex_] && !themeToDisplay_.empty()) {
		int screenWidth = 1280;
		int textWidth = GetDrawStringWidth(themeToDisplay_.c_str(), themeToDisplay_.length());
		int x = (screenWidth - textWidth) / 2;
		DrawString(x, 50, themeToDisplay_.c_str(), GetColor(255, 0, 0));
	}

	

}

void UDPServer::DrawClose()
{
}
void UDPServer::SetDrawingOrder() {
	for (int i = 0; i < connectnum_; i++) {
		drawingOrder_[i] = user[i].name_;
	}

	// Add the server's own name to the drawing order
	drawingOrder_[connectnum_] = name_;
	connectnum_++;

	// Shuffle the drawing order
	std::srand(static_cast<unsigned int>(std::time(nullptr)));
	for (int i = connectnum_ - 1; i > 0; --i) {
		int j = std::rand() % (i + 1);
		std::swap(drawingOrder_[i], drawingOrder_[j]);
	}

	// Add players to the score object
	Score* score = GetParent()->FindGameObject<Score>();
	if (score) {
		for (int i = 0; i < connectnum_; i++) {
			score->AddPlayer(drawingOrder_[i]);
		}
	}
}
void UDPServer::StartNextTurn() {
	if (currentDrawerIndex_ >= connectnum_) {
		// Send Game Over packet
		DataPacket packet;
		packet.packetType = 3; // Game over
		strcpy_s(packet.data, ""); // No additional data needed

		for (int i = 0; i < connectnum_; i++) {
			NetWorkSendUDP(user[i].RecvUDPHandle_, user[i].IpAddr_, CLIENTPORT, &packet, sizeof(packet));
		}
	}
	else {
		// Send Drawer Index packet
		DataPacket packet;
		packet.packetType = 1; // Drawer index update
		int drawerIndex = currentDrawerIndex_;
		memcpy(packet.data, &drawerIndex, sizeof(drawerIndex));

		// Copy drawingOrder_ into packet.data
		memcpy(packet.data + sizeof(drawerIndex), drawingOrder_, sizeof(drawingOrder_));

		for (int i = 0; i < connectnum_; i++) {
			NetWorkSendUDP(user[i].RecvUDPHandle_, user[i].IpAddr_, CLIENTPORT, &packet, sizeof(packet));
		}
		SendUserDataToClients(); // Send initial user data
		// Roll the theme and send it to a random player
		theme_->ThemeRoll();
		SendThemeToRandomPlayer();

		// Reset the timer for the next drawer
		timeElapsed_ = 0.0f;

		currentDrawerIndex_++;
	}
}

void UDPServer::SendThemeToRandomPlayer() {
	// Select a random player index
	int randomIndex = std::rand() % connectnum_;

	DataPacket packet;
	packet.packetType = 2; // Theme update
	strcpy_s(packet.data, theme_->GetCurrentTheme().c_str());

	if (randomIndex == connectnum_ - 1) {
		// The server is the drawer, store the theme directly
		themeToDisplay_ = theme_->GetCurrentTheme();
	}
	else {
		// Send to one of the connected clients
		NetWorkSendUDP(user[randomIndex].RecvUDPHandle_, user[randomIndex].IpAddr_, CLIENTPORT, &packet, sizeof(packet));
	}
}
void  UDPServer::SendUserDataToClients() {
	DataPacket packet;
	packet.packetType = 4; // User data update

	for (int i = 0; i <connectnum_; i++) {
		std::memcpy(packet.data, &user[i], sizeof(User));
		for (int j = 0; j < connectnum_; j++) {
			if (i != j) {
				NetWorkSendUDP(user[j].RecvUDPHandle_, user[j].IpAddr_, CLIENTPORT, &packet, sizeof(packet));
			}
		}
	}
}