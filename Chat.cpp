#include "Chat.h"
#include <DxLib.h>
#include <cstring>

namespace {
	const int MAXLENGTH{ 26 };  //���{��łQ�O����
	const int HISTORYMAX{ 15 };
}

Chat::Chat(GameObject* parent)
	: GameObject(parent, "Chat"), NowKeyinput_(false)
{
}

Chat::~Chat() {
}

void Chat::Initialize() {
	//���p40�����AESC�L�����Z������A���p�A�����̂ݕs��
	hKeyData_ = MakeKeyInput(MAXLENGTH, true, false, false);
	SetActiveKeyInput(hKeyData_);
}

void Chat::Update() {
	SetUseIMEFlag(false);
	//���͂���Ă��邩�ǂ���
	switch (CheckKeyInput(hKeyData_))
	{
	//���͒�
	case 0:
		NowKeyinput_ = true;
		break;
	//���͊���
	case 1:
	{
		GetKeyInputString(str, hKeyData_);
		std::string ans = str;
		//���������ĂȂ������痚���ɓ���Ȃ�
		if (ans != "") {
			StrHistory_.push_front("���Ȃ��F" + ans);
		}
	}
	//���̓L�����Z��
	case 2:
		NowKeyinput_ = false;
		//��x�����č�蒼��
		DeleteKeyInput(hKeyData_);
		
		hKeyData_ = MakeKeyInput(MAXLENGTH, true, false, false);
		SetActiveKeyInput(hKeyData_);

		//���������ʒ����������
		if (StrHistory_.size() > HISTORYMAX)
			StrHistory_.pop_back();

		break;
	default:
		break;
	}
}

void Chat::Draw() {

	//���͒��̕���
	if (NowKeyinput_) {
		DrawKeyInputString(970, 430, hKeyData_);
	}
	//�����ŗ���\��
	int count = 0;
	for (auto itr = StrHistory_.begin(); itr != StrHistory_.end(); itr++) {
		DrawString(940, 400 - 5 - 25 * count, (*itr).c_str(), GetColor(255, 255, 255));
		count++;
	}
}

void Chat::Release() {
}
