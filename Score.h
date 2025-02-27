#pragma once
#include "Engine/GameObject.h"
#include <string>
#include <array>

const int MAX_PLAYERS = 4; // Define the maximum number of players

class Score : public GameObject {
    std::array<std::string, MAX_PLAYERS> playerNames_;
    std::array<int, MAX_PLAYERS> playerScores_;
    int playerCount_;

public:
    Score(GameObject* parent);
    ~Score();

    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

    void AddPointsToPlayer(const std::string& playerName, int points);
    int GetPlayerScore(const std::string& playerName);
    std::string DetermineWinner();
    void AddPlayer(const std::string& playerName);
    std::array<std::string, MAX_PLAYERS> GetPlayerNames() const;
};
