#include "TitleScene.h"
#include "Engine/SceneManager.h"

TitleScene::TitleScene(GameObject* parent)
	:GameObject(parent,"TitleScene")
{
}

void TitleScene::Initialize()
{
}

void TitleScene::Update()
{
	SceneManager::Instance()->ChangeScene(SceneManager::SCENE_ID_PLAY);
}

void TitleScene::Draw()
{
}

void TitleScene::Release()
{
}
