#include "BackGround.h"
#include "ImGui/imgui.h"

namespace {
	const XMINT2 FRAMEPOS{ 914,615 };
}


BackGround::BackGround(GameObject* parent)
	:GameObject(parent,"BackGround"),hPalletImage_(-1),hFrameImage_(-1),hCFrameImage_(-1),hPFrameImage_(-1)
{
	timer_ = 0.0f;
}

BackGround::~BackGround()
{
}

void BackGround::Initialize()
{
	DrawBox(0, 0, 900, 500, GetColor(255, 255, 255), true);	//�X�P�b�`�G���A
	hPalletImage_ = LoadGraph("Assets\\Image\\ColorPallet4.png");
	HandleCheck(hPalletImage_, "�p���b�g���Ȃ�");

	hFrameImage_ = LoadGraph("Assets\\Image\\PalletLineFrame.png");
	HandleCheck(hFrameImage_, "�p���b�g�t���[�����Ȃ�");

	hEFrameImage_ = LoadGraph("Assets\\Image\\PalletEraseFrame.png");
	HandleCheck(hEFrameImage_, "�����S���t���[�����Ȃ�");

	hCFrameImage_ = LoadGraph("Assets\\Image\\ChatFrame.png");
	HandleCheck(hCFrameImage_, "�`���b�g�t���[�����Ȃ�");

	hPFrameImage_ = LoadGraph("Assets\\Image\\PlayerFrame.png");
	HandleCheck(hPFrameImage_, "�v���C���[�t���[�����Ȃ�");

	hTimerBackImage_ = LoadGraph("Assets\\Image\\TimerBack.png");
	HandleCheck(hTimerBackImage_, "���v�w�i���Ȃ�");

	hTimerImage_ = LoadGraph("Assets\\Image\\Timer.png");
	HandleCheck(hTimerImage_, "���v���Ȃ�");

	linesize_ = 0;
	Erase_ = false;
}

void BackGround::Update()
{
}

void BackGround::Draw()
{

	DrawBox(900, 0, 1280, 720, GetColor(0, 0, 255), true);	//�`���b�g 1280,500
	DrawBox(0, 500, 1280, 720, GetColor(255, 0, 0), true);	//�Q����
	DrawBox(900, 500, 1280, 720, GetColor(100, 100, 100), true);	//�p���b�g
	DrawBox(0, 0, 900, 50, GetColor(150, 150, 0), true);	//����g

	DrawGraph(10, 0, hTimerBackImage_, true);
	DrawCircleGauge(35, 25, 100, hTimerImage_, (PLAYTIME - timer_) / PLAYTIME*100.0f);

	for (int i = 0; i < 4; i++) {
		DrawGraph(4 + (i * 224), 504, hPFrameImage_, true);
	}

	DrawGraph(910, 510, hPalletImage_, true);
	DrawGraph(FRAMEPOS.x + 44 * (linesize_ % 3), FRAMEPOS.y + 45 * (linesize_ / 3), hFrameImage_, true);
	if (Erase_)
		DrawGraph(1046, 615, hEFrameImage_, true);
	DrawGraph(910, 10, hCFrameImage_, true);

	DrawBox(960, 420, 1220, 460, GetColor(100, 100, 100), true); // TypeBox
	DrawBox(960, 420, 1220, 460, GetColor(255, 255, 255), false);

	ImGui::Begin("background");
	ImGui::InputFloat("timer", &timer_);
	ImGui::Text(std::to_string((PLAYTIME - timer_) / PLAYTIME * 100.0f).c_str());
	ImGui::End();
}

void BackGround::Release()
{
}

void BackGround::CanvasReset()
{
	DrawBox(0, 0, 1280, 720, GetColor(255, 255, 255), true);
}
