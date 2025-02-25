#include "Chat.h"
#include <DxLib.h>
#include <cstring>
#include<ctime>
#include "ImGui/imgui.h"

namespace {
	const int MAXLENGTH{ 26 };  //日本語で13文字
	const int HISTORYMAX{ 15 };
	const float GUESSING_PERIOD{ 90.0f }; // 90 seconds guessing period
}

Chat::Chat(GameObject* parent)
	: GameObject(parent, "Chat"), NowKeyinput_(false),isGuessingActive_(false),remainingTime_(0.0f),currentDrawerIndex_(0)
	,theme_(nullptr),
	client_(nullptr),
	server_(nullptr)
{
}

Chat::~Chat() {
}

void Chat::Initialize() {
	//半角40文字、ESCキャンセルあり、半角、数字のみ不可
	hKeyData_ = MakeKeyInput(MAXLENGTH, true, false, false);
	SetActiveKeyInput(hKeyData_);
	// Get the Theme instance
	theme_ = GetParent()->FindGameObject<Theme>();
	client_ = GetParent()->FindGameObject<UDPClient>();
	server_ = GetParent()->FindGameObject<UDPServer>();
	score_ = GetParent()->FindGameObject<Score>();
	Time::Init();

	// Seed the random number generator
	std::srand(static_cast<unsigned int>(std::time(nullptr)));

	// Add player names from client and server
	if (client_) {
		score_->AddPlayer(client_->GetName());
	}
	if (server_) {
		score_->AddPlayer(server_->GetName());
		for (const auto& user : server_->GetConnectedUsers()) {
			score_->AddPlayer(user.name_);
		}
	}

	// Initialize drawing order
	//drawingOrder_ = score_->GetPlayerNames();
	srand(static_cast<unsigned int>(time(0)));
	for (int i = drawingOrder_.size() - 1; i > 0; --i) {
		int j = rand() % (i + 1);
		std::swap(drawingOrder_[i], drawingOrder_[j]);
	}
}

void Chat::Update() {
	SetUseIMEFlag(false);

	ans = "";
	if (isGuessingActive_) {
		Time::Refresh();
		remainingTime_ -= Time::DeltaTime();
		if (remainingTime_ <= 0.0f) {
			StopGuessing();
			StartNextTurn();
		}
	}
	switch (CheckKeyInput(hKeyData_)) {
	case 0:
		NowKeyinput_ = true;
		break;
	case 1:
	{
		GetKeyInputString(str, hKeyData_);
		std::string buffer(str);
		if (buffer != "") {
			ans = str;
			StrHistory_.push_front("あなた：" + ans);
			// Call CheckTheme to verify the theme
			if (isGuessingActive_) {
				CheckTheme(ans);
			}
		}
	}
	case 2:
		NowKeyinput_ = false;
		DeleteKeyInput(hKeyData_);
		hKeyData_ = MakeKeyInput(MAXLENGTH, true, false, false);
		SetActiveKeyInput(hKeyData_);
		if (StrHistory_.size() > HISTORYMAX)
			StrHistory_.pop_back();
		break;
	default:
		break;
	}

}


void Chat::Draw() {

	//入力中の文字
	if (NowKeyinput_) {
		DrawKeyInputString(970, 430, hKeyData_);
	}
	//ここで履歴表示
	int count = 0;
	for (auto itr = StrHistory_.begin(); itr != StrHistory_.end(); itr++) {
		DrawString(940, 400 - 5 - 25 * count, (*itr).c_str(), GetColor(0, 0, 0));
		count++;
	}
	// Draw the remaining time at the top left corner
	if (isGuessingActive_) {
		std::string timeText = "Time Left: " + std::to_string(static_cast<int>(remainingTime_)) + "s";
		DrawString(10, 10, timeText.c_str(), GetColor(255, 0, 0));
	}
}

void Chat::Release() {
}

void Chat::CheckTheme(const std::string& answer)
{
		/*if (theme_ && theme_->GetCurrentTheme() == answer) {
			StrHistory_.push_front(playerName + " answered correctly: " + answer);
		}*/
	std::string playerName;
	if (client_ && client_->GetName() != "") {
		playerName = client_->GetName();
	}
	else if (server_ && server_->GetName() != "") {
		playerName = server_->GetName();
	}

	if (theme_ && theme_->GetCurrentTheme() == answer) {
		StrHistory_.push_front(playerName + " answered correctly: " + answer);
		score_->AddPointsToPlayer(playerName, 10);
		score_->AddPointsToPlayer(drawingOrder_[currentDrawerIndex_], 5);
	}
}

void Chat::StartNextTurn()
{
	currentDrawerIndex_++;
	if (currentDrawerIndex_ >= drawingOrder_.size()) {
		// All players have drawn, determine the winner
		std::string winner = score_->DetermineWinner();
		StrHistory_.push_front("Game Over! Winner: " + winner);
		// Reset the game or end it here
	}
	else {
		// Set the next player as the current drawer
		score_->SetCurrentPlayer(drawingOrder_[currentDrawerIndex_]);
		theme_->SetCurrentDrawer(drawingOrder_[currentDrawerIndex_]);
		StartGuessing();
	}
}



void Chat::StartGuessing()
{
	isGuessingActive_ = true; 
	Time::Init();
}

