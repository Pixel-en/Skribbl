#include "UDPServer.h"
#include <cstring>
#include "Engine/SceneManager.h"
#include "Chat.h"
#include "Player.h"
#include "Theme.h"
#include "BackGround.h"
#include "ImGui/imgui.h"

inline bool operator == (const IPDATA& a, const IPDATA& b) {
	if (a.d1 == b.d1 && a.d2 == b.d2 && a.d3 == b.d3 && a.d4 == b.d4)return  true;
	return false;

};

namespace {
	const int SERVERPORT{ 9850 };
	const int CLIENTPORT{ 8888 };
	const XMINT4 CONNECTFRAME{ 900,600,1200,700 };
	const float CORRECTTIME{ 1.0f };
	const int QUESTIONMAX{ 5 };
}

UDPServer::UDPServer(GameObject* parent)
	:GameObject(parent, "UDPServer"), hNameFrame_(-1)
{
	connectnum_ = 0;
	isConnect_ = false;

	for (int i = 0; i < CONNECTMAX; i++) {
		user[i].RecvUDPHandle_ = MakeUDPSocket(SERVERPORT + i);
		HandleCheck(user[i].RecvUDPHandle_, "ソケットが作れてない");

		user[i].IpAddr_ = { 0,0,0,0 };
		user[i].point_ = 0;
	}

	UDPConnectHandle_ = MakeUDPSocket(8888);
	HandleCheck(UDPConnectHandle_, "ソケットが作れてない");

	//自分のIPアドレスを取得
	IPDATA ip[2];
	GetMyIPAddress(ip, 2, NULL);
	MyIpAddr_ = ip[1];

	h64Font_ = CreateFontToHandle("64", 64, -1, -1);
	h32Font_ = CreateFontToHandle("32", 32, -1, -1);
	myPoint_ = 0;

	part = NONE;

	timer_ = -1.0f;
	questionNum_ = 0;
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

	switch (ID)
	{
	case SceneManager::SCENE_ID_TITLE:
		break;
	case SceneManager::SCENE_ID_CONNECT:
		hNameFrame_ = LoadGraph("Assets\\Image\\NameFrame.png");
		HandleCheck(hNameFrame_, "ネームフレーム画像がない");
		break;
	case SceneManager::SCENE_ID_PLAY:
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
		struct PortData
		{
			int port;
			int num;
		};
		PortData portdata;

		for (int i = 0; i < connectnum_; i++) {
			portdata.port = SERVERPORT + i;
			portdata.num = connectnum_;
			NetWorkSendUDP(UDPConnectHandle_, user[i].IpAddr_, CLIENTPORT, &portdata, sizeof(portdata));
		}
		isCorrect_ = true;
		SceneManager* sc = GetParent()->FindGameObject<SceneManager>();
		sc->ChangeScene(SceneManager::SCENE_ID_PLAY);
	}

}

void UDPServer::UpdatePlay()
{
	Player* player = GetRootJob()->FindGameObject<Player>();
	player->SetDraw(true);
	//チャット
	Chat* c = GetRootJob()->FindGameObject<Chat>();

	Theme* theme = GetRootJob()->FindGameObject<Theme>();


	struct NetData
	{
		int port;
		char name[16] = "";
		char text[64] = "";
		Player::Pencil pen;

		//書き込む
		int point;
		bool drawer = false;	//絵描き
		bool correct = false;	//正解
		bool reset = false;	//キャンバスリセット
		int themenum;
	};

	NetData data[CONNECTMAX + 1];

	//クライアントから受信
	for (int i = 0; i < connectnum_; i++) {
		if (CheckNetWorkRecvUDP(user[i].RecvUDPHandle_) == TRUE) {
			NetWorkRecvUDP(user[i].RecvUDPHandle_, NULL, NULL, &data[i], sizeof(data[i]), FALSE);
			//チャットを取得
			if (data[i].text[0] != '\0') {
				
				//チャットに残す
				std::string Rname(data[i].name), Rtext(data[i].text);
				c->AddAns(Rname + ":" + Rtext);

				//正解していないとき
				if (!isCorrect_) {
					//お題とチェックする
					if (theme->CheckTheme(Rtext) == true) {
						//正解していたら
						if (drawerNum_ != i) {
							//ポイントを入れて正解にする
							user[i].point_ += 10;
							//絵描き
							if (drawerNum_ == connectnum_)
								myPoint_ += 10;
							else {
								user[drawerNum_].point_ += 10;
							}
							questionNum_++;
							isCorrect_ = true;
							c->Correct();
						}
					}
				}
			}

			//ペンを取得
			if (data[i].pen.linesize_ != -1) {
				player->RecvPencil(data[i].pen);
			}

		}
	}

	//サーバーの情報を入れる
	//チャット
	std::string ctext = c->GetText();
	if (!isCorrect_) {
		if (theme->CheckTheme(ctext) == true) {
			if (drawerNum_ != connectnum_) {
				myPoint_ += 10;
				user[drawerNum_].point_ += 10;
				isCorrect_ = true;
				c->Correct();
			}
		}
	}

	//その他情報
	data[connectnum_].port = 8888;
	strcpy_s(data[connectnum_].name, sizeof(data[connectnum_].name), name_.c_str());
	strcpy_s(data[connectnum_].text, sizeof(data[connectnum_].text), ctext.c_str());
	data[connectnum_].name[std::strlen(data[connectnum_].name)] = '\0';
	data[connectnum_].text[std::strlen(data[connectnum_].text)] = '\0';
	data[connectnum_].pen = player->GetPencil();
	data[connectnum_].point = myPoint_;

	//もし正解したなら
	if (isCorrect_) {
		//タイマー（演出）
		if (timer_<0.0f) {
			//絵描き決め
			drawerNum_ = GetRand(connectnum_);
			if (drawerNum_ != connectnum_) {	//クライアント
				data[drawerNum_].drawer = true;
				player->SetDraw(false);
			}
			else {	//サーバー
				player->SetDraw(true);
			}

			theme->ThemeRoll();

			for (int i = 0; i <= connectnum_; i++) {
				data[i].themenum = theme->GetThemeNum();
				data[i].reset = true;
			}

			timer_ = CORRECTTIME;
			isCorrect_ = false;
			BackGround* bg = GetRootJob()->FindGameObject<BackGround>();
			bg->CanvasReset();
		}
		else {
			timer_ -= Time::DeltaTime();
			//正解演出
			for (int i = 0; i <= connectnum_; i++) {
				data[i].correct = true;
			}
		}
	}
	else {
		data[drawerNum_].drawer = true;
	}


	for (int i = 0; i < connectnum_; i++) {
		NetWorkSendUDP(user[i].RecvUDPHandle_, user[i].IpAddr_, CLIENTPORT, data, sizeof(data));
	}
	int a = isCorrect_;
	ImGui::Begin("ser");
	ImGui::InputInt("correct", &a);
	ImGui::InputInt("drawer", &drawerNum_);
	ImGui::End();

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
	DrawStringToHandle(200, 100, pass.c_str(), GetColor(0, 0, 0), h64Font_);

	//参加者の名前表示
	for (int i = 0; i < connectnum_ + 1; i++) {
		DrawGraph(300, 180 + i * 90, hNameFrame_, true);
		if (i == 0) {
			DrawStringToHandle(500, 180 + i * 90 + 10, name_.c_str(), GetColor(0, 0, 0), h64Font_);
		}
		else {
			DrawStringToHandle(500, 180 + i * 90 + 10, user[i - 1].name_.c_str(), GetColor(0, 0, 0), h64Font_);
		}
	}

	//スタートボタン表示
	DrawBox(CONNECTFRAME.x, CONNECTFRAME.y, CONNECTFRAME.z, CONNECTFRAME.w, GetColor(200, 200, 200), true);
	DrawBox(CONNECTFRAME.x, CONNECTFRAME.w - 3, CONNECTFRAME.z, CONNECTFRAME.w, GetColor(150, 150, 150), true);
	DrawBox(CONNECTFRAME.z - 3, CONNECTFRAME.y, CONNECTFRAME.z, CONNECTFRAME.w, GetColor(150, 150, 150), true);

}

void UDPServer::DrawPlay()
{


}

void UDPServer::DrawClose()
{
}
