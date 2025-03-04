#include "GameOverScene.h"
#include "UDPServer.h"
#include "UDPClient.h"
#include "Engine/SceneManager.h"

namespace {
	const XMINT2 TITLEBUTTON{ 490,600 };
}

GameOverScene::GameOverScene(GameObject* parent)
	:GameObject(parent,"GameOverScene")
{
	click_ = false;
}

void GameOverScene::Initialize()
{
	hBackTitle_ = LoadGraph("Assets\\Image\\BackTitle.png");
	HandleCheck(hBackTitle_,"タイトルボタンがない");
}

void GameOverScene::Update()
{
	if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0) {

		XMINT2 pos;
		GetMousePoint(&pos.x, &pos.y);

		XMINT2 tb;
		GetGraphSize(hBackTitle_, &tb.x, &tb.y);

		if (pos.x >= TITLEBUTTON.x && pos.x <= TITLEBUTTON.y + tb.x &&
			pos.y >= TITLEBUTTON.y && pos.y <= TITLEBUTTON.y + tb.y) {
			click_ = true;
		}
	}

	if (click_) {
		if (movex < -200) {
			UDPServer* server = GetRootJob()->FindGameObject<UDPServer>();
			if (server != nullptr)
				server->KillMe();

			UDPClient* client = GetRootJob()->FindGameObject<UDPClient>();
			if (client != nullptr)
				client->KillMe();

			SceneManager* sm = GetRootJob()->FindGameObject<SceneManager>();
			sm->ChangeScene(SceneManager::SCENE_ID_TITLE);
		}
		else {
			movex -= 10;
		}
	}

}

void GameOverScene::Draw()
{
	XMINT2 Screen;
	GetScreenState(&Screen.x, &Screen.y, NULL);
	DrawBox(0, 0, Screen.x, Screen.y, GetColor(160, 216, 239), true);	//背景

	DrawGraph(TITLEBUTTON.x, TITLEBUTTON.y, hBackTitle_, true);
}

void GameOverScene::Release()
{
}
