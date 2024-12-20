#include "TestScene.h"

TestScene::TestScene(GameObject* parent)
{
}

void TestScene::Initialize()
{
	pre = { -1,-1 };
}

void TestScene::Update()
{

}

void TestScene::Draw()
{
	if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0) {
		GetMousePoint(&now.x, &now.y);
		if (pre.x >= 0) {
			DrawLine(pre.x, pre.y, now.x, now.y, GetColor(255, 255, 255), 2);
		}
		pre = now;
	}
	else
		pre.x = pre.y = -1;
}

void TestScene::Release()
{
}
