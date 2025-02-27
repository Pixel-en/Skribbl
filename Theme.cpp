#include "Theme.h"
#include "Engine/CsvReader.h"
#include "UDPServer.h"
#include "UDPClient.h"

Theme::Theme(GameObject* parent)
    : GameObject(parent, "Theme") {
}

Theme::~Theme() {}

void Theme::Initialize() {
    CsvReader csv("Assets/Theme.csv");
    for (int i = 0; i < csv.GetLines(); i++) {
        ThemeList_.push_back(csv.GetString(i, 0));
    }
}

void Theme::Update() {
}

void Theme::Draw() {
}

void Theme::Release() {
}

std::string Theme::ThemeRoll() {
    if (ThemeList_.empty())
        return "";

    int num = std::rand() % ThemeList_.size();
    currentTheme_ = ThemeList_[num];

    // Notify server and client of the new theme
   // NotifyThemeUpdate();

    return currentTheme_;
}

std::string Theme::GetCurrentTheme() const {
    return currentTheme_;
}

