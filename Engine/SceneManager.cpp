#include "sceneManager.h"

#include "../TestScene.h"
#include "../TitleScene.h"
#include "../PlayScene.h"
#include "../ConnectScene.h"
#include "../GameOverScene.h"
#include "time.h"

#include "../UDPClient.h"
#include "../UDPServer.h"

//コンストラクタ
SceneManager::SceneManager(GameObject * parent)
	: GameObject(parent, "SceneManager")
{
}

//初期化
void SceneManager::Initialize()
{
	Time::Init();
	//最初のシーンを準備
	currentSceneID_ = SCENE_ID_TITLE;
	nextSceneID_ = currentSceneID_;
	Instantiate<TitleScene>(this);
}

//更新
void SceneManager::Update()
{
	//次のシーンが現在のシーンと違う　＝　シーンを切り替えなければならない
	if (currentSceneID_ != nextSceneID_)
	{
		//そのシーンのオブジェクトを全削除
		KillAllChildren();
		InitGraph();

		std::string filename = "";

		//次のシーンを作成
		switch (nextSceneID_)
		{
		case SCENE_ID_TEST: Instantiate<TestScene>(this); break;
		case SCENE_ID_CONNECT: Instantiate<ConnectScene>(this); break;
		case SCENE_ID_TITLE: Instantiate<TitleScene>(this); break;
		case SCENE_ID_PLAY: Instantiate<PlayScene>(this); break;
		case SCENE_ID_GAMEOVER: Instantiate<GameOverScene>(this); break;

		}
		currentSceneID_ = nextSceneID_;

		UDPServer* server = GetParent()->FindGameObject<UDPServer>();
		UDPClient* client = GetParent()->FindGameObject<UDPClient>();

		if (server != nullptr)
			server->Initialize();
		if (client != nullptr)
			client->Initialize();

	}
}

//描画
void SceneManager::Draw()
{
	Time::Refresh();
}

//開放
void SceneManager::Release()
{
}

//シーン切り替え（実際に切り替わるのはこの次のフレーム）
void SceneManager::ChangeScene(SCENE_ID next)
{
	nextSceneID_ = next;
}