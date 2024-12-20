#include "RootObject.h"
#include "SceneManager.h"


RootObject::RootObject():
	GameObject(nullptr, "RootObject")
{
}


RootObject::~RootObject()
{
}

void RootObject::Initialize()
{
	SceneManager::Instance()->Initialize();
//	Instantiate<SceneManager>(this);
}

void RootObject::Update()
{
	SceneManager::Instance()->UpdateSub();
}

void RootObject::Draw()
{
	SceneManager::Instance()->DrawSub();
}

void RootObject::Release()
{
	SceneManager::Instance()->ReleaseSub();
}
