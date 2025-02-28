#pragma once
#include "Engine/GameObject.h"
#include <string>
#include <vector>

class Theme :public GameObject
{

	std::vector<std::string> ThemeList_;
	std::string currentTheme_;

	int themenum_;
public:

	//コンストラクタ
	//引数：parent  親オブジェクト（ObjectManager）
	Theme(GameObject* parent);

	//デストラクタ
	~Theme();

	//初期化
	void Initialize() override;

	//更新
	void Update() override;

	//描画
	void Draw() override;

	//開放
	void Release() override;

	void ThemeRoll();
	std::string GetCurrentTheme() const; 
	bool CheckTheme(std::string text);
	int GetThemeNum();
};

