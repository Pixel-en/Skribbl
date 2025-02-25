#pragma once
#include "Engine/GameObject.h"
#include <unordered_map>
#include <string>
#include <vector>

class Score : public GameObject {
    std::unordered_map<std::string, int> playerScores_; // Map to hold scores for each player by name
    std::string currentPlayer_; // Current drawer's name
    std::vector<std::string> playerNames_; // List of player names

public:
    Score(GameObject* parent);
    ~Score();

    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

    // Functions to manage scores
    void AddPointsToPlayer(const std::string& playerName, int points);
    int GetPlayerScore(const std::string& playerName) ;
    std::string DetermineWinner() ;
    void SetCurrentPlayer(const std::string& playerName);
    void AddPlayer(const std::string& playerName);
    //std::vector<std::string> GetPlayerNames() ;
};
