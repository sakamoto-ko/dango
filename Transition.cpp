#include "Transition.h"

void Transition::Initialize() {
	transitionTex_ = TextureManager::Load("UI/gamescene.png");
	transitionSprite_.reset(Sprite::Create(transitionTex_, {0.0f, 0.0f}));
	transitionSprite_->SetTextureRect(
	    {
	        0.0f,
	        0.0f,
	    },
	    {1280.0f, 720.0f});
	transitionSprite_->SetSize({1280.0f, 720.0f});
	transitionSprite_->SetPosition({0.0f, 0.0f});

	alpha = 0.0f;
	isFadeIn_ = false;
	isFadeOut_ = false;
	isFadeOutHalf_ = true;
}

void Transition::Update() {
	if (isFadeIn_) {
		FadeInUpdate();
	} else if (isFadeOut_) {
		FadeOutUpdate();
	}
}

void Transition::FadeInUpdate() {
	transitionSprite_->SetColor(Vector4(1, 1, 1, alpha));

	alpha += 0.01f;

	if (alpha >= 1.0f) {
		alpha = 1.0f;
		isFadeIn_ = false;
		isFadeOut_ = false;
	}
}

void Transition::FadeOutUpdate() {
	transitionSprite_->SetColor(Vector4(1, 1, 1, alpha));

	alpha -= 0.01f;

	if (alpha <= 0.3f) {
		isFadeOutHalf_ = false;
		if (alpha <= 0.0f) {
			alpha = 0.0f;
			isFadeOut_ = false;
		}
	}
}

void Transition::Draw() {
	if (isFadeIn_ || isFadeOut_) {
		transitionSprite_->Draw();
	}
}

void Transition::SetFadeIn(bool flag) {
	alpha = 0.0f;
	isFadeIn_ = flag;
	isFadeOut_ = !flag;
	isFadeOutHalf_ = flag;
}
