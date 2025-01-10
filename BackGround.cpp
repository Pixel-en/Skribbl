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
	DrawBox(0, 0, 900, 500, GetColor(255, 255, 255), true);	//�X�P�b�`�G���A
	hImage_ = LoadGraph("Assets\\Image\\ColorPallet.png");

}

void BackGround::Update()
{
}

void BackGround::Draw()
{

	DrawBox(900, 0, 1280, 720, GetColor(0, 0, 255), true);	//�`���b�g
	DrawBox(0, 500, 1280, 720, GetColor(255, 0, 0), true);	//�Q����
	DrawBox(900, 500, 1280, 720, GetColor(100, 100, 100), true);	//�p���b�g
	DrawBox(0, 0, 900, 50, GetColor(150, 150, 0), true);	//����g
	DrawGraph(910, 510, hImage_, true);
}

void BackGround::Release()
{
}
