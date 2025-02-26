#include "Score.h"
#include <vector>

Score::Score(GameObject* parent)
    : GameObject(parent) {
}

Score::~Score() {
}

void Score::Initialize() {
    // Clear scores and player names at the start
    playerScores_.clear();
    playerNames_.clear();
}

void Score::Update() {
    // Placeholder for any updates needed during each frame
}

void Score::Draw() {
    // Placeholder for any drawing needed
}

void Score::Release() {
}

void Score::AddPointsToPlayer(const std::string& playerName, int points) {
    if (playerScores_.find(playerName) != playerScores_.end()) {
        playerScores_[playerName] += points;
    }
}

int Score::GetPlayerScore(const std::string& playerName) {
    if (playerScores_.find(playerName) != playerScores_.end()) {
        return playerScores_.at(playerName);
    }
    return 0;
}

std::string Score::DetermineWinner() {
    int maxScore = 0;
    std::string winner;
    for (const auto& pair : playerScores_) {
        if (pair.second > maxScore) {
            maxScore = pair.second;
            winner = pair.first;
        }
    }
    return winner;
}

void Score::SetCurrentPlayer(const std::string& playerName) {
    currentPlayer_ = playerName;
}

void Score::AddPlayer(const std::string& playerName) {
    if (playerScores_.find(playerName) == playerScores_.end()) {
        playerScores_[playerName] = 0;
        playerNames_.push_back(playerName);
    }
}

std::vector<std::string> Score::GetPlayerNames()const {
    return playerNames_;
}
