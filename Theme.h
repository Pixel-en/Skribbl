#pragma once
#include "Engine/GameObject.h"
#include <string>
#include <vector>

class Theme :public GameObject
{

	std::vector<std::string> ThemeList_;
	std::string currentTheme_;
public:

	//�R���X�g���N�^
	//�����Fparent  �e�I�u�W�F�N�g�iObjectManager�j
	Theme(GameObject* parent);

	//�f�X�g���N�^
	~Theme();

	//������
	void Initialize() override;

	//�X�V
	void Update() override;

	//�`��
	void Draw() override;

	//�J��
	void Release() override;

	std::string ThemeRoll();
	std::string GetCurrentTheme() const; 
};

