#include "PlayScene.h"
#include "BackGround.h"
#include "Chat.h"
#include "Player.h"
#include "Theme.h"
#include "Score.h"


PlayScene::PlayScene(GameObject* parent)
	:GameObject(parent,"PlayScene")
{
}

void PlayScene::Initialize()
{
	Instantiate<BackGround>(this);
	Instantiate<Chat>(this);
	Instantiate<Player>(this);
	Instantiate<Theme>(this);
	Instantiate<Score>(this);

	
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
