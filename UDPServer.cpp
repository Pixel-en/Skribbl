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


}
struct DataPacket {
	int packetType;
	char data[256];
};

UDPServer::UDPServer(GameObject* parent)
	:GameObject(parent, "UDPServer"), hNameFrame_(-1), currentDrawerIndex_(0), timeElapsed_(0.0f)
{
	connectnum_ = 0;
	isConnect_ = false;
	currentDrawerIndex_ = 0;
	for (int i = 0; i < CONNECTMAX; i++) {
		user[i].RecvUDPHandle_ = MakeUDPSocket(SERVERPORT + i);
		HandleCheck(user[i].RecvUDPHandle_, "ƒ\ƒPƒbƒg‚ªì‚ê‚Ä‚È‚¢");

		user[i].IpAddr_ = { 0,0,0,0 };
		user[i].score = 0; // Initialize scores
		user[i].isDrawer_ = false;
	}

	UDPConnectHandle_ = MakeUDPSocket(8888);
	HandleCheck(UDPConnectHandle_, "ƒ\ƒPƒbƒg‚ªì‚ê‚Ä‚È‚¢");

	//Ž©•ª‚ÌIPƒAƒhƒŒƒX‚ðŽæ“¾
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
		HandleCheck(hNameFrame_, "ƒl[ƒ€ƒtƒŒ[ƒ€‰æ‘œ‚ª‚È‚¢");
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
		//ƒ|[ƒg8888iÚ‘±‘ŠŽèƒeƒXƒgj‚É’ÊM‚ª—ˆ‚½Žž
		if (CheckNetWorkRecvUDP(UDPConnectHandle_) == TRUE) {
			IPDATA ip = { 0,0,0,0 };
			char Recvname[256];
			//‰ß‹Ž‚ÉÚ‘±‚µ‚½l‚Å‚È‚¯‚ê‚Î
			NetWorkRecvUDP(UDPConnectHandle_, &ip, NULL, Recvname, sizeof(Recvname), FALSE);
			for (int i = 0; i < CONNECTMAX; i++) {
				if (ip == user[i].IpAddr_) {
					check = true;
					break;
				}
			}
			//IP‚ð•Û‘¶‚µ‚Ä‚¨‚­
			if (!check) {
				user[connectnum_].IpAddr_ = ip;
				Recvname[std::strlen(Recvname)] = '\0';
				std::string _name(Recvname);
				user[connectnum_].name_ = _name;
				user[connectnum_].score = 0;  // Initialize score
				user[connectnum_].isDrawer_ = false;  // Initialize isDrawer flag
				connectnum_++;
				//Ú‘±‚Å‚«‚½‚±‚Æ‚ð‘—M
				NetWorkSendUDP(UDPConnectHandle_, ip, 9876, NULL, 0);
			}

		}
	}

	if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0) {
		XMINT2 pos;
		GetMousePoint(&pos.x, &pos.y);

		if (pos.x >= CONNECTFRAME.x && pos.x <= CONNECTFRAME.z &&
			pos.y >= CONNECTFRAME.y && pos.y <= CONNECTFRAME.w && connectnum_ > 0) {//dont forget
			isConnect_ = true;
		}
	}

	//Ú‘±l”‚ªŠm’è‚µ‚½Žžƒ|[ƒg”Ô†‚ð‘—‚é
	if (isConnect_ == true) {
		for (int i = 0; i < connectnum_; i++) {
			int data = SERVERPORT + i;
			NetWorkSendUDP(UDPConnectHandle_, user[i].IpAddr_, CLIENTPORT, &data, sizeof(data));
		}
		SceneManager* sc = GetParent()->FindGameObject<SceneManager>();
		sc->ChangeScene(SceneManager::SCENE_ID_PLAY);
	}
	// Ensure the server itself is added to the user list
	if (connectnum_ == 0) {
		user[connectnum_].IpAddr_ = MyIpAddr_;
		user[connectnum_].name_ = name_;
		user[connectnum_].score = 0;
		user[connectnum_].isDrawer_ = false;
		connectnum_++;
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

				if (c->CheckTheme()) {
					// Correct answer
					score_->AddPointsToPlayer(user[i].name_, false); // Guesser
					for (auto& user : user) {
						if (user.isDrawer_) {
							score_->AddPointsToPlayer(user.name_, true); // Drawer
							break;
						}
					}

					// Update all clients with new scores
					for (int j = 0; j < connectnum_; j++) {
						DataPacket scorePacket;
						scorePacket.packetType = 4; // User data update
						User userData = user[j];
						userData.score = score_->GetPlayerScore(user[j].name_);
						std::memcpy(scorePacket.data, &userData, sizeof(User));
						NetWorkSendUDP(user[j].RecvUDPHandle_, user[j].IpAddr_, CLIENTPORT, &scorePacket, sizeof(scorePacket));
					}

					// Send chat message indicating the correct answer
					DataPacket correctPacket;
					correctPacket.packetType = 0; // Chat message
					std::string correctMsg = user[i].name_ + " answered correctly!";
					strcpy_s(correctPacket.data, correctMsg.c_str());
					for (int j = 0; j < connectnum_; j++) {
						NetWorkSendUDP(user[j].RecvUDPHandle_, user[j].IpAddr_, CLIENTPORT, &correctPacket, sizeof(correctPacket));
					}
				}
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
	std::string pass = "ƒ‹[ƒ€”Ô†F";
	std::string d3 = std::to_string(MyIpAddr_.d3);
	std::string d4 = std::to_string(MyIpAddr_.d4);
	d3.insert(0, 3 - d3.length(), '0');
	d4.insert(0, 3 - d4.length(), '0');
pass += d3 + d4; 
		//‚¶‚Ô‚ñ‚ÌIP•\Ž¦
		DrawStringToHandle(320, 60, pass.c_str(), GetColor(0, 0, 0), h64Font_);

	//ŽQ‰ÁŽÒ‚Ì–¼‘O•\Ž¦
	for (int i = 0; i < connectnum_; i++) {
		DrawGraph(350, 200 + i * 90, hNameFrame_, true);


		DrawStringToHandle(520, 200 + i * 90 + 10, user[i].name_.c_str(), GetColor(0, 0, 0), h64Font_);

	}

	//ƒXƒ^[ƒgƒ{ƒ^ƒ“•\Ž¦
	DrawBox(CONNECTFRAME.x, CONNECTFRAME.y, CONNECTFRAME.z, CONNECTFRAME.w, GetColor(200, 200, 200), true);
	DrawBox(CONNECTFRAME.x, CONNECTFRAME.w - 3, CONNECTFRAME.z, CONNECTFRAME.w, GetColor(150, 150, 150), true);
	DrawBox(CONNECTFRAME.z - 3, CONNECTFRAME.y, CONNECTFRAME.z, CONNECTFRAME.w, GetColor(150, 150, 150), true);

}

void UDPServer::DrawPlay()
{
	// Draw the theme at the top center for the drawer
	if (name_ == drawingOrder_[currentDrawerIndex_] && !themeToDisplay_.empty()) {
		int screenWidth = 900;
		int textWidth = GetDrawStringWidth(themeToDisplay_.c_str(), themeToDisplay_.length());
		int x = (screenWidth - textWidth) / 2;
		DrawString(x, 20, themeToDisplay_.c_str(), GetColor(255, 0, 0));
	}



}

void UDPServer::DrawClose()
{
}
void UDPServer::SetDrawingOrder() {
	for (int i = 0; i < connectnum_; i++) {
		drawingOrder_[i] = user[i].name_;
		user[i].isDrawer_ = false;  // Initially, no one is the drawer
	}

	// Shuffle the drawing order
	std::srand(static_cast<unsigned int>(std::time(nullptr)));
	for (int i = connectnum_ - 1; i > 0; --i) {
		int j = std::rand() % (i + 1);
		std::swap(drawingOrder_[i], drawingOrder_[j]);
	}

	// Add players to the score object
	Score* score = GetRootJob()->FindGameObject<Score>();
	if (score) {
		for (int i = 0; i < connectnum_; i++) {
			score->AddPlayer(drawingOrder_[i]);
		}
	}

	// Set the initial drawer
	currentDrawerIndex_ = 0;
	user[currentDrawerIndex_].isDrawer_ = true;
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
		// Reset the current drawer flag
		user[currentDrawerIndex_].isDrawer_ = false;

		// Move to the next drawer
		currentDrawerIndex_++;
		if (currentDrawerIndex_ >= connectnum_) {  // Adjust the condition to be >=
			// End the game if all players have had their turn
			DataPacket packet;
			packet.packetType = 3; // Game over
			strcpy_s(packet.data, ""); // No additional data needed

			for (int i = 0; i < connectnum_; i++) {
				NetWorkSendUDP(user[i].RecvUDPHandle_, user[i].IpAddr_, CLIENTPORT, &packet, sizeof(packet));
			}
		}
		else {
			// Set the new drawer flag
			user[currentDrawerIndex_].isDrawer_ = true;

			// Reset the timer for the next drawer
			timeElapsed_ = 0.0f;

			// First, send the updated connection number
			SendConnectNumToClients();

			// Then, send the updated user data
			SendUserDataToClients();

			// Roll the theme and send it to all clients
			theme_->ThemeRoll();
			SendThemeToClients();

			// Finally, notify clients of the drawer change
			DataPacket packet;
			packet.packetType = 1; // Drawer index update
			int drawerIndex = currentDrawerIndex_;
			memcpy(packet.data, &drawerIndex, sizeof(drawerIndex));

			// Copy drawingOrder_ into packet.data
			memcpy(packet.data + sizeof(drawerIndex), drawingOrder_, sizeof(drawingOrder_));

			for (int i = 0; i < connectnum_; i++) {
				NetWorkSendUDP(user[i].RecvUDPHandle_, user[i].IpAddr_, CLIENTPORT, &packet, sizeof(packet));
			}
		}
	}
}


void UDPServer::SendThemeToClients() {
	DataPacket packet;
	packet.packetType = 2; // Theme update
	strcpy_s(packet.data, theme_->GetCurrentTheme().c_str());

	// Send the theme to all clients, ensuring the drawer receives it directly
	for (int i = 0; i < connectnum_; i++) {
		NetWorkSendUDP(user[i].RecvUDPHandle_, user[i].IpAddr_, CLIENTPORT, &packet, sizeof(packet));
	}

	// If the server is the drawer, store the theme directly
	if (currentDrawerIndex_ == connectnum_ - 1) {
		themeToDisplay_ = theme_->GetCurrentTheme();
	}
}

void  UDPServer::SendUserDataToClients() {
	DataPacket packet;
	packet.packetType = 4; // User data update

	for (int i = 0; i < connectnum_; i++) {
		std::memcpy(packet.data, &user[i], sizeof(User));
		for (int j = 0; j < connectnum_; j++) {
			
				NetWorkSendUDP(user[j].RecvUDPHandle_, user[j].IpAddr_, CLIENTPORT, &packet, sizeof(packet));
			
		}
	}
}

void UDPServer::SendConnectNumToClients() {
	DataPacket packet;
	packet.packetType = 5; // Connect number update
	memcpy(packet.data, &connectnum_, sizeof(connectnum_));

	for (int i = 0; i < connectnum_; i++) {
		NetWorkSendUDP(user[i].RecvUDPHandle_, user[i].IpAddr_, CLIENTPORT, &packet, sizeof(packet));
	}
}