#pragma once
#include "Engine/GameObject.h"

namespace {

	const float PLAYTIME{ 120.0f };
}

class BackGround :public GameObject
{
	int hPalletImage_;
	int hFrameImage_;
	int hEFrameImage_;
	int hCFrameImage_;
	int hPFrameImage_;

	int hTimerBackImage_;
	int hTimerImage_;

	int linesize_;

	bool Erase_;

	float timer_;
public:

	//�R���X�g���N�^
	//�����Fparent  �e�I�u�W�F�N�g�iObjectManager�j
	BackGround(GameObject* parent);

	//�f�X�g���N�^
	~BackGround();

	//������
	void Initialize() override;

	//�X�V
	void Update() override;

	//�`��
	void Draw() override;

	//�J��
	void Release() override;

	void SetLineSize(int size) { linesize_ = size; };
	void SetErase(bool set) { Erase_ = set; }
	void CanvasReset();
	void SetTiemr(float time) { timer_ = time; }
};

