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
		HandleCheck(user[i].RecvUDPHandle_, "�\�P�b�g�����ĂȂ�");

		user[i].IpAddr_ = { 0,0,0,0 };
		user[i].point_ = 0;
	}

	UDPConnectHandle_ = MakeUDPSocket(8888);
	HandleCheck(UDPConnectHandle_, "�\�P�b�g�����ĂȂ�");

	//������IP�A�h���X���擾
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
			char Recvname[256];
			//�ߋ��ɐڑ������l�łȂ����
			NetWorkRecvUDP(UDPConnectHandle_, &ip, NULL, Recvname, sizeof(Recvname), FALSE);
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
	//�`���b�g
	Chat* c = GetRootJob()->FindGameObject<Chat>();

	Theme* theme = GetRootJob()->FindGameObject<Theme>();


	struct NetData
	{
		int port;
		char name[16] = "";
		char text[64] = "";
		Player::Pencil pen;

		//��������
		int point;
		bool drawer = false;	//�G�`��
		bool correct = false;	//����
		bool reset = false;	//�L�����o�X���Z�b�g
		int themenum;
	};

	NetData data[CONNECTMAX + 1];

	//�N���C�A���g�����M
	for (int i = 0; i < connectnum_; i++) {
		if (CheckNetWorkRecvUDP(user[i].RecvUDPHandle_) == TRUE) {
			NetWorkRecvUDP(user[i].RecvUDPHandle_, NULL, NULL, &data[i], sizeof(data[i]), FALSE);
			//�`���b�g���擾
			if (data[i].text[0] != '\0') {
				
				//�`���b�g�Ɏc��
				std::string Rname(data[i].name), Rtext(data[i].text);
				c->AddAns(Rname + ":" + Rtext);

				//�������Ă��Ȃ��Ƃ�
				if (!isCorrect_) {
					//����ƃ`�F�b�N����
					if (theme->CheckTheme(Rtext) == true) {
						//�������Ă�����
						if (drawerNum_ != i) {
							//�|�C���g�����Đ����ɂ���
							user[i].point_ += 10;
							//�G�`��
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

			//�y�����擾
			if (data[i].pen.linesize_ != -1) {
				player->RecvPencil(data[i].pen);
			}

		}
	}

	//�T�[�o�[�̏�������
	//�`���b�g
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

	//���̑����
	data[connectnum_].port = 8888;
	strcpy_s(data[connectnum_].name, sizeof(data[connectnum_].name), name_.c_str());
	strcpy_s(data[connectnum_].text, sizeof(data[connectnum_].text), ctext.c_str());
	data[connectnum_].name[std::strlen(data[connectnum_].name)] = '\0';
	data[connectnum_].text[std::strlen(data[connectnum_].text)] = '\0';
	data[connectnum_].pen = player->GetPencil();
	data[connectnum_].point = myPoint_;

	//�������������Ȃ�
	if (isCorrect_) {
		//�^�C�}�[�i���o�j
		if (timer_<0.0f) {
			//�G�`������
			drawerNum_ = GetRand(connectnum_);
			if (drawerNum_ != connectnum_) {	//�N���C�A���g
				data[drawerNum_].drawer = true;
				player->SetDraw(false);
			}
			else {	//�T�[�o�[
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
			//�������o
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
	std::string pass = "���[���ԍ��F";
	std::string d3 = std::to_string(MyIpAddr_.d3);
	std::string d4 = std::to_string(MyIpAddr_.d4);
	d3.insert(0, 3 - d3.length(), '0');
	d4.insert(0, 3 - d4.length(), '0');
	pass += d3 + d4;
	//���Ԃ��IP�\��
	DrawStringToHandle(200, 100, pass.c_str(), GetColor(0, 0, 0), h64Font_);

	//�Q���҂̖��O�\��
	for (int i = 0; i < connectnum_ + 1; i++) {
		DrawGraph(300, 180 + i * 90, hNameFrame_, true);
		if (i == 0) {
			DrawStringToHandle(500, 180 + i * 90 + 10, name_.c_str(), GetColor(0, 0, 0), h64Font_);
		}
		else {
			DrawStringToHandle(500, 180 + i * 90 + 10, user[i - 1].name_.c_str(), GetColor(0, 0, 0), h64Font_);
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
