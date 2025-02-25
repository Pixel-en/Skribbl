#pragma once
#include "Engine/GameObject.h"
#include <string>
#include<vector>
#include <list>
#include"Theme.h"
#include "UDPClient.h"
#include "UDPServer.h"
#include "Score.h"
#include "Engine/Time.h"
class Chat : public GameObject {

    int hKeyData_;

    bool NowKeyinput_;

    char str[256];

    std::list<std::string> StrHistory_;

    std::string ans;
    Theme* theme_; // Pointer to Theme object
    UDPClient* client_;  // Reference to the UDPClient object
    UDPServer* server_;  // Reference to the UDPServer object
    Score* score_;
    bool isGuessingActive_; // Flag to indicate if guessing is active
    float remainingTime_; // Variable to track the remaining time
    std::vector<std::string> drawingOrder_;
    int currentDrawerIndex_;
public:
    Chat(GameObject* parent);
    ~Chat();

    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

    /// <summary>
    /// ï ÇÃêlÇÃìöÇ¶Çì¸ÇÍÇÈ
    /// </summary>
    /// <param name="str">ñºëO+ìöÇ¶</param>
    void AddAns(std::string str) { StrHistory_.push_front(str); }

    void CheckTheme(const std::string& answer);
    
    void StartNextTurn();
    std::string GetText() {return ans; }
    void StartGuessing();
     void StopGuessing() { isGuessingActive_ = false; }
};
