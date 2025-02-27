#include "Player.h"
#include "BackGround.h"
#include "ImGui/imgui.h"

namespace {
	const XMINT2 LINESIZEPOS{ 920,620 };
}


Player::Player(GameObject* parent)
	:GameObject(parent, "Player")
{
	PreMousePos_ = { -1,-1 };
	NowMousePos_ = { -1,-1 };
	isDrawer = false;
}

Player::~Player()
{
}

void Player::Initialize()
{
	Cr_ = 0;
	Erase_ = false;

}

void Player::Update()
{
	drawOK_ = false;

	BackGround* bg = GetParent()->FindGameObject<BackGround>();

	if (isDrawer) {
		if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0) {

			if (NowMousePos_.x >= 0)
				PreMousePos_ = NowMousePos_;

			GetMousePoint(&NowMousePos_.x, &NowMousePos_.y);
			if ((NowMousePos_.x >= 0 && NowMousePos_.x < 900) && (NowMousePos_.y >= 50 && NowMousePos_.y < 500)) {
				if (PreMousePos_.x >= 0) {
					drawOK_ = true;
				}
			}
		}
		else {
			PreMousePos_ = { -1,-1 };
			NowMousePos_ = { -1,-1 };
		}


		if ((GetMouseInput() & MOUSE_INPUT_RIGHT) != 0)
		{
			XMINT2 PixelPos;
			GetMousePoint(&PixelPos.x, &PixelPos.y);

			//パレットから色を取得
			if ((PixelPos.x > 916 && PixelPos.x < 1260) && (PixelPos.y > 516 && PixelPos.y < 610)) {
				Cr_ = GetPixel(PixelPos.x, PixelPos.y);
				Erase_ = false;
			}
			//太さ選択
			for (int i = 0; i < 6; i++) {
				if ((PixelPos.x > LINESIZEPOS.x + ((i % 3) * 44) && PixelPos.x < (LINESIZEPOS.x + 44) + ((i % 3) * 44) &&
					(PixelPos.y > LINESIZEPOS.y + ((i / 3) * 45) && PixelPos.y < (LINESIZEPOS.y + 45) + ((i / 3) * 45)))) {
					bg->SetLineSize(i);
					linesize_ = i;
					break;
				}
			}

			//消しゴム
			if ((PixelPos.x > 1050 && PixelPos.x < 1090) && (PixelPos.y > 620 && PixelPos.y < 700)) {
				if (!RFlag_) {
					Erase_ = !Erase_;
				}
			}
			RFlag_ = true;
		}
		else
			RFlag_ = false;
	}

	bg->SetErase(Erase_);


}

void Player::Draw()
{
	int LineSizes[6] = { 2,5,10,20,25,50 };

	if (drawOK_) {
		//消しゴム
		if (Erase_)
			DrawCircle(NowMousePos_.x, NowMousePos_.y, LineSizes[linesize_], GetColor(255, 255, 255), true);
		else
			DrawLine(PreMousePos_.x, PreMousePos_.y, NowMousePos_.x, NowMousePos_.y, Cr_, LineSizes[linesize_]);
	}
	//ペンの色
	DrawBox(1125, 646, 1218, 674, Cr_, true);

	DrawString(450 - (GetDrawStringWidth(theme_.c_str(), theme_.length()) / 2), 15, theme_.c_str(), GetColor(255, 255, 255));
}

void Player::Release()
{
}

//void Player::DrawCurrentTheme()
//{
//
//	if (theme_) {
//		const std::string& currentTheme = theme_->GetCurrentTheme();
//		if (!currentTheme.empty()) {
//			int stringWidth = GetDrawStringWidth(currentTheme.c_str(), currentTheme.length());
//			int centerX = 450 - (stringWidth / 2); // 450 is half the width of the お題枠
//			DrawString(centerX, 15, currentTheme.c_str(), GetColor(255, 0, 0)); // Adjust Y position as needed
//		}
//	}
//}
