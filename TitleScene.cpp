#include "TitleScene.h"
#include "UDPServer.h"
#include "UDPClient.h"

TitleScene::TitleScene(GameObject* parent)
    : GameObject(parent, "TitleScene"),hImage_(-1){
   
}

void TitleScene::Initialize()
{
    Instantiate<UDPServer>(this);
    hImage_ = LoadGraph("Assets\\Image\\Title.jpg");
    if (hImage_ < 0) {
        MessageBox(NULL, "‰æ‘œ‚ª‚È‚¢", "title", MB_OK);
        exit(0);
    }
}

void TitleScene::Update()
{
}

void TitleScene::Draw()
{
    DrawGraph( 0, 0, hImage_, FALSE);
}

void TitleScene::Release()
{
}
