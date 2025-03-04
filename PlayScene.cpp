#include "PlayScene.h"
#include "BackGround.h"
#include "Chat.h"
#include "Player.h"
#include "Theme.h"
#include "Engine/SceneManager.h"

PlayScene::PlayScene(GameObject* parent)
	:GameObject(parent, "PlayScene"), hImage_(-1)
{
	movex_ = 1280;
	end_ = false;
}

void PlayScene::Initialize()
{
	Instantiate<BackGround>(this);
	Instantiate<Theme>(this);
	Instantiate<Chat>(this);
	Instantiate<Player>(this);
	
	hImage_ = LoadGraph("Assets\\Image\\TitleMove.png");
	HandleCheck(hImage_, "move‚ª‚È‚¢");
}

void PlayScene::Update()
{
	if (end_) {
		if (movex_ < -200) {
			SceneManager* sc = GetRootJob()->FindGameObject<SceneManager>();
			sc->ChangeScene(SceneManager::SCENE_ID_GAMEOVER);
		}
		else {
			movex_ -= 10;
		}
	}
}

void PlayScene::Draw()
{
	DrawGraph(movex_, 0, hImage_, true);
}

void PlayScene::Release()
{
}
