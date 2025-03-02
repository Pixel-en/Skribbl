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
	Player* player = GetRootJob()->FindGameObject<Player>();
	if (player->GetDrawer()) {
		//DrawString(450 - (GetDrawStringWidth(theme_.c_str(), theme_.length()) / 2), 15, theme_.c_str(), GetColor(255, 255, 255));
		DrawString(450 - (GetDrawStringWidth(ThemeList_[themenum_].c_str(), ThemeList_[themenum_].length()) / 2), 15, ThemeList_[themenum_].c_str(), GetColor(255, 255, 255));
	}
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

	return false;
}

int Theme::GetThemeNum()
{
	return themenum_;
}
