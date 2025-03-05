#include "Chat.h"
#include <DxLib.h>
#include <cstring>
#include "ImGui/imgui.h"

namespace {
	const int MAXLENGTH{ 26 };  //日本語で13文字
	const int HISTORYMAX{ 15 };
}

Chat::Chat(GameObject* parent)
	: GameObject(parent, "Chat"), NowKeyinput_(false)
{
}

Chat::~Chat() {
}

void Chat::Initialize() {
	//半角40文字、ESCキャンセルあり、半角、数字のみ不可
	hKeyData_ = MakeKeyInput(MAXLENGTH, true, false, false);
	SetActiveKeyInput(hKeyData_);
}

void Chat::Update() {
	SetUseIMEFlag(false);

	ans = "";

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
}

void Chat::Release() {
}

void Chat::Correct()
{
	AddAns("ゲーム：正解！");
}

