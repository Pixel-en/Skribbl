#include "ConnectScene.h"
#include "UDPServer.h"
#include "UDPClient.h"
#include "Engine/SceneManager.h"

namespace {

	const XMINT2 TITLEFRAME{ 100,600 };
}

ConnectScene::ConnectScene(GameObject* parent)
	:GameObject(parent, "ConnectScene"), hImage_(-1),hBack_(-1)
{
}

void ConnectScene::Initialize()
{
	hImage_ = LoadGraph("Assets\\Image\\BackTitle.png");
	HandleCheck(hImage_, "É^ÉCÉgÉãÉ{É^ÉìÇ™Ç»Ç¢");

	hBack_ = LoadGraph("Assets\\Image\\Back.jpg");
	HandleCheck(hBack_, "îwåiÇ™Ç»Ç¢");
}

void ConnectScene::Update()
{
	if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0) {

		XMINT2 pos;
		GetMousePoint(&pos.x, &pos.y);

		XMINT2 tb;
		GetGraphSize(hImage_, &tb.x, &tb.y);

		if (pos.x >= TITLEFRAME.x && pos.x <= TITLEFRAME.x + tb.x &&
			pos.y >= TITLEFRAME.y && pos.y <= TITLEFRAME.y + tb.y) {
			SceneManager* sc = GetRootJob()->FindGameObject<SceneManager>();
			sc->ChangeScene(SceneManager::SCENE_ID_TITLE);

			UDPServer* server = GetRootJob()->FindGameObject<UDPServer>();
			UDPClient* client = GetRootJob()->FindGameObject<UDPClient>();

			if (server != nullptr)
				server->KillMe();
			if (client != nullptr)
				client->KillMe();
		}
	}
}

void ConnectScene::Draw()
{
	XMINT2 Screen;
	GetScreenState(&Screen.x, &Screen.y, NULL);
	DrawBox(0, 0, Screen.x, Screen.y, GetColor(160, 216, 239), true);	//îwåi
	DrawGraph(0, 0, hBack_, true);
	DrawGraph(TITLEFRAME.x, TITLEFRAME.y, hImage_, true);
}

void ConnectScene::Release()
{
}
