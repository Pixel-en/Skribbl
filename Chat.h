#pragma once
#include "Engine/GameObject.h"
#include <string>
#include <list>

class Chat final : public GameObject {

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
    /// 別の人の答えを入れる
    /// </summary>
    /// <param name="str">名前+答え</param>
    void AddAns(std::string str) { StrHistory_.push_front(str); }

    void CheckTheme(const std::string& answer);

    std::string GetText() {return ans; }
};
