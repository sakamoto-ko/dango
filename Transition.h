#pragma once
#include <Sprite.h>
#include <TextureManager.h>
#include <memory>
#include <vector>

class Transition {
public:
	void Initialize();
	void Update();
	void Draw();

	void SetFadeIn(bool flag);
	bool GetFadeIn() { return isFadeIn_; }
	bool GetFadeOutHalf() { return isFadeOutHalf_; }
	bool GetFadeOut() { return isFadeOut_; }

private:
	void FadeInUpdate();
	void FadeOutUpdate();

private:
	bool isFadeIn_ = false;
	bool isFadeInHalf_ = false;
	bool isFadeOut_ = false;
	bool isFadeOutHalf_ = false;

	// シーン転換エフェクト
	std::unique_ptr<Sprite> transitionSprite_;
	uint32_t transitionTex_ = 0u;

	float alpha = 1.0f;
};
