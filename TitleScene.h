#pragma once
#include "Engine/GameObject.h"

//�e�X�g�V�[�����Ǘ�����N���X
class TitleScene : public GameObject
{
	IPDATA ip;
	int netUDPHandle;

	struct Circle {
		int x;
		int y;
		int size;
		unsigned int color;
	};
	Circle circle;
	Circle recvcircle;
public:
    // �R���X�g���N�^
    // �����Fparent �e�I�u�W�F�N�g�iSceneManager�j
    TitleScene(GameObject* parent);

    // ������
    void Initialize() override;

    // �X�V
    void Update() override;

    // �`��
    void Draw() override;

    // �J��
    void Release() override;
};
