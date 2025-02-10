#include "TitleScene.h"
#include "Engine/SceneManager.h"
#include "DxLib.h"

inline bool operator == (const IPDATA& a, const IPDATA& b) {
    if (a.d1 == b.d1 && a.d2 == b.d2 && a.d3 == b.d3 && a.d4 == b.d4)return  true; 
    return false;

};

TitleScene::TitleScene(GameObject* parent)
    : GameObject(parent, "TitleScene"), NetUDPHandle(-1), isCircleReceived(false) {
   
}

void TitleScene::Initialize()
{
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
