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
    /// �ʂ̐l�̓���������
    /// </summary>
    /// <param name="str">���O+����</param>
    void AddAns(std::string str) { StrHistory_.push_front(str); }

 /*   bool CheckTheme();*/

    std::string GetText() {return ans; }

    void Correct();
};
