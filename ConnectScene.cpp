#include "ConnectScene.h"
#include "UDPServer.h"
#include "UDPClient.h"

ConnectScene::ConnectScene(GameObject* parent)
	:GameObject(parent, "ConnectScene")
{
}

void ConnectScene::Initialize()
{
	UDPServer* server = GetRootJob()->FindGameObject<UDPServer>();
	if (server == nullptr) {
		color_ = GetColor(255, 0, 0);
	}
	else {
		color_ = GetColor(0, 255, 0);
	}
}

void ConnectScene::Update()
{
}

void ConnectScene::Draw()
{
	DrawCircle(50, 50, 10, color_, true);
}

void ConnectScene::Release()
{
}
