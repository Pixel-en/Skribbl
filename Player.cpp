#include "Player.h"
#include "BackGround.h"
#include "ImGui/imgui.h"

namespace {
	const XMINT2 LINESIZEPOS{ 920,620 };
}

Player::Player(GameObject* parent)
	:GameObject(parent, "Player")
{
	pen.PreMousePos_ = { -1,-1 };
	pen.NowMousePos_ = { -1,-1 };
	isDrawer = false;
	youPen.linesize_ = -1;
}

Player::~Player()
{
}

void Player::Initialize()
{
	pen.Cr_ = 0;
	pen.Erase_ = false;

}

void Player::Update()
{
	drawOK_ = false;

	BackGround* bg = GetParent()->FindGameObject<BackGround>();
	if (isDrawer) {
		if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0) {

			if (pen.NowMousePos_.x >= 0)
				pen.PreMousePos_ = pen.NowMousePos_;

			GetMousePoint(&pen.NowMousePos_.x, &pen.NowMousePos_.y);
			if ((pen.NowMousePos_.x >= 0 && pen.NowMousePos_.x < 900) && (pen.NowMousePos_.y >= 50 && pen.NowMousePos_.y < 500)) {
				if (pen.PreMousePos_.x >= 0) {
					drawOK_ = true;
				}
			}
		}
		else {
			pen.PreMousePos_ = { -10,-10 };
			//pen.NowMousePos_ = { -10,-10 };
		}
	}
	else
	{
		pen.PreMousePos_ = { -10,-10 };
		//pen.NowMousePos_ = { -10,-10 };
	}


	if ((GetMouseInput() & MOUSE_INPUT_RIGHT) != 0)
	{
		XMINT2 PixelPos;
		GetMousePoint(&PixelPos.x, &PixelPos.y);

		//ƒpƒŒƒbƒg‚©‚çF‚ðŽæ“¾
		if ((PixelPos.x > 916 && PixelPos.x < 1260) && (PixelPos.y > 516 && PixelPos.y < 610)) {
			pen.Cr_ = GetPixel(PixelPos.x, PixelPos.y);
			pen.Erase_ = false;
		}
		//‘¾‚³‘I‘ð
		for (int i = 0; i < 6; i++) {
			if ((PixelPos.x > LINESIZEPOS.x + ((i % 3) * 44) && PixelPos.x < (LINESIZEPOS.x + 44) + ((i % 3) * 44) &&
				(PixelPos.y > LINESIZEPOS.y + ((i / 3) * 45) && PixelPos.y < (LINESIZEPOS.y + 45) + ((i / 3) * 45)))) {
				bg->SetLineSize(i);
				pen.linesize_ = i;
				break;
			}
		}

		//Á‚µƒSƒ€
		if ((PixelPos.x > 1050 && PixelPos.x < 1090) && (PixelPos.y > 620 && PixelPos.y < 700)) {
			if (!RFlag_) {
				pen.Erase_ = !pen.Erase_;
			}
		}
		RFlag_ = true;
	}
	else
		RFlag_ = false;

	bg->SetErase(pen.Erase_);


}

void Player::Draw()
{
	int LineSizes[6] = { 2,5,10,20,25,50 };

	if (isDrawer) {
		if (pen.drawOK_) {
			//Á‚µƒSƒ€
			if (pen.Erase_)
				DrawCircle(pen.NowMousePos_.x, pen.NowMousePos_.y, LineSizes[pen.linesize_], GetColor(255, 255, 255), true);
			else
				DrawLine(pen.PreMousePos_.x, pen.PreMousePos_.y, pen.NowMousePos_.x, pen.NowMousePos_.y, pen.Cr_, LineSizes[pen.linesize_]);
		}
	}
	else {
		if (pen.drawOK_) {
			//Á‚µƒSƒ€
			if (youPen.Erase_)
				DrawCircle(youPen.NowMousePos_.x, youPen.NowMousePos_.y, LineSizes[youPen.linesize_], GetColor(255, 255, 255), true);
			else
				DrawLine(youPen.PreMousePos_.x, youPen.PreMousePos_.y, youPen.NowMousePos_.x, youPen.NowMousePos_.y, youPen.Cr_, LineSizes[youPen.linesize_]);
		}
	}
	//ƒyƒ“‚ÌF
	DrawBox(1125, 646, 1218, 674, pen.Cr_, true);

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
//			int centerX = 450 - (stringWidth / 2); // 450 is half the width of the ‚¨‘è˜g
//			DrawString(centerX, 15, currentTheme.c_str(), GetColor(255, 0, 0)); // Adjust Y position as needed
//		}
//	}
//}
