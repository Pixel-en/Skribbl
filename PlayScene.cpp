#include "PlayScene.h"

#include "BackGround.h"
#include "Chat.h"
#include "Player.h"

PlayScene::PlayScene(GameObject* parent)
	:GameObject(parent,"PlayScene")
{
}

void PlayScene::Initialize()
{
	Instantiate<BackGround>(this);
	Instantiate<Chat>(this);
	Instantiate<Player>(this);
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
