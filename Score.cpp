#include "Score.h"
#include "UDPServer.h"
#include "UDPClient.h"

Score::Score(GameObject* parent)
    : GameObject(parent) {
}

Score::~Score() {
}

void Score::Initialize() {
    // Clear scores and player names at the start
    for (int i = 0; i < MAX_PLAYERS; i++) {
        playerScores_[i] = 0;
        playerNames_[i] = "";
    }
    playerCount_ = 0;
}

void Score::Update() {
}

void Score::Draw() {
    int screenWidth = 900;
    int screenHeight = 720;
    int boxHeight = 110; // Height for each player score box
    int boxTop = 500; // Top position for the boxes
    int numPlayers = playerCount_;

    // Calculate box width based on the number of players
    int boxWidth = numPlayers > 0 ? screenWidth / numPlayers : screenWidth;

    // Draw the background box
    DrawBox(0, boxTop, screenWidth, screenHeight, GetColor(255, 0, 0), true); // Background box

    // Draw individual player score boxes
    for (int i = 0; i < numPlayers; i++) {
        int left = i * boxWidth;
        int right = left + boxWidth;
        int top = boxTop;
        int bottom = top + boxHeight;

        // Draw the box for the player
        DrawBox(left, top, right, bottom, GetColor(0, 0, 0), true);

        // Draw the player name
        DrawString(left + 10, top + 10, playerNames_[i].c_str(), GetColor(255, 255, 255));

        // Draw the score label
        DrawString(left + 10, top + 50, "SCORE:", GetColor(255, 255, 255));

        // Draw the player score
        std::string scoreStr = std::to_string(playerScores_[i]);
        DrawString(left + 100, top + 50, scoreStr.c_str(), GetColor(255, 255, 255));
    }
}

void Score::Release() {
}

void Score::AddPointsToPlayer(const std::string& playerName, int points) {
    for (int i = 0; i < MAX_PLAYERS; i++) {
        if (playerNames_[i] == playerName) {
            playerScores_[i] += points;
            return;
        }
    }
}

int Score::GetPlayerScore(const std::string& playerName) {
    for (int i = 0; i < MAX_PLAYERS; i++) {
        if (playerNames_[i] == playerName) {
            return playerScores_[i];
        }
    }
    return 0;
}

std::string Score::DetermineWinner() {
    int maxScore = 0;
    std::string winner;
    for (int i = 0; i < MAX_PLAYERS; i++) {
        if (playerScores_[i] > maxScore) {
            maxScore = playerScores_[i];
            winner = playerNames_[i];
        }
    }
    return winner;
}

void Score::AddPlayer(const std::string& playerName) {
    if (playerCount_ < MAX_PLAYERS) {
        playerNames_[playerCount_] = playerName;
        playerScores_[playerCount_] = 0;
        playerCount_++;
    }
}

std::array<std::string, MAX_PLAYERS> Score::GetPlayerNames() const {
    return playerNames_;
}
