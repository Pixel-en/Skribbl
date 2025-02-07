#include "TitleScene.h"
#include "Engine/SceneManager.h"

TitleScene::TitleScene(GameObject* parent)
	:GameObject(parent,"TitleScene")
{
}

void TitleScene::Initialize()
{
	//‘—Mê—p
	netUDPHandle = MakeUDPSocket(8888);
	ip = { 192,168,43,64 };
	circle = { -100,-100,5,GetColor(255,152,0) };

	while (true)
	{
		if (NetWorkSendUDP(netUDPHandle, ip, 8888, &ip, sizeof(ip) >= 0))
			break;
	}
}

void TitleScene::Update()
{
	
	GetMousePoint(&circle.x, &circle.y);
	//SceneManager::Instance()->ChangeScene(SceneManager::SCENE_ID_PLAY);
	NetWorkSendUDP(netUDPHandle, ip, 8888, &circle, sizeof(circle));

	//if (CheckNetWorkRecvUDP(netUDPHandle) == TRUE) {
	//	NetWorkRecvUDP(netUDPHandle, NULL, NULL, &recvcircle, sizeof(recvcircle), FALSE);
	//}
	//else {
	//	recvcircle = { -100,-100,5,GetColor(255,255,0) };
	//	if (ProcessMessage() < 0) {

	//		DeleteUDPSocket(netUDPHandle);
	//	}
	//}

}

void TitleScene::Draw()
{
	DrawCircle(circle.x, circle.y, circle.size, circle.color);
	DrawCircle(recvcircle.x, recvcircle.y, recvcircle.size, recvcircle.color);
}

void TitleScene::Release()
{
	DeleteUDPSocket(netUDPHandle);
}
