#pragma once
#include "Engine/GameObject.h"

//�e�X�g�V�[�����Ǘ�����N���X
class TitleScene : public GameObject
{
    int hImage_;
    int hSButtonImage_;
    int hCButtonImage_;
    int hMoveImage_;

    int movex;

    enum State {
        NONE,
        SERVER,
        CLIENT,
        MAX
    };
    State state_;

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
