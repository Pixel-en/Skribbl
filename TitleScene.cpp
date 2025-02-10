#include "TitleScene.h"
#include "UDPServer.h"

TitleScene::TitleScene(GameObject* parent)
    : GameObject(parent, "TitleScene"){
   
}

void TitleScene::Initialize()
{
    Instantiate<UDPServer>(this);
}

void TitleScene::Update()
{
}

void TitleScene::Draw()
{
}

void TitleScene::Release()
{
}
