#pragma once
#include "Engine/GameObject.h"
#include <vector>
#include <string>

class Chat final : public GameObject {
    int hKeyinput_;

    bool inputFlag_;

public:
    Chat(GameObject* parent);
    ~Chat();

    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

private:
    std::vector<std::string> chatMessages;
    std::string inputBuffer;
    std::string GetUserInput();
};
