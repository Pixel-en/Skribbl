#include "BackGround.h"

BackGround::BackGround(GameObject* parent)
	:GameObject(parent,"BackGround")
{
}

BackGround::~BackGround()
{
}

void BackGround::Initialize()
{
	DrawBox(0, 0, 900, 500, GetColor(255, 255, 255), true);
}

void BackGround::Update()
{
}

void BackGround::Draw()
{

	DrawBox(900, 0, 1280, 720, GetColor(0, 0, 255), true);
	DrawBox(0, 500, 1280, 720, GetColor(255, 0, 0), true);
}

void BackGround::Release()
{
}
