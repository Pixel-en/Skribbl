#include "Theme.h"
#include "Engine/CsvReader.h"
#include "Player.h"

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

void Theme::ThemeRoll() {
	if (ThemeList_.empty())
		return;
	
	int num = std::rand() % ThemeList_.size();
	themenum_ = num;
}

std::string Theme::GetCurrentTheme() const
{
	return currentTheme_;
}

bool Theme::CheckTheme(std::string text)
{
	if (ThemeList_[themenum_] == text)
		return true;

	else false;
}

int Theme::GetThemeNum()
{
	return themenum_;
}
