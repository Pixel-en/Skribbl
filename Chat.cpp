#include "Chat.h"
#include <DxLib.h>
#include <cstring>

namespace {
	const int MAXLENGTH{ 26 };  //日本語で２０文字
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
	//入力されているかどうか
	switch (CheckKeyInput(hKeyData_))
	{
	//入力中
	case 0:
		NowKeyinput_ = true;
		break;
	//入力完了
	case 1:
	{
		GetKeyInputString(str, hKeyData_);
		std::string ans = str;
		//何も入ってなかったら履歴に入れない
		if (ans != "") {
			StrHistory_.push_front("あなた：" + ans);
		}
	}
	//入力キャンセル
	case 2:
		NowKeyinput_ = false;
		//一度消して作り直す
		DeleteKeyInput(hKeyData_);
		
		hKeyData_ = MakeKeyInput(MAXLENGTH, true, false, false);
		SetActiveKeyInput(hKeyData_);

		//履歴が一定量超えたら消す
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
		DrawString(940, 400 - 5 - 25 * count, (*itr).c_str(), GetColor(255, 255, 255));
		count++;
	}
}

void Chat::Release() {
}
