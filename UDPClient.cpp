#include "UDPClient.h"
#include <cstring>
#include "Engine/SceneManager.h"
#include "Chat.h"
#include "Theme.h"
#include "Player.h"
#include "BackGround.h"
#include "PlayScene.h"

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
	HandleCheck(UDPHandle, "�\�P�b�g������ĂȂ�");
	HandleCheck(hConnectCheck_, "�\�P�b�g������ĂȂ�9876");
	isConnect_ = false;
	NowKeyInput_ = false;
	IPSet_ = false;
	name_ = "";

	h64Font_ = CreateFontToHandle("64size", 64, -1, -1);

	for (int i = 0; i < 4; i++) {
		user[i].name_ = "";
		user[i].point_ = 0;
		user[i].drawer_ = false;
	}
}

UDPClient::~UDPClient()
{
	DeleteUDPSocket(UDPHandle);
	DeleteUDPSocket(hConnectCheck_);
}

void UDPClient::Initialize()
{
	SceneManager* sc = GetRootJob()->FindGameObject<SceneManager>();
	SceneManager::SCENE_ID ID = sc->GetCurrentSceneID();

	switch (ID)
	{
	case SceneManager::SCENE_ID_TITLE:
		break;
	case SceneManager::SCENE_ID_CONNECT:
		break;
	case SceneManager::SCENE_ID_PLAY:
		drawerhandle_ = LoadGraph("Assets\\Image\\Drawer.png");
		HandleCheck(drawerhandle_, "���M���Ȃ�");
		break;
	case SceneManager::SCENE_ID_GAMEOVER:
		hFrame_ = LoadGraph("Assets\\Image\\GameOverFrame.png");
		HandleCheck(hFrame_, "�t���[�����Ȃ�");
		break;
	default:
		break;
	}
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
	//IP�̓���
	if (IPSet_ == false) {
		//IP�̓��͘g
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

	//��M�҂�
	else {
		//�ڑ��m�F����M
		if (isConnect_ == false) {
			if (CheckNetWorkRecvUDP(hConnectCheck_) == TRUE) {
				isConnect_ = true;
			}
		}
		else {
			//�|�[�g����M
			if (CheckNetWorkRecvUDP(UDPHandle) == TRUE) {
				struct PortData
				{
					int port;
					int num;
				};
				PortData portdata;

				NetWorkRecvUDP(UDPHandle, NULL, NULL, &portdata, sizeof(portdata), FALSE);

				ServerPort_ = portdata.port;
				playernum_ = portdata.num;
				SceneManager* sc = GetParent()->FindGameObject<SceneManager>();
				sc->ChangeScene(SceneManager::SCENE_ID_PLAY);
			}
		}
	}

}

void UDPClient::UpdatePlay()
{
	if (end_) {
		PlayScene* pc = GetRootJob()->FindGameObject<PlayScene>();
		pc->SetEnd(true);
		return;
	}

	Player* player = GetRootJob()->FindGameObject<Player>();
	//�`���b�g
	Chat* c = GetRootJob()->FindGameObject<Chat>();

	Theme* theme = GetRootJob()->FindGameObject<Theme>();

	struct NetData
	{
		int port;
		char name[16] = "";
		char text[64] = "";
		Player::Pencil pen;

		int point = 0;
		bool drawer = false;	//�G�`��
		bool correct = false;	//����
		bool reset = false;	//�L�����o�X���Z�b�g
		int themenum = 0;
		float timer_ = 0.0f;
		bool end = false;
	};

	NetData data[CONNECTMAX + 1];

	NetData mydata;
	mydata.port = ServerPort_;
	strcpy_s(mydata.name, sizeof(mydata.name), name_.c_str());
	strcpy_s(mydata.text, sizeof(mydata.text), (c->GetText()).c_str());
	mydata.name[std::strlen(mydata.name)] = '\0';
	mydata.text[std::strlen(mydata.text)] = '\0';
	mydata.pen = player->GetPencil();

	NetWorkSendUDP(UDPHandle, IpAddr, ServerPort_, &mydata, sizeof(mydata));

	if (CheckNetWorkRecvUDP(UDPHandle) == TRUE) {
		NetWorkRecvUDP(UDPHandle, NULL, NULL, data, sizeof(data), FALSE);

		for (int i = 0; i <= playernum_; i++) {
			if (data[i].name == name_) {
				//�����̑��������
				BackGround* bg = GetRootJob()->FindGameObject<BackGround>();
				player->SetDraw(data[i].drawer);
				if (data[i].reset) {
					bg->CanvasReset();
				}
				theme->SetThemeNum(data[i].themenum);
				//�^�C�}�[
				bg->SetTiemr(data[i].timer_);
				end_ = data[i].end;

			}
			//�����ȊO�̃f�[�^
			else {

				if (data[i].text[0] != '\0') {
					std::string Rname(data[i].name), Rtext(data[i].text);
					c->AddAns(Rname + ":" + Rtext);
				}
				if (data[i].pen.linesize_ > -1) {
					player->RecvPencil(data[i].pen);
				}
			}
			if (isCorrect_ == false && data[i].correct) {
				c->Correct();
			}
			if (data[i].point >= 0) {
				isCorrect_ = data[i].correct;
				std::string _name(data[i].name);
				user[i].name_ = _name;
				user[i].point_ = data[i].point;
				user[i].drawer_ = data[i].drawer;
			}
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
		DrawKeyInputString(410, 110, hKeyData_);
		SetKeyInputStringFont(-1);
	}
	else {
		if (IPSet_) {
			//0����
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
				DrawStringToHandle(IPFRAME.x, 300, ("�ҋ@��" + bDot).c_str(), GetColor(0, 0, 0), h64Font_);
			}
			else {
				DrawStringToHandle(IPFRAME.x, 300, ("�ڑ���" + bDot).c_str(), GetColor(0, 0, 0), h64Font_);
			}
			count++;
		}
		else {
			DrawStringToHandle(IPFRAME.x, IPFRAME.y, "���[���ԍ�", GetColor(180, 180, 180), h64Font_);
		}
	}


}

void UDPClient::DrawPlay()
{
	for (int i = 0; i <= playernum_; i++) {
		DrawString(4 + (i * 224) + 32, 545, ("�Ȃ܂��F" + user[i].name_).c_str(), GetColor(0, 0, 0));

		DrawString(4 + (i * 224) + 32, 597, ("�X�R�A�F" + std::to_string(user[i].point_)).c_str(), GetColor(0, 0, 0));

		if (user[i].drawer_)
			DrawGraph(4 + (i * 224) + 32, 630, drawerhandle_, true);
	}
}

void UDPClient::DrawClose()
{
	DrawStringToHandle(500, 50, "�ŏI����", GetColor(0, 0, 0), h64Font_);

	for (int i = 0; i < 4; i++)
		DrawGraph(10 + i * 310, 180, hFrame_, true);

	for (int i = 0; i <= playernum_; i++) {

		DrawStringToHandle(50 + i * 310, 190, name_.c_str(), GetColor(0, 0, 0), h64Font_);
		DrawStringToHandle(50 + i * 310, 260, (std::to_string(user[i].point_) + "pt").c_str(), GetColor(0, 0, 0), h64Font_);
	}
}

