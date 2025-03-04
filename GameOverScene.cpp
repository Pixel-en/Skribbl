#include "GameOverScene.h"

GameOverScene::GameOverScene(GameObject* parent)
{
}

void GameOverScene::Initialize()
{
}

void GameOverScene::Update()
{
}

void GameOverScene::Draw()
{
	XMINT2 Screen;
	GetScreenState(&Screen.x, &Screen.y, NULL);
	DrawBox(0, 0, Screen.x, Screen.y, GetColor(160, 216, 239), true);	//îwåi
}

void GameOverScene::Release()
{
}
