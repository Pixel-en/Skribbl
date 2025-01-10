#include "Player.h"

Player::Player(GameObject* parent)
	:GameObject(parent,"Player")
{
	PreMousePos_ = { -1,-1 };
	NowMousePos_ = { -1,-1 };
}

Player::~Player()
{
}

void Player::Initialize()
{
	Cr_ = 0;
}

void Player::Update()
{
	if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0) {
		GetMousePoint(&NowMousePos_.x, &NowMousePos_.y);

		if (PreMousePos_.x >= 0) {
			DrawLine(PreMousePos_.x, PreMousePos_.y, NowMousePos_.x, NowMousePos_.y, Cr_, 2);
		}
		PreMousePos_ = NowMousePos_;
	}
	else {
		PreMousePos_ = { -1,-1 };
	}
	//パレットから色を取得
	if ((GetMouseInput() & MOUSE_INPUT_RIGHT) != 0)
	{
		INT2 PixelPos;
		GetMousePoint(&PixelPos.x, &PixelPos.y);
		Cr_ = GetPixel(PixelPos.x, PixelPos.y);
	}

}

void Player::Draw()
{
}

void Player::Release()
{
}
