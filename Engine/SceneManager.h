#pragma once
#include "global.h"
#include "GameObject.h"

//ゲームに登場するシーン
//-----------------------------------------------------------
//シーン切り替えを担当するオブジェクト
//-----------------------------------------------------------
class SceneManager : public GameObject
{
private:
	//コンストラクタ
	//引数：parent	親オブジェクト（基本的にゲームマネージャー）
	SceneManager(GameObject* parent);
public:
	static SceneManager* Instance() {
		static SceneManager* inst = nullptr;
		if (inst == nullptr)
			inst = new SceneManager(nullptr);
		return inst;
	}
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

	//シーン切り替え（実際に切り替わるのはこの次のフレーム）
	//引数：next	次のシーンのID
	enum SCENE_ID
	{
		SCENE_ID_TEST = 0,
		SCENE_ID_TITLE,
		SCENE_ID_PLAY,
	};
	void ChangeScene(SCENE_ID next);

private:
	SCENE_ID currentSceneID_;	//現在のシーン
	SCENE_ID nextSceneID_;		//次のシーン

};