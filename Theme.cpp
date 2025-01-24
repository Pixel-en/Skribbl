#include "Theme.h"
#include "Engine/CsvReader.h"

Theme::Theme(GameObject* parent)
	:GameObject(parent,"Theme")
{
}

Theme::~Theme()
{
}

void Theme::Initialize()
{
	CsvReader* csv = new CsvReader("Assets\\CSV\\Theme.csv");
	for (int i = 0; i < csv->GetLines(); i++) {
		ThemeList_.push_back(csv->GetString(i, 0));
	}
}

void Theme::Update()
{
}

void Theme::Draw()
{
}

void Theme::Release()
{
}

std::string Theme::ThemeRoll()
{
	int num = rand() % ThemeList_.size();

	return ThemeList_[num];
}
