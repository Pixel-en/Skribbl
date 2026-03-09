#include "BackGround.h"
#include "ImGui/imgui.h"

namespace {
	const XMINT2 FRAMEPOS{ 914,615 };
	const XMINT4 CHATPOS{ 900,0,1280,720 };
	const XMINT4 PLAYERSPOS{ 0, 500, 1280, 720 };
	const XMINT4 PALLETBACKPOS{ 900, 500, 1280, 720 };
	const XMINT4 THEMEPOS{ 0, 0, 900, 50 };
	const XMINT2 TIMERBACKPOS{ 10,0 };
	const XMINT2 TIMERCIRCLEPOS{ 35, 25 };
	const int TIMEPERCENT{ 100 };
	const XMINT2 PLAYERFRAMEPOS{ 4,504 };
	const int PLAYERFRAMEBUFFER{ 224 };
	const XMINT2 PALLETPOS{ 910,510 };
	const XMINT2 PALLETFRAMEBUFFER{ 44,45 };
	const XMINT2 ERASEFRAMEPOS{ 1046,615 };
	const XMINT2 CHATFRAMEPOS{ 910,10 };

	const XMINT4 TYPEBOX{ 960,420,1220,460 };
	const XMINT4 TYPEBOXBACK{ 960,420,1220,460 };

	const int MAXPLAYERCOUNT{ 4 };

	const XMINT4 SKETCHPOS{ 0,0,900,500 };
	const int LINESET{ 3 };
}


BackGround::BackGround(GameObject* parent)
	:GameObject(parent, "BackGround"), hPalletImage_(-1), hFrameImage_(-1), hCFrameImage_(-1), hPFrameImage_(-1)
{
	timer_ = 0.0f;
}

BackGround::~BackGround()
{
}

void BackGround::Initialize()
{
	DrawBox(SKETCHPOS.x, SKETCHPOS.y, SKETCHPOS.z, SKETCHPOS.w, GetColor(255, 255, 255), true);	//スケッチエリア
	hPalletImage_ = LoadGraph("Assets\\Image\\ColorPallet4.png");
	HandleCheck(hPalletImage_, "パレットがない");

	hFrameImage_ = LoadGraph("Assets\\Image\\PalletLineFrame.png");
	HandleCheck(hFrameImage_, "パレットフレームがない");

	hEFrameImage_ = LoadGraph("Assets\\Image\\PalletEraseFrame.png");
	HandleCheck(hEFrameImage_, "消しゴムフレームがない");

	hCFrameImage_ = LoadGraph("Assets\\Image\\ChatFrame.png");
	HandleCheck(hCFrameImage_, "チャットフレームがない");

	hPFrameImage_ = LoadGraph("Assets\\Image\\PlayerFrame.png");
	HandleCheck(hPFrameImage_, "プレイヤーフレームがない");

	hTimerBackImage_ = LoadGraph("Assets\\Image\\TimerBack.png");
	HandleCheck(hTimerBackImage_, "時計背景がない");

	hTimerImage_ = LoadGraph("Assets\\Image\\Timer.png");
	HandleCheck(hTimerImage_, "時計がない");

	linesize_ = 0;
	Erase_ = false;
}

void BackGround::Update()
{
}

void BackGround::Draw()
{
	DrawBox(CHATPOS.x, CHATPOS.y, CHATPOS.z, CHATPOS.w, GetColor(0, 0, 255), true);	//チャット 1280,500
	DrawBox(PLAYERSPOS.x, PLAYERSPOS.y, PLAYERSPOS.z, PLAYERSPOS.w, GetColor(255, 0, 0), true);	//参加者
	DrawBox(PALLETBACKPOS.x, PALLETBACKPOS.y, PALLETBACKPOS.z, PALLETBACKPOS.w, GetColor(100, 100, 100), true);	//パレット
	DrawBox(THEMEPOS.x, THEMEPOS.y, THEMEPOS.z, THEMEPOS.w, GetColor(150, 150, 0), true);	//お題枠
	
	DrawGraph(TIMERBACKPOS.x, TIMERBACKPOS.y, hTimerBackImage_, true);
	DrawCircleGauge(TIMERCIRCLEPOS.x, TIMERCIRCLEPOS.y, TIMEPERCENT, hTimerImage_, (PLAYTIME - timer_) / PLAYTIME * 100.0f);

	for (int i = 0; i < MAXPLAYERCOUNT; i++) {
		DrawGraph(PLAYERFRAMEPOS.x + (i * PLAYERFRAMEBUFFER), PLAYERFRAMEPOS.y, hPFrameImage_, true);
	}

	DrawGraph(PALLETPOS.x, PALLETPOS.y, hPalletImage_, true);
	DrawGraph(FRAMEPOS.x + PALLETFRAMEBUFFER.x * (linesize_ % LINESET), FRAMEPOS.y + PALLETFRAMEBUFFER.y * (linesize_ / LINESET), hFrameImage_, true);
	if (Erase_)
		DrawGraph(ERASEFRAMEPOS.x, ERASEFRAMEPOS.y, hEFrameImage_, true);
	DrawGraph(CHATFRAMEPOS.x, CHATFRAMEPOS.y, hCFrameImage_, true);

	DrawBox(TYPEBOX.x, TYPEBOX.y, TYPEBOX.z, TYPEBOX.w, GetColor(100, 100, 100), true); // TypeBox
	DrawBox(TYPEBOXBACK.x, TYPEBOXBACK.y, TYPEBOXBACK.z, TYPEBOXBACK.w, GetColor(255, 255, 255), false);

}

void BackGround::Release()
{
}

void BackGround::CanvasReset()
{
	DrawBox(0, 0, 1280, 720, GetColor(255, 255, 255), true);
}
