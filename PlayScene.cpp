#include "PlayScene.h"

#include "BackGround.h"

PlayScene::PlayScene(GameObject* parent)
	:GameObject(parent,"PlayScene")
{
}

void PlayScene::Initialize()
{
	Instantiate<BackGround>(this);
}

void PlayScene::Update()
{
}

void PlayScene::Draw()
{
}

void PlayScene::Release()
{
}
