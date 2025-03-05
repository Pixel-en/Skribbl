#pragma once
#include "Engine/GameObject.h"
#include <string>
#include <list>

class Chat : public GameObject {

    int hKeyData_;

    bool NowKeyinput_;

    char str[256];

    std::list<std::string> StrHistory_;

    std::string ans;
public:
    Chat(GameObject* parent);
    ~Chat();

    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

    /// <summary>
    /// •Ê‚Ìl‚Ì“š‚¦‚ğ“ü‚ê‚é
    /// </summary>
    /// <param name="str">–¼‘O+“š‚¦</param>
    void AddAns(std::string str) { StrHistory_.push_front(str); }

 /*   bool CheckTheme();*/

    std::string GetText() {return ans; }

    void Correct();
};
