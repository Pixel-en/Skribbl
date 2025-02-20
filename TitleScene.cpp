#include "TitleScene.h"
#include "UDPServer.h"
#include "UDPClient.h"
#include "Engine/SceneManager.h"

namespace {
	const XMINT2 SERVERBUTTON{ 250,500 };
	const XMINT2 CLIENTBUTTON{ 700,500 };
}

TitleScene::TitleScene(GameObject* parent)
	: GameObject(parent, "TitleScene"), hImage_(-1), hSButtonImage_(-1), hCButtonImage_(-1),hMoveImage_(-1)
{
	movex = 1280;
	state_ = NONE;
}

void TitleScene::Initialize()
{
	hImage_ = LoadGraph("Assets\\Image\\Title.jpg");
	HandleCheck(hImage_, "タイトル画像がない");
	hSButtonImage_ = LoadGraph("Assets\\Image\\ServerStart.png");
	HandleCheck(hSButtonImage_, "サーバーボタンがない");
	hCButtonImage_ = LoadGraph("Assets\\Image\\ClientStart.png");
	HandleCheck(hCButtonImage_, "クライアントボタンがない");

	hMoveImage_ = LoadGraph("Assets\\Image\\TitleMove.png");
	HandleCheck(hMoveImage_, "moveがない");
}

void TitleScene::Update()
{

	if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0&&state_==NONE) {
		XMINT2 pos;
		GetMousePoint(&pos.x, &pos.y);
		//サーバーのボタン
		XMINT2 sb;
		GetGraphSize(hSButtonImage_, &sb.x, &sb.y);
		XMINT2 cb;
		GetGraphSize(hCButtonImage_, &cb.x, &cb.y);

		if (pos.x >= SERVERBUTTON.x && pos.x <= SERVERBUTTON.x + sb.x &&
			pos.y >= SERVERBUTTON.y && pos.y <= SERVERBUTTON.y + sb.y) {
			state_ = SERVER;
		}
		else if (pos.x >= CLIENTBUTTON.x && pos.x <= CLIENTBUTTON.x + cb.x &&
				pos.y >= CLIENTBUTTON.y && pos.y <= CLIENTBUTTON.y + cb.y) {
			state_ = CLIENT;
		}

	}

	switch (state_)
	{
	case TitleScene::NONE:
		break;
	case TitleScene::SERVER:
		if (movex < -200) {
			Instantiate<UDPServer>(GetRootJob());
			SceneManager* sc = GetRootJob()->FindGameObject<SceneManager>();
			sc->ChangeScene(SceneManager::SCENE_ID_CONNECT);
			break;
		}
	case TitleScene::CLIENT:
		if (movex < -200) {
			Instantiate<UDPClient>(GetRootJob());
			SceneManager* sc = GetRootJob()->FindGameObject<SceneManager>();
			sc->ChangeScene(SceneManager::SCENE_ID_CONNECT);
		}
		else {
			movex -= 10;
		}
		break;
	default:
		break;
	}

}

void TitleScene::Draw()
{
	DrawGraph( 0, 0, hImage_, FALSE);
	DrawGraph(SERVERBUTTON.x, SERVERBUTTON.y, hSButtonImage_, true);
	DrawGraph(CLIENTBUTTON.x, CLIENTBUTTON.y, hCButtonImage_, true);
	DrawGraph(movex, 0, hMoveImage_, TRUE);
}

void TitleScene::Release()
{
}
