#include "TitleScene.h"
#include "UDPServer.h"
#include "UDPClient.h"
#include "Engine/SceneManager.h"
#include "ImGui/imgui.h"

namespace {
	const XMINT2 SERVERBUTTON{ 250,500 };
	const XMINT2 CLIENTBUTTON{ 700,500 };
	const XMINT4 NAMEFRAME{ 555,430,695,470 };
}

TitleScene::TitleScene(GameObject* parent)
	: GameObject(parent, "TitleScene"), hImage_(-1), hSButtonImage_(-1), hCButtonImage_(-1), hMoveImage_(-1)
{
	movex = 1280;
	state_ = NONE;
	NowNameInput_ = false;
	nameOK = false;
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
	if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0 && state_ == NONE) {


		XMINT2 pos;
		GetMousePoint(&pos.x, &pos.y);

		if (nameOK) {
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

		else {
			//ネームフレームをクリックしたとき
			if (pos.x >= NAMEFRAME.x && pos.x <= NAMEFRAME.z &&
				pos.y >= NAMEFRAME.y && pos.y <= NAMEFRAME.w) {
				//名前を入力できるようにする
				if (NowNameInput_ == false) {
					hKeyData_ = MakeKeyInput(6, true, false, false);
					SetActiveKeyInput(hKeyData_);
				}
			}
		}
	}

	//名前入力
	switch (CheckKeyInput(hKeyData_))
	{
	case 0:
		NowNameInput_ = true;
		break;
	case 1:
	{
		char name[256] = "";
		GetKeyInputString(name, hKeyData_);
		name_ = name;
		//名前が入っていたら
		if (name_ != "") {
			nameOK = true;
		}
		NowNameInput_ = false;
	}
		break;
	case 2:
		NowNameInput_ = false;
		break;
	default:
		break;
	}

	switch (state_)
	{
	case TitleScene::NONE:
		break;
	case TitleScene::SERVER:
		if (movex < -200) {
			UDPServer* us = Instantiate<UDPServer>(GetRootJob());
			us->SetName(name_);
			SceneManager* sc = GetRootJob()->FindGameObject<SceneManager>();
			sc->ChangeScene(SceneManager::SCENE_ID_CONNECT);
			break;
		}
	case TitleScene::CLIENT:
		if (movex < -200) {
			UDPClient* uc = Instantiate<UDPClient>(GetRootJob());
			uc->SetName(name_);
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
	DrawGraph(0, 0, hImage_, FALSE);
	DrawGraph(SERVERBUTTON.x, SERVERBUTTON.y, hSButtonImage_, true);
	DrawGraph(CLIENTBUTTON.x, CLIENTBUTTON.y, hCButtonImage_, true);


	DrawBox(NAMEFRAME.x, NAMEFRAME.y, NAMEFRAME.z, NAMEFRAME.w, GetColor(100, 100, 100), true); // TypeBox
	DrawBox(NAMEFRAME.x, NAMEFRAME.y, NAMEFRAME.z, NAMEFRAME.w, GetColor(0, 0, 0), false);

	if (NowNameInput_) {
		DrawKeyInputString(NAMEFRAME.x + 10, NAMEFRAME.y + 10, hKeyData_);
	}
	else {
		if (nameOK)
			DrawString(NAMEFRAME.x + 10, NAMEFRAME.y + 10, name_.c_str(), GetColor(200, 200, 200));
		else
			DrawString(NAMEFRAME.x + 10, NAMEFRAME.y + 10, "3文字以内の名前", GetColor(200, 200, 200));
	}


	DrawGraph(movex, 0, hMoveImage_, true);
}

void TitleScene::Release()
{
}
