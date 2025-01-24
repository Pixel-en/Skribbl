#include "Chat.h"
#include <DxLib.h>
#include <cstring>

Chat::Chat(GameObject* parent)
    : GameObject(parent,"Chat"), inputBuffer("") {
}

Chat::~Chat() {
}

void Chat::Initialize() {

    //日本語２０文字、ESCキャンセル、全角可、文字可
    hKeyinput_ = MakeKeyInput(40, true, false, false);
    SetActiveKeyInput(hKeyinput_);
}

void Chat::Update() {
    //char inputChar = GetInputChar(TRUE);

    //if (inputChar != 0) {
    //    if (inputChar == CTRL_CODE_CR) { // Enter key
    //        if (!inputBuffer.empty()) {
    //            chatMessages.push_back("You: " + inputBuffer);
    //            inputBuffer.clear();
    //        }
    //    }
    //    else if (inputChar == CTRL_CODE_BS) { // Backspace key
    //        if (!inputBuffer.empty()) {
    //            inputBuffer.pop_back();
    //        }
    //    }
    //    else if (inputChar >= CTRL_CODE_CMP) {
    //        inputBuffer += inputChar;
    //    }
    //}

    //日本語入力ちょっと考える
    inputFlag_ = true;
    char buffer[256];
    if (CheckKeyInput(hKeyinput_) == 1) {
        GetKeyInputString(buffer, hKeyinput_);
        inputBuffer.push_back((*buffer));
        //日本語２０文字、ESCキャンセル、全角可、文字可
        hKeyinput_ = MakeKeyInput(40, true, false, false);
        SetActiveKeyInput(hKeyinput_);
    }

}

void Chat::Draw() {
    DrawBox(930, 420, 1200, 460, GetColor(100, 100, 100), true); // TypeBox
    DrawBox(1210, 420, 1260, 460, GetColor(255, 255, 255), true); // SendBox

    //int y = 400 - static_cast<int>(chatMessages.size()) * 20; // Adjust starting position for messages
    //for (const auto& msg : chatMessages) {
    //    DrawString(935, y, msg.c_str(), GetColor(0, 0, 0));
    //    y += 20;
    //}

    DrawString(935, 430, inputBuffer.c_str(), GetColor(0, 0, 0)); // Input buffer inside the type box

    if (inputFlag_) {
        DrawKeyInputString(935, 430, hKeyinput_);
    }
}

void Chat::Release() {
}

std::string Chat::GetUserInput() {
    return inputBuffer;
}
