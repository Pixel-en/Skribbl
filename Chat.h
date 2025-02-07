#pragma once
#include "Engine/GameObject.h"
#include <string>
#include <list>
#include "Theme.h"
#include "Player.h"
class Player;
class Theme;
class Chat final : public GameObject {

    int hKeyData_;

    bool NowKeyinput_;

    char str[256];

    std::list<std::string> StrHistory_;
    Theme* theme_;
    Player* player_;
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
    void SetTheme(Theme* theme) { theme_ = theme; }// Method to set the theme
    void SetPlayer(Player* player) { player_=player; } // Method to set the player

    void CheckTheme(const std::string& answer);
};
