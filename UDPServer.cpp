#include "UDPServer.h"
#include <cstring>
#include "Engine/SceneManager.h"

inline bool operator == (const IPDATA& a, const IPDATA& b) {
	if (a.d1 == b.d1 && a.d2 == b.d2 && a.d3 == b.d3 && a.d4 == b.d4)return  true;
	return false;

};

namespace {
	const int SERVERPORT{ 9850 };
	const int CLIENTPORT{ 8888 };
	const XMINT4 CONNECTFRAME{ 900,600,1200,700 };
}

UDPServer::UDPServer(GameObject* parent)
	:GameObject(parent, "UDPServer"),hNameFrame_(-1)
{
	connectnum_ = 0;
	isConnect_ = false;

	for (int i = 0; i < CONNECTMAX; i++) {
		user[i].RecvUDPHandle_ = MakeUDPSocket(SERVERPORT + i);
		HandleCheck(user[i].RecvUDPHandle_, "�\�P�b�g�����ĂȂ�");

		user[i].IpAddr_ = { 0,0,0,0 };
	}

	UDPConnectHandle_ = MakeUDPSocket(8888);
	HandleCheck(UDPConnectHandle_, "�\�P�b�g�����ĂȂ�");

	//������IP�A�h���X���擾
	IPDATA ip[2];
	GetMyIPAddress(ip, 2, NULL);
	MyIpAddr_ = ip[1];

	me = { -1,-1,5,GetColor(155,155,0) };
	you = { -1,-1,5,GetColor(0,0,0) };

	h64Font_ = CreateFontToHandle("64", 64, -1, -1);
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
		HandleCheck(hNameFrame_, "�l�[���t���[���摜���Ȃ�");
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

	if (connectnum_ < CONNECTMAX) {
		//�|�[�g8888�i�ڑ�����e�X�g�j�ɒʐM��������
		if (CheckNetWorkRecvUDP(UDPConnectHandle_) == TRUE) {
			IPDATA ip = { 0,0,0,0 };
			int rPort;
			char Recvname[256];
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
				Recvname[std::strlen(Recvname)] = '\0';
				std::string _name(Recvname);
				user[connectnum_].name_ = _name;
				connectnum_++;

				//�ڑ��ł������Ƃ𑗐M
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

	//�ڑ��l�����m�肵�����|�[�g�ԍ��𑗂�
	if (isConnect_ == true) {
		for (int i = 0; i < connectnum_; i++) {
			int data = SERVERPORT + i;
			NetWorkSendUDP(UDPConnectHandle_, user[i].IpAddr_, CLIENTPORT, &data, sizeof(data));
		}
		SceneManager* sc = GetParent()->FindGameObject<SceneManager>();
		sc->ChangeScene(SceneManager::SCENE_ID_PLAY);
	}

}

void UDPServer::UpdatePlay()
{


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
	//���Ԃ��IP�\��
	DrawStringToHandle(320, 60,pass.c_str(), GetColor(0, 0, 0),h64Font_);

	//�Q���҂̖��O�\��
	for (int i = 0; i < connectnum_ + 1; i++) {
		DrawGraph(350, 200+i*90, hNameFrame_, true);
		if (i == 0) {
			DrawStringToHandle(520, 200 + i * 90+10, name_.c_str(), GetColor(0, 0, 0),h64Font_);
		}
		else {
			DrawStringToHandle(520, 200 + i * 90+10, user[i - 1].name_.c_str(), GetColor(0, 0, 0),h64Font_);
		}
	}

	//�X�^�[�g�{�^���\��
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
