#include "ConnectScene.h"
#include "UDPServer.h"
#include "UDPClient.h"

ConnectScene::ConnectScene(GameObject* parent)
	:GameObject(parent, "ConnectScene")
{
}

void ConnectScene::Initialize()
{
}

void ConnectScene::Update()
{
}

void ConnectScene::Draw()
{
	XMINT2 Screen;
	GetScreenState(&Screen.x, &Screen.y, NULL);
	DrawBox(0, 0, Screen.x, Screen.y, GetColor(153, 255, 255), true);	//îwåi
}

void ConnectScene::Release()
{
}
