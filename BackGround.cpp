#include "BackGround.h"
#include "ImGui/imgui.h"

namespace {
	const XMINT2 FRAMEPOS{ 914,615 };
}


BackGround::BackGround(GameObject* parent)
	:GameObject(parent,"BackGround"),hPalletImage_(-1),hFrameImage_(-1)
{
}

BackGround::~BackGround()
{
}

void BackGround::Initialize()
{
	DrawBox(0, 0, 900, 500, GetColor(255, 255, 255), true);	//スケッチエリア
	hPalletImage_ = LoadGraph("Assets\\Image\\ColorPallet3.png");
	assert(hPalletImage_ >= 0);

	hFrameImage_ = LoadGraph("Assets\\Image\\PalletLineFrame.png");
	assert(hFrameImage_ >= 0);

	hEFrameImage_ = LoadGraph("Assets\\Image\\PalletEraseFrame.png");
	assert(hEFrameImage_ >= 0);

	linesize_ = 0;
	Erase_ = false;
}

void BackGround::Update()
{
}

void BackGround::Draw()
{

	DrawBox(900, 0, 1280, 720, GetColor(0, 0, 255), true);	//チャット
	DrawBox(0, 500, 1280, 720, GetColor(255, 0, 0), true);	//参加者
	DrawBox(900, 500, 1280, 720, GetColor(100, 100, 100), true);	//パレット
	DrawBox(0, 0, 900, 50, GetColor(150, 150, 0), true);	//お題枠
	DrawGraph(910, 510, hPalletImage_, true);
	DrawGraph(FRAMEPOS.x + 44 * (linesize_ % 3), FRAMEPOS.y + 45 * (linesize_ / 3), hFrameImage_, true);
	if (Erase_)
		DrawGraph(1046, 615, hEFrameImage_, true);

}

void BackGround::Release()
{
}
